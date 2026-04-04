# TESA Top Gun Rally 2025 - Quick Start Guide

## 🚀 Project Structure Overview

```
TESA-Top-Gun-Rally-2025/
├── README.md                     # Main documentation (English)
├── README_TH.md                  # Main documentation (Thai)
├── QUICK_START.md               # This file
├── INSTALLATION.md              # Detailed installation guide
├── FILES_GUIDE.md               # Guide to key files in each system
│
├── Tesa_Offense/                # MATLAB Quadrotor Control System
│   ├── Offense-Day1-Problem/    # Challenge 1 files
│   └── Offense-Day2-Problem/    # Challenge 2 files
│
└── Tesa_Defense/                # Python Drone Detection System
    ├── 4_gps_testing.py         # Main detection script
    └── tesa_api.py              # API integration
```

---

## ⚡ 5-Minute Quick Start

### Option 1: Run Defense System (Python)

```bash
# Navigate to Defense directory
cd Tesa_Defense

# Install dependencies
pip install opencv-python numpy ultralytics joblib scikit-learn torch requests

# Edit video path in 4_gps_testing.py
# Change: video_path = r'path\to\your\video.mp4'

# Run detection
python 4_gps_testing.py
```

**Expected Result**: 
- Real-time detection window showing drone tracking
- Output video saved in same directory
- GPS coordinates logged in console

---

### Option 2: Run Offense System (MATLAB)

```matlab
% Open MATLAB and navigate to Offense-Day1-Problem
cd Offense-Day1-Problem

% Run workspace setup
TESA_Workspace1

% Open and run Simulink model
% File: quadrotorsmodel2.slx
```

**Expected Result**:
- Simulink model opens with quadrotor dynamics
- Trajectory planning runs automatically
- Position and attitude tracking plots displayed

---

## 📋 System Requirements

| System | Requirement | Download Link |
|--------|-------------|---------------|
| **Defense** | Python 3.8+ | https://python.org |
| **Defense** | CUDA (Optional) | https://developer.nvidia.com/cuda-toolkit |
| **Offense** | MATLAB R2020b+ | https://mathworks.com |
| **Offense** | Simulink | Included in MATLAB |
| **Offense** | Control System Toolbox | Included in MATLAB |

---

## 🔴 Common Issues & Solutions

### Defense System

**Issue**: `ModuleNotFoundError: No module named 'ultralytics'`
```bash
Solution: pip install ultralytics --upgrade
```

**Issue**: Video not found
```bash
Solution: Use absolute path or place video in same directory as script
video_path = r'C:\full\path\to\video.mp4'  # Use full path
```

**Issue**: CUDA not available
```bash
Solution: System will automatically fall back to CPU
# No action needed - process will be slower but functional
```

---

### Offense System

**Issue**: `Simulink model won't open`
```matlab
Solution: Check if Simulink is installed
simulink % This should open Simulink start menu
```

**Issue**: Undefined variables in workspace
```matlab
Solution: Run TESA_Workspace1.m or TESA_Workspace2.m first
TESA_Workspace1  % Initializes all variables
```

---

## 📁 Key Files You Need to Know

### TESA Defense
- `4_gps_testing.py` - **Main entry point** ✅ Start here
- `best_6.1.pt` - YOLO model (260MB) - Auto-downloaded if missing
- `tesa_api.py` - API integration (modify if changing API endpoint)

### TESA Offense
- `TESA_Workspace1.m` - **Main setup script** ✅ Start here
- `quadrotorsmodel2.slx` - **Main Simulink model** ✅ Run this
- `min_snap_Coef.m` - Trajectory generation (don't modify unless needed)
- `getTrajectory.m` - Trajectory evaluation (don't modify unless needed)

---

## 🎯 What Each System Does

### 🛡️ Defense: Detect & Track Drones
1. **Input**: Video file from camera
2. **Process**: 
   - Detect drones using YOLO
   - Convert detection to GPS coordinates
   - Track multiple drones
3. **Output**: 
   - Annotated video
   - GPS locations
   - Detection reports

### 🚁 Offense: Simulate & Control
1. **Input**: Waypoint targets
2. **Process**:
   - Plan smooth trajectory
   - Simulate quadrotor dynamics
   - Generate control commands
3. **Output**:
   - Desired flight path
   - Control signals
   - Motion data

---

## 🔗 Integration Flow

```
  Waypoints (Target)
        ↓
  [TESA Offense] Plan Trajectory
        ↓
  Send Target Position
        ↓
  [TESA Defense] Measure Actual Position
        ↓
  Calculate Error
        ↓
  Send to TESA API
```

---

## 📊 Model Files Included

| File | Size | Purpose |
|------|------|---------|
| `best_6.1.pt` | ~260MB | YOLO drone detector |
| `improved_gps_model.pkl` | ~5MB | GPS estimator |
| `scaler_features.pkl` | ~1KB | Feature normalizer |
| `scaler_targets.pkl` | ~1KB | Target normalizer |

---

## 💾 Output Files Generated

### Defense System Outputs
- `output_####.mp4` - Annotated video with detection overlays
- `detections.csv` - GPS coordinates and timestamps (if enabled)
- Console logs - Real-time detection info

### Offense System Outputs
- `trajectory.mat` - Saved workspace with trajectory data
- Console plots - Position and attitude tracking
- Saved logs - Simulation results (if enabled)

---

## 🌐 API Configuration (Defense)

To send data to TESA API:

```bash
# Windows PowerShell
$env:TESA_API_BASE_URL = "https://tesa-api.crma.dev/api"
$env:TESA_CAMERA_ID = "camera_001"
$env:TESA_CAMERA_TOKEN = "your_token_here"

# Linux/Mac Bash
export TESA_API_BASE_URL="https://tesa-api.crma.dev/api"
export TESA_CAMERA_ID="camera_001"
export TESA_CAMERA_TOKEN="your_token_here"
```

---

## 📞 Quick Reference

### Defense Commands
```python
# Run drone detection
python 4_gps_testing.py

# Modify video source
video_path = r'your_video.mp4'

# Check GPU usage
# See CUDA status in console output
```

### Offense Commands
```matlab
% Initialize system
TESA_Workspace1

% Generate trajectory
trajectory_coef = min_snap_Coef(waypoints, timepoints)

% Query trajectory at time t
[pos, vel, acc] = getTrajectory(trajectory_coef, t)

% Run simulation
% Press Play in Simulink editor
```

---

## 🎓 Learning Resources

### For Defense (Python)
- YOLO Documentation: https://docs.ultralytics.com/
- OpenCV Tutorials: https://docs.opencv.org/
- scikit-learn Guide: https://scikit-learn.org/

### For Offense (MATLAB)
- MATLAB Getting Started: https://www.mathworks.com/help/matlab/
- Simulink Tutorial: https://www.mathworks.com/help/simulink/
- Control Systems: https://www.mathworks.com/help/control/

---

## ✅ Verification Checklist

Before running either system:

### Defense System
- [ ] Python 3.8+ installed
- [ ] All packages installed: `pip list | findstr opencv ultralytics torch`
- [ ] Video file exists and path is correct
- [ ] GPU drivers updated (optional but recommended)

### Offense System
- [ ] MATLAB R2020b or newer is installed
- [ ] Simulink is installed
- [ ] Control System Toolbox is available
- [ ] Math functions are working

---

## 🚨 Emergency Troubleshooting

If something isn't working:

1. **Check Python versions** (Defense):
   ```bash
   python --version
   pip list
   ```

2. **Check MATLAB workspace** (Offense):
   ```matlab
   ver  % Show installed toolboxes
   path  % Show MATLAB path
   ```

3. **Check file permissions**:
   - Ensure read/write access to directories
   - Check file is not corrupted

4. **Check dependencies**:
   - Update all packages: `pip install --upgrade --all`
   - Verify CUDA compatibility if using GPU

5. **Clear cache/temp files**:
   - Delete `__pycache__` directories
   - Restart Python/MATLAB

---

## 📞 Support

For issues:
1. Check README.md for detailed documentation
2. Check specific system error messages
3. Review code comments in files
4. Contact TESA support team

---

**Last Updated**: November 2025  
**Version**: 1.0
