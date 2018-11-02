//
//  PrismMesh.cpp
//  CityBuilder
//
//  Created by Alessandro Profenna on 2018-11-01.
//  Copyright © 2018 Alessandro Profenna. All rights reserved.
//

#include "PrismMesh.hpp"

PrismMesh::PrismMesh():
    numBaseEdges(8),
    rotation(Vector3D(0.0,0.0,0.0)),
    height(7.0)
    {
        build();
    }

void PrismMesh::draw()
{
    glPushMatrix();
    glRotatef(rotation.y, 0.0, 1.0, 0.0);
    glRotatef(rotation.x, 1.0, 0.0, 0.0);
    baseBottom.draw();
    baseTop.draw();
    for (auto& quad : quads)
    {
        quad.draw();
    }
    glPopMatrix();
}

void PrismMesh::build()
{
    for(int i = 0; i < numBaseEdges; i++)
    {
        float angle = (360.0/numBaseEdges)*i;
        float x = sin(angle * PI / 180.0)*(height/2);
        float z = cos(angle * PI / 180.0)*(height/2);
        baseBottom.verts.push_back(Vector3D(x, -(height/2), z));
        baseTop.verts.push_back(Vector3D(x, (height/2), z));
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
    
    std::cout << baseBottom.normal.y << "\n";
    std::cout << baseTop.normal.y << "\n";
}

