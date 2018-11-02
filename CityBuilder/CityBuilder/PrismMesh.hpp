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
#include <vector>
#define PI 3.14159265

class PrismMesh
{
public:
    int numBaseEdges;
    const float height;
    Polygon baseBottom;
    Polygon baseTop;
    Vector3D rotation;
    std::vector<Polygon> quads;
    
    PrismMesh();
    void draw();
    void build();
};

#endif /* PrismMesh_hpp */
