import cv2
import numpy as np
from ultralytics import YOLO
import joblib
import pickle
from datetime import datetime
import time
import os
import torch

from tesa_api import upload_detection_cv2_frame

class DroneTracking:
    def __init__(self, yolo_model_path, gps_model_path, scaler_features_path, scaler_targets_path):
        """Initialize YOLO, GPS model/scalers, device, and tracking state."""
        self.device = 'cuda' if torch.cuda.is_available() else 'cpu'
        self.use_half = torch.cuda.is_available()

        self.yolo_model = YOLO(yolo_model_path)

        model_data = joblib.load(gps_model_path)
        self.gps_model = model_data['model']

        with open(scaler_features_path, 'rb') as f:
            self.scaler_features = pickle.load(f)
        with open(scaler_targets_path, 'rb') as f:
            self.scaler_targets = pickle.load(f)

        self.previous_positions = {}
        self.drone_paths = {1: [], 2: []}
        self.drone_colors = {1: (0, 0, 255), 2: (0, 255, 255)}
        self.frame_count = 0
        self.detection_history = []
        self.last_gps = {}
        self.trail_seconds = 2
        self.tail_len = 100
        self.warmup_tta_frames = 100
        self.show_preview = True
        self.preview_detected_only = True
        self.preview_max_width = 1280
        self.save_output = True

        self.gps_ready = True
        try:
            _X = np.array([[100, 100, 50, 50, 0.5]], dtype=float)
            _Xs = self.scaler_features.transform(_X)
            _yhat = self.gps_model.predict(_Xs)
            _ = self.scaler_targets.inverse_transform(_yhat)
        except Exception as e:
            print(f"⚠️ GPS .pkl pipeline ตรวจสอบไม่ผ่าน: {e}")
            self.gps_ready = False
    
    def detect_drone_bbox(self, frame):
        """Detect small drones in the frame and return up to two boxes."""

        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
        h, s, v = cv2.split(hsv)
        clahe = cv2.createCLAHE(clipLimit=2.0, tileGridSize=(8, 8))
        v = clahe.apply(v)
        hsv = cv2.merge([h, s, v])
        enhanced_frame = cv2.cvtColor(hsv, cv2.COLOR_HSV2BGR)
        enhanced_frame = cv2.bilateralFilter(enhanced_frame, 5, 50, 50)

        results = self.yolo_model(
            enhanced_frame,
            conf=0.18,
            iou=0.30,
            max_det=150,
            imgsz=(1280, 1280),
            device=self.device,
            half=self.use_half,
            augment=self.frame_count < self.warmup_tta_frames,
            agnostic_nms=True,
            verbose=False
        )
        
        detected_drones = []
        
        if results[0].boxes is not None and len(results[0].boxes) > 0:
            boxes_data = []
            
            for box in results[0].boxes:
                x1, y1, x2, y2 = map(int, box.xyxy[0].tolist())
                conf = float(box.conf[0])
                width = x2 - x1
                height = y2 - y1
                center_x = (x1 + x2) // 2
                center_y = (y1 + y2) // 2
                if (4 <= width <= 700 and 4 <= height <= 700 and
                    conf >= 0.10 and
                    5 <= center_x <= frame.shape[1]-5 and
                    5 <= center_y <= frame.shape[0]-5):
                    
                    boxes_data.append({
                        'x1': x1, 'y1': y1, 'x2': x2, 'y2': y2,
                        'center_x': center_x, 'center_y': center_y,
                        'width': width, 'height': height,
                        'confidence': conf
                    })
            
            boxes_data.sort(key=lambda x: x['confidence'], reverse=True)
            filtered_boxes = self.apply_custom_nms(boxes_data, overlap_threshold=0.20)
            detected_drones = filtered_boxes[:2]

        return detected_drones

    def apply_custom_nms(self, boxes_data, overlap_threshold=0.3):
        """Apply simple IoU-based NMS to reduce duplicate boxes."""
        if not boxes_data:
            return []
            
        filtered = []
        
        for current_box in boxes_data:
            is_duplicate = False
            
            for existing_box in filtered:
                x1_i = max(current_box['x1'], existing_box['x1'])
                y1_i = max(current_box['y1'], existing_box['y1'])
                x2_i = min(current_box['x2'], existing_box['x2'])
                y2_i = min(current_box['y2'], existing_box['y2'])
                
                if x1_i < x2_i and y1_i < y2_i:
                    intersection = (x2_i - x1_i) * (y2_i - y1_i)
                    area_current = current_box['width'] * current_box['height']
                    area_existing = existing_box['width'] * existing_box['height']
                    union = area_current + area_existing - intersection
                    
                    iou = intersection / union if union > 0 else 0
                    
                    if iou > overlap_threshold:
                        is_duplicate = True
                        break
            
            if not is_duplicate:
                filtered.append(current_box)
                
        return filtered
    
    def calculate_distance(self, point1, point2):
        """Compute Euclidean distance between two points."""
        return np.sqrt((point1[0] - point2[0])**2 + (point1[1] - point2[1])**2)
    
    def assign_stable_ids(self, detected_drones):
        """Assign stable track IDs using nearest-neighbor distance."""
        if not self.previous_positions:
            assigned_drones = {}
            for idx, drone in enumerate(detected_drones[:2]):
                track_id = idx + 1
                assigned_drones[track_id] = drone
            return assigned_drones

        assigned_drones = {}
        used_drones = set()

        distance_matrix = {}
        for track_id, prev_pos in self.previous_positions.items():
            for idx, drone in enumerate(detected_drones):
                if idx not in used_drones:
                    current_pos = (drone['center_x'], drone['center_y'])
                    distance = self.calculate_distance(prev_pos, current_pos)
                    distance_matrix[(track_id, idx)] = distance

        sorted_pairs = sorted(distance_matrix.items(), key=lambda x: x[1])
        
        for (track_id, drone_idx), distance in sorted_pairs:
            if track_id not in assigned_drones and drone_idx not in used_drones:
                if distance < 200:
                    assigned_drones[track_id] = detected_drones[drone_idx]
                    used_drones.add(drone_idx)

        available_ids = [1, 2]
        for drone_idx, drone in enumerate(detected_drones):
            if drone_idx not in used_drones and len(assigned_drones) < 2:
                for track_id in available_ids:
                    if track_id not in assigned_drones:
                        assigned_drones[track_id] = drone
                        break
        
        return assigned_drones
    
    def predict_gps(self, bbox_data):
        """Predict GPS coordinates from bounding box features."""
        if not self.gps_ready:
            return {'latitude': 0.0, 'longitude': 0.0, 'altitude': 0.0}

        try:
            features = np.array([[float(bbox_data['center_x']), float(bbox_data['center_y']),
                                   float(bbox_data['width']), float(bbox_data['height']),
                                   float(bbox_data['confidence'])]], dtype=float)
            features_scaled = self.scaler_features.transform(features)
            gps_scaled = self.gps_model.predict(features_scaled)
            gps_coords = self.scaler_targets.inverse_transform(gps_scaled)
            return {'latitude': float(gps_coords[0][0]),
                    'longitude': float(gps_coords[0][1]),
                    'altitude': float(gps_coords[0][2])}
        except Exception as e:
            print(f"⚠️ คำนวณ GPS จาก .pkl ไม่สำเร็จ: {e}")
            return {'latitude': 0.0, 'longitude': 0.0, 'altitude': 0.0}
    
    def draw_drone_info(self, frame, track_id, drone, gps_coords):
        """Render bounding box and per-drone info overlays."""
        color = self.drone_colors[track_id]
        cv2.rectangle(frame, (drone['x1'], drone['y1']), (drone['x2'], drone['y2']), color, 3)
        cv2.circle(frame, (drone['center_x'], drone['center_y']), 8, color, -1)
        cv2.circle(frame, (drone['center_x'], drone['center_y']), 4, (255, 255, 255), -1)
        cv2.putText(frame, f"ID: {track_id}", 
                   (drone['x1'], drone['y1'] - 10), 
                   cv2.FONT_HERSHEY_SIMPLEX, 0.8, color, 2)
        info_x = drone['x2'] + 10
        info_y = drone['y1']
        line_height = 25
        
        info_texts = [
            f"Lat: {gps_coords['latitude']:.5f}",
            f"Lon: {gps_coords['longitude']:.5f}", 
            f"Alt: {gps_coords['altitude']:.1f}m",
            f"Conf: {drone['confidence']:.2f}",
            f"Path: {len(self.drone_paths[track_id])}"
        ]
        
        for i, text in enumerate(info_texts):
            y_pos = info_y + i * line_height
            cv2.putText(frame, text, (info_x + 1, y_pos + 1), 
                       cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 0, 0), 2)
            cv2.putText(frame, text, (info_x, y_pos), 
                       cv2.FONT_HERSHEY_SIMPLEX, 0.6, (255, 255, 255), 2)
    
    def draw_path(self, frame, track_id):
        """Render recent path polyline for the given track ID."""
        path = self.drone_paths[track_id]
        if len(path) > 1:
            color = self.drone_colors[track_id]
            points = np.array(path[-self.tail_len:], dtype=np.int32)
            thickness = 3
            for i in range(len(points) - 1):
                cv2.line(frame, tuple(points[i]), tuple(points[i + 1]), color, thickness)

    def draw_summary_block(self, frame):
        """Render summary block for track IDs 1 and 2 (top-left)."""
        base_x, base_y = 20, 40
        line_h = 26
        for tid in [1, 2]:
            color = self.drone_colors[tid]
            header = f"track_id: {tid}"
            cv2.putText(frame, header, (base_x, base_y), cv2.FONT_HERSHEY_SIMPLEX, 0.8, color, 2)
            gps = self.last_gps.get(tid)
            if gps:
                cv2.putText(frame, f"- lat: {gps['latitude']:.5f}", (base_x, base_y + line_h), cv2.FONT_HERSHEY_SIMPLEX, 0.8, color, 2)
                cv2.putText(frame, f"- lon: {gps['longitude']:.5f}", (base_x, base_y + 2*line_h), cv2.FONT_HERSHEY_SIMPLEX, 0.8, color, 2)
                cv2.putText(frame, f"- alt: {gps['altitude']:.2f}", (base_x, base_y + 3*line_h), cv2.FONT_HERSHEY_SIMPLEX, 0.8, color, 2)
            base_y += 4*line_h + 14

    def draw_timestamp(self, frame):
        """Render current timestamp at the top-right corner."""
        ts = datetime.now().strftime("%Y-%m-%d %H:%M:%S %a")
        size = cv2.getTextSize(ts, cv2.FONT_HERSHEY_SIMPLEX, 0.8, 2)[0]
        x = frame.shape[1] - size[0] - 20
        y = 40
        cv2.putText(frame, ts, (x, y), cv2.FONT_HERSHEY_SIMPLEX, 0.8, (200, 200, 200), 2)
    
    def track_drones(self, video_path):
        """Process video, track drones, preview live, and optionally save output."""
        cap = cv2.VideoCapture(video_path)
        
        if not cap.isOpened():
            print(f"❌ ไม่สามารถเปิดไฟล์วิดีโอ: {video_path}")
            return

        width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
        height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
        fps = cap.get(cv2.CAP_PROP_FPS)
        total_frames = int(cap.get(cv2.CAP_PROP_FRAME_COUNT))

        print(f"📹 ขนาดวิดีโอ: {width}x{height}")
        print(f"🎬 FPS: {fps}, เฟรมทั้งหมด: {total_frames:,}")

        try:
            if fps and fps > 0:
                self.tail_len = max(30, int(fps * self.trail_seconds))
        except Exception:
            pass

        timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
        output_filename = f'drone_tracking_result_{timestamp}.mp4'
        out = None
        if self.save_output:
            fourcc = cv2.VideoWriter_fourcc(*'mp4v')
            out = cv2.VideoWriter(output_filename, fourcc, fps, (width, height))

        if self.show_preview:
            cv2.namedWindow("Detections", cv2.WINDOW_NORMAL)
            init_w = min(width, self.preview_max_width)
            init_h = int(height * (init_w / width)) if width > 0 else height
            cv2.resizeWindow("Detections", init_w, init_h)
            self.detected_delay_ms = 50
        
        device_name = "GPU" if self.device == 'cuda' else "CPU"
        print(f"⚡ เริ่มประมวลผลด้วย {device_name} และความแม่นยำสูง...") 
        start_time = time.time()
        total_detections = 0
        
        while cap.isOpened():
            ret, frame = cap.read()
            if not ret:
                break
                
            self.frame_count += 1
            
            detected_drones = self.detect_drone_bbox(frame)
            detected = bool(detected_drones)
            
            if detected:
                total_detections += len(detected_drones)
                assigned_drones = self.assign_stable_ids(detected_drones)
                for track_id in [1, 2]:
                    if track_id in assigned_drones:
                        self.draw_path(frame, track_id)
                current_positions = {}
                frame_objects = []
                for track_id, drone in assigned_drones.items():
                    gps_coords = self.predict_gps(drone)
                    cx, cy = drone['center_x'], drone['center_y']
                    if track_id in self.previous_positions:
                        px, py = self.previous_positions[track_id]
                        cx = int(0.7 * px + 0.3 * cx)
                        cy = int(0.7 * py + 0.3 * cy)
                    center_pos = (cx, cy)
                    self.drone_paths[track_id].append(center_pos)
                    if len(self.drone_paths[track_id]) > self.tail_len:
                        self.drone_paths[track_id] = self.drone_paths[track_id][-self.tail_len:]
                    current_positions[track_id] = center_pos
                    self.last_gps[track_id] = gps_coords
                    self.draw_drone_info(frame, track_id, drone, gps_coords)

                    # Build API object payload per detection
                    frame_objects.append({
                        "obj_id": f"track_{track_id}",
                        "type": "drone",
                        "lat": gps_coords.get('latitude', 0.0),
                        "lng": gps_coords.get('longitude', 0.0),
                        "objective": "unknown",
                        "size": "unknown",
                    })
                
                self.previous_positions = current_positions.copy()

                # Upload detections to TESA API
                try:
                    if frame_objects:
                        success = upload_detection_cv2_frame(frame, frame_objects)
                        if not success:
                            print("✗ API upload failed")
                except Exception as e:
                    print(f"✗ API upload error: {e}")

            self.draw_summary_block(frame)
            self.draw_timestamp(frame)

            if self.save_output and out is not None:
                out.write(frame)

            if self.show_preview and (not self.preview_detected_only or detected):
                if width > self.preview_max_width:
                    new_w = self.preview_max_width
                    new_h = int(height * (new_w / width))
                    preview = cv2.resize(frame, (new_w, new_h), interpolation=cv2.INTER_AREA)
                else:
                    preview = frame
                cv2.imshow("Detections", preview)
                wait_ms = self.detected_delay_ms if detected else 1
                key = cv2.waitKey(wait_ms) & 0xFF
                if key in (27, ord('q')):
                    break

        cap.release()
        if self.save_output and out is not None:
            out.release()
        if self.show_preview:
            cv2.destroyAllWindows()

        end_time = time.time()
        processing_time = end_time - start_time
        
        print(f"\n✅ ประมวลผลเสร็จแล้ว! ({processing_time:.1f} วินาที)")
        print(f"📹 ไฟล์ผลลัพธ์: {output_filename}")
        print(f"🎯 พบโดรน: {total_detections:,} ครั้ง จาก {self.frame_count:,} เฟรม")
        print(f"⚡ ความเร็ว: {self.frame_count/processing_time:.1f} FPS")
        
        # เปิดไฟล์ผลลัพธ์ทันที
        try:
            os.startfile(output_filename)
            print("🎬 เปิดวิดีโอผลลัพธ์แล้ว!")
        except Exception as e:
            print(f"❌ กรุณาเปิดไฟล์ {output_filename} ด้วยตัวเอง")

if __name__ == "__main__":
    print("Drone Tracking System with GPU/CPU Auto-Detection")
    print("=" * 55)

    yolo_model_path = 'best_6.1.pt'
    gps_model_path = 'improved_gps_model.pkl'
    scaler_features_path = 'scaler_features.pkl'
    scaler_targets_path = 'scaler_targets.pkl'

    tracker = DroneTracking(yolo_model_path, gps_model_path, scaler_features_path, scaler_targets_path)

    video_path = r'C:\Users\MSII\OneDrive\Desktop\P3_DRONE\P3_VIDEO.mp4'

    print(f"กำลังประมวลผลวิดีโอ: {video_path}")
    tracker.track_drones(video_path)
    print("\nเสร็จสิ้น!")