
#include "Ellipsoid.h"

XMFLOAT3 Ellipsoid::surfaceNormal( float lat, float lon ) {
	float cosLat = cos( lat );
	return XMFLOAT3 ( cosLat * cos(lon),
					  sin(lat), 
					  cosLat * sin(lon) );
}

XMFLOAT3 Ellipsoid::geodeticToLocal( float lat, float lon, float height ) {
	XMFLOAT3 n = surfaceNormal( lat, lon );
	XMFLOAT3 k( radius2.x * n.x,
				radius2.y * n.y,
				radius2.z * n.z );
	float gamma = sqrt( k.x * n.x + 
						k.y * n.y + 
						k.z * n.z);
	return XMFLOAT3( k.x / gamma + height * n.x,
					 k.y / gamma + height * n.y,
					 k.z / gamma + height * n.z );
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
#if 1
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

	XMFLOAT3 radius(a,b,c);
	XMFLOAT3 oneOverR2(1.0f/(a), 1.0f/(b), 1.0f/(c));
	XMVECTOR r = XMLoadFloat3( &radius );
	XMVECTOR overR2 = XMLoadFloat3( &oneOverR2 );
	XMFLOAT3 tmp;
	float length;
	XMVECTOR snorm;
	XMVECTOR pos;
		    
	// Fill in the front face vertex data.
	// 0
	verts[0] = Vertex(-a, -c, -b, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	verts[0].Position = geodeticToLocal( 7.0f * PI / 4.0f, 
										 0, 
										 0 );
	verts[0].Normal = surfaceNormal( 7.0f * PI / 4.0f, 
									 0 );
	verts[0].TexC.x = 0;
	verts[0].TexC.y = 7.0f / 8.0f;
	
	// 1
	verts[1] = Vertex(-a, +c, -b, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	verts[1].Position = geodeticToLocal( 1.0f * PI / 4.0f, 
										 0, 
										 0 );
	verts[1].Normal = surfaceNormal( 1.0f * PI / 4.0f, 
									 0 );
	verts[1].TexC.x = 0;
	verts[1].TexC.y = 1.0f / 8.0f;
	
	// 2
	verts[2] = Vertex(+a, +c, -b, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	verts[2].Position = geodeticToLocal( 1.0f * PI / 4.0f,  
										 PI / 2.0f, 
										 0 );
	verts[2].Normal = surfaceNormal( 1.0f * PI / 4.0f, 
									 PI / 2.0f );
	verts[2].TexC.x = 1.0f / 4.0f;
	verts[2].TexC.y = 1.0f / 8.0f;
	
	// 3
	verts[3] = Vertex(+a, -c, -b, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	verts[3].Position = geodeticToLocal( 7.0f * PI / 4.0f,  
										 PI / 2.0f, 
										 0 );
	verts[3].Normal = surfaceNormal( 7.0f * PI / 4.0f, 
									 PI / 2.0f );
	verts[3].TexC.x = 1.0f / 4.0f;
	verts[3].TexC.y = 7.0f / 8.0f;

	// Fill in the back face vertex data.
	// 4
	verts[4] = Vertex(+a, -c, +b, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	verts[4].Position = geodeticToLocal( 7.0f * PI / 4.0f,  
										 PI , 
										 0 );
	verts[4].Normal = surfaceNormal( 7.0f * PI / 4.0f, 
									 PI );
	verts[4].TexC.x = 1.0f / 2.0f;
	verts[4].TexC.y = 7.0f / 8.0f;
	
	// 5
	verts[5] = Vertex(+a, +c, +b, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	verts[5].Position = geodeticToLocal( 1.0f * PI / 4.0f,  
										 PI , 
										 0 );
	verts[5].Normal = surfaceNormal( 1.0f * PI / 4.0f, 
									 PI );
	verts[5].TexC.x = 1.0f / 2.0f;
	verts[5].TexC.y = 1.0f / 8.0f;
	
	// 6
	verts[6] = Vertex(-a, +c, +b, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	verts[6].Position = geodeticToLocal( 1.0f * PI / 4.0f,  
										 3.0f * PI / 2.0f, 
										 0 );
	verts[6].Normal = surfaceNormal( 1.0f * PI / 4.0f, 
									 3.0f * PI / 2.0f );
	verts[6].TexC.x = 3.0f / 4.0f;
	verts[6].TexC.y = 1.0f / 8.0f;
	
	// 7
	verts[7] = Vertex(-a, -c, +b, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	verts[7].Position = geodeticToLocal( 7.0f * PI / 4.0f,  
										 3.0f * PI / 2.0f, 
										 0 );
	verts[7].Normal = surfaceNormal( 7.0f * PI / 4.0f, 
									 3.0f * PI / 2.0f );
	verts[7].TexC.x = 3.0f / 4.0f;
	verts[7].TexC.y = 7.0f / 8.0f;

	
	// 8 = 0
	verts[8] = Vertex(-a, -c, -b, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	verts[8].Position = geodeticToLocal( 7.0f * PI / 4.0f, 
										 0, 
										 0 );
	verts[8].Normal = surfaceNormal( 7.0f * PI / 4.0f, 
									 0 );
	verts[8].TexC.x = 1.0f;
	verts[8].TexC.y = 7.0f / 8.0f;
	
	// 9 = 1
	verts[9] = Vertex(-a, +c, -b, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	verts[9].Position = geodeticToLocal( 1.0f * PI / 4.0f, 
										 0, 
										 0 );
	verts[9].Normal = surfaceNormal( 1.0f * PI / 4.0f, 
									 0 );
	verts[9].TexC.x = 1.0f;
	verts[9].TexC.y = 1.0f / 8.0f;

 
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
	XMFLOAT3 radius(a,b,c);
	XMVECTOR r = XMLoadFloat3( &radius );
	XMVECTOR bottomLeft = XMLoadFloat3( &Vertices[node->indices[0]].Position );
	XMVECTOR topLeft = XMLoadFloat3( &Vertices[node->indices[1]].Position );
	XMVECTOR topRight = XMLoadFloat3( &Vertices[node->indices[2]].Position );
	XMVECTOR bottomRight = XMLoadFloat3( &Vertices[node->indices[5]].Position );
	
	int v0 = node->indices[0];
	int v1 = node->indices[1];
	int v2 = node->indices[2];
	int v3 = node->indices[5];

	Vertex tmp = Vertices[node->indices[0]];

	XMFLOAT3 oneOverR2(1.0f/(a), 1.0f/(b), 1.0f/(c));
	XMVECTOR overR2 = XMLoadFloat3( &oneOverR2 );
	XMFLOAT3 normal;
	float length;
	XMVECTOR snorm;
	XMVECTOR pos;
	
	// NEW VERTEX ID 4
	XMFLOAT3 midLeft;
	XMStoreFloat3(&midLeft, XMVector3Normalize((bottomLeft - topLeft) / 2.0f + topLeft) * r);
	
	tmp.Position = midLeft;		// 4
	Vertices.push_back( tmp );

	pos = XMLoadFloat3( &Vertices.back().Position );
	snorm = pos * overR2 ;
	XMStoreFloat( & length, XMVector3Length( snorm ) );
	snorm = XMVector3Normalize( snorm );
	XMStoreFloat3( &normal, snorm );
	Vertices.back().Normal = normal;
	Vertices.back().TexC.x = atan( normal.z / normal. x ) / 3.14159265358f + 0.5f;
	Vertices.back().TexC.y = 1.0f - asin( normal.y / length ) / 3.14159265358f + 0.5f;
	
	int v4 = Vertices.size() - 1;

	XMFLOAT3 midTop;
	XMStoreFloat3(&midTop, XMVector3Normalize((topRight - topLeft) / 2.0f + topLeft) * r);
	
	tmp.Position = midTop;		// 5
	Vertices.push_back( tmp );

	pos = XMLoadFloat3( &Vertices.back().Position );
	snorm = pos * overR2 ;
	XMStoreFloat( & length, XMVector3Length( snorm ) );
	snorm = XMVector3Normalize( snorm );
	XMStoreFloat3( &normal, snorm );
	Vertices.back().Normal = normal;
	Vertices.back().TexC.x = atan( normal.z / normal. x ) / 3.14159265358f + 0.5f;
	Vertices.back().TexC.y = 1.0f - asin( normal.y / length ) / 3.14159265358f + 0.5f;
	
	int v5 = Vertices.size() - 1;
		
	XMFLOAT3 midBottom;
	XMStoreFloat3(&midBottom, XMVector3Normalize((bottomRight - bottomLeft) / 2.0f + bottomLeft) * r);

	tmp.Position = midBottom;	// 6
	Vertices.push_back( tmp );

	pos = XMLoadFloat3( &Vertices.back().Position );
	snorm = pos * overR2 ;
	XMStoreFloat( & length, XMVector3Length( snorm ) );
	snorm = XMVector3Normalize( snorm );
	XMStoreFloat3( &normal, snorm );
	Vertices.back().Normal = normal;
	Vertices.back().TexC.x = atan( normal.z / normal. x ) / 3.14159265358f + 0.5f;
	Vertices.back().TexC.y = 1.0f - asin( normal.y / length ) / 3.14159265358f + 0.5f;
	
	int v6 = Vertices.size() - 1;
	
	XMFLOAT3 center;
	XMStoreFloat3(&center, XMVector3Normalize((bottomRight - topLeft) / 2.0f + topLeft) * r);

	tmp.Position = center;		// 7
	Vertices.push_back( tmp );

	pos = XMLoadFloat3( &Vertices.back().Position );
	snorm = pos * overR2 ;
	XMStoreFloat( & length, XMVector3Length( snorm ) );
	snorm = XMVector3Normalize( snorm );
	XMStoreFloat3( &normal, snorm );
	Vertices.back().Normal = normal;
	Vertices.back().TexC.x = atan( normal.z / normal. x ) / 3.14159265358f + 0.5f;
	Vertices.back().TexC.y = 1.0f - asin( normal.y / length ) / 3.14159265358f + 0.5f;

	int v7 = Vertices.size() - 1;
	
	XMFLOAT3 midRight;
	XMStoreFloat3(&midRight, XMVector3Normalize((bottomRight - topRight) / 2.0f + topRight) * r);

	tmp.Position = midRight;	// 8
	Vertices.push_back( tmp );

	pos = XMLoadFloat3( &Vertices.back().Position );
	snorm = pos * overR2 ;
	XMStoreFloat( & length, XMVector3Length( snorm ) );
	snorm = XMVector3Normalize( snorm );
	XMStoreFloat3( &normal, snorm );
	Vertices.back().Normal = normal;
	Vertices.back().TexC.x = atan( normal.z / normal. x ) / 3.14159265358f + 0.5f;
	Vertices.back().TexC.y = 1.0f - asin( normal.y / length ) / 3.14159265358f + 0.5f;

	int v8 = Vertices.size() - 1;
	
	node->children[0]->indices[0] = v0;
	node->children[0]->indices[1] = v4;
	node->children[0]->indices[2] = v7;
	node->children[0]->indices[3] = v0;
	node->children[0]->indices[4] = v7;
	node->children[0]->indices[5] = v6;
	
	node->children[1]->indices[0] = v4;
	node->children[1]->indices[1] = v1;
	node->children[1]->indices[2] = v5;
	node->children[1]->indices[3] = v4;
	node->children[1]->indices[4] = v5;
	node->children[1]->indices[5] = v7;
	
	node->children[2]->indices[0] = v7;
	node->children[2]->indices[1] = v5;
	node->children[2]->indices[2] = v2;
	node->children[2]->indices[3] = v7;
	node->children[2]->indices[4] = v2;
	node->children[2]->indices[5] = v8;
	
	node->children[3]->indices[0] = v6;
	node->children[3]->indices[1] = v7;
	node->children[3]->indices[2] = v8;
	node->children[3]->indices[3] = v6;
	node->children[3]->indices[4] = v8;
	node->children[3]->indices[5] = v3;
}