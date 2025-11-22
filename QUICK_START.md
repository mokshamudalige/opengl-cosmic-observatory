# 🚀 QUICK START GUIDE

**Get your Cosmic Observatory running in 5 minutes!**

---

## ⚡ Fastest Path to Success

### Step 1: Install FreeGLUT (if needed)

#### Windows - MSYS2 (Recommended)
```powershell
# Open MSYS2 terminal and run:
pacman -S mingw-w64-x86_64-freeglut mingw-w64-x86_64-gcc
```

#### Windows - Manual Download
Download from: https://www.transmissionzero.co.uk/software/freeglut-devel/
- Extract to `C:\freeglut`
- Note the `include` and `lib` paths

---

### Step 2: Build the Project

#### Option A: Automated Build (Easiest)
```powershell
cd "c:\Users\draup\Documents\GitHub\opengl-cosmic-observatory"
.\build.ps1
```

#### Option B: Manual Build
```powershell
cd "c:\Users\draup\Documents\GitHub\opengl-cosmic-observatory"

# If FreeGLUT is installed system-wide:
g++ -o build/cosmic_observatory.exe src/main.cpp -lfreeglut -lopengl32 -lglu32 -std=c++11 -O2

# If FreeGLUT is in custom location:
g++ -o build/cosmic_observatory.exe src/main.cpp `
    -I"C:\freeglut\include" `
    -L"C:\freeglut\lib" `
    -lfreeglut -lopengl32 -lglu32 `
    -std=c++11 -O2

# Copy DLL to build folder (if needed):
Copy-Item "C:\freeglut\bin\freeglut.dll" -Destination ".\build\"
```

---

### Step 3: Run the Application

```powershell
# From project root directory:
.\build\cosmic_observatory.exe
```

---

## 🎮 Using the Application

Once running, you'll see:
```
========================================
  COSMIC OBSERVATORY DESIGNER v1.0
  Creative Coding Assignment - Part 01
========================================
```

### Controls
```
Movement:
  W - Move forward
  S - Move backward
  A - Move left
  D - Move right
  R - Move up
  F - Move down

View:
  Q - Rotate left
  E - Rotate right

Other:
  SPACE - Reset camera
  ESC - Exit
```

---

## ✅ What You Should See

1. **Black space background**
2. **Grid floor** (blue-gray lines)
3. **Yellow constellation pattern** (Bresenham lines)
4. **Three colored planets** (Midpoint circles)
   - Blue (right)
   - Red (left)
   - Green (bottom)
5. **3D Telescope** (gold and metallic)
6. **White stars** scattered across floor

---

## 🔧 Troubleshooting Quick Fixes

### Problem: "Cannot find -lfreeglut"
**Fix**: Install FreeGLUT (see Step 1)

### Problem: Black screen / nothing visible
**Fix**: 
```powershell
# Make sure you're running from project root:
cd "c:\Users\draup\Documents\GitHub\opengl-cosmic-observatory"
.\build\cosmic_observatory.exe
```

### Problem: "telescope.obj not found"
**Fix**: Ensure you're in the project root directory when running

### Problem: Console closes immediately
**Fix**: Run from PowerShell/CMD, not by double-clicking

---

## 📁 Required File Structure

Verify you have:
```
opengl-cosmic-observatory/
├── src/
│   └── main.cpp          ✅ 
├── assets/
│   └── models/
│       ├── telescope.obj ✅
│       └── telescope.mtl ✅
└── build/
    └── cosmic_observatory.exe ✅ (after building)
```

---

## 🎯 Assignment Checklist

Before demo/submission:
- [ ] Program compiles without errors
- [ ] All visual elements appear
- [ ] Camera controls work
- [ ] Can navigate scene freely
- [ ] Telescope is visible and lit
- [ ] Planets show distinct colors
- [ ] Constellation lines are yellow
- [ ] Grid provides spatial reference

---

## 📹 Recording Demo Video

### Best Camera Angles

1. **Overview shot**: Start at default position (0, 80, 150)
2. **Grid closeup**: Move to (0, 30, 0) and look down
3. **Telescope detail**: Move to (0, 20, 50) and rotate
4. **Planet view**: Move to (-50, 40, 0) and rotate
5. **Constellation detail**: Move to (0, 15, 0) and look down

### Video Structure (3 minutes)
```
0:00-0:30  Introduction + concept explanation
0:30-1:00  Algorithm demonstration (point out each)
1:00-2:00  Interactive navigation
2:00-2:30  Different viewing angles
2:30-3:00  Summary + final shot
```

---

## 🆘 Need More Help?

See detailed guides:
- **COMPILATION_GUIDE.md** - Full compilation instructions
- **README.md** - Complete project documentation  
- **OPTIMIZATION_SUMMARY.md** - What was changed and why

---

## ⚡ Ultra-Quick Reference

```powershell
# Build
.\build.ps1

# Run
.\build\cosmic_observatory.exe

# Move: W/A/S/D
# Height: R/F
# Rotate: Q/E
# Reset: SPACE
# Exit: ESC
```

---

**Ready to impress! 🌟**

*Your cosmic observatory is now optimized and ready for Part 01 submission!*
