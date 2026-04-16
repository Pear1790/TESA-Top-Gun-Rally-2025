# TESA Top Gun Rally 2025 - Integrated Drone System

## Project Overview

This is a comprehensive drone detection, tracking, and control system developed for the **TESA Top Gun Rally 2025** competition. The project is divided into two main subsystems:

1. **TESA Defense (Python)** - Drone Detection & GPS Tracking System
2. **TESA Offense (MATLAB)** - Quadrotor Control & Trajectory Planning System

---

## 🛡️ TESA Defense - Drone Detection & Tracking System

### Overview
An advanced computer vision and machine learning system for detecting, tracking, and estimating GPS coordinates of small drones from video footage. This system integrates YOLO object detection with ML-based GPS coordinate estimation.

### Key Features
- **🎯 Accurate Drone Detection**: Powered by YOLO v8 model (`best_6.1.pt`)
- **📍 Real-time GPS Estimation**: Converts bounding box values to GPS coordinates (Latitude/Longitude/Altitude)
- **👁️ Smart Tracking System**: Tracks up to 2 drones simultaneously with stable IDs and flight path visualization
- **🎬 Video Output**: Generates `.mp4` output with timestamps, detected data, and flight paths
- **⚡ GPU/CPU Auto-switching**: Utilizes CUDA for fast processing when available
- **🔗 TESA API Integration**: Real-time data transmission via REST API

### Project Structure
```
Tesa_Defense/
├── 4_gps_testing.py              # Main tracking script
├── tesa_api.py                   # API integration module
├── best_6.1.pt                   # YOLO v8 model weights
├── improved_gps_model.pkl        # GPS coordinate prediction model
├── scaler_features.pkl           # Feature scaling for ML model
├── scaler_targets.pkl            # Target scaling for ML model
├── README.md                      # Defense system documentation
└── Demo/                          # Demo videos and screenshots
```

### Technology Stack
- **Python 3.x**
- **OpenCV** - Video processing
- **YOLOv8** - Object detection
- **scikit-learn** - Machine learning models
- **PyTorch** - Deep learning framework
- **Requests** - API communication

### Installation & Setup

#### Prerequisites
```bash
python -m pip install --upgrade pip
```

#### Install Dependencies
```bash
pip install opencv-python numpy ultralytics joblib scikit-learn torch requests
```

#### Configuration
Set up environment variables for API access:
```bash
set TESA_API_BASE_URL=https://tesa-api.crma.dev/api
set TESA_CAMERA_ID=your_camera_id
set TESA_CAMERA_TOKEN=your_authentication_token
```

### Usage

1. **Set video source** in `4_gps_testing.py`:
   ```python
   video_path = r'C:\path\to\your\video.mp4'
   ```

2. **Run the drone tracker**:
   ```bash
   python 4_gps_testing.py
   ```

3. **Watch real-time detection**:
   - Press `Q` or `ESC` to exit live preview
   - System will process the entire video and generate output

4. **Results**:
   - Output video saved in same directory with detection overlays
   - GPS coordinates logged with timestamps
   - Flight paths drawn on output video

---

## 🚁 TESA Offense - Quadrotor Control System

### Overview
A MATLAB/Simulink-based system for simulating quadrotor drone dynamics, trajectory planning, and control. Implements geometric tracking control with PD controllers for autonomous flight.

### Key Features
- **🔧 Rotor Dynamics Simulation**: Complete quadrotor physical model
- **📈 Trajectory Planning**: Minimum-snap trajectory generation through multiple waypoints
- **🎮 Control System**:
  - Geometric Tracking Control (GTC)
  - PD Controllers for attitude and thrust control
  - Electronic Speed Controller (ESC) integration
- **⚡ ODE-based Simulation**: First-order and higher-order dynamics
- **📊 Real-time Visualization**: Position and attitude tracking plots

### Project Structure
```
Tesa_Offense/
├── Quadrotor_Drone_PD_Control.m           # Main control algorithm
├── quadrotorsmodel2.slx                   # Simulink model (main)
├── TESA_Workspace1.1.txt                  # Configuration notes
├── license.txt                            # Project license
│
├── Offense-Day1-Problem/                  # Day 1 Challenge
│   ├── TESA_Workspace1.m                  # Main workspace setup
│   ├── TESA_Workspace2.m                  # Alternative configurations
│   ├── quadrotorsmodel1.slx               # Model variant 1
│   ├── quadrotorsmodel2.slx               # Model variant 2
│   ├── quadrotorsmodel3.slx               # Model variant 3
│   ├── min_snap_Coef.m                    # Trajectory coefficient generator
│   ├── getTrajectory.m                    # Trajectory interpolation
│   ├── hatmap.m                           # Attitude mapping
│   ├── veemap.m                           # Vector field mapping
│   ├── custom_neg_sqrt.m                  # Custom math function
│   └── quadrotorsmodel2_grt_rtw/          # Generated code
│
├── Offense-Day2-Problem/                  # Day 2 Challenge
│   ├── TESA_Workspace2.m                  # Day 2 setup
│   ├── quadrotorsmodel2.slx               # Updated model
│   ├── min_snap_Coef.m                    # Trajectory generation
│   ├── getTrajectory.m                    # Trajectory functions
│   ├── hatmap.m                           # Hat mapping operators
│   ├── veemap.m                           # Vector field operators
│   └── data.mat                           # Stored data/results
│
└── matlab_pre_camp/                       # Pre-competition training
    ├── pre_camp.slx                       # Training model
    ├── pre_camp_workspace.m               # Training workspace
    └── hatmap.m                           # Vector operators
```

### Technology Stack
- **MATLAB R2020b+** (Simulink)
- **Simulink Control System Toolbox**
- **Symbolic Math Toolbox**

### System Architecture

#### Control Architecture
```
Waypoints → Trajectory Planning → Geometric Tracking → Attitude Controller → 
Motor Commands → ESC → Rotor Thrust → Dynamics → State Feedback
```

#### Quadrotor Dynamics
- **Rotor Model**: Individual motor dynamics with thrust proportional to rotational speed squared
- **Attitude Dynamics**: Euler angle kinematics and angular rate dynamics
- **Position Dynamics**: Linear motion equations under gravitational force
- **State Feedback**: Complete state (position, velocity, attitude, angular rate) available for control

### Key Control Equations

#### 1. Geometric Tracking Control
```
Track​ = [-k_e·e - k_ė·ė]

where:
  e = position error
  ė = velocity error
  k_e, k_ė = control gains
```

#### 2. Rotor Dynamics
```
L·f = L·τ + ω·(ω × I·ω) + (ld m·g_earth)

where:
  L = attitude error correction matrix
  f = collective thrust
  τ = torque command
  ω = angular velocity
```

#### 3. Minimum-Snap Trajectory
- Generates smooth 8th-order polynomial trajectories
- Ensures zero acceleration at waypoints
- Optimizes for minimal derivative (snap)

### Running the System

#### Basic Setup
1. Open MATLAB
2. Navigate to `Offense-Day1-Problem` or `Offense-Day2-Problem`
3. Run the workspace setup script:
   ```matlab
   TESA_Workspace1
   % or
   TESA_Workspace2
   ```

#### Simulation in Simulink
1. Open model: `quadrotorsmodel2.slx`
2. Set waypoints in workspace script:
   ```matlab
   waypoints = [0 0 0 0;      % x y z yaw
                0 0 -10 1;
                10 10 -10 1;
                10 30 -20 1;
                30 40 -30 1];
   timepoints = [0 5 10 15 20];  % time at each waypoint (seconds)
   ```
3. Run simulation and visualize results

#### Trajectory Planning
```matlab
% Generate minimum-snap coefficients
trajectory_coef = min_snap_Coef(waypoints, timepoints);

% Query trajectory at specific time
[position, velocity, acceleration] = getTrajectory(trajectory_coef, t);
```

### Key Files Description

| File | Purpose |
|------|---------|
| `min_snap_Coef.m` | Computes minimum-snap polynomial coefficients |
| `getTrajectory.m` | Evaluates trajectory at given time |
| `hatmap.m` | Converts vector to skew-symmetric matrix |
| `veemap.m` | Extracts vector from skew-symmetric matrix |
| `quadrotorsmodel2.slx` | Main Simulink simulation model |
| `Quadrotor_Drone_PD_Control.m` | PD controller implementation |

---

## 📊 Integration & Data Flow

```
TESA Offense (Control)          →        Real-world/Simulation
  Position & Trajectory Generated
         ↓
  Sends Target Waypoints
         ↓
TESA Defense (Detection)        ←        Drone / Video Feed
  Detects Actual Position
         ↓
  Calculates Error & Sends to API
         ↓
  TESA API Server
```

---

## 🚀 Getting Started

### For Defense (Python)
1. Clone the repository
2. Install Python dependencies: `pip install -r requirements.txt`
3. Download model weights if needed
4. Set environment variables for API
5. Run `python 4_gps_testing.py`

### For Offense (MATLAB)
1. Open MATLAB
2. Ensure Simulink and Control Toolbox are installed
3. Navigate to Offense directory
4. Run workspace setup script
5. Open and run Simulink model

---

## 📝 Requirements & Dependencies

### Defense (Python)
- Python 3.8+
- opencv-python
- numpy
- ultralytics (YOLOv8)
- scikit-learn
- torch
- requests
- joblib

### Offense (MATLAB)
- MATLAB R2020b or newer
- Simulink
- Control System Toolbox
- Symbolic Math Toolbox

---

## 📄 License
See `license.txt` in Tesa_Offense directory for license information.

---

## 👥 Project Contributors
- TESA Top Gun Rally 2025 Team
Member
1.[Gorrawit Puttabarramee](https://github.com/Ampsoria)
2.[กนกพร รุ่งรัตนพงศ์](https://github.com/Ampsoria)
3.[Thassanawalai Manusayanont](https://github.com/thassanawalai)
4.[Nontapat Boonsup](https://github.com/AozoraSycro)
5.[Siriwimon Sitasen(ME)](https://github.com/Pear1790)

---

## 📞 Support & Resources

- **MATLAB Documentation**: https://www.mathworks.com/help/matlab/
- **YOLO Documentation**: https://docs.ultralytics.com/

---

## 📌 Notes

- Ensure MATLAB licenses are valid before running simulations
- GPU support recommended for faster video processing in Defense system
- All video paths should be absolute or relative from workspace root
- API tokens should be stored securely in environment variables

---

**Last Updated**: November 2025  
**Status**: Active Development
