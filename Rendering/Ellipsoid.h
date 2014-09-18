
#ifndef ELLIPSOID_H
#define ELLIPSOID_H

#include "Object.h"
#include "QuadTreeNode.h"

class Ellipsoid : public Object
{
private:
	Vector3D radius;  // c is towards poles & is therefore y dimension
	Vector3D radius2;  // c is towards poles & is therefore y dimension

	QuadTreeNode*	rootQT;
public:
	Ellipsoid() : Object() {

		rootQT = NULL;

		radius = Vector3D(1,1,1);	// y dimension (c) is towards poles
		radius2 = Vector3D( 1, 1, 1 );
	}

	Ellipsoid( double _a, double _b, double _c ) : Object() {

		rootQT = NULL;
		if ( _a > 0 && _b > 0 && _c > 0 ) {
			radius = Vector3D(_a,_c,_b);
			radius2 = Vector3D( _a*_a, _c*_c, _b*_b );
		}
	}

	~Ellipsoid() { 
		if ( rootQT ) 
			delete rootQT;
	}

	Vector3D surfaceNormal( double lon, double lat );
	Vector3D geodeticToNormal( const Vector3D& geo );
	Vector3D surfaceTangent( const Vector3D& normal );
	Vector3D surfaceNormal( const Vector3D& surf );
	Vector3D geodeticToLocal( const Vector3D& geo );
	Vector3D surfaceToGeodedic( const Vector3D& surf );
	Vector2D geodeticToTexCoord( const Vector3D& geo );
	Vector2D surfaceToTexCoord( const Vector3D& surf );

	void generateMeshes( int qtDepth = 10.0f );

	std::vector<UINT> getIndices();
	
private:
	void generateQT( QuadTreeNode* node, int numChildren, int numSubdivisions );
	void subdividePlanarQuad( QuadTreeNode* node );
	void subdivideEquilateralTriangle( QuadTreeNode* node );
	Vector3D midpoint( const Vector3D& start, const Vector3D& end );
	Vertex midpoint( const Vertex& start, const Vertex& end );
};

#endif // ELLIPSOID_H