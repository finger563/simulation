
#include "Ellipsoid.h"

void Ellipsoid::generateMeshes( int qtDepth ) {
	MeshData faces[6];

	Vertex v[24];
	    
	// Fill in the front face vertex data.
	v[0] = Vertex(-a, -c, -b, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[1] = Vertex(-a, +c, -b, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[2] = Vertex(+a, +c, -b, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[3] = Vertex(+a, -c, -b, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	faces[0].Vertices.assign(&v[0], &v[4]);

	// Fill in the back face vertex data.
	v[4] = Vertex(-a, -c, +b, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	v[5] = Vertex(+a, -c, +b, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[6] = Vertex(+a, +c, +b, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[7] = Vertex(-a, +c, +b, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	faces[1].Vertices.assign(&v[4], &v[8]);

	// Fill in the top face vertex data.
	v[8]  = Vertex(-a, +c, -b, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[9]  = Vertex(-a, +c, +b, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[10] = Vertex(+a, +c, +b, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[11] = Vertex(+a, +c, -b, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	faces[2].Vertices.assign(&v[8], &v[12]);

	// Fill in the bottom face vertex data.
	v[12] = Vertex(-a, -c, -b, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	v[13] = Vertex(+a, -c, -b, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[14] = Vertex(+a, -c, +b, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[15] = Vertex(-a, -c, +b, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	faces[3].Vertices.assign(&v[12], &v[16]);

	// Fill in the left face vertex data.
	v[16] = Vertex(-a, -c, +b, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[17] = Vertex(-a, +c, +b, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[18] = Vertex(-a, +c, -b, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[19] = Vertex(-a, -c, -b, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	faces[4].Vertices.assign(&v[16], &v[20]);

	// Fill in the right face vertex data.
	v[20] = Vertex(+a, -c, -b, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	v[21] = Vertex(+a, +c, -b, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	v[22] = Vertex(+a, +c, +b, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
	v[23] = Vertex(+a, -c, +b, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

	faces[5].Vertices.assign(&v[20], &v[24]);
 
	//
	// Create the indices.
	//

	UINT i[36];

	// Fill in the front face index data
	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 0; i[4] = 2; i[5] = 3;

	faces[0].Indices.assign(&i[0], &i[6]);

	// Fill in the back face index data
	i[6] = 4; i[7]  = 5; i[8]  = 6;
	i[9] = 4; i[10] = 6; i[11] = 7;

	faces[1].Indices.assign(&i[6], &i[12]);

	// Fill in the top face index data
	i[12] = 8; i[13] =  9; i[14] = 10;
	i[15] = 8; i[16] = 10; i[17] = 11;

	faces[2].Indices.assign(&i[12], &i[18]);

	// Fill in the bottom face index data
	i[18] = 12; i[19] = 13; i[20] = 14;
	i[21] = 12; i[22] = 14; i[23] = 15;

	faces[3].Indices.assign(&i[18], &i[24]);

	// Fill in the left face index data
	i[24] = 16; i[25] = 17; i[26] = 18;
	i[27] = 16; i[28] = 18; i[29] = 19;

	faces[4].Indices.assign(&i[24], &i[30]);

	// Fill in the right face index data
	i[30] = 20; i[31] = 21; i[32] = 22;
	i[33] = 20; i[34] = 22; i[35] = 23;

	faces[5].Indices.assign(&i[30], &i[36]);

	if ( rootQT ) {
		delete rootQT;
	}
	rootQT = new QuadTreeNode( NULL, 36, 10.0f, 6);	// create root QTNode, error = 10, 1 child for each face of the starting cube
	Vertices.assign(&v[0], &v[24]);
	std::copy(&i[0],&i[36],rootQT->indices);
	//rootQT->mesh.Indices.assign(&i[0], &i[36]);

	for (int i=0; i < 6; i++) {
		rootQT->children[i] = new QuadTreeNode( rootQT, 6, rootQT->error/2.0f, 4 );
		//rootQT->children[i]->mesh = faces[i];
		std::copy(&faces[i].Indices[0],&faces[i].Indices[6],rootQT->children[i]->indices);
		generateQT( rootQT->children[i], 4, qtDepth );
	}
}

void Ellipsoid::generateQT( QuadTreeNode* node, int numChildren, int numSubdivisions ) {
	int remainingSubdivisions = numSubdivisions - 1;
	for (int i=0; i < numChildren; i++) {
		node->children[i] = new QuadTreeNode(node, 6, node->error/2.0f, numChildren);
		subdividePlanarQuad( node );
		if ( remainingSubdivisions > 0 ) 
			generateQT( node->children[i], numChildren, remainingSubdivisions );
	}
}

void Ellipsoid::subdividePlanarQuad( QuadTreeNode* node ) {
	XMVECTOR bottomLeft = XMLoadFloat3( &Vertices[node->indices[0]].Position );
	XMVECTOR topLeft = XMLoadFloat3( &Vertices[node->indices[1]].Position );
	XMVECTOR topRight = XMLoadFloat3( &Vertices[node->indices[2]].Position );
	XMVECTOR bottomRight = XMLoadFloat3( &Vertices[node->indices[5]].Position );

	XMFLOAT3 midTop;
	XMStoreFloat3(&midTop,(topRight - topLeft) / 2.0f + topLeft);
	XMFLOAT3 midBottom;
	XMStoreFloat3(&midBottom,(bottomRight - bottomLeft) / 2.0f + bottomLeft);
	XMFLOAT3 midRight;
	XMStoreFloat3(&midRight,(bottomRight - topRight) / 2.0f + topRight);
	XMFLOAT3 midLeft;
	XMStoreFloat3(&midLeft,(bottomLeft - topLeft) / 2.0f + topLeft);
	XMFLOAT3 center;
	XMStoreFloat3(&center,(bottomRight - topLeft) / 2.0f + topLeft);
	
	int v0 = node->indices[0];
	int v1 = node->indices[1];
	int v2 = node->indices[2];
	int v3 = node->indices[5];

	Vertex tmp = Vertices[node->indices[0]];
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