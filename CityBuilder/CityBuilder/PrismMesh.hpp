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
#include "Spline.hpp"
#include <vector>
#define PI 3.14159265

class PrismMesh
{
    int numBaseEdges;
    float rotationY;
    const float initialHeight;
    float currentHeight;
    Vector3D scaleFactors;
    Vector3D position;
    const float minScaleFactor = 0.2;
    
public:
    
    Polygon baseBottom;
    Polygon baseTop;
    std::vector<Polygon> quads;
    
    PrismMesh();
    void draw();
    void build();
    void buildwithFloors();
    void changeNumSides(int changeNum);
    void rotateY(float deltaY);
    void moveAlongGround(float deltaX, float deltaY);
    void changeScaleFactors(Vector3D scaleDeltas);
};

#endif /* PrismMesh_hpp */
