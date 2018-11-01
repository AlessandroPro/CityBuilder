#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#include <stdlib.h>
#else
#include <GL/glut.h>
#endif

#include "QuadMesh.h"

const int minMeshSize = 1;

QuadMesh NewQuadMesh(int maxMeshSize)
{
    QuadMesh qm;        // The new quad mesh to be returned
	qm.numVertices = 0;
	qm.vertices = NULL;
	qm.numQuads = 0;
    qm.quads = NULL;
    qm.numFacesDrawn = 0;
	
	qm.maxMeshSize = maxMeshSize < minMeshSize ? minMeshSize : maxMeshSize;
	CreateMemoryQM(&qm);

	// Set up default material used for the mesh
	qm.mat_ambient[0] = 0.0;
    qm.mat_ambient[1] = 0.0;
    qm.mat_ambient[2] = 0.0;
    qm.mat_ambient[3] = 1.0;
    qm.mat_specular[0] = 0.0;
    qm.mat_specular[1] = 0.0;
    qm.mat_specular[2] = 0.0;
    qm.mat_specular[3] = 1.0;
    qm.mat_diffuse[0] = 0.75;
    qm.mat_diffuse[1] = 0.5;
    qm.mat_diffuse[2] = 0.0;
    qm.mat_diffuse[3] = 1.0;
    qm.mat_shininess[0] = 0.0;
    
    return qm;
}

void SetMaterialQM(QuadMesh* qm, Vector3D ambient, Vector3D diffuse, Vector3D specular, double shininess)
{
	qm->mat_ambient[0] = ambient.x;
    qm->mat_ambient[1] = ambient.y;
    qm->mat_ambient[2] = ambient.z;
    qm->mat_ambient[3] = 1.0;
    qm->mat_specular[0] = specular.x;
    qm->mat_specular[1] = specular.y;
    qm->mat_specular[2] = specular.z;
    qm->mat_specular[3] = 1.0;
    qm->mat_diffuse[0] = diffuse.x;
    qm->mat_diffuse[1] = diffuse.y;
    qm->mat_diffuse[2] = diffuse.z;
    qm->mat_diffuse[3] = 1.0;
    qm->mat_shininess[0] = (float)shininess;
}

// Allocate dynamic arrays.
bool CreateMemoryQM(QuadMesh* qm)
{
    const int maxVertices = (qm->maxMeshSize + 1) * (qm->maxMeshSize + 1);
	qm->vertices = malloc(sizeof(MeshVertex) * maxVertices);
	if (qm->vertices == NULL)
	{
		return false;
	}

    const int maxQuads = qm->maxMeshSize * qm->maxMeshSize;
    qm->quads = malloc(sizeof(MeshQuad) * maxQuads);
    if (qm->quads == NULL)
	{
		return false;
	}

	return true;
}
		

// Fills the array of vertices and the array of quads.
bool InitMeshQM(QuadMesh* qm, int meshSize, Vector3D origin, double meshLength, double meshWidth, Vector3D dir1, Vector3D dir2)
{
	Vector3D o;
	double sf1, sf2; 
    
	Vector3D v1,v2;
	
	v1.x = dir1.x;
	v1.y = dir1.y;
	v1.z = dir1.z;

	sf1 = meshLength/meshSize;
	ScalarMul(&v1, (float)sf1, &v1);

	v2.x = dir2.x;
	v2.y = dir2.y;
	v2.z = dir2.z;
	sf2 = meshWidth/meshSize;
	ScalarMul(&v2, (float)sf2, &v2);

	Vector3D meshpt;
	
	// Build Vertices
	qm->numVertices=(meshSize+1)*(meshSize+1);
    int currentVertex = 0;

	// Starts at front left corner of mesh 
	Set(&o, origin.x,origin.y,origin.z);

	for(int i=0; i< meshSize+1; i++)
	{
		for(int j=0; j< meshSize+1; j++)
		{
			// compute vertex position along mesh row (along x direction)
			meshpt.x = o.x + j * v1.x;
			meshpt.y = o.y + j * v1.y;
			meshpt.z = o.z + j * v1.z;
            
			Set(&qm->vertices[currentVertex].position, meshpt.x,meshpt.y,meshpt.z);
			currentVertex++;
		}
		// go to next row in mesh (negative z direction)
		Add(&o, &v2, &o);
	}
	
	// Build Quad Polygons
	qm->numQuads=(meshSize)*(meshSize);
	int currentQuad=0;

	for (int j=0; j < meshSize; j++)
	{
		for (int k=0; k < meshSize; k++)
		{
			// Counterclockwise order
            qm->quads[currentQuad].vertices[0]=&qm->vertices[j*    (meshSize+1)+k];
            qm->quads[currentQuad].vertices[1]=&qm->vertices[j*    (meshSize+1)+k+1];
            qm->quads[currentQuad].vertices[2]=&qm->vertices[(j+1)*(meshSize+1)+k+1];
            qm->quads[currentQuad].vertices[3]=&qm->vertices[(j+1)*(meshSize+1)+k];
			currentQuad++;
		}
	}

    ComputeNormalsQM(qm);

	return true;
}

// Draw the mesh by drawing all quads.
void DrawMeshQM(QuadMesh* qm, int meshSize)
{
	int currentQuad=0;

	glMaterialfv(GL_FRONT, GL_AMBIENT, qm->mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, qm->mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, qm->mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, qm->mat_shininess);

	for(int j=0; j < meshSize; j++)
	{
		for(int k=0; k < meshSize; k++)
		{
			glBegin(GL_QUADS);
			
			glNormal3f(qm->quads[currentQuad].vertices[0]->normal.x,
				       qm->quads[currentQuad].vertices[0]->normal.y,
					   qm->quads[currentQuad].vertices[0]->normal.z);
			glVertex3f(qm->quads[currentQuad].vertices[0]->position.x,
				       qm->quads[currentQuad].vertices[0]->position.y,
					   qm->quads[currentQuad].vertices[0]->position.z);
			
			glNormal3f(qm->quads[currentQuad].vertices[1]->normal.x,
				       qm->quads[currentQuad].vertices[1]->normal.y,
					   qm->quads[currentQuad].vertices[1]->normal.z);			
			glVertex3f(qm->quads[currentQuad].vertices[1]->position.x,
				       qm->quads[currentQuad].vertices[1]->position.y,
					   qm->quads[currentQuad].vertices[1]->position.z);
			
			glNormal3f(qm->quads[currentQuad].vertices[2]->normal.x,
				       qm->quads[currentQuad].vertices[2]->normal.y,
					   qm->quads[currentQuad].vertices[2]->normal.z);			
			glVertex3f(qm->quads[currentQuad].vertices[2]->position.x,
				       qm->quads[currentQuad].vertices[2]->position.y,
					   qm->quads[currentQuad].vertices[2]->position.z);
			
			glNormal3f(qm->quads[currentQuad].vertices[3]->normal.x,
				       qm->quads[currentQuad].vertices[3]->normal.y,
					   qm->quads[currentQuad].vertices[3]->normal.z);			
			glVertex3f(qm->quads[currentQuad].vertices[3]->position.x,
				       qm->quads[currentQuad].vertices[3]->position.y,
					   qm->quads[currentQuad].vertices[3]->position.z);

			glEnd();
			currentQuad++;
		}
	}
}

// Deallocate dynamic arrays.
void FreeMemoryQM(QuadMesh* qm)
{
	if (qm->vertices != NULL)
		free(qm->vertices);
    qm->vertices=NULL;
    qm->numVertices=0;

	if (qm->quads != NULL)
		free(qm->quads);
    qm->quads=NULL;
    qm->numQuads=0;
}

// Use cross-products to compute the normal vector at each vertex
void ComputeNormalsQM(QuadMesh* qm)
{
	int currentQuad=0;

	for(int j=0; j < qm->maxMeshSize; j++)
	{
		for(int k=0; k < qm->maxMeshSize; k++)
		{
            Vector3D n0, n1, n2, n3;
            Vector3D e0, e1, e2, e3;
			
			for (int i=0; i < 4; i++)
			{ 
				LoadZero(&qm->quads[currentQuad].vertices[i]->normal);
			}

			Subtract(&qm->quads[currentQuad].vertices[1]->position, &qm->quads[currentQuad].vertices[0]->position, &e0);
			Subtract(&qm->quads[currentQuad].vertices[2]->position, &qm->quads[currentQuad].vertices[1]->position, &e1);
			Subtract(&qm->quads[currentQuad].vertices[3]->position, &qm->quads[currentQuad].vertices[2]->position, &e2);
			Subtract(&qm->quads[currentQuad].vertices[0]->position, &qm->quads[currentQuad].vertices[3]->position, &e3);
			Normalize(&e0);
			Normalize(&e1);
			Normalize(&e2);
			Normalize(&e3);

			Vector3D w;    // Working vector;

			Negate(&e3, &w);
			CrossProduct(&e0, &w, &n0);
			Normalize(&n0);
			Add(&qm->quads[currentQuad].vertices[0]->normal, &n0, &qm->quads[currentQuad].vertices[0]->normal);
			
			Negate(&e0, &w);
			CrossProduct(&e1, &w, &n1);
			Normalize(&n1);
			Add(&qm->quads[currentQuad].vertices[1]->normal, &n1, &qm->quads[currentQuad].vertices[1]->normal);

			Negate(&e1, &w);
			CrossProduct(&e2, &w, &n2);
			Normalize(&n2);
			Add(&qm->quads[currentQuad].vertices[2]->normal, &n2, &qm->quads[currentQuad].vertices[2]->normal);

			Negate(&e2, &w);
			CrossProduct(&e3, &w, &n3);
			Normalize(&n3);
			Add(&qm->quads[currentQuad].vertices[3]->normal, &n3, &qm->quads[currentQuad].vertices[3]->normal);
			
			for (int i = 0; i < 4; i++)
			{
				Normalize(&qm->quads[currentQuad].vertices[i]->normal);
			}
			
			currentQuad++;
		}
	}
}
