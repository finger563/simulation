
#include "Ellipsoid.h"

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
	return retInd;
}

void Ellipsoid::generateMeshes( int qtDepth ) {
	MeshData faces[6];

	Vertex verts[8];

	XMFLOAT3 radius(a,b,c);
	XMVECTOR r = XMLoadFloat3( &radius );
	    
	// Fill in the front face vertex data.
	verts[0] = Vertex(-a, -c, -b, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	XMVECTOR pos = XMVector3Normalize( XMLoadFloat3( & verts[0].Position ) ) * r;
	XMStoreFloat3( &verts[0].Position, pos );

	verts[1] = Vertex(-a, +c, -b, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	pos = XMVector3Normalize( XMLoadFloat3( & verts[1].Position ) ) * r;
	XMStoreFloat3( &verts[1].Position, pos );

	verts[2] = Vertex(+a, +c, -b, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	pos = XMVector3Normalize( XMLoadFloat3( & verts[2].Position ) ) * r;
	XMStoreFloat3( &verts[2].Position, pos );

	verts[3] = Vertex(+a, -c, -b, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	pos = XMVector3Normalize( XMLoadFloat3( & verts[3].Position ) ) * r;
	XMStoreFloat3( &verts[3].Position, pos );

	// Fill in the back face vertex data.
	verts[4] = Vertex(+a, -c, +b, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	pos = XMVector3Normalize( XMLoadFloat3( & verts[4].Position ) ) * r;
	XMStoreFloat3( &verts[4].Position, pos );

	verts[5] = Vertex(+a, +c, +b, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	pos = XMVector3Normalize( XMLoadFloat3( & verts[5].Position ) ) * r;
	XMStoreFloat3( &verts[5].Position, pos );

	verts[6] = Vertex(-a, +c, +b, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	pos = XMVector3Normalize( XMLoadFloat3( & verts[6].Position ) ) * r;
	XMStoreFloat3( &verts[6].Position, pos );

	verts[7] = Vertex(-a, -c, +b, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	pos = XMVector3Normalize( XMLoadFloat3( & verts[7].Position ) ) * r;
	XMStoreFloat3( &verts[7].Position, pos );

 
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
	ind[12] = 1; ind[13] =  6; ind[14] = 5;
	ind[15] = 1; ind[16] = 5; ind[17] = 2;

	faces[2].Indices.assign(&ind[12], &ind[18]);

	// Fill in the bottom face index data
	ind[18] = 7; ind[19] = 0; ind[20] = 3;
	ind[21] = 7; ind[22] = 3; ind[23] = 4;

	faces[3].Indices.assign(&ind[18], &ind[24]);

	// Fill in the left face index data
	ind[24] = 7; ind[25] = 6; ind[26] = 1;
	ind[27] = 7; ind[28] = 1; ind[29] = 0;

	faces[4].Indices.assign(&ind[24], &ind[30]);

	// Fill in the right face index data
	ind[30] = 3; ind[31] = 2; ind[32] = 5;
	ind[33] = 3; ind[34] = 5; ind[35] = 4;

	faces[5].Indices.assign(&ind[30], &ind[36]);

	if ( rootQT ) {
		delete rootQT;
	}
	rootQT = new QuadTreeNode( NULL, 36, 10.0f, 6);
	for (int i=0; i < 8; i++)
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

	XMFLOAT3 midTop;
	XMStoreFloat3(&midTop, XMVector3Normalize((topRight - topLeft) / 2.0f + topLeft) * r);
	
	XMFLOAT3 midBottom;
	XMStoreFloat3(&midBottom, XMVector3Normalize((bottomRight - bottomLeft) / 2.0f + bottomLeft) * r);
	
	XMFLOAT3 midRight;
	XMStoreFloat3(&midRight, XMVector3Normalize((bottomRight - topRight) / 2.0f + topRight) * r);
	
	XMFLOAT3 midLeft;
	XMStoreFloat3(&midLeft, XMVector3Normalize((bottomLeft - topLeft) / 2.0f + topLeft) * r);
	
	XMFLOAT3 center;
	XMStoreFloat3(&center, XMVector3Normalize((bottomRight - topLeft) / 2.0f + topLeft) * r);
	
	int v0 = node->indices[0];
	int v1 = node->indices[1];
	int v2 = node->indices[2];
	int v3 = node->indices[5];

	Vertex tmp (Vertices[node->indices[0]]);
	tmp.Position = midLeft;		// 4
	Vertices.push_back( tmp );
	
	int v4 = Vertices.size() - 1;

	tmp.Position = midTop;		// 5
	Vertices.push_back( tmp );
	
	int v5 = Vertices.size() - 1;

	tmp.Position = midBottom;	// 6
	Vertices.push_back( tmp );
	
	int v6 = Vertices.size() - 1;

	tmp.Position = center;		// 7
	Vertices.push_back( tmp );

	int v7 = Vertices.size() - 1;

	tmp.Position = midRight;	// 8
	Vertices.push_back( tmp );

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