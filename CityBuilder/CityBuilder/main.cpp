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
#include <iostream>
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

// Material properties
static GLfloat mat_ambient[] = { 0.3F, 0.2F, 0.05F, 1.0F };
static GLfloat mat_specular[] = { 0.4F, 0.2F, 0.4F, 1.0F };
static GLfloat mat_diffuse[] = { 0.6F, 0.9F, 0.9F, 0.0F };
static GLfloat mat_shininess[] = { 0.8F };

const int groundLength = 36;    // Default ground length
const int groundWidth = 36;    // Default ground height
const int vWidth = 850;     // Viewport width in pixels
const int vHeight = 500;    // Viewport height in pixels

// Light properties
static GLfloat light_position0[] = { -6.0F, 12.0F, 0.0F, 1.0F };
static GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
static GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
static GLfloat light_ambient[] = { 0.2F, 0.2F, 0.2F, 1.0F };

PrismMesh prism;

// Prototypes for functions in this module
void initOpenGL(int w, int h);
void display(void);
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);
void keyboardUp(unsigned char key, int x, int y);
void functionKeys(int key, int x, int y);
void functionKeysUp(int key, int x, int y);
Vector3D* ScreenToWorld(int x, int y);
void printControls();


int main(int argc, char **argv)
{
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(vWidth, vHeight);
    glutInitWindowPosition(200, 30);
    glutCreateWindow("Assignment 2 - City Builder");
    
    // Initialize GL
    initOpenGL(vWidth, vHeight);
    
    // Register callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(functionKeys);
    glutSpecialUpFunc(functionKeysUp);
    glutKeyboardUpFunc(keyboardUp);
    glutIdleFunc(display);
    
    
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
    
//    // Set up ground quad mesh
//    Vector3D origin = NewVector3D(-18.0f, 0.0f, 18.0f);
//    Vector3D dir1v = NewVector3D(1.0f, 0.0f, 0.0f);
//    Vector3D dir2v = NewVector3D(0.0f, 0.0f, -1.0f);
//    groundMesh = NewQuadMesh(meshSize);
//    InitMeshQM(&groundMesh, meshSize, origin, 36.0, 36.0, dir1v, dir2v);
//
//    Vector3D ambient = NewVector3D(0.0f, 0.05f, 0.0f);
//    Vector3D diffuse = NewVector3D(0.4f, 0.8f, 0.4f);
//    Vector3D specular = NewVector3D(0.04f, 0.04f, 0.04f);
//    SetMaterialQM(&groundMesh, ambient, diffuse, specular, 0.2);
    
}


// Callback, called whenever GLUT determines that the window should be redisplayed
// or glutPostRedisplay() has been called.
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
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
        prism.draw();
    glPopMatrix();
    
    glutSwapBuffers();   // Double buffering, swap buffers
}

// Callback, called at initialization and whenever user resizes the window.
void reshape(int w, int h)
{
    // Set up viewport, projection, then change to modelview matrix mode -
    // display function will then set up camera and do modeling transforms.
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLdouble)w / h, 0.2, 40.0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Set up the camera at position (0, 6, 22) looking at the origin, up along positive y axis
    gluLookAt(0.0, 6.0, 22.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

// Callback, handles input from the keyboard, non-arrow keys
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'n':
            prism.rotateY(-5.0);
            break;
        case 'm':
            prism.rotateY(5.0);
            break;
        case 'a':
            prism.moveAlongGround(-1.0, 0);
            break;
        case 'd':
            prism.moveAlongGround(1.0, 0);
            break;
        case 'w':
            prism.moveAlongGround(0, -1.0);
            break;
        case 's':
            prism.moveAlongGround(0, 1.0);
            break;
        case 'z':
            prism.changeNumSides(1);
            break;
        case 'x':
            prism.changeNumSides(-1);
            break;
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
        prism.changeScaleFactors(Vector3D(0.0, -0.1, 0.0));
    }
    else if (key == GLUT_KEY_UP){
        prism.changeScaleFactors(Vector3D(0.0, 0.1, 0.0));
    }
    else if (key == GLUT_KEY_LEFT){
        prism.changeScaleFactors(Vector3D(-0.1, 0.0, -0.0));
    }
    else if (key == GLUT_KEY_RIGHT){
        prism.changeScaleFactors(Vector3D(0.1, 0.0, 0.0));
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


Vector3D* ScreenToWorld(int x, int y)
{
    // you will need to finish this if you use the mouse
    return new Vector3D(0.0, 0.0, 0.0);
}

// Prints the controls to the standard output
void printControls()
{
    std::string controls = "\nHere are the controls for building the city:\n\n";
    std::cout << controls;
};
