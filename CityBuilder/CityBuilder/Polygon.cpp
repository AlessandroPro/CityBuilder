//
//  Polygon.cpp
//  CityBuilder
//
//  Created by Alessandro Profenna on 2018-11-01.
//  Copyright © 2018 Alessandro Profenna. All rights reserved.
//

#include "Polygon.hpp"

Polygon::Polygon():
    normal(Vector3D()),
    isFrontFacing(true),
    verts{}{}

void Polygon::calculateNormal()
{
    if(verts.size() >= 3) {
        if(isFrontFacing)
        {
            normal = *Vector3D::crossProduct(verts[0], verts[1]);
        }
        else
        {
            normal = *Vector3D::crossProduct(verts[1], verts[0]);
        }
        normal.normalize();
    }
}

void Polygon::draw()
{
    glBegin(GL_POLYGON);
    glNormal3f(normal.x, normal.y, normal.z);
    for(int i = 0; i < verts.size(); i++){
        //std::cout << i << "\n";
        glVertex3f(verts[i].x, verts[i].y, verts[i].z);
    }
    glEnd();
}
