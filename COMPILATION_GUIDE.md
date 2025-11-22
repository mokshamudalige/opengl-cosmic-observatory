# 🔧 Compilation Guide - Cosmic Observatory

This guide covers multiple compilation methods for different environments.

---

## 📋 Prerequisites

Before compiling, ensure you have:

1. **C++ Compiler** (GCC/MinGW, MSVC, or Clang)
2. **OpenGL** (Usually comes with graphics drivers)
3. **GLUT/FreeGLUT** library

---

## 🪟 Windows Compilation

### Method 1: MinGW with FreeGLUT

#### Install FreeGLUT
```powershell
# Using MSYS2 (recommended)
pacman -S mingw-w64-x86_64-freeglut

# Or download from: https://www.transmissionzero.co.uk/software/freeglut-devel/
```

#### Compile
```powershell
cd "c:\Users\draup\Documents\GitHub\opengl-cosmic-observatory"

g++ -o build/cosmic_observatory.exe src/main.cpp `
    -I"C:/path/to/freeglut/include" `
    -L"C:/path/to/freeglut/lib" `
    -lfreeglut -lopengl32 -lglu32 `
    -std=c++11 -O2
```

### Method 2: Visual Studio (MSVC)

#### Using Developer Command Prompt:
```cmd
cd "c:\Users\draup\Documents\GitHub\opengl-cosmic-observatory"

cl /EHsc /std:c++11 /O2 ^
   /I"C:\path\to\freeglut\include" ^
   src\main.cpp ^
   /link ^
   /LIBPATH:"C:\path\to\freeglut\lib" ^
   freeglut.lib opengl32.lib glu32.lib ^
   /OUT:build\cosmic_observatory.exe
```

### Method 3: CMake (Cross-platform)

Create `CMakeLists.txt`:
```cmake
cmake_minimum_required(VERSION 3.10)
project(CosmicObservatory)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})

add_executable(cosmic_observatory src/main.cpp)

target_link_libraries(cosmic_observatory 
    ${OPENGL_LIBRARIES} 
    ${GLUT_LIBRARY}
)
```

Then compile:
```powershell
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

---

## 🐧 Linux Compilation

### Install Dependencies
```bash
# Ubuntu/Debian
sudo apt-get install freeglut3-dev libglu1-mesa-dev

# Fedora/RHEL
sudo dnf install freeglut-devel mesa-libGLU-devel

# Arch Linux
sudo pacman -S freeglut glu
```

### Compile
```bash
cd ~/opengl-cosmic-observatory

g++ -o build/cosmic_observatory src/main.cpp \
    -lglut -lGLU -lGL \
    -std=c++11 -O2 -Wall

# Run
./build/cosmic_observatory
```

---

## 🍎 macOS Compilation

### Prerequisites
```bash
# OpenGL and GLUT are included with macOS
# Just need Xcode Command Line Tools
xcode-select --install
```

### Compile
```bash
cd ~/opengl-cosmic-observatory

g++ -o build/cosmic_observatory src/main.cpp \
    -framework OpenGL -framework GLUT \
    -std=c++11 -O2 -Wno-deprecated

# Run
./build/cosmic_observatory
```

---

## 🚨 Troubleshooting

### Issue: "Cannot find -lfreeglut" or "-lglut32"

**Solution**: Install FreeGLUT or specify library path:
```powershell
# Find where freeglut is installed
Get-ChildItem -Path "C:\" -Recurse -Filter "freeglut.lib" -ErrorAction SilentlyContinue

# Use the found path in compilation
g++ ... -L"C:\path\to\lib" -lfreeglut ...
```

### Issue: "glut.h not found"

**Solution**: Specify include directory:
```powershell
g++ -I"C:\path\to\include" ...
```

### Issue: Program compiles but doesn't run

**Solution**: Copy DLL files to executable directory:
```powershell
# For Windows with FreeGLUT
Copy-Item "C:\path\to\freeglut\bin\freeglut.dll" -Destination ".\build\"
```

### Issue: Black screen or nothing visible

**Checklist**:
1. ✅ Telescope model files exist in `assets/models/`
2. ✅ Working directory is project root (not `build/`)
3. ✅ OpenGL drivers are up to date
4. ✅ Console shows "Telescope loaded successfully!"

---

## 🎮 Running the Application

### From Project Root
```powershell
# Windows
.\build\cosmic_observatory.exe

# Linux/Mac
./build/cosmic_observatory
```

### Expected Console Output
```
========================================
  COSMIC OBSERVATORY DESIGNER v1.0
  Creative Coding Assignment - Part 01
========================================
Loading 3D telescope model...
Telescope loaded successfully!
Shapes: 3, Materials: 3

=== COSMIC OBSERVATORY CONTROLS ===
W/S: Move Forward/Backward
A/D: Move Left/Right
R/F: Move Up/Down
Q/E: Rotate View
SPACE: Reset Camera
ESC: Exit
===================================

Starting Cosmic Observatory...
```

---

## 📁 Required Files

Ensure these files exist before running:
```
opengl-cosmic-observatory/
├── src/main.cpp                    ✅ Main source file
├── assets/
│   └── models/
│       ├── telescope.obj           ✅ 3D model
│       └── telescope.mtl           ✅ Material file
└── build/                          📁 Output directory
```

---

## 🔍 Verification

After successful compilation, test with:

```powershell
# Check if executable was created
Test-Path ".\build\cosmic_observatory.exe"

# Check file size (should be > 100KB)
(Get-Item ".\build\cosmic_observatory.exe").Length

# Run the program
.\build\cosmic_observatory.exe
```

---

## 📦 Alternative: Precompiled Binary

If compilation issues persist, you can:

1. Use Visual Studio IDE (File → New → Project from Existing Code)
2. Use Code::Blocks with GLUT template
3. Request a precompiled binary from instructor/TA

---

## 💡 Quick Start Script

Create `build.ps1`:
```powershell
# Quick build script for Windows
$ErrorActionPreference = "Stop"

Write-Host "Building Cosmic Observatory..." -ForegroundColor Cyan

# Create build directory
if (-not (Test-Path ".\build")) {
    New-Item -ItemType Directory -Path ".\build" | Out-Null
}

# Compile (adjust paths as needed)
g++ -o build/cosmic_observatory.exe src/main.cpp `
    -lfreeglut -lopengl32 -lglu32 `
    -std=c++11 -O2

if ($LASTEXITCODE -eq 0) {
    Write-Host "✅ Build successful!" -ForegroundColor Green
    Write-Host "Run with: .\build\cosmic_observatory.exe"
} else {
    Write-Host "❌ Build failed!" -ForegroundColor Red
}
```

Run with:
```powershell
.\build.ps1
```

---

## 🎯 Final Checklist

Before submission:
- [ ] Code compiles without errors
- [ ] Executable runs and shows all elements
- [ ] Grid, stars, constellations, planets visible
- [ ] 3D telescope renders with materials
- [ ] Camera controls work (W/A/S/D/R/F)
- [ ] Console shows proper output
- [ ] No crashes or freezes

---

**Need help?** Check the README.md for project details and expected output.
