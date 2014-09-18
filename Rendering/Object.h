
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

#include "Vector.h"

class Object
{
public:

	static float PI;	

	struct Vertex
	{
		Vertex(){}
		Vertex(const Vector3D& p, const Vector3D& n, const Vector3D& t, const Vector2D& uv, const Vector3D& geo)
			: Position(p), Normal(n), TangentU(t), TexC(uv), Geodetic(geo){}
		Vertex(
			double px, double py, double pz, 
			double nx, double ny, double nz,
			double tx, double ty, double tz,
			double u, double v,
			double lon, double lat, double h)
			: Position(px,py,pz), Normal(nx,ny,nz),
				TangentU(tx, ty, tz), TexC(u,v), Geodetic(lon,lat,h){}

		Vector3D Position;
		Vector3D Normal;
		Vector3D TangentU;
		Vector2D TexC;
		Vector3D Geodetic;
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