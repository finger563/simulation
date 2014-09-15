
#ifndef ELLIPSOID_H
#define ELLIPSOID_H

#include "Object.h"
#include "QuadTreeNode.h"

class Ellipsoid : public Object
{
private:
	float a, b, c; // radii in the local x,z,y dimensions respectively
	XMFLOAT3 radius;  // c is towards poles & is therefore y dimension
	XMFLOAT3 radius2;  // c is towards poles & is therefore y dimension

	QuadTreeNode*	rootQT;
public:
	Ellipsoid() : Object() {

		rootQT = NULL;

		a = b = c = 1.0f;	// default to unit sphere
		radius = XMFLOAT3(a,c,b);	// y dimension (c) is towards poles
		radius2 = XMFLOAT3( a*a, c*c, b*b );
	}

	Ellipsoid( float _a, float _b, float _c ) : Object() {

		rootQT = NULL;

		if ( _a >= 0 )
			a = _a;
		if ( _b >= 0 )
			b = _b;
		if ( _c >= 0 )
			c = _c;

		radius = XMFLOAT3(a,c,b);
		radius2 = XMFLOAT3( a*a, c*c, b*b );
	}

	~Ellipsoid() { 
		if ( rootQT ) 
			delete rootQT;
	}

	XMFLOAT3 surfaceNormal( float lat, float lon );
	XMFLOAT3 geodeticToLocal( float lat, float lon, float height );

	float getA() { return a; }
	float getB() { return b; }
	float getC() { return c; }

	void generateMeshes( int qtDepth = 10.0f );

	std::vector<UINT> getIndices();
	
private:
	void generateQT( QuadTreeNode* node, int numChildren, int numSubdivisions );
	void subdividePlanarQuad( QuadTreeNode* node );
};

#endif // ELLIPSOID_H