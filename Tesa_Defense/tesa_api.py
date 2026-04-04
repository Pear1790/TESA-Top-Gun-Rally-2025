import os
import io
import json
import requests
import cv2
from datetime import datetime, timezone

# === TESA API Config ===
API_BASE_URL = os.getenv("TESA_API_BASE_URL", "https://tesa-api.crma.dev/api")
CAMERA_ID = os.getenv("TESA_CAMERA_ID", "xxxxxx")
CAMERA_TOKEN = os.getenv("TESA_CAMERA_TOKEN", "xxxxxx")

def upload_detection_cv2_frame(
    frame,
    objects,
    api_base_url: str = API_BASE_URL,
    camera_id: str = CAMERA_ID,
    camera_token: str = CAMERA_TOKEN,
    timeout: int = 15,
) -> bool:
    """Upload an OpenCV frame (numpy array) and detection objects to TESA API.

    Args:
        frame: BGR image as numpy array.
        objects: list of dicts describing detections.
        api_base_url: API base URL.
        camera_id: camera identifier.
        camera_token: auth token in header.
        timeout: request timeout seconds.
    Returns:
        True if HTTP status is 200 or 201, else False.
    """
    url = f"{api_base_url}/object-detection/{camera_id}"
    headers = {"x-camera-token": camera_token}

    ok, buffer = cv2.imencode(".jpg", frame)
    if not ok:
        print("✗ Failed to encode frame as JPEG")
        return False

    img_file = io.BytesIO(buffer.tobytes())
    files = {"image": ("frame.jpg", img_file, "image/jpeg")}

    utc_time = datetime.now(timezone.utc)
    data = {
        "objects": json.dumps(objects),
        "timestamp": utc_time.isoformat().replace("+00:00", "Z"),
    }

    resp = requests.post(url, files=files, data=data, headers=headers, timeout=timeout)
    print("TESA API status:", resp.status_code)
    return resp.status_code in (200, 201)
