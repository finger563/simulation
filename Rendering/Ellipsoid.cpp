
#include "Ellipsoid.h"

QuadTreeNode::MeshData Ellipsoid::getMesh( XMFLOAT3 relPos ) {
	QuadTreeNode::MeshData retMesh;
	int vertOffset = 0;
	for (int i=0;i<6;i++) {
		for (int j=0;j<4;j++) {
			for (int k=0;k<4;k++) {
				for (int n=0;n<rootQT->children[i]->children[j]->children[k]->mesh.Vertices.size();n++) {
					retMesh.Vertices.push_back(rootQT->children[i]->children[j]->children[k]->mesh.Vertices[n]);
				}
				for (int n=0;n<rootQT->children[i]->children[j]->children[k]->mesh.Indices.size();n++) {
					retMesh.Indices.push_back(rootQT->children[i]->children[j]->children[k]->mesh.Indices[n] + vertOffset);
				}
				vertOffset += rootQT->children[i]->children[j]->children[k]->mesh.Vertices.size();
			}
		}
	}
	//retMesh = rootQT->mesh;
	return retMesh;
}

void Ellipsoid::generateMeshes( int qtDepth ) {
	QuadTreeNode::MeshData faces[6];

	QuadTreeNode::Vertex v[24];
	    
	// Fill in the front face vertex data.
	v[0] = QuadTreeNode::Vertex(-a, -c, -b, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[1] = QuadTreeNode::Vertex(-a, +c, -b, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[2] = QuadTreeNode::Vertex(+a, +c, -b, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[3] = QuadTreeNode::Vertex(+a, -c, -b, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	faces[0].Vertices.assign(&v[0], &v[4]);

	// Fill in the back face vertex data.
	v[4] = QuadTreeNode::Vertex(-a, -c, +b, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	v[5] = QuadTreeNode::Vertex(+a, -c, +b, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[6] = QuadTreeNode::Vertex(+a, +c, +b, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[7] = QuadTreeNode::Vertex(-a, +c, +b, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	faces[1].Vertices.assign(&v[4], &v[8]);

	// Fill in the top face vertex data.
	v[8]  = QuadTreeNode::Vertex(-a, +c, -b, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[9]  = QuadTreeNode::Vertex(-a, +c, +b, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[10] = QuadTreeNode::Vertex(+a, +c, +b, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[11] = QuadTreeNode::Vertex(+a, +c, -b, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	faces[2].Vertices.assign(&v[8], &v[12]);

	// Fill in the bottom face vertex data.
	v[12] = QuadTreeNode::Vertex(-a, -c, -b, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	v[13] = QuadTreeNode::Vertex(+a, -c, -b, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[14] = QuadTreeNode::Vertex(+a, -c, +b, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[15] = QuadTreeNode::Vertex(-a, -c, +b, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	faces[3].Vertices.assign(&v[12], &v[16]);

	// Fill in the left face vertex data.
	v[16] = QuadTreeNode::Vertex(-a, -c, +b, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[17] = QuadTreeNode::Vertex(-a, +c, +b, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[18] = QuadTreeNode::Vertex(-a, +c, -b, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[19] = QuadTreeNode::Vertex(-a, -c, -b, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	faces[4].Vertices.assign(&v[16], &v[20]);

	// Fill in the right face vertex data.
	v[20] = QuadTreeNode::Vertex(+a, -c, -b, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	v[21] = QuadTreeNode::Vertex(+a, +c, -b, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	v[22] = QuadTreeNode::Vertex(+a, +c, +b, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
	v[23] = QuadTreeNode::Vertex(+a, -c, +b, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

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
	rootQT = new QuadTreeNode( NULL, 10.0f, 6);	// create root QTNode, error = 10, 1 child for each face of the starting cube
	rootQT->mesh.Vertices.assign(&v[0], &v[24]);
	rootQT->mesh.Indices.assign(&i[0], &i[36]);

	int numSubdivisions = 4;
	for (int i=0; i < 6; i++) {
		rootQT->children[i] = new QuadTreeNode( rootQT, rootQT->error/2.0f, 4 );
		rootQT->children[i]->mesh = faces[i];
		generateQT( rootQT->children[i], 4, numSubdivisions );
	}
}

void Ellipsoid::generateQT( QuadTreeNode* node, int numChildren, int numSubdivisions ) {
	int remainingSubdivisions = numSubdivisions - 1;
	for (int i=0; i < numChildren; i++) {
		node->children[i] = new QuadTreeNode(node, node->error/2.0f, numChildren);
		node->children[i]->mesh = node->mesh;
		subdividePlanar(node->children[i]->mesh, i);
		if ( remainingSubdivisions > 0 ) 
			generateQT( node->children[i], numChildren, remainingSubdivisions );
	}
}

void Ellipsoid::subdividePlanar( QuadTreeNode::MeshData& mesh, int quadrant ) {
	XMFLOAT3 normal = mesh.Vertices[0].Normal;
	XMFLOAT3 down = XMFLOAT3(0,0,0);
	XMFLOAT3 right = mesh.Vertices[0].TangentU;
	if ( abs(normal.x) ) {
		down.y = -1.0f;
	}
	else if ( abs(normal.y) ) {
		if ( normal.y > 0 ) {
			down.z = -1.0f;
		}
		else {
			down.z = 1.0f;
		}
	}
	else if ( abs(normal.z) ) {
		down.y = -1.0f;
	}
	right = XMFLOAT3(right.x/2.0f, right.y/2.0f, right.z/2.0f);
	down = XMFLOAT3(down.x/2.0f, down.y/2.0f, down.z/2.0f);
	XMVECTOR d = XMLoadFloat3( & down );
	XMVECTOR r = XMLoadFloat3( & right );
	
	XMVECTOR bottomLeft = XMLoadFloat3( &mesh.Vertices[0].Position );
	XMVECTOR topLeft = XMLoadFloat3( &mesh.Vertices[1].Position );
	XMVECTOR topRight = XMLoadFloat3( &mesh.Vertices[2].Position );
	r = (topRight - topLeft) / 2.0f;
	d = (bottomLeft - topLeft) / 2.0f;

	XMVECTOR pos;
	switch ( quadrant ) {
	case 0:	// upper left
		pos = XMLoadFloat3( &mesh.Vertices[0].Position );
		pos = pos - d;
		XMStoreFloat3(&mesh.Vertices[0].Position,pos);
		
		pos = XMLoadFloat3( &mesh.Vertices[2].Position );
		pos = pos - r;
		XMStoreFloat3(&mesh.Vertices[2].Position,pos);
		
		pos = XMLoadFloat3( &mesh.Vertices[3].Position );
		pos = pos - d;
		pos = pos - r;
		XMStoreFloat3(&mesh.Vertices[3].Position,pos);
		break;
	case 1: // upper right
		pos = XMLoadFloat3( &mesh.Vertices[0].Position );
		pos = pos - d;
		pos = pos + r;
		XMStoreFloat3(&mesh.Vertices[0].Position,pos);
		
		pos = XMLoadFloat3( &mesh.Vertices[1].Position );
		pos = pos + r;
		XMStoreFloat3(&mesh.Vertices[1].Position,pos);
		
		pos = XMLoadFloat3( &mesh.Vertices[3].Position );
		pos = pos - d;
		XMStoreFloat3(&mesh.Vertices[3].Position,pos);
		break;
	case 2: // lower left
		pos = XMLoadFloat3( &mesh.Vertices[1].Position );
		pos = pos + d;
		XMStoreFloat3(&mesh.Vertices[1].Position,pos);
		
		pos = XMLoadFloat3( &mesh.Vertices[2].Position );
		pos = pos - r;
		pos = pos + d;
		XMStoreFloat3(&mesh.Vertices[2].Position,pos);
		
		pos = XMLoadFloat3( &mesh.Vertices[3].Position );
		pos = pos - r;
		XMStoreFloat3(&mesh.Vertices[3].Position,pos);
		break;
	case 3: // lower right
		pos = XMLoadFloat3( &mesh.Vertices[0].Position );
		pos = pos + r;
		XMStoreFloat3(&mesh.Vertices[0].Position,pos);
		
		pos = XMLoadFloat3( &mesh.Vertices[1].Position );
		pos = pos + d;
		pos = pos + r;
		XMStoreFloat3(&mesh.Vertices[1].Position,pos);
		
		pos = XMLoadFloat3( &mesh.Vertices[2].Position );
		pos = pos + d;
		XMStoreFloat3(&mesh.Vertices[2].Position,pos);
		break;
	default:
		break;
	}
}