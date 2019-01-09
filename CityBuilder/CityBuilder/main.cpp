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
#include "Street.hpp"
#include "Camera.hpp"
#include <iostream>
#include <fstream>
#include <string>
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

using namespace std;

static string CityMetaDataFile = "CityMetaData.txt";

// Building and Road aterial properties
static GLfloat structure_ambient[] = { 0.3F, 0.3F, 0.3F, 1.0F };
static GLfloat structure_specular[] = { 0.3F, 0.2F, 0.3F, 1.0F };
static GLfloat structure_diffuse[] = { 0.6F, 0.9F, 0.9F, 0.0F };
static GLfloat structure_shininess[] = { 0.5F };

// Ground material properties
static GLfloat ground_ambient[] = { 0.3F, 0.2F, 0.1F, 1.0F };
static GLfloat ground_specular[] = { 0.1F, 0.1F, 0.1F, 0.1F };
static GLfloat ground_diffuse[] = { 0.3F, 0.3F, 0.4F, 1.0F };
static GLfloat ground_shininess[] = { 0.1F };

// Active Building material properties
static GLfloat activeBld_ambient[] = { 0.3F, 0.3F, 0.3F, 1.0F };
static GLfloat activeBld_specular[] = { 0.4F, 0.3F, 0.3F, 1.0F };
static GLfloat activeBld_diffuse[] = { 0.6F, 0.7F, 0.9F, 0.3F };
static GLfloat activeBld_shininess[] = { 0.1F };

const int groundLength = 36;         // Default ground length 100 meters/unit
const int groundWidth = 36;          // Default ground height 100 meters/unit
static int windowWidth = 1050;       // Window width in pixels
static int windowHeight = 700;       // Window height in pixels
const double cityViewportRatio = 0.8;// Window Width Ratio for the city viewport
static int currentButton;            //Current mouse button being pressed

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

//Boundaries of the base viewport
static GLdouble baseViewportX;
static GLdouble baseViewportY;
static GLdouble baseViewportWidth;
static GLdouble baseViewportHeight;

//Boundaries of the base view world
static GLdouble baseWorldLeft;
static GLdouble baseWorldRight;
static GLdouble baseWorldBottom;
static GLdouble baseWorldTop;
static const GLdouble baseWorldHeight = 3;

// Light properties
static GLfloat light_position0[] = { -12.0F, 15.0F, 5.0F, 1.0F };
static GLfloat light_position1[] = { 0.0F, 0.0F, 5.0F, 1.0F };
static GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
static GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
static GLfloat light_ambient[] = { 0.4F, 0.4F, 0.4F, 1.0F };

static vector<Building*> buildings;                //array of buildings
static vector<Street*> streets;                    //array of streets
static Building* building = new Building();             //current building
static Street* street = new Street();             //current street
static PrismMesh* buildingTemplate = new PrismMesh();
static Camera camera;                                   //Camera for the scene
static bool buildingMode = false;


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
void saveCity();
void loadCity(string filename);


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
    
    camera.updatePosition();
    
    //City view viewport
    // Set up viewport, projection, then change to modelview matrix mode -
    // display function will then set up camera and do modeling transforms.
    glViewport(0, 0, (GLsizei)windowWidth*cityViewportRatio, (GLsizei)windowHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLdouble)windowWidth*cityViewportRatio / windowHeight, 0.2, 400.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // Set up the camera
    gluLookAt(camera.position.x, camera.position.y, camera.position.z, camera.focus.x, camera.focus.y, camera.focus.z, 0, 1, 0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
    
    // Set material properties of the ground
    glMaterialfv(GL_FRONT, GL_AMBIENT, ground_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, ground_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, ground_diffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, ground_shininess);
    
    //Draw ground quad
    glBegin(GL_QUADS);
    glNormal3f(0.0, 1.0, 0.0);
    glVertex3f(-groundLength/2, -0.01, -groundWidth/2);
    glVertex3f(-groundLength/2, -0.01, groundWidth/2);
    glVertex3f(groundLength/2, -0.01, groundWidth/2);
    glVertex3f(groundLength/2, -0.01, -groundWidth/2);
    glEnd();

    // Set material properties of all inactive buildings
    glMaterialfv(GL_FRONT, GL_AMBIENT, structure_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, structure_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, structure_diffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, structure_shininess);
    
    //glPushMatrix();
    for(int i = 0; i < buildings.size(); i++)
    {
        buildings.at(i)->draw();
    }
    
    for(int i = 0; i < streets.size(); i++)
    {
        streets.at(i)->draw();
    }
    
    // Set material properties of the active building
    glMaterialfv(GL_FRONT, GL_AMBIENT, activeBld_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, activeBld_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, activeBld_diffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, activeBld_shininess);
    
    if(buildingMode)
    {
        building->draw();
    }
    else
    {
        street->draw();
    }

    
    //Spline view viewport
    // Set up viewport, projection, then change to modelview matrix mode -
    // display function will then set up camera and do modeling transforms.
    glViewport((GLint)splineViewportX, (GLint)splineViewportY, (GLsizei)splineViewportWidth, (GLsizei)splineViewportHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(splineWorldLeft, splineWorldRight, splineWorldBottom, splineWorldTop);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    building->drawSpline((float)(splineWorldHeight-20.0)); //10 units of top padding, 10 units bottom padding
    
    //Base view viewport
    // Set up viewport, projection, then change to modelview matrix mode -
    // display function will then set up camera and do modeling transforms.
    glViewport((GLint)baseViewportX, (GLint)baseViewportY, (GLsizei)baseViewportWidth, (GLsizei)baseViewportHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(baseWorldLeft, baseWorldRight, baseWorldBottom, baseWorldTop);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_position1);
    glPushMatrix();
    glRotatef(90, 1.0, 0, 0);
    building->drawBase();
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
    splineViewportHeight = windowHeight*0.6;
    
    //Boundaries of the spline view world
    double splineWorldWidth = splineWorldHeight*(splineViewportWidth/splineViewportHeight);
    splineWorldLeft =  -(splineWorldWidth/2.0);
    splineWorldRight = splineWorldWidth/2.0;
    splineWorldBottom = -10.0;
    splineWorldTop = splineWorldBottom + splineWorldHeight;
    
    //Boundaries of the base viewport
    baseViewportX  = windowWidth*cityViewportRatio;
    baseViewportY =  splineViewportHeight;
    baseViewportWidth = windowWidth-baseViewportX;
    baseViewportHeight = windowHeight-splineViewportHeight;
    
    //Boundaries of the base view world
    double baseWorldWidth = baseWorldHeight*(baseViewportWidth/baseViewportHeight);
    baseWorldLeft =  -(baseWorldWidth/2.0);
    baseWorldRight = baseWorldWidth/2.0;
    baseWorldBottom = -1.5;
    baseWorldTop = baseWorldBottom + baseWorldHeight;
}

// Callback, handles input from the keyboard, non-arrow keys
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'h':
        {
            camera.setAzimuthChangeRate(0.01);
            camera.controlActions[0] = true;
            break;
        }
        case 'k':
        {
            camera.setAzimuthChangeRate(-0.01);
            camera.controlActions[0] = true;
            break;
        }
        case 'u':
        {
            camera.setElevationChangeRate(1);
            camera.controlActions[1] = true;
            break;
        }
        case 'j':
        {
            camera.setElevationChangeRate(-1);
            camera.controlActions[1] = true;
            break;
        }
        case 'i':
        {
            camera.setZoomChangeRate(0.5);
            camera.controlActions[2] = true;
            break;
        }
        case 'o':
        {
            camera.setZoomChangeRate(-0.5);
            camera.controlActions[2] = true;
            break;
        }
        case 'f':
        {
            printControls();
            break;
        }
        case 'l':
        {
            saveCity();
            break;
        }
        case 'v':
        {
            loadCity(CityMetaDataFile);
            break;
        }
        case 'q':
        {
            exit(0);
            break;
        }
        case 'e':
        {
            buildingMode = !buildingMode;
        }
    }
    
            
    if(buildingMode)
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
            {
                Vector3D perpForward = Vector3D::crossProduct(Vector3D(0,1,0), camera.forward);
                building->moveAlongGround(perpForward.x, perpForward.z);
                break;
            }
            case 'd':
            {
                Vector3D perpForward = Vector3D::crossProduct(Vector3D(0,1,0), camera.forward);
                building->moveAlongGround(-perpForward.x, -perpForward.z);
                break;
            }
            case 'w':
                building->moveAlongGround(camera.forward.x, camera.forward.z);
                break;
            case 's':
                building->moveAlongGround(-camera.forward.x, -camera.forward.z);
                break;
            case 'z':
                building->changeNumSides(1);
                break;
            case 'x':
                building->changeNumSides(-1);
                break;
            case 't':
            {
                building->changeScaleFactors(Vector3D(0.0, -0.1, 0.0));
                break;
            }
            case 'y':
            {
                building->changeScaleFactors(Vector3D(0.0, 0.1, 0.0));
                break;
            }
            case 'g':
            {
                buildings.push_back(building);
                building = new Building();
                break;
            }
        }
    }
    else
    {
        switch (key)
        {
            case 'n':
                street->rotateY(-5.0);
                break;
            case 'm':
                street->rotateY(5.0);
                break;
            case 'a':
            {
                Vector3D perpForward = Vector3D::crossProduct(Vector3D(0,1,0), camera.forward);
                street->moveAlongGround(perpForward.x, perpForward.z);
                break;
            }
            case 'd':
            {
                Vector3D perpForward = Vector3D::crossProduct(Vector3D(0,1,0), camera.forward);
                street->moveAlongGround(-perpForward.x, -perpForward.z);
                break;
            }
            case 'w':
                street->moveAlongGround(camera.forward.x, camera.forward.z);
                break;
            case 's':
                street->moveAlongGround(-camera.forward.x, -camera.forward.z);
                break;
            case 'g':
            {
                streets.push_back(street);
                street = new Street();
                break;
            }
        }
    }
    camera.updatePosition();
    glutPostRedisplay();   // Trigger a window redisplay
}

void keyboardUp(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'h':
            camera.controlActions[0] = false;
            break;
        case 'k':
            camera.controlActions[0] = false;
            break;
        case 'u':
            camera.controlActions[1] = false;
            break;
        case 'j':
            camera.controlActions[1] = false;
            break;
        case 'i':
            camera.controlActions[2] = false;
            break;
        case 'o':
            camera.controlActions[2] = false;
            break;
    }
    camera.updatePosition();
    glutPostRedisplay();
}

// Callback, handles input from the keyboard, function and arrow keys
void functionKeys(int key, int x, int y)
{
    if (key == GLUT_KEY_DOWN && buildingMode){
        building->changeScaleFactors(Vector3D(0.0, 0.0, -0.1));
    }
    else if (key == GLUT_KEY_UP && buildingMode){
        building->changeScaleFactors(Vector3D(0.0, 0.0, 0.1));
    }
    if (key == GLUT_KEY_DOWN && !buildingMode){
        street->changeScaleFactors(0.0, -0.1);
    }
    else if (key == GLUT_KEY_UP && !buildingMode){
        street->changeScaleFactors(0.0, 0.1);
    }
    else if (key == GLUT_KEY_LEFT && buildingMode){
        building->changeScaleFactors(Vector3D(-0.1, 0.0, 0.0));
    }
    else if (key == GLUT_KEY_RIGHT && buildingMode){
        building->changeScaleFactors(Vector3D(0.1, 0.0, 0.0));
    }
    else if (key == GLUT_KEY_LEFT && !buildingMode){
        street->changeScaleFactors(-0.1, 0.0);
    }
    else if (key == GLUT_KEY_RIGHT && !buildingMode){
        street->changeScaleFactors(0.1, 0.0);
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
                if(xMouse >= splineViewportX && xMouse <= windowWidth && yMouse <= windowHeight)
                {
                    Vector3D splineWorldPoint = screenToWorld2D(xMouse - splineViewportX, yMouse, splineWorldRight, splineWorldLeft,
                    splineWorldTop, splineWorldBottom, splineViewportWidth, splineViewportHeight, windowHeight);
                    building->checkSplineControlPoint(splineWorldPoint.x, splineWorldPoint.y, splineWorldHeight-20.0);

                    Vector3D baseWorldPoint = screenToWorld2D(xMouse - baseViewportX, windowHeight-baseViewportHeight+yMouse, baseWorldRight,baseWorldLeft,baseWorldTop, baseWorldBottom, baseViewportWidth, baseViewportHeight, windowHeight);
                    building->checkBaseControlPoint(baseWorldPoint.x, baseWorldPoint.y);
                }
                else if(xMouse < splineViewportX && xMouse > 0 && yMouse <= windowHeight && yMouse > 0)
                {
                    camera.clickX = xMouse;
                    camera.clickY = yMouse;
                    camera.clickAndDrag = true;
                }
                break;
            }
            case GLUT_UP:
                building->selectSplineControlPoint(-1);
                building->selectBaseControlPoint(-1);
                camera.clickAndDrag = false;
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
        if(xMouse >= splineViewportX && xMouse <= windowWidth && yMouse <= windowHeight)
        {
            Vector3D splineWorldPoint = screenToWorld2D(xMouse - splineViewportX, yMouse, splineWorldRight, splineWorldLeft,
                                              splineWorldTop, splineWorldBottom, splineViewportWidth, splineViewportHeight, windowHeight);
            building->shiftSelectedSplineControlPoint(splineWorldPoint.x, splineWorldHeight-20.0);

            Vector3D baseWorldPoint = screenToWorld2D(xMouse - baseViewportX, windowHeight-baseViewportHeight+yMouse, baseWorldRight, baseWorldLeft,baseWorldTop, baseWorldBottom, baseViewportWidth, baseViewportHeight, windowHeight);
            building->shiftSelectedBaseControlPoint(baseWorldPoint.x, baseWorldPoint.y);
        }
        
        if(camera.clickAndDrag)
        {
            camera.move(xMouse, yMouse);
        }
    }
    
    /* Schedule a call to display() */
    glutPostRedisplay();
}

// Prints the controls to the standard output
void printControls()
{
    string controls = "\nHere are the controls for building the city:\n\n";
    controls += "-----------------------------------------------------\n";
    controls += "There are two Edit modes: Building Mode and Street Mode\n";
    controls += "Toggle between Edit modes:             e key\n";
    controls += "Generate new building/street:          g key\n";
    controls += "(Becomes the active building/street)\n\n";
    
    controls += "Move building/street forward:          w key\n";
    controls += "Move building/street backward:         s key\n";
    controls += "Move building/street left:             a key\n";
    controls += "Move building/street right:            d key\n\n";
    
    controls += "Increase building height:              y key\n";
    controls += "Decrease building height:              t key\n";
    controls += "Decrease building/street size in x:    left arrow key\n";
    controls += "Increase building/street size in x:    right arrow key\n";
    controls += "Increase building/street size in z:    up arrow key\n";
    controls += "Decrease building/street size in z:    left arrow key\n\n";
    
    controls += "Rotate building/street cw:             n key\n";
    controls += "Rotate building/street ccw:            m key\n";
    
    controls += "Move camera                            click and drag on world viewport\n";
    controls += "OR\n";
    controls += "Change camera azimuth cw               h key\n";
    controls += "Change camera azimuth ccw              k key\n";
    controls += "Change camera elevation up             u key\n";
    controls += "Change camera elevation down           j key\n";
    controls += "Zoom in                                i key\n";
    controls += "Zoom out                               o key\n";
    
    
    controls += "Save city data                         l key\n";
    controls += "Load city data                         v key\n\n";
    
    controls += "Increase number of building sides      z key\n";
    controls += "Decrease number of building sides      x key\n";
    
    controls += "Change base control points             click and drag points in base viewport\n";
    controls += "Change spline control points           click and drag points in spline viewport\n\n";
    
    controls += "Quit the program                       q key\n";
    controls += "-----------------------------------------------------\n";
    
    cout << controls;
};

void saveCity()
{
    ofstream myfile("CityMetaData.txt");
    if (myfile.is_open())
    {
        //save building meta data for all completed buildings
        for (auto& bld : buildings)
        {
            myfile << bld->getMetaData();
        }
        myfile << "END_BUILDING_LIST\n";
        //save street meta data for all completed streets
        for (auto& strt : streets)
        {
            myfile << strt->getMetaData();
        }
        myfile << "END_STREET_LIST";
        myfile.close();
    }
    else cout << "Unable to open file";
}

void loadCity(string filename)
{
    string line;
    string metaData;
    vector<Building*> loadedBuildings;
    vector<Street*> loadedStreets;
    ifstream myfile (filename);
    if (myfile.is_open())
    {
        while ( getline (myfile,line) )
        {
            if(!line.compare("---------"))
            {
                if(loadedBuildings.size() > 0)
                {
                    loadedBuildings.at(loadedBuildings.size() - 1)->processMetaData(metaData);
                }
                Building* bd = new Building();
                loadedBuildings.push_back(bd);
                metaData = "";
            }
            else if(!line.compare("END_BUILDING_LIST"))
            {
                if(loadedBuildings.size() > 0)
                {
                    loadedBuildings.at(loadedBuildings.size() - 1)->processMetaData(metaData);
                }
            }
            else if(!line.compare("+++++"))
            {
                if(loadedStreets.size() > 0)
                {
                    loadedStreets.at(loadedStreets.size() - 1)->processMetaData(metaData);
                }
                Street* bd = new Street();
                loadedStreets.push_back(bd);
                metaData = "";
            }
            else if(!line.compare("END_STREET_LIST"))
            {
                if(loadedStreets.size() > 0)
                {
                    loadedStreets.at(loadedStreets.size() - 1)->processMetaData(metaData);
                }
            }
            else
            {
                metaData += line + "\n";
            }
        }
        
        for (auto& bld : loadedBuildings)
        {
            buildings.push_back(bld);
        }
        for (auto& strt : loadedStreets)
        {
            streets.push_back(strt);
        }
        
        glutPostRedisplay();
        myfile.close();
    }
    else cout << "Unable to open file";
}



