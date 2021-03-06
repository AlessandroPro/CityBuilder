//
//  PrismMesh.hpp
//  CityBuilder
//
//  Created by Alessandro Profenna on 2018-11-01.
//  Copyright © 2018 Alessandro Profenna. All rights reserved.
//

#ifndef PrismMesh_hpp
#define PrismMesh_hpp

#include "Polygon.hpp"
#include "Spline.cpp"
#include <vector>
#include <iostream>
#define PI 3.14159265

class PrismMesh
{
    
protected:
    
    int numBaseEdges;
    float rotationY;
    float initialHeight;
    float currentHeight;
    Vector3D scaleFactors;
    Vector3D position;
    const float minScaleFactor = 0.05;
    
public:
    
    Polygon baseBottom;
    Polygon baseTop;
    std::vector<Polygon> quads;
    
    PrismMesh();
    PrismMesh(int numEdges, float height, float rotY, float posX, float posZ, Vector3D scale);
    void draw();
    void build();
    void changeNumSides(int changeNum);
    void rotateY(float deltaY);
    void moveAlongGround(float deltaX, float deltaY);
    void changeScaleFactors(Vector3D scaleDeltas);
};

#endif /* PrismMesh_hpp */
