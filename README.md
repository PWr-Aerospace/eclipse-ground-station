# eclipse-ground-station

A desktop ground-station application for the PWr Aerospace student-built rover.
Written in Qt6 (C++17) with SDL2 support for gamepad input.

## Features

- **Device discovery**
  - UDP-based discovery of the rover on port 5001.
  - Automatic `PC_RESPONSE` handshake.

- **Video Streaming**
  - Receives JPEG video packets over UDP
  - Reassembles packets into full frames
  - Displays live video with connection-status indicator

- **Gamepad Control**  
  - Reads Xbox/PlayStation/etc. controller via SDL2  
  - Normalizes analog sticks and sends control commands over UDP 

- **Logging Panel**  
  - Real-time log viewer (All / Gamepad / UDP)  
  - Filter messages by category 

## Prerequisites

- **C++17** compiler
- **CMake ≥ 3.16**  
- **Qt 6** (Core, Widgets, Network modules)  
- **SDL2** (fetched automatically via CMake)  
- UDP-capable network connection  

## Building the project
### Windows
1. **Clone the repository**
```bash
git clone https://github.com/PWr-Aerospace/eclipse-ground-station.git
cd eclipse-ground-station
```
2. **Create and enter build directory**
```bash
mkdir build
cd build
```
3. **Configure with CMake**
```bash
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=/path/to/Qt6
```
4. **Compile**
```bash
cmake --build . --config Release
```

## Usage
1. Connect your gamepad
2. Run the application
3. Discover the rover:
   - Click refresh in the top-right panel
   - Wait for device to appear in the dropdown.
   - Select it and click Connect

