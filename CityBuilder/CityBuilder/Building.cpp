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
    selectedControlPoint(-1)
    {
        for(int i = 0; i < numControlPoints; i++)
        {
            cpScales.push_back(1.0);
        }
        build();
    }

Building::Building(int numEdges, float height, float rotY, float posX, float posY, Vector3D scale, float floorHeight):
    PrismMesh(numEdges, height, rotY, posX, posY, scale),
    floorHeight(floorHeight),
    selectedControlPoint(-1)
    {
        for(int i = 0; i < numControlPoints; i++)
        {
            cpScales.push_back(1.0);
        }
        build();
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
        float x = (sin(angle * PI / 180.0)*(initialHeight/2));
        float z = (cos(angle * PI / 180.0)*(initialHeight/2));
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
    s.set_points(cpIndices, cpScales);
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
    for(int i = 0; i < cpScales.size(); i++){
        glPushMatrix();
        float cpX = verticalSpline(cpIndexInterval*i)*lengthHeightRatio-lengthHeightRatio;
        float cpY = splineFloorHeight*cpIndexInterval*i;
        glTranslatef(cpX, cpY, 0);
        glutSolidSphere(2.5, 20, 20);
        glPopMatrix();
    }
}

void Building::changeSplineControlPoint(int cpIndex, float newX)
{
    if(cpIndex >= 0 && cpIndex < numControlPoints)
    {
        cpScales.at(cpIndex) = newX;
        build();
    }
}

void Building::checkSplineControlPoint(float wvX, float wvY, float yLength)
{
    int numFloors = getNumFloors();
    float splineFloorHeight = yLength/numFloors;
    float lengthHeightRatio = yLength/initialHeight;
    
    double cpIndexInterval = numFloors/(numControlPoints-1);
    for(int i = 0; i < cpScales.size(); i++)
    {
        float cpX = verticalSpline(cpIndexInterval*i)*lengthHeightRatio-lengthHeightRatio;
        float cpY = splineFloorHeight*cpIndexInterval*i;
        //std::cout << "ORIGINAL X: " << cpX << "\n";
        //std::cout << "ORIGINAL Y: " << cpY << "\n\n";
        if(abs(cpX - wvX) < 2.5 && abs(cpY - wvY) < 2.5){
            //This is the selected control point
            selectSplineControlPoint(i);
            break;
        }
    }
}

void Building::selectSplineControlPoint(int cpIndex)
{
    selectedControlPoint = cpIndex;
}

void Building::shiftSelectedSplineControlPoint(float wvX, float yLength)
{
    int numFloors = getNumFloors();
    float splineFloorHeight = yLength/numFloors;
    float lengthHeightRatio = yLength/initialHeight;
    changeSplineControlPoint(selectedControlPoint, (wvX + lengthHeightRatio)/lengthHeightRatio);
}
