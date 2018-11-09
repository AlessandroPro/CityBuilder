//
//  Building.cpp
//  CityBuilder
//
//  Created by Alessandro Profenna on 2018-11-05.
//  Copyright © 2018 Alessandro Profenna. All rights reserved.
//

#include "Building.hpp"

Building::Building():
    PrismMesh(),
    floorHeight(0.1),
    selectedSplineCP(-1),
    selectedBaseCP(-1)
    {
        initializeCpScales();
        build();
    }

Building::Building(int numEdges, float height, float rotY, float posX, float posY, Vector3D scale, float floorHeight):
    PrismMesh(numEdges, height, rotY, posX, posY, scale),
    floorHeight(floorHeight),
    selectedSplineCP(-1),
    selectedBaseCP(-1)
    {
        initializeCpScales();
        build();
    }

void Building::initializeCpScales()
{
    for(int i = 0; i < numControlPoints; i++)
    {
        cpSplineScales.push_back(1.0);
    }
    for(int i = 0; i < numBaseEdges; i++)
    {
        cpBaseScales.push_back(1.0);
    }
}


void Building::build()
{
    int numFloors = getNumFloors();
    float preFloorHeight = initialHeight/numFloors; //Floor height before vertical scaling is applied
    std::vector<std::vector<Vector3D>> floors = {};
    
    //Spline curve function used for determining the extrusion factor for each floor
    verticalSpline = createSpline();
    
    //Creates vertices for the building
    //redo with vector3D pointers
    std::vector<Vector3D> floor1Verts = {};
    for(int i = 0; i < numBaseEdges; i++)
    {
        float angle = (360.0/numBaseEdges)*i;
        if(numBaseEdges == 4)
        {
            //Better alignment for scaling rectangles/squares
            angle += 45;
        }
        float x = (sin(angle * PI / 180.0)*(initialHeight/2))*cpBaseScales.at(i);
        float z = (cos(angle * PI / 180.0)*(initialHeight/2))*cpBaseScales.at(i);
        floor1Verts.push_back(Vector3D(x, -(initialHeight/2), z));
    }
    
    //redo with vector3D pointers
    for(int i = 0; i <= numFloors; i++)
    {
        std::vector<Vector3D> nextFloorVerts = floor1Verts;
        for(int j = 0; j < numBaseEdges; j++)
        {
            nextFloorVerts.at(j).x *= (float)verticalSpline(i);
            nextFloorVerts.at(j).y = -(initialHeight/2) + (preFloorHeight*i);
            nextFloorVerts.at(j).z *= (float)verticalSpline(i);
        }
        floors.push_back(nextFloorVerts);
    }
    
    //Creates quads for the building
    quads.clear();
    for(int i = 0; i < numFloors; i++)
    {
        for(int j = 0; j < numBaseEdges; j++)
        {
            Polygon newQuad = Polygon();
            newQuad.verts.push_back(floors.at(i).at(j));
            newQuad.verts.push_back(floors.at(i + 1).at(j));
            if(j == 0)
            {
                newQuad.verts.push_back(floors.at(i + 1).at(numBaseEdges-1));
                newQuad.verts.push_back(floors.at(i).at(numBaseEdges-1));
            }
            else{
                newQuad.verts.push_back(floors.at(i + 1).at(j-1));
                newQuad.verts.push_back(floors.at(i).at(j-1));
            }
            newQuad.calculateNormal();
            quads.push_back(newQuad);
        }
    }
    
    //Creates bottom floor and roof polygons
    baseBottom.verts.clear();
    baseTop.verts.clear();
    if(floors.size() > 0)
    {
        baseBottom.verts = floors.at(0);
        baseTop.verts = floors.at(floors.size()-1);
    }
    baseBottom.isFrontFacing = false;
    baseBottom.calculateNormal();
    baseTop.calculateNormal();
}

void Building::changeNumSides(int changeNum)
{
    float diff = abs(changeNum);
    for(int i = 0; i < diff; i++)
    {
        if(changeNum < 0 && numBaseEdges > 3)
        {
            cpBaseScales.pop_back();
        }
        else if(changeNum > 0)
        {
            cpBaseScales.push_back(1.0);
        }
    }
    
    numBaseEdges += changeNum;
    if (numBaseEdges < 3)
    {
        numBaseEdges = 3;
    }
    build();
}

int Building::getNumFloors()
{
    return ceil(currentHeight/floorHeight);
}


tk::spline Building::createSpline()
{
    int numFloors = getNumFloors();
    std::vector<double> cpIndices = {};
    double cpIndexInterval = numFloors/(numControlPoints-1);
    for(int i = 0; i < numControlPoints; i++)
    {
        cpIndices.push_back(cpIndexInterval*i);
    }
    tk::spline s;
    s.set_points(cpIndices, cpSplineScales);
    return s;
}

void Building::drawSpline(float yLength)
{
    int numFloors = getNumFloors();
    float splineFloorHeight = yLength/numFloors;
    float lengthHeightRatio = yLength/initialHeight;
    
    //Draws the spline curve using each floor number as inputs
    glBegin(GL_LINE_STRIP);
    for(int i = 0; i <= numFloors; i++){
        float vX = verticalSpline(i)*lengthHeightRatio-lengthHeightRatio;
        float vY = splineFloorHeight*i;
        //std::cout << vX << ", " << vY << "\n";
        glVertex3f(vX, vY, 0);
    }
    glEnd();
    
    //Draws the control points along the spline curve
    double cpIndexInterval = numFloors/(numControlPoints-1);
    for(int i = 0; i < cpSplineScales.size(); i++){
        glPushMatrix();
        float cpX = verticalSpline(cpIndexInterval*i)*lengthHeightRatio-lengthHeightRatio;
        float cpY = splineFloorHeight*cpIndexInterval*i;
        glTranslatef(cpX, cpY, 0);
        glutSolidSphere(2.5, 20, 20);
        glPopMatrix();
    }
}

void Building::drawBase()
{
    glPushMatrix();
    glRotatef(rotationY, 0.0, 1.0, 0.0);
    for(int i = 0; i <baseBottom.verts.size(); i++)
    {
        glPushMatrix();
        float cpX = baseBottom.verts.at(i).x*scaleFactors.x;
        float cpZ = baseBottom.verts.at(i).z*scaleFactors.z;
        glTranslatef(cpX, 0.0, cpZ);
        glutSolidSphere(2.7/numBaseEdges, 20, 20);
        glPopMatrix();
    }
    glScalef(scaleFactors.x, 1.0, scaleFactors.z);
    glTranslatef(0.0, position.y, 0.0);
    //std::cout << baseBottom.verts.at(0).x << ", ";
    //std::cout << baseBottom.verts.at(0).y << "\n";
    baseBottom.draw();
    glPopMatrix();
}

void Building::changeSplineControlPoint(int cpIndex, float newX)
{
    if(cpIndex >= 0 && cpIndex < numControlPoints)
    {
        cpSplineScales.at(cpIndex) = newX;
        build();
    }
}

void Building::checkSplineControlPoint(float wvX, float wvY, float yLength)
{
    int numFloors = getNumFloors();
    float splineFloorHeight = yLength/numFloors;
    float lengthHeightRatio = yLength/initialHeight;
    
    double cpIndexInterval = numFloors/(numControlPoints-1);
    for(int i = 0; i < cpSplineScales.size(); i++)
    {
        float cpX = verticalSpline(cpIndexInterval*i)*lengthHeightRatio-lengthHeightRatio;
        float cpY = splineFloorHeight*cpIndexInterval*i;
        if(abs(cpX - wvX) < 2.5 && abs(cpY - wvY) < 2.5){
            //This is the selected control point
            selectSplineControlPoint(i);
            break;
        }
    }
}

void Building::selectSplineControlPoint(int cpIndex)
{
    selectedSplineCP = cpIndex;
}

void Building::shiftSelectedSplineControlPoint(float wvX, float yLength)
{
    float lengthHeightRatio = yLength/initialHeight;
    changeSplineControlPoint(selectedSplineCP, (wvX + lengthHeightRatio)/lengthHeightRatio);
}

void Building::checkBaseControlPoint(float wvX, float wvY)
{
    for(int i = 0; i < baseBottom.verts.size(); i++)
    {
        float cpXscaled = baseBottom.verts.at(i).x * scaleFactors.x;
        float cpZscaled = baseBottom.verts.at(i).z * scaleFactors.z;
        
        float cpX = cpXscaled*cos(rotationY) - cpZscaled*sin(rotationY);
        float cpZ = cpXscaled*sin(rotationY) + cpZscaled*cos(rotationY);
        cpZ = -cpZ;
        
        float cpRadius = 2.7/numBaseEdges;
        if(abs(cpX - wvX) < cpRadius && abs(cpZ - wvY) < cpRadius){
            //This is the selected control point
            selectBaseControlPoint(i);
            //std::cout << "SELECTED " << i << "\n";
            //break;
        }
        std::cout << wvX << ", " << wvY << " <<<\n";
        std::cout << cpX << ", " << cpZ << " ****\n";
    }
}

void Building::selectBaseControlPoint(int cpIndex)
{
    selectedBaseCP = cpIndex;
}

void Building::shiftSelectedBaseControlPoint(float wvX, float wvY)
{
    //If the user changes the number of sides of the building while a control point is selected
    //the selected control point will be reset back to -1
    if(selectedBaseCP >= cpBaseScales.size())
    {
        selectBaseControlPoint(-1);
    }
    //Else, the selected control point of the base can be moved
    else
    {
        //Distance of the mouse from the centre of the base
        float mouseDistance = sqrt(wvX * wvX + wvY * wvY);
        //Distance of control point from the centre of the base, before scaling
        float cpDistance = initialHeight/2;
        float newBaseScale = mouseDistance/cpDistance;
        changeBaseControlPoint(selectedBaseCP, newBaseScale);
    }
}

void Building::changeBaseControlPoint(int cpIndex, float newScale)
{
    if(cpIndex >= 0 && cpIndex < numBaseEdges)
    {
        cpBaseScales.at(cpIndex) = newScale;
        build();
    }
}
