#include <math.h>
#include "gvector.h"

GVector::GVector( double tx, double ty )
{
	x = tx;
	y = ty;
}

GVector::~GVector(void)
{
}

GVector GVector::operator+( GVector v ) const
{
	return GVector( x + v.x, y + v.y );
}

GVector GVector::operator-( GVector v ) const
{
	return GVector( x - v.x, y - v.y );
}

GVector GVector::operator/( double d ) const
{
	return GVector( x / d, y / d );
}

GVector GVector::operator*( double d ) const
{
	return GVector( x * d, y * d );
}

double GVector::operator*( GVector v ) const
{
	return x * v.x + y * v.y;
}

double GVector::operator~() const
{
	return sqrt( x * x + y * y );
}