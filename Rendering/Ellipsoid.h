
#ifndef ELLIPSOID_H
#define ELLIPSOID_H

#include "Object.h"
#include "QuadTreeNode.h"

class Ellipsoid : public Object
{
private:
	XMFLOAT3 radius;  // c is towards poles & is therefore y dimension
	XMFLOAT3 radius2;  // c is towards poles & is therefore y dimension

	QuadTreeNode*	rootQT;
public:
	Ellipsoid() : Object() {

		rootQT = NULL;

		radius = XMFLOAT3(1,1,1);	// y dimension (c) is towards poles
		radius2 = XMFLOAT3( 1, 1, 1 );
	}

	Ellipsoid( float _a, float _b, float _c ) : Object() {

		rootQT = NULL;
		if ( _a > 0 && _b > 0 && _c > 0 ) {
			radius = XMFLOAT3(_a,_c,_b);
			radius2 = XMFLOAT3( _a*_a, _c*_c, _b*_b );
		}
	}

	~Ellipsoid() { 
		if ( rootQT ) 
			delete rootQT;
	}

	XMFLOAT3 surfaceNormal( float lat, float lon );
	XMFLOAT3 surfaceNormal( XMFLOAT3 surface );
	XMFLOAT3 geodeticToLocal( float lat, float lon, float height );
	XMFLOAT3 surfaceToGeodedic( XMFLOAT3 pos );

	void generateMeshes( int qtDepth = 10.0f );

	std::vector<UINT> getIndices();
	
private:
	void generateQT( QuadTreeNode* node, int numChildren, int numSubdivisions );
	void subdividePlanarQuad( QuadTreeNode* node );
};

#endif // ELLIPSOID_H