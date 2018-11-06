//
//  main.cpp
//  CityBuilder
//
//  Created by Alessandro Profenna on 2018-11-01.
//  Copyright © 2018 Alessandro Profenna. All rights reserved.
//

/*******************************************************************
 City Builder
 ********************************************************************/

#include <stdio.h>
#include <string.h>
#include <math.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#include <stdlib.h>
#else
#include <GL/glut.h>
#endif
#include "Vector3D.hpp"
#include "Polygon.hpp"
#include "PrismMesh.hpp"
#include "Building.hpp"
#include <iostream>
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

// Material properties
static GLfloat mat_ambient[] = { 0.3F, 0.2F, 0.05F, 1.0F };
static GLfloat mat_specular[] = { 0.4F, 0.2F, 0.4F, 1.0F };
static GLfloat mat_diffuse[] = { 0.6F, 0.9F, 0.9F, 0.0F };
static GLfloat mat_shininess[] = { 0.8F };

const int groundLength = 36;        // Default ground length
const int groundWidth = 36;         // Default ground height
static int windowWidth = 850;       // Window width in pixels
static int windowHeight = 500;      // Window height in pixels
const double cityViewportRatio = 0.8;// Window Width Ratio for the city viewport
static int currentButton;           //Current mouse button being pressed

//Boundaries of the spline viewport
static GLdouble splineViewportX;
static GLdouble splineViewportY;
static GLdouble splineViewportWidth;
static GLdouble splineViewportHeight;

//Boundaries of the spline view world
static GLdouble splineWorldLeft;
static GLdouble splineWorldRight;
static GLdouble splineWorldBottom;
static GLdouble splineWorldTop;
static const GLdouble splineWorldHeight = 120;

// Light properties
static GLfloat light_position0[] = { -6.0F, 12.0F, 0.0F, 1.0F };
static GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
static GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
static GLfloat light_ambient[] = { 0.2F, 0.2F, 0.2F, 1.0F };

static std::vector<Building*> buildings = {};
static Building* building = new Building();


// Prototypes for functions in this module
void initOpenGL(int w, int h);
void display(void);
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);
void keyboardUp(unsigned char key, int x, int y);
void functionKeys(int key, int x, int y);
void functionKeysUp(int key, int x, int y);
void mouseButtonHandler(int button, int state, int xMouse, int yMouse);
void mouseMotionHandler(int xMouse, int yMouse);
Vector3D screenToWorld2D(int x, int y, float wvRight, float wvLeft, float wvTop, float wvBottom, float vpWidth, float vpHeight);
void printControls();


int main(int argc, char **argv)
{
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(200, 30);
    glutCreateWindow("Assignment 2 - City Builder");
    
    // Initialize GL
    initOpenGL(windowWidth, windowHeight);
    
    // Register callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(functionKeys);
    glutSpecialUpFunc(functionKeysUp);
    glutKeyboardUpFunc(keyboardUp);
    glutIdleFunc(display);
    glutMouseFunc(mouseButtonHandler);
    glutMotionFunc(mouseMotionHandler);
    
    // Start event loop, never returns
    glutMainLoop();
    
    return 0;
}


// Set up OpenGL. For viewport and projection setup see reshape(). */
void initOpenGL(int w, int h)
{
    // Set up and enable lighting
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    // Other OpenGL setup
    glEnable(GL_DEPTH_TEST);   // Remove hidded surfaces
    glShadeModel(GL_SMOOTH);   // Use smooth shading, makes boundaries between polygons harder to see
    glClearColor(0.6F, 0.6F, 0.6F, 0.0F);  // Color and depth for glClear
    glClearDepth(1.0f);
    glEnable(GL_NORMALIZE);    // Renormalize normal vectors
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);   // Nicer perspective
}


// Callback, called whenever GLUT determines that the window should be redisplayed
// or glutPostRedisplay() has been called.
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //City view viewport
    // Set up viewport, projection, then change to modelview matrix mode -
    // display function will then set up camera and do modeling transforms.
    glViewport(0, 0, (GLsizei)windowWidth*cityViewportRatio, (GLsizei)windowHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLdouble)windowWidth*cityViewportRatio / windowHeight, 0.2, 40.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // Set up the camera at position (0, 6, 22) looking at the origin, up along positive y axis
    gluLookAt(0.0, 6.0, 22.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    
    //Draw ground quad
    glBegin(GL_QUADS);
    glNormal3f(0.0, 1.0, 0.0);
    glVertex3f(-groundLength/2, 0.0, -groundWidth/2);
    glVertex3f(-groundLength/2, 0.0, groundWidth/2);
    glVertex3f(groundLength/2, 0.0, groundWidth/2);
    glVertex3f(groundLength/2, 0.0, -groundWidth/2);
    glEnd();

    // Set material properties
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    
    glPushMatrix();
    for(int i = 0; i < buildings.size(); i++)
    {
        buildings.at(i)->draw();
    }
    building->draw();
    glPopMatrix();
    
    //Spline view viewport
    // Set up viewport, projection, then change to modelview matrix mode -
    // display function will then set up camera and do modeling transforms.
    glViewport((GLint)splineViewportX, (GLint)splineViewportY, (GLsizei)splineViewportWidth, (GLsizei)splineViewportHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(splineWorldLeft, splineWorldRight, splineWorldBottom, splineWorldTop);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPushMatrix();
    building->drawSpline((float)(splineWorldHeight-20.0)); //10 units of top padding, 10 units bottom padding
    glPopMatrix();
    
    glutSwapBuffers();   // Double buffering, swap buffers
}

// Callback, called at initialization and whenever user resizes the window.
void reshape(int w, int h)
{
    windowWidth = w;
    windowHeight = h;
    
    //Boundaries of the spline viewport
    splineViewportX  = windowWidth*cityViewportRatio;
    splineViewportY =  0;
    splineViewportWidth = windowWidth-splineViewportX;
    splineViewportHeight = windowHeight*0.8;
    
    //Boundaries of the spline view world
    double splineWorldWidth = splineWorldHeight*(splineViewportWidth/splineViewportHeight);
    splineWorldLeft =  -(splineWorldWidth/2.0);
    splineWorldRight = splineWorldWidth/2.0;
    splineWorldBottom = -10.0;
    splineWorldTop = splineWorldBottom + splineWorldHeight;
}

// Callback, handles input from the keyboard, non-arrow keys
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'n':
            building->rotateY(-5.0);
            break;
        case 'm':
            building->rotateY(5.0);
            break;
        case 'a':
            building->moveAlongGround(-1.0, 0);
            break;
        case 'd':
            building->moveAlongGround(1.0, 0);
            break;
        case 'w':
            building->moveAlongGround(0, -1.0);
            break;
        case 's':
            building->moveAlongGround(0, 1.0);
            break;
        case 'z':
            building->changeNumSides(1);
            break;
        case 'x':
            building->changeNumSides(-1);
            break;
        case 'g':
        {
            buildings.push_back(building);
            building = new Building();
            break;
        }
        case 'f':
            printControls();
            break;
    }
    glutPostRedisplay();   // Trigger a window redisplay
}

void keyboardUp(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'f':
            break;
        case 'b':
            break;
    }
    glutPostRedisplay();
}

// Callback, handles input from the keyboard, function and arrow keys
void functionKeys(int key, int x, int y)
{
    if (key == GLUT_KEY_DOWN){
        building->changeScaleFactors(Vector3D(0.0, -0.1, 0.0));
    }
    else if (key == GLUT_KEY_UP){
        building->changeScaleFactors(Vector3D(0.0, 0.1, 0.0));
    }
    else if (key == GLUT_KEY_LEFT){
        building->changeScaleFactors(Vector3D(-0.1, 0.0, -0.0));
    }
    else if (key == GLUT_KEY_RIGHT){
        building->changeScaleFactors(Vector3D(0.1, 0.0, 0.0));
    }
    glutPostRedisplay();   // Trigger a window redisplay
}

void functionKeysUp(int key, int x, int y)
{
    if (key == GLUT_KEY_DOWN){
    }
    else if (key == GLUT_KEY_UP){
    }
    else if (key == GLUT_KEY_LEFT){
    }
    else if (key == GLUT_KEY_RIGHT){
    }
    glutPostRedisplay();   // Trigger a window redisplay
    
}


Vector3D screenToWorld2D(int x, int y, float wvRight, float wvLeft, float wvTop, float wvBottom, float vpWidth, float vpHeight, float windowHeight)
{
    float xCamera = ((wvRight-wvLeft)/vpWidth)  * x;
    float yCamera = ((wvTop-wvBottom)/vpHeight) * (windowHeight-y);
    
    return Vector3D(xCamera + wvLeft, yCamera + wvBottom, 0);
}

void mouseButtonHandler(int button, int state, int xMouse, int yMouse)
{
    currentButton = button;
    
    if (button == GLUT_LEFT_BUTTON)
    {
        switch (state)
        {
            case GLUT_DOWN:
            {
                Vector3D worldPoint = screenToWorld2D(xMouse - splineViewportX, yMouse, splineWorldRight, splineWorldLeft,
                                                      splineWorldTop, splineWorldBottom, splineViewportWidth, splineViewportHeight, windowHeight);
                std::cout << worldPoint.x << ", " << worldPoint.y << "\n";
                building->checkSplineControlPoint(worldPoint.x, worldPoint.y, splineWorldHeight-20.0);
                break;
            }
            case GLUT_UP:
                building->selectSplineControlPoint(-1);
        }
    }
    else if (button == GLUT_MIDDLE_BUTTON)
    {
        
    }
    
    /* Schedule a call to display() */
    glutPostRedisplay();
}

void mouseMotionHandler(int xMouse, int yMouse)
{
    if (currentButton == GLUT_LEFT_BUTTON)
    {
        Vector3D worldPoint = screenToWorld2D(xMouse - splineViewportX, yMouse, splineWorldRight, splineWorldLeft,
                                              splineWorldTop, splineWorldBottom, splineViewportWidth, splineViewportHeight, windowHeight);
        building->shiftSelectedSplineControlPoint(worldPoint.x, splineWorldHeight-20.0);
    }
    
    /* Schedule a call to display() */
    glutPostRedisplay();
}

// Prints the controls to the standard output
void printControls()
{
    std::string controls = "\nHere are the controls for building the city:\n\n";
    std::cout << controls;
};
