# Quick Build Script for Cosmic Observatory
# Adjust library paths as needed for your system

$ErrorActionPreference = "Stop"

Write-Host "`n========================================" -ForegroundColor Cyan
Write-Host "  COSMIC OBSERVATORY - BUILD SCRIPT" -ForegroundColor Cyan
Write-Host "========================================`n" -ForegroundColor Cyan

# Create build directory
if (-not (Test-Path ".\build")) {
    Write-Host "Creating build directory..." -ForegroundColor Yellow
    New-Item -ItemType Directory -Path ".\build" | Out-Null
}

# Check if source file exists
if (-not (Test-Path ".\src\main.cpp")) {
    Write-Host "❌ Error: src\main.cpp not found!" -ForegroundColor Red
    exit 1
}

# Try different compilation methods
Write-Host "Attempting compilation..." -ForegroundColor Yellow

# Method 1: Try with freeglut
Write-Host "`nMethod 1: Trying with freeglut..." -ForegroundColor Gray
$output = & g++ -o build/cosmic_observatory.exe src/main.cpp -lfreeglut -lopengl32 -lglu32 -std=c++11 -O2 2>&1
if ($LASTEXITCODE -eq 0) {
    Write-Host "✅ Build successful with freeglut!" -ForegroundColor Green
    Write-Host "`nExecutable: .\build\cosmic_observatory.exe" -ForegroundColor Cyan
    Write-Host "Run with: .\build\cosmic_observatory.exe`n"
    exit 0
}

# Method 2: Try with glut32
Write-Host "`nMethod 2: Trying with glut32..." -ForegroundColor Gray
$output = & g++ -o build/cosmic_observatory.exe src/main.cpp -lglut32 -lopengl32 -lglu32 -std=c++11 -O2 2>&1
if ($LASTEXITCODE -eq 0) {
    Write-Host "✅ Build successful with glut32!" -ForegroundColor Green
    Write-Host "`nExecutable: .\build\cosmic_observatory.exe" -ForegroundColor Cyan
    Write-Host "Run with: .\build\cosmic_observatory.exe`n"
    exit 0
}

# Method 3: Try with glut
Write-Host "`nMethod 3: Trying with glut..." -ForegroundColor Gray
$output = & g++ -o build/cosmic_observatory.exe src/main.cpp -lglut -lopengl32 -lglu32 -std=c++11 -O2 2>&1
if ($LASTEXITCODE -eq 0) {
    Write-Host "✅ Build successful with glut!" -ForegroundColor Green
    Write-Host "`nExecutable: .\build\cosmic_observatory.exe" -ForegroundColor Cyan
    Write-Host "Run with: .\build\cosmic_observatory.exe`n"
    exit 0
}

# If all methods fail
Write-Host "`n❌ Build failed with all methods!" -ForegroundColor Red
Write-Host "`nPossible solutions:" -ForegroundColor Yellow
Write-Host "1. Install FreeGLUT:" -ForegroundColor White
Write-Host "   pacman -S mingw-w64-x86_64-freeglut" -ForegroundColor Gray
Write-Host "`n2. Or download from:" -ForegroundColor White
Write-Host "   https://www.transmissionzero.co.uk/software/freeglut-devel/" -ForegroundColor Gray
Write-Host "`n3. Specify library path manually:" -ForegroundColor White
Write-Host "   g++ -o build/cosmic_observatory.exe src/main.cpp -L`"C:\path\to\lib`" -lfreeglut -lopengl32 -lglu32 -std=c++11 -O2" -ForegroundColor Gray
Write-Host "`n4. See COMPILATION_GUIDE.md for detailed instructions`n" -ForegroundColor White

exit 1
