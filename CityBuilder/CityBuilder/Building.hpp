//
//  Building.hpp
//  CityBuilder
//
//  Created by Alessandro Profenna on 2018-11-05.
//  Copyright © 2018 Alessandro Profenna. All rights reserved.
//

#ifndef Building_hpp
#define Building_hpp

#include <stdio.h>
#include "Polygon.hpp"
#include "PrismMesh.hpp"
#include "Spline.hpp"
#include <vector>
#include <iostream>
#define PI 3.14159265

class Building : public PrismMesh
{
    float floorHeight;
    const double numControlPoints = 6;
    std::vector<double> cpScales;
    tk::spline verticalSpline;
    int selectedControlPoint;
    
public:
    
    Building();
    Building(int numEdges, float height, float rotY, float posX, float posY, Vector3D scale, float floorHeight);
    void build();
    void changeNumSides(int changeNum);
    int getNumFloors();
    tk::spline createSpline();
    void drawSpline(float yLength);
    void changeSplineControlPoint(int cpIndex, float deltaX);
    void checkSplineControlPoint(float wvX, float wvY, float yLength);
    void selectSplineControlPoint(int cpIndex);
    void shiftSelectedSplineControlPoint(float wvX, float yLength);
};

#endif /* Building_hpp */
