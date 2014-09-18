#ifndef VECTOR_H
#define VECTOR_H

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
};

#endif	// VECTOR_H
