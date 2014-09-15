
#ifndef OBJECT_H
#define OBJECT_H

#include <Windows.h>
#include <D3DX11.h>
#include <xnamath.h>
#include <dxerr.h>
#include <cassert>
#include <ctime>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

class Object
{
public:

	static float PI;	

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
		std::vector<UINT>	Indices;
	};

protected:
	XMFLOAT3		position;
	XMFLOAT4		rotation;	// quaternion
	float			scale;
	XMFLOAT4X4		RTS;	// rotation translation & scale matrix

	XMFLOAT3		velocity;
	float			mass;	

	std::vector<Vertex> Vertices;
public:

	XMFLOAT3	getPosition()	{ return position; }
	XMFLOAT3	getVelocity()	{ return velocity; }
	float		getMass()		{ return mass; }

	void	setPosition ( XMFLOAT3 p ) { }
	void	setVelocity ( XMFLOAT3 v ) { }
	void	setMass		( float m ) { }

	std::vector<Vertex> getVertices() { return Vertices; }
};

#endif // OBJECT_H