//////////////////////////////////////////////////////////////////////////////////////////
//	Module for 3D vector operations using floats.
//	Based on VECTOR3D C++ class downloaded from: www.paulsprojects.net
//
//	Created:	20th July 2002
//	Modified:	8th November 2002	-	Changed Constructor layout
//									-	Some speed Improvements
//									-	Corrected Lerp
//				7th January 2003	-	Added QuadraticInterpolate
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	

#include <math.h>
#include "Vector3D.h"

Vector3D NewVector3D(float x, float y, float z)
{
	Vector3D v;
	Set(&v, x, y, z);
	return v;
}

void Set(Vector3D* v, float newX, float newY, float newZ)
{
	v->x = newX;  v->y = newY;  v->z = newZ;
}

void LoadZero(Vector3D* v)
{
	v->x = v->y = v->z = 0;
}

void LoadOne(Vector3D* v)
{
	v->x = v->y = v->z = 1;
}
	
// Vector algebra

// Calculates lhs = lhs X rhs
void CrossProduct(const Vector3D* lhs, const Vector3D* rhs, Vector3D* result)
{
	Set(result, lhs->y*rhs->z - lhs->z*rhs->y, lhs->z*rhs->x - lhs->x*rhs->z, lhs->x*rhs->y - lhs->y*rhs->x);
}

// Calculates and returns lhs . rhs
float DotProduct(const Vector3D* lhs, const Vector3D* rhs)
{	return lhs->x*rhs->x + lhs->y*rhs->y + lhs->z*rhs->z;	}
	
// Normalizes 'lhs' (if possible).
void Normalize(Vector3D* lhs)
{
	const float norm = GetLength(lhs);
	if (norm > 0)
	{
		lhs->x /= norm; lhs->y /= norm; lhs->z /= norm;
	}
}

// Returns the vector length (magnitude).
float GetLength(Vector3D* lhs)
{	
	return (float)sqrt(lhs->x*lhs->x + lhs->y*lhs->y + lhs->z*lhs->z);
}

//Add, subtract etc.
void Add(const Vector3D* v1, const Vector3D* v2, Vector3D* result)
{
	Set(result, v1->x + v2->x, v1->y + v2->y, v1->z + v2->z);
}

void Subtract(const Vector3D* v1, const Vector3D* v2, Vector3D* result)
{
	Set(result, v1->x - v2->x, v1->y - v2->y, v1->z - v2->z);
}

void ScalarMul(const Vector3D* v, const float rhs, Vector3D* result)
{
	Set(result, v->x * rhs, v->y * rhs, v->z * rhs);
}

void Negate(const Vector3D* v, Vector3D* result)
{
	Set(result, -v->x, -v->y, -v->z);
}

