
#include "Ellipsoid.h"

Vector3D Ellipsoid::surfaceNormal( double lon, double lat ) {
	double cosLat = cos( lat );
	Vector3D n( 
		cosLat * cos(lon),
		sin(lat), 
		cosLat * sin(lon) );
	return n.Normalized();
}

Vector3D Ellipsoid::geodeticToNormal( const Vector3D& geo ) {
	double cosLat = cos( geo.latitude );
	Vector3D n( 
		cosLat * cos(geo.longitude),
		sin(geo.latitude), 
		cosLat * sin(geo.longitude) );
	return n.Normalized();
}

Vector3D Ellipsoid::surfaceTangent( const Vector3D& normal ) {

	XMVECTOR n = XMLoadFloat3(&XMFLOAT3(normal.x, 0, normal.z));
	XMMATRIX R = XMMatrixRotationY( 90.0f );
	XMFLOAT3 result;
	XMStoreFloat3(&result, XMVector3Transform(n,R));
	Vector3D tangent(result.x, result.y, result.z );
	return tangent.Normalized();
}

Vector3D Ellipsoid::surfaceNormal( const Vector3D& surf ) {
	Vector3D n(
		surf.x / radius2.x,
		surf.y / radius2.y,
		surf.z / radius2.z
		);
	return n.Normalized();
}

Vector3D Ellipsoid::geodeticToLocal( const Vector3D& geo ) {
	Vector3D n = geodeticToNormal( geo );
	Vector3D k( 
		radius2.x * n.x,
		radius2.y * n.y,
		radius2.z * n.z );
	double gamma = sqrt( k.x * n.x + 
						k.y * n.y + 
						k.z * n.z);
	return Vector3D( k.x / gamma + geo.height * n.x,
					 k.y / gamma + geo.height * n.y,
					 k.z / gamma + geo.height * n.z );
}

Vector3D Ellipsoid::surfaceToGeodedic( const Vector3D& surf ) {
	Vector3D n = surfaceNormal( surf );
	Vector3D ret(
		atan( n.z / n.x ),
		asin( n.y / n.Length() ),
		0
		);
	return ret;
}

Vector2D Ellipsoid::geodeticToTexCoord( const Vector3D& geo ) {
	return Vector2D( 
		geo.x / ( 2.0 * PI ),
		1.0 - (geo.y / ( PI ) + 0.5)
		);
}

Vector2D Ellipsoid::surfaceToTexCoord( const Vector3D& surf ) {
	// convert from cartesian to spherical
	// doesnt solve the discontinuity at international dateline
	double r = surf.Length();
	double phi = acos( surf.y / r );
	double theta = asin( surf.z / ( r * sin(phi) )  );

	if ( surf.x < 0 ) {
		theta += PI;
	}
	else if ( surf.z < 0 ) {
		theta = 2.0 * PI + theta;
	}

	double x, y;
	x = theta / ( 2.0 * PI );
	y = phi / PI;
	Vector2D texCoord( x, y );
	return texCoord;
}

std::vector<UINT> Ellipsoid::getIndices( int depth ) {
	std::vector<UINT> retVec;
	getQT( rootQT, depth, retVec );
	return retVec;
}

void Ellipsoid::getQT( QuadTreeNode* node, int depth, std::vector<UINT>& inds ) {
	if ( depth == 0 || node->numChildren == 0 || node->children == NULL ) {
		for (int i=0;i<node->numIndices;i++)
			inds.push_back(node->indices[i]);
	}
	else {
		for (int i=0;i<node->numChildren;i++)
			getQT(node->children[i], depth - 1, inds );
	}
}

void Ellipsoid::generateMeshes( int qtDepth ) {
	MeshData faces[6];

	const int numVerts = 12;
	Vertex verts[numVerts];

	double lat[10] = { 
		-1.0f * PI / 4.0f,
		1.0f * PI / 4.0f,
		1.0f * PI / 4.0f,
		-1.0f * PI / 4.0f,
		-1.0f * PI / 4.0f,
		1.0f * PI / 4.0f,
		1.0f * PI / 4.0f,
		-1.0f * PI / 4.0f,
		-1.0f * PI / 4.0f,
		1.0f * PI / 4.0f
	};
	double lon[10] = {
		0,
		0,
		PI / 2.0f,
		PI / 2.0f,
		PI,
		PI,
		3.0f * PI / 2.0f,
		3.0f * PI / 2.0f,
		2.0f * PI,
		2.0f * PI,
	};

	for (int i=0; i < 10; i++) {
		verts[i] = Vertex();
		verts[i].Geodetic = Vector3D( lon[i], lat[i], 0 );
		verts[i].Position = geodeticToLocal( verts[i].Geodetic );
		verts[i].Normal = geodeticToNormal( verts[i].Geodetic );
		verts[i].TexC = geodeticToTexCoord( verts[i].Geodetic );
		verts[i].TangentU = surfaceTangent( verts[i].Normal );
	}

	verts[10] = Vertex();
	verts[10].Geodetic = Vector3D( 0, PI / 2.0 , 0 );
	verts[10].Position = Vector3D( 0, radius.y, 0 );
	verts[10].Normal = Vector3D( 0, 1, 0 );
	verts[10].TexC = Vector2D( 0, 0 );
	verts[10].TangentU = Vector3D( 1, 0, 0 );;

	verts[11] = Vertex();
	verts[11].Geodetic = Vector3D( 0, - PI / 2.0 , 0 );
	verts[11].Position = Vector3D( 0, - radius.y, 0 );
	verts[11].Normal = Vector3D( 0, -1, 0 );
	verts[11].TexC = Vector2D( 0, 1 );
	verts[11].TangentU = Vector3D( -1, 0, 0 );
 
	//
	// Create the indices.
	//

	const int numIndices = 48;
	UINT ind[numIndices];

	// Fill in the front face index data
	ind[0] = 0; ind[1] = 1; ind[2] = 2;
	ind[3] = 0; ind[4] = 2; ind[5] = 3;

	faces[0].Indices.assign(&ind[0], &ind[6]);

	// Fill in the back face index data
	ind[6] = 4; ind[7]  = 5; ind[8]  = 6;
	ind[9] = 4; ind[10] = 6; ind[11] = 7;

	faces[1].Indices.assign(&ind[6], &ind[12]);

	// Fill in the left face index data
	ind[12] = 7; ind[13] = 6; ind[14] = 9;
	ind[15] = 7; ind[16] = 9; ind[17] = 8;

	faces[2].Indices.assign(&ind[12], &ind[18]);

	// Fill in the right face index data
	ind[18] = 3; ind[19] = 2; ind[20] = 5;
	ind[21] = 3; ind[22] = 5; ind[23] = 4;

	faces[3].Indices.assign(&ind[18], &ind[24]);

	// Fill in the top face index data
	ind[24] = 6; ind[25] =  10; ind[26] = 9;
	ind[27] = 5; ind[28] = 10; ind[29] = 6;
	ind[30] = 2; ind[31] =  10; ind[32] = 5;
	ind[33] = 1; ind[34] = 10; ind[35] = 2;

	faces[4].Indices.assign(&ind[24], &ind[36]);

	// Fill in the bottom face index data
	ind[36] = 7; ind[37] = 11; ind[38] = 4;
	ind[39] = 8; ind[40] = 11; ind[41] = 7;
	ind[42] = 3; ind[43] = 11; ind[44] = 0;
	ind[45] = 4; ind[46] = 11; ind[47] = 3;

	faces[5].Indices.assign(&ind[36], &ind[48]);

	if ( rootQT ) {
		delete rootQT;
	}
	rootQT = new QuadTreeNode( NULL, numIndices, 10.0f, 12);
	for (int i=0; i < numVerts; i++)
		Vertices.push_back(verts[i]);
	for (int i=0; i < rootQT->numIndices; i++)
		rootQT->indices[i] = ind[i];

	for (int i=0; i < 4; i++) {
		rootQT->children[i] = new QuadTreeNode( rootQT, 6, rootQT->error/2.0f, 4 );
		for (int n=0; n < rootQT->children[i]->numIndices; n++)
			rootQT->children[i]->indices[n] = faces[i].Indices[n];
		generateQT( rootQT->children[i], 4, qtDepth, false );
	}
	// now for top and bottom face children
	// these are not 1 face = 1 child, but instead are 4 children per face (triangles)
	for (int i=0; i< 8; i++) {
		rootQT->children[i + 4] = new QuadTreeNode( rootQT, 3, rootQT->error/2.0f, 4 );

		for (int n=0;n<3;n++)
			rootQT->children[i + 4]->indices[n] = faces[i / 4 + 4].Indices[n+ (i%4)*3];
		generateQT( rootQT->children[i + 4], 4, qtDepth, true );
	}
}

void Ellipsoid::generateQT( QuadTreeNode* node, int numChildren, int numSubdivisions, bool polarFace ) {
	int remainingSubdivisions = numSubdivisions - 1;
	if ( polarFace )
		subdivideEquilateralTriangle( node );
	else
		subdividePlanarQuad( node );
	if ( remainingSubdivisions > 0 ) {
		for (int i=0; i < numChildren; i++) {
			generateQT( node->children[i], numChildren, remainingSubdivisions, polarFace );
		}
	}
}

Vector3D Ellipsoid::midpoint( const Vector3D& start, const Vector3D& end ) {
	Vector3D mid = ( end - start ) / 2.0 + start;
	double beta = 1.0 / sqrt( 
		mid.x * mid.x / radius2.x +
		mid.y * mid.y / radius2.y +
		mid.z * mid.z / radius2.z
		);
	return mid * beta;
}

Object::Vertex Ellipsoid::midpoint( const Vertex& start, const Vertex& end ) {
	Vertex retVert;
	retVert.Geodetic = ( end.Geodetic - start.Geodetic ) / 2.0 + start.Geodetic;
	retVert.Position = geodeticToLocal( retVert.Geodetic );
	retVert.Normal = geodeticToNormal( retVert.Geodetic );
	retVert.TexC = geodeticToTexCoord( retVert.Geodetic );
	retVert.TangentU = surfaceTangent( retVert.Normal );
	return retVert;
}

void Ellipsoid::subdivideEquilateralTriangle( QuadTreeNode* node ) {
	
	node->numChildren = 4;
	for (int i=0; i < node->numChildren; i++) {
		node->children[i] = new QuadTreeNode(node, 3, node->error/2.0f, node->numChildren);
	}

	int ind[6]; // 3 original + 3 new verts (mid points)
	
	ind[0] = node->indices[0];
	ind[1] = node->indices[1];
	ind[2] = node->indices[2];
		
	Vertex newVerts[3];
		
	newVerts[0] = midpoint( Vertices[ind[1]], Vertices[ind[0]] );	// midLeft
	newVerts[1] = midpoint( Vertices[ind[2]], Vertices[ind[1]] );	// midRight
	newVerts[2] = midpoint( Vertices[ind[0]], Vertices[ind[2]] );	// midBottom

	if ( (Vertices[ind[0]].Geodetic.latitude < Vertices[ind[1]].Geodetic.latitude && Vertices[ind[0]].Geodetic.latitude > 0) || // not upside down triangle in north pole
		 (Vertices[ind[0]].Geodetic.latitude > Vertices[ind[1]].Geodetic.latitude && Vertices[ind[0]].Geodetic.latitude < 0) ) { // not upside down triangle in south pole
		if ( fmod( Vertices[ind[0]].Geodetic.longitude, PI / 2.0 ) != 0 ) {
		}
		else {
			newVerts[0].Geodetic.latitude = ( Vertices[ind[0]].Geodetic.latitude - Vertices[ind[1]].Geodetic.latitude ) / 2.0 + Vertices[ind[1]].Geodetic.latitude;
			newVerts[0].Geodetic.longitude = Vertices[ind[0]].Geodetic.longitude;
			newVerts[0].Position = geodeticToLocal( newVerts[0].Geodetic );
			newVerts[0].Normal = geodeticToNormal( newVerts[0].Geodetic );
			newVerts[0].TexC = geodeticToTexCoord( newVerts[0].Geodetic );
			newVerts[0].TangentU = surfaceTangent( newVerts[0].Normal );
		}
	
		if ( fmod( Vertices[ind[2]].Geodetic.longitude, PI / 2.0 ) != 0 ) {
		}
		else {
			newVerts[1].Geodetic.latitude = ( Vertices[ind[1]].Geodetic.latitude - Vertices[ind[2]].Geodetic.latitude ) / 2.0 + Vertices[ind[2]].Geodetic.latitude;
			newVerts[1].Geodetic.longitude = Vertices[ind[2]].Geodetic.longitude;
			newVerts[1].Position = geodeticToLocal( newVerts[1].Geodetic );
			newVerts[1].Normal = geodeticToNormal( newVerts[1].Geodetic );
			newVerts[1].TexC = geodeticToTexCoord( newVerts[1].Geodetic );
			newVerts[1].TangentU = surfaceTangent( newVerts[1].Normal );
		}
	}
	
	for (int i=0;i<3;i++) {
		Vertices.push_back( newVerts[i] );
		ind[i+3] = Vertices.size() - 1;
	}
	
	node->children[0]->indices[0] = ind[0];
	node->children[0]->indices[1] = ind[3];
	node->children[0]->indices[2] = ind[5];
	
	node->children[1]->indices[0] = ind[3];
	node->children[1]->indices[1] = ind[1];
	node->children[1]->indices[2] = ind[4];
	
	node->children[2]->indices[0] = ind[4];
	node->children[2]->indices[1] = ind[5];
	node->children[2]->indices[2] = ind[3];
	
	node->children[3]->indices[0] = ind[5];
	node->children[3]->indices[1] = ind[4];
	node->children[3]->indices[2] = ind[2];
}

void Ellipsoid::subdividePlanarQuad( QuadTreeNode* node ) {
	node->numChildren = 4;
	for (int i=0; i < node->numChildren; i++) {
		node->children[i] = new QuadTreeNode(node, 6, node->error/2.0f, node->numChildren);
	}

	int ind[9]; // 4 original + 5 new verts (mid points & center)
	
	ind[0] = node->indices[0];
	ind[1] = node->indices[1];
	ind[2] = node->indices[2];
	ind[3] = node->indices[5];
		
	Vertex newVerts[5];

	newVerts[0] = midpoint( Vertices[ind[1]], Vertices[ind[0]] );	// midLeft
	newVerts[1] = midpoint( Vertices[ind[1]], Vertices[ind[2]] );	// midTop
	newVerts[2] = midpoint( Vertices[ind[0]], Vertices[ind[3]] );	// midBottom
	newVerts[3] = midpoint( Vertices[ind[1]], Vertices[ind[3]] );	// center
	newVerts[4] = midpoint( Vertices[ind[2]], Vertices[ind[3]] );	// midRight
	
	for (int i=0;i<5;i++) {
		Vertices.push_back( newVerts[i] );
		ind[i+4] = Vertices.size() - 1;
	}
	
	node->children[0]->indices[0] = ind[0];
	node->children[0]->indices[1] = ind[4];
	node->children[0]->indices[2] = ind[7];
	node->children[0]->indices[3] = ind[0];
	node->children[0]->indices[4] = ind[7];
	node->children[0]->indices[5] = ind[6];
	
	node->children[1]->indices[0] = ind[4];
	node->children[1]->indices[1] = ind[1];
	node->children[1]->indices[2] = ind[5];
	node->children[1]->indices[3] = ind[4];
	node->children[1]->indices[4] = ind[5];
	node->children[1]->indices[5] = ind[7];
	
	node->children[2]->indices[0] = ind[7];
	node->children[2]->indices[1] = ind[5];
	node->children[2]->indices[2] = ind[2];
	node->children[2]->indices[3] = ind[7];
	node->children[2]->indices[4] = ind[2];
	node->children[2]->indices[5] = ind[8];
	
	node->children[3]->indices[0] = ind[6];
	node->children[3]->indices[1] = ind[7];
	node->children[3]->indices[2] = ind[8];
	node->children[3]->indices[3] = ind[6];
	node->children[3]->indices[4] = ind[8];
	node->children[3]->indices[5] = ind[3];
}