# Building and Running in MSYS2

## Quick Start (MinGW 64-bit Terminal)

### 1. Open MSYS2 MinGW 64-bit Terminal
- **NOT** the default MSYS2 terminal
- Look for "MSYS2 MinGW 64-bit" in your Start Menu

### 2. Navigate to Project Directory
```bash
cd /c/Users/draup/Documents/GitHub/opengl-cosmic-observatory
```

### 3. Build the Project
```bash
g++ -std=c++11 \
    -I"src/utils" \
    -I"/c/Users/draup/Documents/CS Y3S1/GV/GV_lab4/freeglut/include" \
    src/main.cpp \
    src/algorithms/bresenham.cpp \
    src/algorithms/midpoint_circle.cpp \
    src/algorithms/primitives.cpp \
    src/utils/camera.cpp \
    src/utils/transform.cpp \
    -o build/cosmic_observatory.exe \
    -L"/c/Users/draup/Documents/CS Y3S1/GV/GV_lab4/freeglut/lib/x64" \
    -lfreeglut -lopengl32 -lglu32
```

### 4. Run the Program
```bash
# Add freeglut DLL to PATH
export PATH="/c/Users/draup/Documents/CS Y3S1/GV/GV_lab4/freeglut/bin/x64:$PATH"

# Run
./build/cosmic_observatory.exe
```

---

## One-Line Build & Run

```bash
cd /c/Users/draup/Documents/GitHub/opengl-cosmic-observatory && g++ -std=c++11 -I"src/utils" -I"/c/Users/draup/Documents/CS Y3S1/GV/GV_lab4/freeglut/include" src/main.cpp src/algorithms/*.cpp src/utils/*.cpp -o build/cosmic_observatory.exe -L"/c/Users/draup/Documents/CS Y3S1/GV/GV_lab4/freeglut/lib/x64" -lfreeglut -lopengl32 -lglu32 && export PATH="/c/Users/draup/Documents/CS Y3S1/GV/GV_lab4/freeglut/bin/x64:$PATH" && ./build/cosmic_observatory.exe
```

---

## Troubleshooting

### Issue: `freeglut.dll not found`
**Solution**: Make sure you've added the DLL path:
```bash
export PATH="/c/Users/draup/Documents/CS Y3S1/GV/GV_lab4/freeglut/bin/x64:$PATH"
```

### Issue: `cannot find -lfreeglut`
**Solution**: Verify the library path exists:
```bash
ls "/c/Users/draup/Documents/CS Y3S1/GV/GV_lab4/freeglut/lib/x64"
```

### Issue: Slow compilation
**Reason**: Loading 91,288-line telescope model takes 2-5 seconds (normal)

### Issue: Terminal closes immediately
**Solution**: The program opens in a new window. Look for the OpenGL window.

---

## Path Translation (MSYS2 vs Windows)

| Windows Path | MSYS2 Path |
|-------------|------------|
| `C:\Users\` | `/c/Users/` |
| `D:\Projects\` | `/d/Projects/` |
| `C:\Program Files\` | `/c/Program Files/` |

**Rule**: Replace `C:\` with `/c/` and use forward slashes `/`

---

## Alternative: Using PowerShell Build Script

If you prefer PowerShell (not in MSYS2):
```powershell
.\build.ps1
```

Or manually:
```powershell
$env:PATH = "C:\Users\draup\Documents\CS Y3S1\GV\GV_lab4\freeglut\bin\x64;$env:PATH"
.\build\cosmic_observatory.exe
```

---

## Controls (Once Running)

### Camera Navigation
- `W`/`S` - Forward/Backward
- `A`/`D` - Strafe Left/Right  
- `R`/`F` - Up/Down
- `Q`/`E` - Rotate Left/Right
- `ESC` - Quit

### Customization
- `1`/`2` - Decrease/Increase stars
- `3`/`4` - Decrease/Increase planets
- `5` - Toggle constellation lines
- `6` - Toggle planet orbits
- `7`/`8` - Rotate telescope
- `9`/`+`/`-` - Scale telescope
- `0` - Toggle Orion's Belt

---

## Expected Output

```
Loading telescope model...
Telescope model loaded successfully!
Shapes: 19
Materials: 3

=== COSMIC OBSERVATORY DESIGNER ===
[Camera controls and customization options listed]
```

**Window Title**: "Cosmic Observatory - Interactive Astronomy Designer"

---

## Quick Reference Card

```bash
# MSYS2 Build Commands
cd /c/Users/draup/Documents/GitHub/opengl-cosmic-observatory
g++ -std=c++11 -I"src/utils" -I"/c/Users/draup/Documents/CS Y3S1/GV/GV_lab4/freeglut/include" src/main.cpp src/algorithms/*.cpp src/utils/*.cpp -o build/cosmic_observatory.exe -L"/c/Users/draup/Documents/CS Y3S1/GV/GV_lab4/freeglut/lib/x64" -lfreeglut -lopengl32 -lglu32
export PATH="/c/Users/draup/Documents/CS Y3S1/GV/GV_lab4/freeglut/bin/x64:$PATH"
./build/cosmic_observatory.exe
```

Save this to a file for easy copy-paste!
