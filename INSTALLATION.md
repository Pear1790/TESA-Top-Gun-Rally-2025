# TESA Top Gun Rally 2025 - Installation Guide

## Complete Setup Instructions

---

## Part 1: Defense System Install (Python)

### Step 1: Install Python

#### Windows
1. Download Python from https://python.org (version 3.8+)
2. Run installer
3. **IMPORTANT**: Check "Add Python to PATH"
4. Click Install

#### macOS
```bash
# Using Homebrew
brew install python3

# Verify
python3 --version
```

#### Linux (Ubuntu/Debian)
```bash
sudo apt update
sudo apt install python3 python3-pip

# Verify
python3 --version
```

---

### Step 2: Install PyTorch (Optional but Recommended)

PyTorch with CUDA support gives significant speed improvements.

#### Option A: With GPU Support (NVIDIA CUDA)

```bash
# Visit: https://pytorch.org/get-started/locally/
# Select: PyTorch > Stable > Your OS/Python/CUDA version

# Example for Windows with CUDA 11.8:
pip install torch torchvision torchaudio --index-url https://download.pytorch.org/whl/cu118
```

#### Option B: CPU Only (Slower, but works)
```bash
pip install torch torchvision torchaudio
```

---

### Step 3: Install Defense Dependencies

```bash
# Navigate to Tesa_Defense directory
cd Tesa_Defense

# Install all required packages
pip install -r requirements.txt

# Or install manually
pip install opencv-python numpy ultralytics joblib scikit-learn requests
```

#### Verify Installation
```bash
python -c "import cv2, numpy, ultralytics, joblib, sklearn, requests; print('All packages installed!')"
```

---

### Step 4: Configure Defense System

1. Edit `4_gps_testing.py`
2. Find this line (around line 20-30):
   ```python
   video_path = r'C:\Users\MSII\OneDrive\Desktop\P3_DRONE\P3_VIDEO.mp4'
   ```
3. Replace with your video path:
   ```python
   video_path = r'C:\path\to\your\video.mp4'
   ```

#### (Optional) Configure API
If you want to send results to TESA API, set environment variables:

**Windows (PowerShell)**:
```powershell
$env:TESA_API_BASE_URL = "https://tesa-api.crma.dev/api"
$env:TESA_CAMERA_ID = "camera_001"
$env:TESA_CAMERA_TOKEN = "your_token_here"
```

**Windows (Command Prompt)**:
```cmd
set TESA_API_BASE_URL=https://tesa-api.crma.dev/api
set TESA_CAMERA_ID=camera_001
set TESA_CAMERA_TOKEN=your_token_here
```

**macOS/Linux**:
```bash
export TESA_API_BASE_URL="https://tesa-api.crma.dev/api"
export TESA_CAMERA_ID="camera_001"
export TESA_CAMERA_TOKEN="your_token_here"
```

---

### Step 5: Test Defense System

```bash
# Simple Python test
python -c "from ultralytics import YOLO; print('YOLO loaded successfully')"

# Run the detection system
python 4_gps_testing.py
```

**Expected Output**:
- Console shows detection frame rate
- Video display opens with detection overlays
- When complete, output video is saved

---

## Part 2: Offense System Install (MATLAB)

### Step 1: Install MATLAB

#### Requirements
- MATLAB R2020b or newer
- Simulink
- Control System Toolbox

#### Installation
1. Visit https://www.mathworks.com/downloads/
2. Download MATLAB installer for your OS
3. Run installer and follow prompts
4. Enter license information
5. Install required toolboxes (Simulink, Control System Toolbox)

#### Verify Installation
```matlab
% Open MATLAB and run:
ver
```

Look for these in the output:
- MATLAB (any recent version)
- Simulink
- Control System Toolbox

---

### Step 2: Configure MATLAB Path

1. Open MATLAB
2. Set working directory to `Tesa_Offense`
3. Run in Command Window:
   ```matlab
   % Add paths
   addpath(genpath(pwd))
   savepath
   ```

---

### Step 3: Verify Offense Setup

```matlab
% Test trajectory functions
waypoints = [0 0 0 0; 0 0 -10 1; 10 10 -10 1];
timepoints = [0 5 10];

try
    coef = min_snap_Coef(waypoints, timepoints);
    disp('Trajectory generation: OK')
catch
    disp('Error in trajectory generation')
end

% Test Simulink
try
    open('quadrotorsmodel2.slx')
    disp('Simulink model: OK')
catch
    disp('Error opening Simulink model')
end
```

---

### Step 4: Run Offense System

#### Method 1: Workspace + Simulink (Recommended)

```matlab
% 1. Initialize workspace
cd Offense-Day1-Problem
TESA_Workspace1

% 2. Open Simulink model
open quadrotorsmodel2.slx

% 3. Click "Run" button in Simulink interface
```

#### Method 2: Script Only

```matlab
cd Offense-Day1-Problem
TESA_Workspace2  % Runs full simulation
```

---

## Part 3: Verification & Testing

### Quick Test Checklist

#### Defense System
- [ ] Python 3.8+ installed: `python --version`
- [ ] PyTorch available: `python -c "import torch; print(torch.cuda.is_available())"`
- [ ] YOLO working: `python -c "from ultralytics import YOLO; YOLO('yolov8n.pt')"`
- [ ] Video file accessible: `Test with a sample video`
- [ ] Output vector: `Check if output video is created`

#### Offense System
- [ ] MATLAB installed and opened
- [ ] Simulink available: `simulink` command works
- [ ] Control Toolbox present: `ver` shows Control System Toolbox
- [ ] Workspace initializes: `TESA_Workspace1` runs without errors
- [ ] Model opens: `quadrotorsmodel2.slx` opens in Simulink

---

## Part 4: Common Installation Issues

### Defense System Issues

#### Issue: `pip: command not found`
**Solution**: Python not in PATH
```bash
# Windows: Reinstall Python with "Add to PATH" checked
# macOS/Linux: Use python3 instead of python
python3 -m pip install ...
```

#### Issue: `ModuleNotFoundError: No module named 'cv2'`
**Solution**: OpenCV not installed
```bash
pip install opencv-python --upgrade
```

#### Issue: CUDA not found but installed
**Solution**: Update PyTorch with correct CUDA version
```bash
# Remove old version
pip uninstall torch torchvision torchaudio -y

# Reinstall with correct CUDA (check https://pytorch.org)
pip install torch torchvision torchaudio --index-url https://download.pytorch.org/whl/cu<version>
```

---

### Offense System Issues

#### Issue: `Simulink:mdlInitialization:MdlInitFcn`
**Solution**: Simulink model initialization error
```matlab
% Try:
1. Clear workspace: clear all
2. Delete old compiled files: delete *.slxc
3. Reopen model
```

#### Issue: `Undefined function or variable 'min_snap_Coef'`
**Solution**: Path not set correctly
```matlab
% Ensure you're in correct directory
which min_snap_Coef

% If not found, add path manually
addpath(pwd)
addpath(genpath(pwd))
```

#### Issue: Simulink License not found
**Solution**: Use MATLAB activation
```matlab
% Run license manager
>> activate command in MATLAB startup screen
% Or contact your MATLAB license administrator
```

---

## Part 5: Performance Optimization

### Defense System Optimization

#### Enable GPU (if available)
- PyTorch with CUDA automatically uses GPU
- Monitor with: `python -c "import torch; print(torch.cuda.is_available())"`

#### Increase Processing Speed
```bash
# Use faster model weights
# In 4_gps_testing.py, change:
model_size = "yolov8n"  # nano (fastest)
# to
model_size = "yolov8m"  # medium (faster)
```

---

### Offense System Optimization

#### MATLAB Performance
```matlab
% Disable graphics (speeds up simulation)
set_param(gcs, 'ShowPortDataTypes', 'off')
set_param(gcs, 'ShowPortUnits', 'off')

% Set solver to fixed-step
set_param(gcs, 'SolverType', 'fixed')
set_param(gcs, 'FixedStep', '0.01')
```

---

## Part 6: Troubleshooting Uninstall/Reinstall

### Clean Python Installation

```bash
# Windows
pip list | findstr TESA  # Find TESA packages
pip uninstall [package_name] -y
pip cache purge
pip install [packages] --upgrade

# macOS/Linux
pip3 list | grep TESA
pip3 uninstall [package] -y
pip3 install [packages] --upgrade
```

### Clean MATLAB Installation

```matlab
% Clear workspace
clear all
clc

% Clear path
restoredefaultpath
savepath

% Search for and delete .slxc files (compiled models)
% Use File Explorer to search for *.slxc in project directory
```

---

## Part 7: Additional Resources

### Python Testing Tools
```bash
# Check environment
pip show opencv-python
pip show ultralytics
pip show torch

# Run diagnostic
python -m pip check

# Update all packages
pip install --upgrade pip
pip install --upgrade -r requirements.txt
```

### MATLAB Debugging Tools
```matlab
% Check toolbox availability
ver

% Test specific functions
help min_snap_Coef
help quadrotorsmodel2

% Verify all paths
path

% Clear and rebuild
rehash toolboxcache
```

---

## Part 8: Getting Help

If you encounter issues:

1. **Check Console Output**: Most errors include helpful messages
2. **Search Project Documentation**: README.md has answers
3. **Test Individual Components**: Isolate the problem
4. **Update Everything**: Ensure latest versions installed
5. **Restart Applications**: Python kernel, MATLAB, IDE

---

**Installation Complete!** 🎉

You can now:
- Run any video through the Defense system
- Simulate quadrotor control in the Offense system
- Integrate both systems for complete autonomy

---

**Last Updated**: November 2025  
**Version**: 1.0
