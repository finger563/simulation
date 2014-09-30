#include "pch.h"
#include "QuadTree.h"

using namespace Base::DataStructures;

template<typename T>
QuadTreeNode<T>::QuadTreeNode(
	QuadTreeNode	*_parent,
	float			_error
	) : 
	parent(_parent),
	error(_error)
{
}
