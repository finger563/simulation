
#include "Ellipsoid.h"

XMFLOAT3 Ellipsoid::surfaceNormal( float lat, float lon ) {
	float cosLat = cos( lat );
	XMFLOAT3 n( 
		cosLat * cos(lon),
		sin(lat), 
		cosLat * sin(lon) );
	XMVECTOR normal = XMLoadFloat3( &n );
	XMStoreFloat3( &n, XMVector3Normalize( normal ) );
	return n;
}

XMFLOAT3 Ellipsoid::surfaceNormal( XMFLOAT3 surf ) {
	XMFLOAT3 n(
		surf.x / radius2.x,
		surf.y / radius2.y,
		surf.z / radius2.z
		);
	XMVECTOR normal = XMLoadFloat3( &n );
	XMStoreFloat3( &n, XMVector3Normalize( normal ) );
	return n;
}

XMFLOAT3 Ellipsoid::geodeticToLocal( float lon, float lat, float height ) {
	XMFLOAT3 n = surfaceNormal( lat, lon );
	XMFLOAT3 k( 
		radius2.x * n.x,
		radius2.y * n.y,
		radius2.z * n.z );
	float gamma = sqrt( k.x * n.x + 
						k.y * n.y + 
						k.z * n.z);
	return XMFLOAT3( k.x / gamma + height * n.x,
					 k.y / gamma + height * n.y,
					 k.z / gamma + height * n.z );
}

XMFLOAT3 Ellipsoid::surfaceToGeodedic( XMFLOAT3 surf ) {
	XMFLOAT3 n = surfaceNormal( surf );
	float len;
	XMStoreFloat( &len, XMVector3Length( XMLoadFloat3( &n ) ) );
	XMFLOAT3 ret(
		atan( n.z / n.x ),
		asin( n.y / len ),
		0
		);
	return ret;
}

XMFLOAT2 Ellipsoid::geodeticToTexCoord( XMFLOAT3 geo ) {
	return XMFLOAT2( 
		geo.x / ( 2.0f * PI ) + 0.5f,
		geo.y / ( 2.0f * PI )
		);
}

XMFLOAT2 Ellipsoid::surfaceToTexCoord( XMFLOAT3 surf ) {
	// convert from cartesion to spherical
	float r = sqrt( surf.x * surf.x + surf.y * surf.y + surf.z * surf.z );
	float phi = acos( surf.y / r );
	float theta = asin( surf.z / ( r * sin(phi) )  );

	if ( surf.x < 0 ) {
		theta += PI;
	}
	else if ( surf.z < 0 ) {
		theta = 2.0f * PI + theta;
	}

	float x, y;
	x = theta / ( 2.0f * PI );
	y = phi / PI;
	XMFLOAT2 texCoord( x, y );
	return texCoord;
}

std::vector<UINT> Ellipsoid::getIndices() {
	std::vector<UINT> retInd;
	for (int i=0;i<6;i++) {
		for (int j=0;j<4;j++) {
			for (int k=0;k<4;k++) {
				for (int l=0;l<4;l++) {
					for (int m=0;m<4;m++) {
						for (int n=0;n<6;n++) {
							retInd.push_back(rootQT->children[i]->children[j]->children[k]->children[l]->children[m]->indices[n]);
						}
					}
				}
			}
		}
	}
#if 0
	retInd.clear();
	for (int i=0;i<6;i++) {
		for (int n=0;n<6;n++) {
			retInd.push_back(rootQT->children[i]->indices[n]);
		}
	}
#endif
	return retInd;
}

void Ellipsoid::generateMeshes( int qtDepth ) {
	MeshData faces[6];

	Vertex verts[10];

	float lat[10] = { 
		7.0f * PI / 4.0f,
		1.0f * PI / 4.0f,
		1.0f * PI / 4.0f,
		7.0f * PI / 4.0f,
		7.0f * PI / 4.0f,
		1.0f * PI / 4.0f,
		1.0f * PI / 4.0f,
		7.0f * PI / 4.0f,
		7.0f * PI / 4.0f,
		1.0f * PI / 4.0f
	};
	float lon[10] = {
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
		verts[i].Position = geodeticToLocal( lon[i], lat[i], 0 );
		if ( abs(verts[i].Position.x) < 1.0f ) 
			verts[i].Position.x = 0;
		if ( abs(verts[i].Position.y) < 1.0f ) 
			verts[i].Position.y = 0;
		if ( abs(verts[i].Position.z) < 1.0f ) 
			verts[i].Position.z = 0;
		verts[i].Normal = surfaceNormal( lat[i], lon[i] );
		verts[i].TexC = geodeticToTexCoord( XMFLOAT3( lon[i], lat[i], 0 ) );
	}
 
	//
	// Create the indices.
	//

	UINT ind[36];

	// Fill in the front face index data
	ind[0] = 0; ind[1] = 1; ind[2] = 2;
	ind[3] = 0; ind[4] = 2; ind[5] = 3;

	faces[0].Indices.assign(&ind[0], &ind[6]);

	// Fill in the back face index data
	ind[6] = 4; ind[7]  = 5; ind[8]  = 6;
	ind[9] = 4; ind[10] = 6; ind[11] = 7;

	faces[1].Indices.assign(&ind[6], &ind[12]);

	// Fill in the top face index data
	ind[12] = 9; ind[13] =  6; ind[14] = 5;
	ind[15] = 9; ind[16] = 5; ind[17] = 2;

	faces[2].Indices.assign(&ind[12], &ind[18]);

	// Fill in the bottom face index data
	ind[18] = 7; ind[19] = 8; ind[20] = 3;
	ind[21] = 7; ind[22] = 3; ind[23] = 4;

	faces[3].Indices.assign(&ind[18], &ind[24]);

	// Fill in the left face index data
	ind[24] = 7; ind[25] = 6; ind[26] = 9;
	ind[27] = 7; ind[28] = 9; ind[29] = 8;

	faces[4].Indices.assign(&ind[24], &ind[30]);

	// Fill in the right face index data
	ind[30] = 3; ind[31] = 2; ind[32] = 5;
	ind[33] = 3; ind[34] = 5; ind[35] = 4;

	faces[5].Indices.assign(&ind[30], &ind[36]);

	if ( rootQT ) {
		delete rootQT;
	}
	rootQT = new QuadTreeNode( NULL, 36, 10.0f, 6);
	for (int i=0; i < 10; i++)
		Vertices.push_back(verts[i]);
	for (int i=0; i < rootQT->numIndices; i++)
		rootQT->indices[i] = ind[i];

	for (int i=0; i < 6; i++) {
		rootQT->children[i] = new QuadTreeNode( rootQT, 6, rootQT->error/2.0f, 4 );
		for (int n=0; n < rootQT->children[i]->numIndices; n++)
			rootQT->children[i]->indices[n] = faces[i].Indices[n];
		generateQT( rootQT->children[i], 4, qtDepth );
	}
}

void Ellipsoid::generateQT( QuadTreeNode* node, int numChildren, int numSubdivisions ) {
	int remainingSubdivisions = numSubdivisions - 1;
	for (int i=0; i < numChildren; i++) {
		node->children[i] = new QuadTreeNode(node, 6, node->error/2.0f, numChildren);
	}
	subdividePlanarQuad( node );
	if ( remainingSubdivisions > 0 ) {
		for (int i=0; i < numChildren; i++) {
			generateQT( node->children[i], numChildren, remainingSubdivisions );
		}
	}
}

void Ellipsoid::subdividePlanarQuad( QuadTreeNode* node ) {

	int ind[9]; // 4 original + 5 new verts (mid points & center)
	
	ind[0] = node->indices[0];
	ind[1] = node->indices[1];
	ind[2] = node->indices[2];
	ind[3] = node->indices[5];

	Vertex originals[4];
	for (int i=0; i<4;i++) {
		originals[i] = Vertices[ind[i]];
	}
	
	XMVECTOR r = XMLoadFloat3( &radius );
	XMVECTOR bottomLeft = XMLoadFloat3( &originals[0].Position );	// bottom left vertex
	XMVECTOR topLeft = XMLoadFloat3( &originals[1].Position );		// top left vertex
	XMVECTOR topRight = XMLoadFloat3( &originals[2].Position );		// top right vertex
	XMVECTOR bottomRight = XMLoadFloat3( &originals[3].Position );	// bottom right vertex
		
	Vertex newVerts[5];

	float longitudes[5] = { 
		originals[0].TexC.x,
		originals[0].TexC.x + (originals[3].TexC.x - originals[0].TexC.x) / 2.0f,
		originals[0].TexC.x + (originals[3].TexC.x - originals[0].TexC.x) / 2.0f,
		originals[0].TexC.x + (originals[3].TexC.x - originals[0].TexC.x) / 2.0f,
		originals[3].TexC.x
	};
	
	XMStoreFloat3(&newVerts[0].Position, XMVector3Normalize((bottomLeft - topLeft) / 2.0f + topLeft) * r);			// midLeft
	XMStoreFloat3(&newVerts[1].Position, XMVector3Normalize((topRight - topLeft) / 2.0f + topLeft) * r);			// midTop
	XMStoreFloat3(&newVerts[2].Position, XMVector3Normalize((bottomRight - bottomLeft) / 2.0f + bottomLeft) * r);	// midBottom
	XMStoreFloat3(&newVerts[3].Position, XMVector3Normalize((bottomRight - topLeft) / 2.0f + topLeft) * r);			// center
	XMStoreFloat3(&newVerts[4].Position, XMVector3Normalize((bottomRight - topRight) / 2.0f + topRight) * r);		// midRight
		
	for (int i=0;i<5;i++) {
		if ( abs(newVerts[i].Position.x) < 1.0f ) 
			newVerts[i].Position.x = 0;
		if ( abs(newVerts[i].Position.y) < 1.0f ) 
			newVerts[i].Position.y = 0;
		if ( abs(newVerts[i].Position.z) < 1.0f ) 
			newVerts[i].Position.z = 0;
		newVerts[i].Normal = surfaceNormal( newVerts[i].Position );
		newVerts[i].TexC = surfaceToTexCoord( newVerts[i].Position );
		newVerts[i].TexC.x = longitudes[i];
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