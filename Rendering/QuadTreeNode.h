
#ifndef QUADTREENODE_H
#define QUADTREENODE_H

#include "Mesh.h"

class QuadTreeNode
{
private:
	QuadTreeNode*	children[4];
	QuadTreeNode*	parent;
	float			error;
	MeshData		mesh;
public:
	QuadTreeNode ( QuadTreeNode* _parent = NULL ) {
		parent = _parent;
		children[0] = children[1] = children[2] = children[3] = NULL;
		error = 100.0f;
	}

	~QuadTreeNode() {
		if ( children[0] )
			delete children[0];
		if ( children[1] )
			delete children[1];
		if ( children[2] )
			delete children[2];
		if ( children[3] )
			delete children[3];
	}
};

#endif // QUADTREENODE_H