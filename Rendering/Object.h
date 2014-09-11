
#ifndef OBJECT_H
#define OBJECT_H

#include <xnamath.h>
#include <dxerr.h>
#include <cassert>
#include <ctime>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#include "QuadTreeNode.h"

class Object
{
private:
	XMFLOAT3		position;
	XMFLOAT4		rotation;	// quaternion
	float			scale;
	XMFloat4x4		RTS;	// rotation translation & scale matrix

	XMFLOAT3		velocity;
	float			mass;	
public:

	XMFLOAT3	getPosition()	{ return position; }
	XMFLOAT3	getVelocity()	{ return velocity; }
	float		getMass()		{ return mass; }

	void	setPosition ( XMFLOAT3 p ) { }
	void	setVelocity ( XMFLOAT3 v ) { }
	void	setMass		( float m ) { }
};

#endif // OBJECT_H