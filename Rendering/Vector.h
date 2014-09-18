#ifndef VECTOR_H
#define VECTOR_H

#include <Windows.h>
#include <D3DX11.h>
#include <xnamath.h>
#include <math.h>

struct Vector3D {  // 3-dimensional vector of doubles
	union {
		double vals[3];
		struct {
			double x,y,z;
		};
		struct {
			double longitude, latitude, height;
		};
	};
	Vector3D ( ) { x = y = z = 0.0f; }
	Vector3D ( double _x, double _y, double _z ) {
		x = _x;
		y = _y;
		z = _z;
	}
	double Length() const { return sqrt(x*x + y*y + z*z); }
	Vector3D Normalized() const { return Vector3D( x / Length(), y / Length(), z / Length() ); }

	XMFLOAT3 toXMFloat3() const { return XMFLOAT3( x, y, z ); }
	
	bool operator==(const Vector3D& other) const {
		for (int i=0;i<3;i++) {
			if ( this->vals[i] != other.vals[i] )
				return false;
		}
		return true;
	}

	bool operator!=(const Vector3D& other) const {
		return !( *this == other );
	}

	Vector3D operator+(const Vector3D& other) const {
		return Vector3D( 
			this->x + other.x,
			this->y + other.y,
			this->z + other.z
			);
	}
	
	Vector3D operator-(const Vector3D& other) const {
		return Vector3D( 
			this->x - other.x,
			this->y - other.y,
			this->z - other.z
			);
	}
	
	Vector3D operator/(const double val) const {
		return Vector3D( 
			this->x / val,
			this->y / val,
			this->z / val
			);
	}
	
	Vector3D operator*(const double val) const {
		return Vector3D( 
			this->x * val,
			this->y * val,
			this->z * val
			);
	}
};


struct Vector2D { // 2-dimensional vector of doubles
	union {
		double vals[2];
		struct {
			double x,y;
		};
		struct {
			double longitude,latitude;
		};
	};
	Vector2D() { x = y = 0.0; }
	Vector2D( double _x, double _y ) {
		x = _x;
		y = _y;
	}

	XMFLOAT2 toXMFloat2() { return XMFLOAT2( x, y ); }
};

#endif	// VECTOR_H
