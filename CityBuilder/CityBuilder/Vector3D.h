//////////////////////////////////////////////////////////////////////////////////////////
//	Module for 3D vector operations using floats.
//	Based on VECTOR3D class downloaded from: www.paulsprojects.net
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

#ifndef VECTOR3D_H
#define VECTOR3D_H
#ifdef __cplusplus
extern "C" {
#endif

typedef struct Vector3D {
	float x;
	float y;
	float z;
} Vector3D;

Vector3D NewVector3D(float x, float y, float z);
void Set(Vector3D* v, float newX, float newY, float newZ);
void LoadZero(Vector3D* v);
void LoadOne(Vector3D* v);

void CrossProduct(const Vector3D* lhs, const Vector3D* rhs, Vector3D* result);
float DotProduct(const Vector3D* lhs, const Vector3D* rhs);
void Normalize(Vector3D* lhs);
float GetLength(Vector3D* lhs);

void Add(const Vector3D* v1, const Vector3D* v2, Vector3D* result);
void Subtract(const Vector3D* v1, const Vector3D* v2, Vector3D* result);
void ScalarMul(const Vector3D* v, const float rhs, Vector3D* result);
void Negate(const Vector3D* v, Vector3D* result);

#ifdef __cplusplus
	}
#endif
#endif	//VECTOR3D_H