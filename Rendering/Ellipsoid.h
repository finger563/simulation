
#ifndef ELLIPSOID_H
#define ELLIPSOID_H

#include "Object.h"
#include "QuadTreeNode.h"

class Ellipsoid : Object
{
private:
	float a, b, c; // radii in the local x,y,z dimensions respectively

	QuadTreeNode*	rootQT;
public:
	Ellipsoid() : Object() {

		rootQT = NULL;

		a = b = c = 1.0f;	// default to unit sphere
	}

	Ellipsoid( float _a, float _b, float _c ) : Object() {

		rootQT = NULL;

		if ( _a >= 0 )
			a = _a;
		if ( _b >= 0 )
			b = _b;
		if ( _c >= 0 )
			c = _c;
	}

	~Ellipsoid() { 
		if ( rootQT ) 
			delete rootQT;
	}

	float getA() { return a; }
	float getB() { return b; }
	float getC() { return c; }

	void generateMeshes( int qtDepth = 10.0f );

private:
	void generateQT( QuadTreeNode* node, int numChildren, int numSubdivisions );
	void subdividePlanar( QuadTreeNode::MeshData mesh, int quadrant );
};

#endif // ELLIPSOID_H