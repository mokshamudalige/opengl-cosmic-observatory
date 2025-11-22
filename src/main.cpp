// ======================
// Cosmic Observatory Part 01
// ======================

#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <cmath>

// ----------------------
// TinyOBJLoader
// ----------------------
#define TINYOBJLOADER_IMPLEMENTATION
#include "utils/tiny_obj_loader.h"

// ----------------------
// Function forward declarations
// ----------------------
void drawBresenhamLine(int x0, int y0, int x1, int y1);
void drawMidpointCircle(int xc, int yc, int r);
void displayInfo();
void printSettings();

// ----------------------
// Camera
// ----------------------
float camX = 10, camY = 25, camZ = 60;
float camAngleY = 0.0f;

// ----------------------
// User-Customizable Parameters
// ----------------------
int numStars = 60;              // Number of background stars
int numPlanets = 5;             // Number of planets to display
bool showOrionBelt = true;      // Toggle Orion's belt
bool showConstellationLines = true;  // Toggle constellation lines
float telescopeScale = 2.5f;    // Telescope size
float telescopeRotation = 45.0f;  // Telescope Y-axis rotation
bool showOrbits = true;         // Show planetary orbits
int starBrightness = 80;        // Star brightness (0-100)

// ----------------------
// Model data
// ----------------------
std::vector<tinyobj::shape_t> shapes;
std::vector<tinyobj::material_t> materials;
tinyobj::attrib_t attrib;

// ----------------------
// Load telescope OBJ + MTL
// ----------------------
void loadTelescope() {
    std::string warn, err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err,
                                "assets/models/telescope.obj",
                                "assets/models/");
    if (!warn.empty()) std::cout << "WARN: " << warn << std::endl;
    if (!err.empty()) std::cerr << "ERR: " << err << std::endl;
    if (!ret) exit(1);
}

// ----------------------
// Draw telescope with MTL colors
// ----------------------
void drawTelescope() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    glPushMatrix();
        // Position telescope as centerpiece with user-controlled parameters
        glTranslatef(0, 5, 10);
        glRotatef(telescopeRotation, 0, 1, 0);  // User-controlled rotation
        glRotatef(-15, 1, 0, 0);  // Tilt up slightly
        glScalef(telescopeScale, telescopeScale, telescopeScale);  // User-controlled scale

        for (const auto& shape : shapes) {
            size_t index_offset = 0;
            for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) {
                int fv = shape.mesh.num_face_vertices[f];
                int matID = shape.mesh.material_ids[f];
                
                // Set material properties with MAXIMUM visibility boost
                if (matID >= 0 && matID < materials.size()) {
                    tinyobj::material_t mat = materials[matID];
                    
                    // EXTREME color amplification (5-6x) with emission
                    GLfloat ambient[] = {
                        mat.ambient[0] * 3.0f, 
                        mat.ambient[1] * 3.0f, 
                        mat.ambient[2] * 3.0f, 
                        1.0f
                    };
                    GLfloat diffuse[] = {
                        mat.diffuse[0] * 5.0f, 
                        mat.diffuse[1] * 5.0f, 
                        mat.diffuse[2] * 5.0f, 
                        1.0f
                    };
                    GLfloat specular[] = {
                        mat.specular[0] * 4.0f, 
                        mat.specular[1] * 4.0f, 
                        mat.specular[2] * 4.0f, 
                        1.0f
                    };
                    // ADD EMISSION - makes material self-luminous!
                    GLfloat emission[] = {
                        mat.diffuse[0] * 0.8f,
                        mat.diffuse[1] * 0.8f,
                        mat.diffuse[2] * 0.8f,
                        1.0f
                    };
                    GLfloat shininess = mat.shininess > 0 ? mat.shininess * 1.5f : 80.0f;
                    
                    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
                    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
                    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
                    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);  // <-- NEW!
                    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
                    
                    // Force color
                    glColor3f(diffuse[0], diffuse[1], diffuse[2]);
                } else {
                    // Default material (bright)
                    GLfloat defaultDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
                    GLfloat defaultEmission[] = {0.3f, 0.3f, 0.3f, 1.0f};
                    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, defaultDiffuse);
                    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, defaultEmission);
                    glColor3f(1.0f, 1.0f, 1.0f);
                }

                glBegin(GL_TRIANGLES);
                for (size_t v = 0; v < fv; v++) {
                    tinyobj::index_t idx = shape.mesh.indices[index_offset + v];
                    
                    // Set normal
                    if (!attrib.normals.empty() && idx.normal_index >= 0) {
                        glNormal3f(attrib.normals[3*idx.normal_index+0],
                                   attrib.normals[3*idx.normal_index+1],
                                   attrib.normals[3*idx.normal_index+2]);
                    }
                    
                    // Set vertex
                    glVertex3f(attrib.vertices[3*idx.vertex_index+0],
                               attrib.vertices[3*idx.vertex_index+1],
                               attrib.vertices[3*idx.vertex_index+2]);
                }
                glEnd();
                index_offset += fv;
            }
        }
    glPopMatrix();
}

// ----------------------
// Bresenham Line Algorithm (3D space)
// ----------------------
void drawBresenhamLine(int x0, int y0, int x1, int y1) {
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;
    int err = dx - dy;

    glBegin(GL_POINTS);
    while (true) {
        // Convert 2D to 3D by placing on Y=0 plane
        glVertex3f(x0, 0.1f, y0);
        
        if (x0 == x1 && y0 == y1) break;
        
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x0 += sx; }
        if (e2 < dx) { err += dx; y0 += sy; }
    }
    glEnd();
}

// ----------------------
// Midpoint Circle Algorithm (3D space)
// ----------------------
void drawMidpointCircle(int xc, int yc, int r) {
    int x = 0, y = r;
    int d = 1 - r;

    glBegin(GL_POINTS);
    while (x <= y) {
        // Draw 8 symmetric points on Y=0.1 plane
        glVertex3f(xc + x, 0.1f, yc + y);
        glVertex3f(xc - x, 0.1f, yc + y);
        glVertex3f(xc + x, 0.1f, yc - y);
        glVertex3f(xc - x, 0.1f, yc - y);
        glVertex3f(xc + y, 0.1f, yc + x);
        glVertex3f(xc - y, 0.1f, yc + x);
        glVertex3f(xc + y, 0.1f, yc - x);
        glVertex3f(xc - y, 0.1f, yc - x);

        if (d < 0) {
            d += 2*x + 3;
        } else {
            d += 2*(x - y) + 5;
            y--;
        }
        x++;
    }
    glEnd();
}

// ----------------------
// Draw 2D elements with all algorithms
// ----------------------
void draw2D() {
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glLineWidth(1.5f);
    glPointSize(2.0f);

    // ===================
    // 1. Basic OpenGL Lines - EXPANDED Grid Floor
    // ===================
    glColor3f(0.2f, 0.3f, 0.4f);
    glBegin(GL_LINES);
        // Larger grid: 200x200 units
        for(int i = -100; i <= 100; i += 10) {
            glVertex3f(i, 0, -100);
            glVertex3f(i, 0, 100);
            glVertex3f(-100, 0, i);
            glVertex3f(100, 0, i);
        }
    glEnd();

    // Center cross (basic lines) - extended
    glLineWidth(2.0f);
    glColor3f(0.4f, 0.5f, 0.6f);
    glBegin(GL_LINES);
        glVertex3f(-100, 0, 0);
        glVertex3f(100, 0, 0);
        glVertex3f(0, 0, -100);
        glVertex3f(0, 0, 100);
    glEnd();

    // ===================
    // 2. Bresenham's Line Algorithm - THREE CONSTELLATIONS
    // ===================
    
    glPointSize(2.5f);
    
    // ===== ORION (Center) =====
    int betelgeuse_x = -20, betelgeuse_z = 15;
    int bellatrix_x = 20, bellatrix_z = 15;
    int rigel_x = 15, rigel_z = -20;
    int saiph_x = -15, saiph_z = -20;
    int alnitak_x = -8, alnitak_z = 0;
    int alnilam_x = 0, alnilam_z = 0;
    int mintaka_x = 8, mintaka_z = 0;
    
    // ===== BIG DIPPER (Right) =====
    int dubhe_x = 50, dubhe_z = 50;
    int merak_x = 60, merak_z = 45;
    int phecda_x = 65, phecda_z = 35;
    int megrez_x = 60, megrez_z = 30;
    int alioth_x = 55, alioth_z = 25;
    int mizar_x = 50, mizar_z = 18;
    int alkaid_x = 45, alkaid_z = 10;
    
    // ===== CASSIOPEIA (Left) =====
    int cas1_x = -60, cas1_z = 50;
    int cas2_x = -55, cas2_z = 40;
    int cas3_x = -50, cas3_z = 45;
    int cas4_x = -45, cas4_z = 40;
    int cas5_x = -40, cas5_z = 50;
    
    // Draw constellation lines
    if(showConstellationLines) {
        // ORION (Yellow-gold)
        glColor3f(1.0f, 0.95f, 0.3f);
        drawBresenhamLine(betelgeuse_x, betelgeuse_z, alnitak_x, alnitak_z);
        drawBresenhamLine(bellatrix_x, bellatrix_z, mintaka_x, mintaka_z);
        drawBresenhamLine(alnitak_x, alnitak_z, saiph_x, saiph_z);
        drawBresenhamLine(mintaka_x, mintaka_z, rigel_x, rigel_z);
        drawBresenhamLine(saiph_x, saiph_z, rigel_x, rigel_z);
        
        // BIG DIPPER (Cyan)
        glColor3f(0.4f, 0.9f, 1.0f);
        drawBresenhamLine(dubhe_x, dubhe_z, merak_x, merak_z);
        drawBresenhamLine(merak_x, merak_z, phecda_x, phecda_z);
        drawBresenhamLine(phecda_x, phecda_z, megrez_x, megrez_z);
        drawBresenhamLine(megrez_x, megrez_z, dubhe_x, dubhe_z);
        drawBresenhamLine(megrez_x, megrez_z, alioth_x, alioth_z);
        drawBresenhamLine(alioth_x, alioth_z, mizar_x, mizar_z);
        drawBresenhamLine(mizar_x, mizar_z, alkaid_x, alkaid_z);
        
        // CASSIOPEIA (Magenta W-shape)
        glColor3f(1.0f, 0.4f, 0.9f);
        drawBresenhamLine(cas1_x, cas1_z, cas2_x, cas2_z);
        drawBresenhamLine(cas2_x, cas2_z, cas3_x, cas3_z);
        drawBresenhamLine(cas3_x, cas3_z, cas4_x, cas4_z);
        drawBresenhamLine(cas4_x, cas4_z, cas5_x, cas5_z);
    }
    
    // Orion's Belt - separate toggle (FIXED!)
    if(showOrionBelt) {
        glColor3f(1.0f, 1.0f, 0.5f);
        drawBresenhamLine(alnitak_x, alnitak_z, alnilam_x, alnilam_z);
        drawBresenhamLine(alnilam_x, alnilam_z, mintaka_x, mintaka_z);
    }
    
    // Draw all constellation stars
    glPointSize(7.0f);
    glBegin(GL_POINTS);
        // ORION stars
        glColor3f(1.0f, 0.4f, 0.2f);
        glVertex3f(betelgeuse_x, 0.1f, betelgeuse_z);
        glColor3f(0.7f, 0.8f, 1.0f);
        glVertex3f(bellatrix_x, 0.1f, bellatrix_z);
        glColor3f(0.9f, 0.95f, 1.0f);
        glVertex3f(alnitak_x, 0.1f, alnitak_z);
        glVertex3f(alnilam_x, 0.1f, alnilam_z);
        glVertex3f(mintaka_x, 0.1f, mintaka_z);
        glColor3f(0.8f, 0.9f, 1.0f);
        glVertex3f(rigel_x, 0.1f, rigel_z);
        glColor3f(0.75f, 0.85f, 1.0f);
        glVertex3f(saiph_x, 0.1f, saiph_z);
        
        // BIG DIPPER stars
        glColor3f(0.5f, 0.95f, 1.0f);
        glVertex3f(dubhe_x, 0.1f, dubhe_z);
        glVertex3f(merak_x, 0.1f, merak_z);
        glVertex3f(phecda_x, 0.1f, phecda_z);
        glVertex3f(megrez_x, 0.1f, megrez_z);
        glVertex3f(alioth_x, 0.1f, alioth_z);
        glVertex3f(mizar_x, 0.1f, mizar_z);
        glVertex3f(alkaid_x, 0.1f, alkaid_z);
        
        // CASSIOPEIA stars
        glColor3f(1.0f, 0.5f, 0.95f);
        glVertex3f(cas1_x, 0.1f, cas1_z);
        glVertex3f(cas2_x, 0.1f, cas2_z);
        glVertex3f(cas3_x, 0.1f, cas3_z);
        glVertex3f(cas4_x, 0.1f, cas4_z);
        glVertex3f(cas5_x, 0.1f, cas5_z);
    glEnd();
    
    // THREE Constellations: Orion (Yellow), Big Dipper (Cyan), Cassiopeia (Magenta)

    // ===================
    // 3. Midpoint Circle Algorithm - SOLAR SYSTEM MODEL
    // ===================
    // Planetary orbits centered at origin (Sun)
    
    glPointSize(2.0f);
    
    // Draw orbital paths first (user can toggle)
    if(showOrbits) {
        glColor3f(0.25f, 0.25f, 0.35f);
        drawMidpointCircle(0, 0, 15);  // Mercury orbit
        glColor3f(0.27f, 0.27f, 0.37f);
        drawMidpointCircle(0, 0, 20);  // Venus orbit
        glColor3f(0.3f, 0.3f, 0.4f);
        drawMidpointCircle(0, 0, 25);  // Earth orbit
        glColor3f(0.28f, 0.28f, 0.38f);
        drawMidpointCircle(0, 0, 32);  // Mars orbit
        glColor3f(0.32f, 0.32f, 0.42f);
        drawMidpointCircle(0, 0, 42);  // Jupiter orbit (outer planets)
    }
    
    // Draw the Sun at center
    glColor3f(1.0f, 0.9f, 0.2f); // Bright yellow
    glPointSize(8.0f);
    glBegin(GL_POINTS);
        glVertex3f(0, 0.1f, 0);
    glEnd();
    
    // Draw planets based on user selection (1-8 planets)
    glPointSize(2.5f);
    
    if(numPlanets >= 1) {
        // Mercury - small, gray (closest to sun)
        glColor3f(0.7f, 0.7f, 0.7f);
        drawMidpointCircle(15, 0, 3);
    }
    if(numPlanets >= 2) {
        // Venus - bright, yellowish
        glColor3f(0.9f, 0.85f, 0.6f);
        drawMidpointCircle(0, -20, 4);
    }
    if(numPlanets >= 3) {
        // Earth - blue marble
        glColor3f(0.2f, 0.5f, 1.0f);
        drawMidpointCircle(-25, 0, 5);
    }
    if(numPlanets >= 4) {
        // Mars - red planet
        glColor3f(1.0f, 0.4f, 0.2f);
        drawMidpointCircle(30, 8, 4);
    }
    if(numPlanets >= 5) {
        // Jupiter - largest
        glColor3f(0.85f, 0.7f, 0.5f);
        drawMidpointCircle(-20, 35, 8);
    }
    if(numPlanets >= 6) {
        // Saturn - rings (bonus)
        glColor3f(0.9f, 0.8f, 0.6f);
        drawMidpointCircle(35, -30, 7);
    }
    if(numPlanets >= 7) {
        // Uranus - ice giant
        glColor3f(0.6f, 0.8f, 0.9f);
        drawMidpointCircle(-40, -20, 5);
    }
    if(numPlanets >= 8) {
        // Neptune - deep blue
        glColor3f(0.3f, 0.4f, 0.9f);
        drawMidpointCircle(40, 30, 5);
    }
    
    // Add planet labels (in comments for presentation)
    // Inner planets: Mercury, Venus, Earth, Mars
    // Outer planet shown: Jupiter

    // ===================
    // Realistic star field with Milky Way band
    // ===================
    
    // Background stars - varying brightness (user customizable)
    glPointSize(1.5f);
    glBegin(GL_POINTS);
        for(int i = 0; i < numStars; i++) {
            float x = (rand() % 100) - 50.0f;
            float z = (rand() % 100) - 50.0f;
            float brightness = 0.6f + (rand() % 25) / 100.0f;
            glColor3f(brightness, brightness, brightness * 0.98f);
            glVertex3f(x, 0.1f, z);
        }
    glEnd();
    
    // Milky Way band - denser stars along diagonal
    glPointSize(2.0f);
    glBegin(GL_POINTS);
        for(int i = 0; i < 30; i++) {
            float t = i / 30.0f;
            float x = -45 + t * 90 + ((rand() % 20) - 10);
            float z = -45 + t * 90 + ((rand() % 20) - 10);
            float brightness = 0.75f + (rand() % 25) / 100.0f;
            glColor3f(brightness, brightness * 0.98f, brightness * 0.96f);
            glVertex3f(x, 0.1f, z);
        }
    glEnd();
    
    // Pleiades-like star cluster (Seven Sisters)
    glPointSize(3.5f);
    glBegin(GL_POINTS);
        glColor3f(0.85f, 0.9f, 1.0f); // Blue-white cluster
        glVertex3f(35, 0.1f, 30);
        glVertex3f(37, 0.1f, 33);
        glVertex3f(33, 0.1f, 32);
        glVertex3f(36, 0.1f, 28);
        glVertex3f(38, 0.1f, 31);
        glVertex3f(34, 0.1f, 29);
        glVertex3f(35, 0.1f, 34);
    glEnd();
    
    // Nebula regions - colorful gas clouds
    glPointSize(5.0f);
    glBegin(GL_POINTS);
        glColor3f(1.0f, 0.3f, 0.5f); // Pink nebula (like Rosette Nebula)
        glVertex3f(-40, 0.1f, -35);
        glColor3f(0.5f, 0.8f, 1.0f); // Blue nebula
        glVertex3f(42, 0.1f, -38);
        glColor3f(0.8f, 0.4f, 1.0f); // Purple nebula
        glVertex3f(-38, 0.1f, 40);
    glEnd();

    glEnable(GL_DEPTH_TEST);
    glLineWidth(1.0f);
    glPointSize(1.0f);
}

// ----------------------
// Keyboard camera controls + customization
// ----------------------
void keyboard(unsigned char key, int x, int y) {
    float speed = 5.0f;
    float rotSpeed = 5.0f;
    
    switch(key) {
        // Camera movement
        case 'w': case 'W':
            camZ -= speed;
            break;
        case 's': case 'S':
            camZ += speed;
            break;
        case 'a': case 'A':
            camX -= speed;
            break;
        case 'd': case 'D':
            camX += speed;
            break;
        case 'q': case 'Q':
            camAngleY -= rotSpeed;
            break;
        case 'e': case 'E':
            camAngleY += rotSpeed;
            break;
        case 'r': case 'R':
            camY += speed;
            break;
        case 'f': case 'F':
            camY -= speed;
            if(camY < 10) camY = 10;
            break;
        case ' ': // Reset camera
            camX = 10; camY = 25; camZ = 60;
            camAngleY = 0;
            std::cout << "Camera reset!\n";
            break;
            
        // User customization inputs
        case '1': // Increase stars
            numStars += 10;
            if(numStars > 150) numStars = 150;
            printSettings();
            break;
        case '2': // Decrease stars
            numStars -= 10;
            if(numStars < 10) numStars = 10;
            printSettings();
            break;
        case '3': // Increase planets
            numPlanets++;
            if(numPlanets > 8) numPlanets = 8;
            printSettings();
            break;
        case '4': // Decrease planets
            numPlanets--;
            if(numPlanets < 1) numPlanets = 1;
            printSettings();
            break;
        case '5': // Toggle constellation lines
            showConstellationLines = !showConstellationLines;
            std::cout << "Constellation lines: " << (showConstellationLines ? "ON" : "OFF") << "\n";
            break;
        case '6': // Toggle Orion's belt
            showOrionBelt = !showOrionBelt;
            std::cout << "Orion's Belt: " << (showOrionBelt ? "ON" : "OFF") << "\n";
            break;
        case '7': // Toggle orbits
            showOrbits = !showOrbits;
            std::cout << "Planetary orbits: " << (showOrbits ? "ON" : "OFF") << "\n";
            break;
        case '8': // Rotate telescope left
            telescopeRotation -= 15.0f;
            std::cout << "Telescope rotation: " << telescopeRotation << "°\n";
            break;
        case '9': // Rotate telescope right
            telescopeRotation += 15.0f;
            std::cout << "Telescope rotation: " << telescopeRotation << "°\n";
            break;
        case '+': case '=': // Increase telescope size
            telescopeScale += 0.5f;
            if(telescopeScale > 5.0f) telescopeScale = 5.0f;
            std::cout << "Telescope scale: " << telescopeScale << "x\n";
            break;
        case '-': case '_': // Decrease telescope size
            telescopeScale -= 0.5f;
            if(telescopeScale < 0.5f) telescopeScale = 0.5f;
            std::cout << "Telescope scale: " << telescopeScale << "x\n";
            break;
        case '0': // Show menu
            displayInfo();
            break;
        case 27: // ESC key
            std::cout << "\nExiting Cosmic Observatory...\n";
            exit(0);
            break;
    }
    
    glutPostRedisplay();
}

// ----------------------
// Display info text
// ----------------------
void displayInfo() {
    std::cout << "\n=== COSMIC OBSERVATORY CONTROLS ===\n";
    std::cout << "\nCAMERA MOVEMENT:\n";
    std::cout << "  W/S: Move Forward/Backward\n";
    std::cout << "  A/D: Move Left/Right\n";
    std::cout << "  R/F: Move Up/Down\n";
    std::cout << "  Q/E: Rotate View\n";
    std::cout << "  SPACE: Reset Camera\n";
    std::cout << "\nCUSTOMIZATION (Press keys to modify):\n";
    std::cout << "  1/2: Increase/Decrease Stars (" << numStars << ")\n";
    std::cout << "  3/4: Increase/Decrease Planets (" << numPlanets << ")\n";
    std::cout << "  5: Toggle Constellation Lines (" << (showConstellationLines ? "ON" : "OFF") << ")\n";
    std::cout << "  6: Toggle Orion's Belt (" << (showOrionBelt ? "ON" : "OFF") << ")\n";
    std::cout << "  7: Toggle Planetary Orbits (" << (showOrbits ? "ON" : "OFF") << ")\n";
    std::cout << "  8/9: Rotate Telescope (" << telescopeRotation << "°)\n";
    std::cout << "  +/-: Telescope Size (" << telescopeScale << "x)\n";
    std::cout << "  0: Show This Menu\n";
    std::cout << "  ESC: Exit\n";
    std::cout << "===================================\n\n";
}

// ----------------------
// Print current settings
// ----------------------
void printSettings() {
    std::cout << "\n[SETTINGS UPDATED]\n";
    std::cout << "Stars: " << numStars << " | ";
    std::cout << "Planets: " << numPlanets << " | ";
    std::cout << "Telescope: " << telescopeScale << "x @ " << telescopeRotation << "°\n";
    std::cout << "Constellation: " << (showConstellationLines ? "ON" : "OFF") << " | ";
    std::cout << "Orbits: " << (showOrbits ? "ON" : "OFF") << "\n\n";
}

// ----------------------
// Main display
// ----------------------
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    // Camera setup with rotation and better look-at point
    float lookX = camX + sin(camAngleY * 3.14159f / 180.0f) * 60;
    float lookZ = camZ - cos(camAngleY * 3.14159f / 180.0f) * 60;
    float lookY = camY - 25;  // Look slightly down to see floor and telescope
    gluLookAt(camX, camY, camZ, lookX, lookY, lookZ, 0, 1, 0);

    // Draw 2D elements first (floor, stars, planets)
    draw2D();
    
    // Draw 3D telescope model
    drawTelescope();

    glutSwapBuffers();
}

// ----------------------
// Reshape callback
// ----------------------
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (double)w / (double)h, 1.0, 1000.0);
    glMatrixMode(GL_MODELVIEW);
}

// ----------------------
// Init OpenGL
// ----------------------
void initGL() {
    // Clear color - deep space black
    glClearColor(0.0f, 0.0f, 0.05f, 1.0f);
    
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    // Lighting setup
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);  // Add second light
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    
    // Light 0 - Main overhead light (brighter)
    GLfloat light0_pos[] = {30.0f, 80.0f, 30.0f, 1.0f};
    GLfloat light0_ambient[] = {0.4f, 0.4f, 0.5f, 1.0f};
    GLfloat light0_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat light0_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    
    glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
    
    // Light 1 - Fill light from opposite side
    GLfloat light1_pos[] = {-40.0f, 50.0f, -40.0f, 1.0f};
    GLfloat light1_ambient[] = {0.2f, 0.2f, 0.3f, 1.0f};
    GLfloat light1_diffuse[] = {0.5f, 0.5f, 0.6f, 1.0f};
    GLfloat light1_specular[] = {0.3f, 0.3f, 0.4f, 1.0f};
    
    glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
    
    // Light 2 - Strong spotlight on telescope
    glEnable(GL_LIGHT2);
    GLfloat light2_pos[] = {10.0f, 30.0f, 5.0f, 1.0f};
    GLfloat light2_diffuse[] = {2.0f, 2.0f, 1.8f, 1.0f};
    GLfloat light2_specular[] = {2.5f, 2.5f, 2.0f, 1.0f};
    
    glLightfv(GL_LIGHT2, GL_POSITION, light2_pos);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diffuse);
    glLightfv(GL_LIGHT2, GL_SPECULAR, light2_specular);
    
    // Enable smooth shading
    glShadeModel(GL_SMOOTH);
    
    // Enable anti-aliasing hints
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 800.0/600.0, 1.0, 1000.0);
    glMatrixMode(GL_MODELVIEW);
}

// ----------------------
// Main
// ----------------------
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitWindowSize(1024, 768);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Cosmic Observatory Designer - Part 01");

    std::cout << "\n========================================\n";
    std::cout << "  COSMIC OBSERVATORY DESIGNER v1.0\n";
    std::cout << "  Creative Coding Assignment - Part 01\n";
    std::cout << "========================================\n";

    initGL();
    
    std::cout << "Loading 3D telescope model (91,000+ vertices)...\n";
    std::cout << "This may take a few seconds...\n";
    loadTelescope();
    std::cout << "\n✓ Telescope loaded successfully!\n";
    std::cout << "  Shapes: " << shapes.size() << " | Materials: " << materials.size() << "\n";
    std::cout << "  Vertices: " << attrib.vertices.size() / 3 << "\n";

    displayInfo();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    
    std::cout << "\nStarting Cosmic Observatory...\n\n";
    glutMainLoop();
    return 0;
}
