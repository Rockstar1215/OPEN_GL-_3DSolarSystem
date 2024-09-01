#include <GL/gl.h>
#include <GL/glut.h>
#include <iostream>
#include <string>
#include <cmath>
#include <vector>

using namespace std;

float xRotated = 90.0, yRotated = 0.0, zRotated = 0.0;

#define FPS 1000
#define KEY_ESC 27

struct Planet {
    float radius;
    float distance;
    float rotationSpeed;
    string name;
    string description;
    float color[3];
};

Planet planets[] = {
    {4.2, -12.4, 1.5, "Sun", "4.5 Billion Year old", {1.0, 1.0, 0.0}},
    {0.6, -6.1, 1.5, "Mercury", "", {0.0, 0.5, 0.0}},
    {0.7, -4.6, 1.5, "Venus", "Earth Neighbour", {1.0, 0.7, 0.0}},
    {0.7, -2.5, 1.5, "Earth", "    5th largest planet", {0.1, 0.4, 0.8}},
    {0.5, -0.9, 1.5, "Mars", "", {0.8, 0.2, 0.1}},
    {1.0, 0.7, 1.5,"Jupiter", "", {1.0, 0.5, 0.0}},
    {1.0, 3.0, 1.5, "Saturn", "", {0.0, 0.5, 0.5}},
    {0.7, 5.0, 2.0, "Uranus", "", {0.5, 1.0, 1.0}},
    {0.7, 7.0, 2.5, "Neptune", "", {0.1, 0.0, 1.0}}
};

bool showInfoBox = false;

vector<string> splitDescription(const string& description, int lineLength) {
    vector<string> lines;
    int start = 0;
    while (start < description.length()) {
        int end = start + lineLength;
        if (end >= description.length()) {
            lines.push_back(description.substr(start));
            break;
        }
        int spacePos = description.rfind(' ', end);
        if (spacePos == string::npos || spacePos <= start) {
            spacePos = end;
        }
        lines.push_back(description.substr(start, spacePos - start));
        start = spacePos + 1;
    }
    return lines;
}

void reshapeFunc(int x, int y) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0, (GLdouble)x / (GLdouble)y, 0.5, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, x, y);
}

void drawString(float x, float y, float z, const char* string) {
    glRasterPos3f(x, y, z);
    for (const char* c = string; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
}

void Draw_Spheres() {
    glClearColor(0, 0.0, 0.0, 0); // Black background
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -15.0);

    // Draw planets
    for (const auto& planet : planets) {
        glColor3fv(planet.color);
        glPushMatrix();
        glTranslatef(planet.distance, 0.0, 0.0);
        glRotatef(60.0, 1, 0, 0);
        glRotatef(zRotated * planet.rotationSpeed, 0, 0, 1);
        glutSolidSphere(planet.radius, 50, 50);
        glPopMatrix();
    }

    // Switch to orthographic projection for text
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, 1300, 0, 800, -1, 1); // Match window size

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glColor3f(1.0, 1.0, 1.0); // White text

    for (const auto& planet : planets) {
        float xPos = 650 + (planet.distance * 50); // Adjust position based on planet distance
        float yPos = 10; // Fixed position for text below each planet

        // Draw planet name
        drawString(xPos - 10, yPos + 40, 0.0f, planet.name.c_str());

        // Draw planet description
        vector<string> descriptionLines = splitDescription(planet.description, 40);
        float lineSpacing = 20.0f;
        for (int i = 0; i < descriptionLines.size(); ++i) {
            drawString(xPos - 10, yPos - i * lineSpacing, 0.0f, descriptionLines[i].c_str());
        }
    }

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glutSwapBuffers();
}

void idleFunc() {
    zRotated += 0.1;
    glutPostRedisplay();
}

void SetCanvasSize(int width, int height) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -1, 1); // Set the screen size to given width and height.
}

void PrintableKeys(unsigned char key, int x, int y) {
    if (key == KEY_ESC) {
        exit(1); // Exit the program when escape key is pressed.
    }
}

void MouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

        showInfoBox = true;
        GLdouble modelview[16], projection[16];
        GLint viewport[4];
        GLdouble objX, objY, objZ;

        glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
        glGetDoublev(GL_PROJECTION_MATRIX, projection);
        glGetIntegerv(GL_VIEWPORT, viewport);

        for (const auto& planet : planets) {
            gluProject(planet.distance, 0.0, 0.0, modelview, projection, viewport, &objX, &objY, &objZ);
            objY = viewport[3] - objY; // Flip y coordinate

            float dx = objX - x;
            float dy = objY - y;
            float distance = sqrt(dx * dx + dy * dy);

//            cout << "Planet: " << planet.name << ", Distance: " << distance << endl;
                if (distance <= planet.radius * 20) { // Adjust factor based on viewport size

                        clickedPlanet = planet.name + ": " + planet.description;
                        showInfoBox = false;
                        cout << "Clicked Planet: " << clickedPlanet << ", Distance: " << distance << endl;
                        break;
                    }


        }
    }
}

int main(int argc, char* argv[]) {
    int width = 1300, height = 800; // Set window size
    glutInit(&argc, argv); // Initialize the graphics library
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); // Color display mode and depth buffer
    glutInitWindowPosition(50, 50); // Initial window position
    glutInitWindowSize(width, height); // Set window size
    glutCreateWindow("Computer Graphics: 3D Solar System Scene"); // Set window title
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    SetCanvasSize(width, height); // Set canvas size
    glutDisplayFunc(Draw_Spheres); // Function to call for drawing
    glutReshapeFunc(reshapeFunc);
    glutIdleFunc(idleFunc);
    glutKeyboardFunc(PrintableKeys); // Function to call for keyboard input
    glutMouseFunc(MouseClick); // Function to call for mouse clicks
    glutMainLoop();
    return 1;
}
