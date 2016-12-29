
class GVector
{
public:
	GVector( double x, double y );
	~GVector(void);

	double x;
	double y;

	GVector operator+( GVector ) const;
	GVector operator-( GVector ) const;
	GVector operator/( double ) const;
	GVector operator*( double ) const;

	double operator*( GVector ) const; //dot product
	double operator~() const; // magnitude
};
