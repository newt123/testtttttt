class GParticle;
class GWorld;

class GSpring
{
public:
	GSpring() { next = 0 ;};

	GParticle* part;
	double K;
	GSpring* next;
};

class GParticle
{
public:
	GParticle( double x, double y );
	~GParticle(void);

	void Init( GWorld* );
	void AddSpring( GParticle* p );
	bool HasSpring( GParticle* p );
	void HideSprings( GWorld* w );
	
	double Q;	//charge
	double m;	//mass
	double r;   //radius
	GVector F;	//net force
	GVector a;	//acceleration
	GVector v;	//velocity
	GVector pos;	//position

	bool init;
	int initn;

	bool inworld;
	char* name;

	void ComputeForce( GWorld* w );
	void Step( GWorld* w );
	void Render();

	GVector NearBy();

	GParticle* next;
	GSpring* springs; //connected springs
};