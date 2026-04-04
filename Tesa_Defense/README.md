# Drone Tracking System

## Overview
A system for detecting, tracking, and estimating GPS coordinates for small drones from video. This system uses a **YOLO** model to find the drone's position in the video frame, and uses a **Machine Learning Model** to convert Bounding Box values into GPS coordinates (latitude, longitude, altitude). It also automatically sends the latest detected location data and images to the **TESA API**.

## Features
- **Accurate Drone Detection:** Powered by the `best_6.1.pt` model (YOLO).
- **Real-time GPS Estimation:** Calculates and estimates target coordinates (Lat/Lon/Alt) from size and position using the `improved_gps_model.pkl` model.
- **Smart Tracking System:** Can track up to 2 drones simultaneously (Max Detections), create stable IDs, and draw flight paths.
- **Save Results as Video:** Creates an `.mp4` output file with Timestamps and complete data, ready to play immediately when processing is finished.
- **Auto GPU/CPU Switching:** Utilizes CUDA graphics card processing power for fast framerates (if available).
- **TESA API Integration:** Collects JSON data and images, sending real-time POST requests to the server via the `tesa_api.py` module.

## Demo

[![Watch the video](https://img.shields.io/badge/YouTube-Watch%20Demo-red?style=for-the-badge&logo=youtube)](https://youtu.be/CXnDO8Qoxrs)

![Demo](Demo/Screenshot%202026-03-08%20152559.png)

## Project Structure
- `4_gps_testing.py`: Main script to start reading the video and running all models.
- `tesa_api.py`: File managing the part of sending Tracking results and images to the API.
- `best_6.1.pt`: YOLO model weights.
- `improved_gps_model.pkl`: Model for coordinate prediction.
- `scaler_features.pkl` & `scaler_targets.pkl`: Helper files to manage scaling of Feature data and targets before/after entering the GPS model.

## Prerequisites
Language: **Python 3.x**

Install all required libraries with the following command:
```bash
pip install opencv-python numpy ultralytics joblib scikit-learn torch requests
```

## Usage
1. **Set the source video path** in the `4_gps_testing.py` file. Edit the variable below to match the file location on your machine:
   ```python
   video_path = r'C:\Users\MSII\OneDrive\Desktop\P3_DRONE\P3_VIDEO.mp4'
   ```
2. **Setup API Environment Variables** (can be skipped if the existing test API works):
   - `TESA_API_BASE_URL` - Base URL of the API
   - `TESA_CAMERA_ID` - Camera ID
   - `TESA_CAMERA_TOKEN` - Authentication token for sending camera data
3. **Run the drone tracker**:
   ```bash
   python 4_gps_testing.py
   ```
4. **View results**: A detection preview screen will be displayed in real-time (you can press `q` or `ESC` to exit). When the video has finished processing, the system will save an `.mp4` video in the same folder and try to open it automatically.