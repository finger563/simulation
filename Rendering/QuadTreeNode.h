
#ifndef QUADTREENODE_H
#define QUADTREENODE_H

#include <Windows.h>
#include <D3DX11.h>
#include <xnamath.h>
#include <vector>

struct QuadTreeNode
{	
	QuadTreeNode*	*children;	// pointer to array of children pointers of this node
	int				numChildren;// number of children this QT node has
	QuadTreeNode*	parent;		// parent QT node
	float			error;		// deviation of this QTnode from actual mesh
	UINT*			indices;	// indices into some mesh
	int				numIndices;	// number of indices this node has 

	QuadTreeNode ( QuadTreeNode* _parent = NULL, int _numIndices = 6, float _error = 100.0f, int _numChildren = 0 ) 
	: parent(_parent), numIndices(_numIndices), error(_error) {
		if ( _numChildren > 0 )
		{
			children = new QuadTreeNode*[_numChildren];
			numChildren = _numChildren;
			for (int i=0; i< numChildren; i++)
				children[i] = NULL;
		}
		else {
			children = NULL;
			numChildren = 0;
		}

		if ( _numIndices > 0 ) {
			numIndices = _numIndices;
			indices = new UINT[numIndices];
			for (int i=0; i< numIndices; i++)
				indices[i] = 0;
		}
		else {
			numIndices = 0;
			indices = NULL;
		}
	}

	~QuadTreeNode() {
		if ( children ) {
			if ( numChildren > 0 ) {
				for (int i=0;i < numChildren; i++) {
					delete children[i];
				}
			}
			delete children;
		}
		
		if ( indices ) {
			delete indices;
		}
	}
};

#endif // QUADTREENODE_H