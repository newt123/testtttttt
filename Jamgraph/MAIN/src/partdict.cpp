#include <string.h>

#include "gvector.h"
#include "gparticle.h"
#include "partdict.h"

PartDict::PartDict(void)
{
	pages = 0;
}

PartDict::~PartDict(void)
{
}

GParticle* PartDict::GetNode( char* s )
{
	DictPage* p;
	for ( p = pages ; p ; p = p->next )
	{
		if ( !strcmp( p->key, s ) ) return p->value;
	}

	p = new DictPage;
	p->key = s;
	p->value = new GParticle( 0.0, 0.0 );
	p->value->name = s;
	p->next = pages;

	pages = p;
	return p->value;
}