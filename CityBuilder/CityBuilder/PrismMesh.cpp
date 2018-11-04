//
//  PrismMesh.cpp
//  CityBuilder
//
//  Created by Alessandro Profenna on 2018-11-01.
//  Copyright © 2018 Alessandro Profenna. All rights reserved.
//

#include "PrismMesh.hpp"

PrismMesh::PrismMesh():
    numBaseEdges(4),
    rotationY(0.0),
    initialHeight(7.0),
    currentHeight(initialHeight),
    scaleFactors(Vector3D(1.0, 1.0, 1.0)),
    position(Vector3D(0.0, initialHeight/2, 0.0))
    {
        buildwithFloors();
    }

void PrismMesh::draw()
{
    glPushMatrix();
    glTranslatef(position.x, 0.0, position.z);
    glRotatef(rotationY, 0.0, 1.0, 0.0);
    glScalef(scaleFactors.x, scaleFactors.y, scaleFactors.z);
    glTranslatef(0.0, position.y, 0.0);
    //baseBottom.draw();
    //baseTop.draw();
    for (auto& quad : quads)
    {
        quad.draw();
    }
    glPopMatrix();
}

void PrismMesh::build()
{
    baseBottom.verts.clear();
    baseTop.verts.clear();
    quads.clear();
    for(int i = 0; i < numBaseEdges; i++)
    {
        float angle = (360.0/numBaseEdges)*i;
        float x = sin(angle * PI / 180.0)*(initialHeight/2);
        float z = cos(angle * PI / 180.0)*(initialHeight/2);
        baseBottom.verts.push_back(Vector3D(x, -(initialHeight/2), z));
        baseTop.verts.push_back(Vector3D(x, (initialHeight/2), z));
    }
    
    for(int i = 0; i < numBaseEdges; i++)
    {
        Polygon newQuad = Polygon();
        newQuad.verts.push_back(baseBottom.verts.at(i));
        newQuad.verts.push_back(baseTop.verts.at(i));
        if(i == 0)
        {
            newQuad.verts.push_back(baseTop.verts.at(numBaseEdges-1));
            newQuad.verts.push_back(baseBottom.verts.at(numBaseEdges-1));
        }
        else{
            newQuad.verts.push_back(baseTop.verts.at(i-1));
            newQuad.verts.push_back(baseBottom.verts.at(i-1));
        }
        newQuad.calculateNormal();
        quads.push_back(newQuad);
    }
    baseBottom.isFrontFacing = false;
    baseBottom.calculateNormal();
    baseTop.calculateNormal();
}


void PrismMesh::buildwithFloors()
{
    float floorHeight = 1.0;
    int numFloors = ceil(initialHeight/floorHeight);
    std::vector<std::vector<Vector3D>> floors = {};
    
    //SPLINE//
    std::vector<double> cpIndices = {};
    std::vector<double> cpScales = {1.0, 1.2, 1.0, 1.2, 1.0, 1.2};
    double numControlPoints = 6;
    double cpIndexInterval = numFloors/(numControlPoints-1);
    for(int i = 0; i < numControlPoints; i++)
    {
        cpIndices.push_back(cpIndexInterval*i);
        //cpScales.push_back(1.0);
    }
    tk::spline s;
    s.set_points(cpIndices, cpScales);
    
    //END SPLINE//
    
    
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
            nextFloorVerts.at(j).x *= (float)s(i);
            nextFloorVerts.at(j).y = -(initialHeight/2) + (floorHeight*i);
            nextFloorVerts.at(j).z *= (float)s(i);
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
}


void PrismMesh::changeNumSides(int changeNum)
{
    numBaseEdges += changeNum;
    if (numBaseEdges < 3)
    {
        numBaseEdges = 3;
    }
    buildwithFloors();
}

void PrismMesh::rotateY(float deltaY)
{
    rotationY += deltaY;
}

void PrismMesh::moveAlongGround(float deltaX, float deltaZ)
{
    position.x += deltaX;
    position.z += deltaZ;
}

void PrismMesh::changeScaleFactors(Vector3D scaleDeltas)
{
    scaleFactors.x += scaleDeltas.x;
    scaleFactors.y += scaleDeltas.y;
    scaleFactors.z += scaleDeltas.z;
    
    if(scaleFactors.x < minScaleFactor)
    {
        scaleFactors.x = minScaleFactor;
    }
    
    if(scaleFactors.y < minScaleFactor)
    {
        scaleFactors.y = minScaleFactor;
    }
    
    if(scaleFactors.z < minScaleFactor)
    {
        scaleFactors.z = minScaleFactor;
    }
    
    currentHeight *= scaleFactors.y;
}




