# TESA Top Gun Rally 2025 - Files Guide

Comprehensive explanation of every important file in the project.

---

## 🛡️ TESA Defense Files

### Main Script
**File**: `4_gps_testing.py`

**Purpose**: 
- Main entry point for drone detection system
- Reads video file frame by frame
- Runs YOLO detection on each frame
- Converts detections to GPS coordinates
- Sends results to API (optional)
- Generates output video with overlays

**Key Variables to Modify**:
```python
video_path           # Set to your video file path
max_detections = 2   # Change to track more/fewer drones
conf_threshold = 0.5 # Detection confidence threshold (0-1)
```

**Usage**:
```bash
python 4_gps_testing.py
```

**Output**:
- Real-time detection window
- `.mp4` video file with annotations
- Console logs with GPS coordinates

---

### API Integration Module
**File**: `tesa_api.py`

**Purpose**:
- Handles communication with TESA API
- Sends detection results as JSON
- Uploads annotated frames to server
- Manages authentication tokens

**Key Function**: `upload_detection_cv2_frame()`
- Input: OpenCV frame (numpy array) + detection objects
- Output: HTTP response (True if successful)

**Configuration**:
```python
API_BASE_URL = os.getenv("TESA_API_BASE_URL")
CAMERA_ID = os.getenv("TESA_CAMERA_ID")
CAMERA_TOKEN = os.getenv("TESA_CAMERA_TOKEN")
```

**When to Modify**:
- Only if API endpoint changes
- Otherwise, use environment variables

---

### Model Files

#### YOLO Object Detector
**File**: `best_6.1.pt` (260 MB)

**Purpose**: 
- Trained YOLO v8 model for drone detection
- 260MB pre-trained weights
- ~95% accuracy on test drone dataset

**Technical Details**:
- Model: YOLOv8 (Ultralytics)
- Input: 640×640 RGB image
- Output: Bounding boxes + confidence scores
- Classes: Drone

**Auto-Download**:
- Downloaded automatically on first run if missing
- Cached in `~/.cache/ultralytics/`

---

#### GPS Prediction Model
**File**: `improved_gps_model.pkl` (5 MB)

**Purpose**:
- Machine learning model to convert box coordinates → GPS
- Trained on drone altitude/distance/perspective data

**Input Features**:
- Bounding box: [x_min, y_min, x_max, y_max]
- Image size: [width, height]
- Camera calibration parameters

**Output Targets**:
- GPS: [latitude, longitude, altitude]

**How It Works**:
```
Camera Field of View + Box Position + Box Size 
    ↓
Predict GPS Coordinates
    ↓
Returned as [lat, lon, alt]
```

---

#### Feature Scaler
**File**: `scaler_features.pkl` (1 KB)

**Purpose**:
- Normalizes input features for GPS model
- Transforms raw bounding box values to proper scale

**Used By**: `improved_gps_model` prediction

**Important**: 
- Don't modify without retraining the GPS model
- Must be used with matching `scaler_targets.pkl`

---

#### Target Scaler
**File**: `scaler_targets.pkl` (1 KB)

**Purpose**:
- Denormalizes output from GPS model
- Converts model predictions back to GPS format

**Used By**: GPS model output processing

**Important**: 
- Pair with `scaler_features.pkl`
- Required for accurate GPS coordinates

---

### Documentation
**File**: `README.md` (in Tesa_Defense)

**Contains**:
- System overview
- Feature list
- Installation instructions
- Usage guide
- Output format description

---

## 🚁 TESA Offense Files

### Master Control Algorithm
**File**: `Quadrotor_Drone_PD_Control.m`

**Purpose**:
- Core PD controller for quadrotor
- Implements geometric tracking control
- Calculates motor commands from errors

**Main Function**: Control law
```
u = -k_p * error - k_d * error_rate
```

**Parameters to Tune**:
- `k_p`: Proportional gain (position control strength)
- `k_d`: Derivative gain (velocity damping)
- `m`: Drone mass (kg)
- `L`: Arm length (m)
- `g`: Gravity (9.81 m/s²)

**When to Modify**:
- Tuning for specific drone hardware
- Adjusting control responsiveness

---

### Main Simulink Model
**File**: `quadrotorsmodel2.slx`

**Purpose**:
- Complete quadrotor simulation environment
- Integrates all control blocks and dynamics
- Generates trajectory and control outputs

**Model Contents**:
1. **Trajectory Generation Block**
   - Inputs: waypoints, times
   - Outputs: desired position, velocity, acceleration

2. **Geometric Tracking Control Block**
   - Inputs: measured vs. desired state
   - Outputs: attitude command, thrust command

3. **Rotor Dynamics Block**
   - Inputs: motor commands (0-1 throttle)
   - Outputs: thrust force per motor

4. **Quadrotor Dynamics Block**
   - Inputs: thrust forces from motors
   - Outputs: position, velocity, attitude

5. **Visualization Block**
   - Plots position tracking
   - Plots attitude tracking

**How to Run**:
1. Open in MATLAB: `open quadrotorsmodel2.slx`
2. Run workspace first: `TESA_Workspace1`
3. Click Play button in Simulink window

**Simulation Time**: Default 20 seconds

---

### Trajectory Generation
**File**: `min_snap_Coef.m`

**Purpose**:
- Computes minimum-snap polynomial coefficients
- Generates smooth 8th-order polynomials between waypoints
- Ensures zero acceleration/jerk at endpoints

**Algorithm**:
```
Given: Waypoints + Times
Output: Polynomial coefficients for each segment
```

**Input**:
```matlab
waypoints = [x1 y1 z1 ψ1;    % (x, y, z, yaw)
             x2 y2 z2 ψ2;
             ...];
timepoints = [t1 t2 t3 ...];  % times (seconds)
```

**Output**:
```matlab
trajectory_coef % 8×4×N matrix of polynomial coefficients
```

**Math Background**:
- 8th-order polynomial = 9 coefficients
- Constraint: position, velocity, acceleration at keyframes
- Similar to robot arm trajectory planning

---

### Trajectory Interpolation
**File**: `getTrajectory.m`

**Purpose**:
- Evaluates trajectory at any point in time
- Returns position, velocity, acceleration (optional)

**Function Signature**:
```matlab
[position, velocity, acceleration] = getTrajectory(coef, t)
```

**Outputs**:
- `position`: [x, y, z, ψ] at time t
- `velocity`: [ẋ, ẏ, ż, ψ̇] at time t  
- `acceleration`: [ẍ, ÿ, z̈, ψ̈] at time t

**Example Usage**:
```matlab
coef = min_snap_Coef(waypoints, timepoints);
for t = 0:0.01:20
    [pos, vel, acc] = getTrajectory(coef, t);
    % Use pos, vel, acc for control
end
```

---

### Attitude Mapping
**File**: `hatmap.m`

**Purpose**:
- Converts 3D vector to skew-symmetric matrix
- Used in rotation matrix calculations

**Function**:
```matlab
S = hatmap(v)
```

**Input**: 3D vector `v = [v1; v2; v3]`

**Output**: Skew-symmetric matrix
```
S = [  0  -v3   v2 ]
    [ v3    0  -v1 ]
    [-v2   v1    0 ]
```

**Property**: `S * u = v × u` (cross product)

**Used By**: Geometric control for attitude computation

---

### Vector Field Mapping
**File**: `veemap.m`

**Purpose**:
- Inverse of `hatmap`
- Extracts vector from skew-symmetric matrix

**Function**:
```matlab
v = veemap(S)
```

**Input**: Skew-symmetric matrix `S`

**Output**: 3D vector `v = [v1; v2; v3]`

**Relationship**:
```matlab
v = veemap(hatmap(v))  % Returns original v
```

---

### Custom Math Functions
**File**: `custom_neg_sqrt.m`

**Purpose**:
- Computes square root for negative numbers
- Used in complex control calculations

**Function**:
```matlab
y = custom_neg_sqrt(x)
```

**Handles**:
- Positive numbers: normal sqrt
- Negative numbers: returns -sqrt(|x|)
- Used in geometric control law

---

### Workspace Initialization (Day 1)
**File**: `TESA_Workspace1.m`

**Purpose**:
- Initializes all variables for Day 1 challenge
- Sets up paths and configurations
- Generates trajectory coefficients
- Ready for Simulink model execution

**Content**:
1. Clear workspace
2. Define quadrotor parameters (mass, inertia, etc.)
3. Set control gains
4. Define waypoints and times
5. Generate trajectory coefficients
6. Initialize simulation parameters

**How to Run**:
```matlab
cd Offense-Day1-Problem
TESA_Workspace1
```

**After Running**:
- All variables in workspace
- Trajectory computed and ready
- Can now open `quadrotorsmodel2.slx`

---

### Workspace Initialization (Day 2)
**File**: `TESA_Workspace2.m`

**Purpose**:
- Initializes for Day 2 challenge (different scenarios)
- May have different waypoints or parameters
- Adapted to Day 2 requirements

**Differences from Workspace1**:
- Different trajectory/waypoints
- Possibly different control gains
- Updated simulation parameters

**Usage**:
```matlab
cd Offense-Day2-Problem
TESA_Workspace2
```

---

### Simulink Models (Alternatives)
**Files**: 
- `quadrotorsmodel1.slx` - Simple version
- `quadrotorsmodel3.slx` - Advanced version

**Purpose**:
- Different levels of complexity
- Model 1: Basic dynamics only
- Model 2: Full control system (MAIN - **USE THIS**)
- Model 3: Advanced with disturbances

**Recommendation**: Use `quadrotorsmodel2.slx` for normal operation

---

### Pre-Competition Training
**Directory**: `matlab_pre_camp/`

**Files**:
- `pre_camp.slx` - Training model
- `pre_camp_workspace.m` - Training setup
- `hatmap.m` - Vector operations

**Purpose**:
- Educational materials for team training
- Simpler examples for learning
- Builds understanding before competition

---

### Data Storage
**File**: `data.mat` (in Offense-Day2-Problem)

**Purpose**:
- Stores simulation results as MATLAB variables
- Contains:
  - Trajectory coordinates over time
  - Simulated quadrotor states
  - Control command history
  - Performance metrics

**How to Load**:
```matlab
load data.mat
whos  % See all variables
plot(time, position)  % Visualize results
```

---

### Configuration Files
**Files**: 
- `TESA_Workspace1.1.txt` - Alternate configuration
- `TESA_Workspace1.2.txt` - Another variant

**Purpose**:
- Documentation of different setups
- Quick reference for parameter values
- Backup configurations

**Format**: Plain text notes (not executable)

---

### Generated Code
**Directory**: `quadrotorsmodel2_grt_rtw/`

**Purpose**:
- Auto-generated by MATLAB from Simulink model
- Contains C source code version of model
- Used for (Real-Time Workshop) code generation

**Files Include**:
- `quadrotorsmodel2.c` - C implementation
- `quadrotorsmodel2.h` - C header file
- Build files (.bat, .mk, .rsp)

**When Present**: Auto-created, can be deleted safely

---

## 📋 Quick File Reference

### Defense System

| File | Type | Size | Purpose |
|------|------|------|---------|
| 4_gps_testing.py | Python | 5KB | Main script |
| tesa_api.py | Python | 3KB | API integration |
| best_6.1.pt | Model | 260MB | YOLO detector |
| improved_gps_model.pkl | Model | 5MB | GPS predictor |
| scaler_features.pkl | Model | 1KB | Feature normalizer |
| scaler_targets.pkl | Model | 1KB | Output normalizer |
| README.md | Docs | 10KB | Documentation |

### Offense System

| File | Type | Size | Purpose |
|------|------|------|---------|
| Quadrotor_Drone_PD_Control.m | MATLAB | 5KB | Core controller |
| quadrotorsmodel2.slx | Simulink | 500KB | Main model |
| min_snap_Coef.m | MATLAB | 3KB | Trajectory gen |
| getTrajectory.m | MATLAB | 2KB | Trajectory query |
| hatmap.m | MATLAB | 1KB | Vector operations |
| veemap.m | MATLAB | 1KB | Inverse mapping |
| TESA_Workspace1.m | MATLAB | 5KB | Day 1 setup |
| TESA_Workspace2.m | MATLAB | 5KB | Day 2 setup |

---

## 🔄 Data Flow Through Files

### Defense Flow
```
Video File
    ↓
4_gps_testing.py loads best_6.1.pt
    ↓
Detect drones (YOLO)
    ↓
improved_gps_model.pkl predicts GPS
    (with scaler_features.pkl, scaler_targets.pkl)
    ↓
tesa_api.py sends results (optional)
    ↓
Output video + GPS coordinates
```

### Offense Flow
```
Run TESA_Workspace1.m/TESA_Workspace2.m
    ↓
min_snap_Coef.m generates trajectory
    ↓
Open quadrotorsmodel2.slx
    ↓
Simulink calls getTrajectory.m continuously
    ↓
PD controller uses hatmap.m/veemap.m
    ↓
Simulate quadrotor dynamics
    ↓
Plot results + save data.mat
```

---

## 💾 When to Backup

Important files to backup before major changes:

1. **All .slx files** - Simulink models (hard to recreate)
2. **All .m files** - Source code
3. **Model files** (.pt, .pkl) - Trained models (very hard to recreate)
4. **data.mat** - Valuable simulation results

**Less critical** (can regenerate):
- .slxc files (compiled models)
- Generated code directory
- Output videos

---

## 🚨 Don't Delete These

**Critical Files** - Project won't work without them:

✗ **DON'T DELETE**:
- `best_6.1.pt`
- `improved_gps_model.pkl`
- `quadrotorsmodel2.slx`
- `min_snap_Coef.m`
- `getTrajectory.m`
- `TESA_Workspace1.m`

✓ **SAFE TO DELETE**:
- `quadrotorsmodel2_grt_rtw/` (regenerated)
- `.slxc` files (regenerated)
- Output videos (recreatable)
- `slprj/` directories (cache)

---

## 📝 File Naming Convention

### MATLAB Files
- `*_Workspace*.m` - Initialization scripts
- `*_Coef.m` - Coefficient generation
- `*.slx` - Simulink models
- `get*.m` - Query/evaluation functions
- `hat*` / `vec*` - Vector operations

### Python Files
- `*_testing.py` - Main test/script files
- `*_api.py` - API integration
- `*.pkl` - Serialized ML models
- `*.pt` - PyTorch model weights

---

**Last Updated**: November 2025  
**Version**: 1.0
