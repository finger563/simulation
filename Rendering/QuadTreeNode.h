
#ifndef QUADTREENODE_H
#define QUADTREENODE_H

#include <Windows.h>
#include <D3DX11.h>
#include <xnamath.h>
#include <vector>

struct QuadTreeNode
{
	
	struct Vertex
	{
		Vertex(){}
		Vertex(const XMFLOAT3& p, const XMFLOAT3& n, const XMFLOAT3& t, const XMFLOAT2& uv)
			: Position(p), Normal(n), TangentU(t), TexC(uv){}
		Vertex(
			float px, float py, float pz, 
			float nx, float ny, float nz,
			float tx, float ty, float tz,
			float u, float v)
			: Position(px,py,pz), Normal(nx,ny,nz),
				TangentU(tx, ty, tz), TexC(u,v){}

		XMFLOAT3 Position;
		XMFLOAT3 Normal;
		XMFLOAT3 TangentU;
		XMFLOAT2 TexC;
	};

	struct MeshData
	{
		std::vector<Vertex> Vertices;
		std::vector<UINT> Indices;
	};

	QuadTreeNode*	*children;
	int				numChildren;
	QuadTreeNode*	parent;
	float			error;
	MeshData		mesh;

	QuadTreeNode ( QuadTreeNode* _parent = NULL, float _error = 100.0f, int _children = 0 ) 
	: parent(_parent), error(_error) {
		if ( _children > 0 )
		{
			children = new QuadTreeNode*[_children];
			numChildren = _children;
			for (int i=0; i< numChildren; i++)
				children[i] = NULL;
		}
		else {
			children = NULL;
			numChildren = 0;
		}
	}

	~QuadTreeNode() {
		if ( children ) {
			if ( numChildren > 0 ) {
				for (int i=0;i < numChildren; i++) {
					delete children[i];
				}
			}
		}
		delete children;
	}
};

#endif // QUADTREENODE_H