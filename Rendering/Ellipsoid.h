
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

	XMFLOAT3 surfaceNormal( float lon, float lat );
	XMFLOAT3 surfaceNormal( XMFLOAT3 surf );
	XMFLOAT3 geodeticToLocal( float lon, float lat, float height );
	XMFLOAT3 surfaceToGeodedic( XMFLOAT3 surf );
	XMFLOAT2 geodeticToTexCoord( XMFLOAT3 geo );
	XMFLOAT2 surfaceToTexCoord( XMFLOAT3 surf );

	void generateMeshes( int qtDepth = 10.0f );

	std::vector<UINT> getIndices();
	
private:
	void generateQT( QuadTreeNode* node, int numChildren, int numSubdivisions );
	void subdividePlanarQuad( QuadTreeNode* node );
};

#endif // ELLIPSOID_H