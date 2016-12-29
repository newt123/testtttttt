class GParticle;

struct DictPage
{
	char* key;
	GParticle* value;
	DictPage* next;
};

class PartDict
{
public:
	PartDict(void);
	~PartDict(void);

	GParticle* GetNode( char* string );

	DictPage* pages;
};
