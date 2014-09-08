
#ifndef ELLIPSOID_H
#define ELLIPSOID_H

#include "Object.h"

class Ellipsoid : Object
{
private:
	float a, b, c;
public:
	float getA() { return a; }
	float getB() { return b; }
	float getC() { return c; }
};

#endif // ELLIPSOID_H