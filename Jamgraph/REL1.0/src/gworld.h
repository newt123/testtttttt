#define CONST_G 0.0000001	// gravitational constant
#define CONST_k 0.001	// 1/(4*pi*permittivity of free space)
#define CONST_f 0.1 	//Coefficient of kinetic friction
#define SPRING_K 0.005	//Standard spring constant
#define V_INIT 0.001
#define F_INIT 0.01
#define STATIC_v 0.000001
#define STATIC_f 0.001

class GWorld
{
public:
	GWorld(void);
	~GWorld(void);

	void ComputeForce();
	void Step();
	void Render();
	void ReScale();
	void RenderHelp();

	void Init();
	void Add( GParticle* p );
	void RemoveAllBut( GParticle* p );
	void Remove( GParticle* p );

	GParticle* ParticleAt( double x, double y );

	GParticle* parts;
	GParticle* root;

	double mass;
	double scale;
	bool greased;
	bool autoscale;

	bool nofric;
	bool heavyg;
};
