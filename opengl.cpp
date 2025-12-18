#include <GL/glut.h>   // GLUT, OpenGL Utility Toolkit
#include <cmath>       // Math functions

// Variable to control rotation animation
float angle = 0.0f;           // Rotation angle around Y-axis
float angleX = 0.0f;          // Rotation around X-axis (up/down)
float angleY = 0.0f;          // Rotation around Y-axis (left/right)
int lastMouseX = 0;           // Previous mouse X position
int lastMouseY = 0;           // Previous mouse Y position
bool mousePressed = false;    // True while left mouse button is held

// Draw the 3D Snowman
void drawSnowman3D() {

    // Bottom Sphere (Body) 
    glColor3f(1.0f, 1.0f, 1.0f);         // Set color to white (snow)
    glPushMatrix();                      // Save current transformation state
    glTranslatef(0.0f, -0.6f, 0.0f);     // Move sphere downward
    glutSolidSphere(0.5, 50, 50);        // Draw solid sphere (radius, slices, stacks)
    glPopMatrix();                       // Restore transformation state

    // Middle Sphere 
    glPushMatrix();                       // Save matrix state
    glTranslatef(0.0f, 0.1f, 0.0f);       // Move upward above bottom sphere
    glutSolidSphere(0.35, 50, 50);        // Draw middle sphere
    glPopMatrix();

    // Head Sphere 
    glPushMatrix();
    glTranslatef(0.0f, 0.6f, 0.0f);       // Position head on top
    glutSolidSphere(0.25, 50, 50);        // Draw head sphere
    glPopMatrix();

    // Eyes 
    glColor3f(0.0f, 0.0f, 0.0f);          // Black color for eyes

    // Left eye
    glPushMatrix();
    glTranslatef(-0.08f, 0.65f, 0.22f);   // Position left eye on head
    glutSolidSphere(0.03, 20, 20);        // Small black sphere
    glPopMatrix();

    // Right eye
    glPushMatrix();
    glTranslatef(0.08f, 0.65f, 0.22f);    // Position right eye
    glutSolidSphere(0.03, 20, 20);
    glPopMatrix();

    // Nose
    glColor3f(1.0f, 0.5f, 0.0f);          // Orange carrot color
    glPushMatrix();
    glTranslatef(0.0f, 0.60f, 0.25f);     // Place nose in front of head
    glutSolidCone(0.05, 0.30, 30, 30);    // Radius, height, smoothness
    glPopMatrix();
}

// Display Function
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear screen & depth buffer
    glLoadIdentity();                                   // Reset model-view matrix

  // Set camera position and orientation
    gluLookAt(0.0, 1.0, 3.0,   // Camera position (x, y, z)
        0.0, 0.0, 0.0,         // Look-at point
        0.0, 1.0, 0.0);        // Up direction

    glRotatef(angleX, 1.0f, 0.0f, 0.0f); // Rotate around X-axis
    glRotatef(angleY, 0.0f, 1.0f, 0.0f); // Rotate around Y-axis

    glRotatef(angle, 0.0f, 1.0f, 0.0f);  // Rotate snowman around Y-axis
    drawSnowman3D();                     // Draw the snowman

    glutSwapBuffers();                   // Swap buffers (double buffering)
}

// Animation Update
void update(int value) {
    angle += 0.5f;                       // Increase rotation angle
    if (angle > 360) angle -= 360;       // Keep angle in range

    glutPostRedisplay();                 // Request redraw
    glutTimerFunc(16, update, 0);        // Call update every ~16 ms (60 FPS)
}

// Mouse Button Handler
void mouseButton(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            mousePressed = true;  // Mouse button pressed
            lastMouseX = x;       // Store mouse X position
            lastMouseY = y;       // Store mouse Y position
        }
        else {
            mousePressed = false; // Mouse button released
        }
    }
}


// Mouse Motion Handler
void mouseMotion(int x, int y) {
    if (mousePressed) {
        angleY += (x - lastMouseX); // Horizontal drag rotates around Y
        angleX += (y - lastMouseY); // Vertical drag rotates around X


        lastMouseX = x; // Update last X
        lastMouseY = y; // Update last Y


        glutPostRedisplay(); // Redraw scene
    }
}

// Lighting Setup
void initLighting() {
    glEnable(GL_DEPTH_TEST);                               // Enable depth testing
    glEnable(GL_LIGHTING);                                 // Enable lighting calculations
    glEnable(GL_LIGHT0);                                   // Enable light source 0
	glEnable(GL_COLOR_MATERIAL);                           // Enable color tracking
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);     // Set material properties to follow glColor values

    GLfloat lightPos[] = { 2.0f, 2.0f, 2.0f, 1.0f };   // Light position
    GLfloat whiteLight[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // White light color

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);    // Set light position
    glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteLight);   // Diffuse lighting
    glLightfv(GL_LIGHT0, GL_SPECULAR, whiteLight);  // Specular lighting
}

// Window Resize Handler
void reshape(int w, int h) {
    glViewport(0, 0, w, h);              // Set viewport size
    glMatrixMode(GL_PROJECTION);         // Switch to projection matrix
    glLoadIdentity();                    // Reset projection matrix
    gluPerspective(60.0,                 // Field of view
        (float)w / h,                    // Aspect ratio
        1.0,                             // Near clipping plane
        20.0);                           // Far clipping plane
    glMatrixMode(GL_MODELVIEW);          // Back to model-view matrix
}

// Main Function
int main(int argc, char** argv) {
    glutInit(&argc, argv);                                      // Initialize GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);   // Display mode
    glutInitWindowSize(800, 600);                               // Window size
    glutCreateWindow("3D OpenGL Snowman");                      // Window title

    glClearColor(0.5f, 0.8f, 1.0f, 1.0f); // Sky-blue background color
    initLighting();                       // Initialize lighting

    glutDisplayFunc(display);             // Register display callback
    glutReshapeFunc(reshape);             // Register reshape callback
    glutTimerFunc(0, update, 0);          // Start animation timer

    glutMouseFunc(mouseButton);           // Mouse press/release
    glutMotionFunc(mouseMotion);          // Mouse dragging

    glutMainLoop();                       // Enter main loop
    return 0;                             // Program ends here
}
