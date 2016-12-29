#include <math.h>
#include <time.h>
#include <windows.h>
#include <GL/glut.h>

#include "gvector.h"
#include "gparticle.h"

#include "gworld.h"

extern bool showhelp;

GWorld::GWorld(void)
{
	parts = root = 0;
	mass = 100.0;
	scale = 1.0;
	greased = false;
	autoscale = true;
	heavyg = nofric = false;
}

GWorld::~GWorld(void)
{
}

void GWorld::Add( GParticle* p )
{
	if ( !root ) root = p;
	if ( p->inworld ) return;
	p->next = parts;
	parts = p;
	p->inworld = true;
}

void GWorld::RemoveAllBut( GParticle* a )
{
	for ( GParticle* p = parts ; p ; p = p->next )
	{
		p->inworld = false;
		p->init = true;
	}
	parts = 0;
	Add( a );
}

void GWorld::Remove( GParticle* a )
{	
	a->inworld = false;
	if ( a == parts ) parts = parts->next;
	for ( GParticle* p = parts ; p ; p = p->next )
	{
		if ( p->HasSpring( a ) ) p->init = true;
		if ( p->next == a && !p->next->inworld ) p->next = p->next->next;
	}
}

void GWorld::Init()
{
	srand( time( 0 ) );

	parts = new GParticle( 0, 0 );
	parts->name = "all";
	parts->initn = 5;
	parts->Init( this );
}

void GWorld::ComputeForce()
{
	//Forces get cleared in the ReScale() function.

	GParticle* p;
	for ( p = parts ; p ; p = p->next )
	{
		p->ComputeForce( this );
	}
}

void GWorld::Step()
{
	GParticle* p;
	for ( p = parts ; p ; p = p->next )
	{
		p->Step( this );
		//if ( p->init && ~(p->v) < V_INIT && ~(p->F) < F_INIT  ) 
		//	p->Init( this );
	}
	ReScale();
}

void GWorld::Render()
{
	glClear( GL_COLOR_BUFFER_BIT );

	if ( autoscale )
	{
		glLoadIdentity();
		glScalef( scale, scale, scale );
	}

	GParticle* p;
	for ( p = parts ; p ; p = p->next )
	{
		p->Render();
	}

	if ( showhelp )
	{
		glPushMatrix();
		glLoadIdentity();
		RenderHelp();
		glPopMatrix();
	}
}

GParticle* GWorld::ParticleAt( double x, double y )
{
	GVector m( x, y );
	for ( GParticle* p = parts ; p ; p = p->next )
	{
		if ( ~( p->pos - m ) < p->r ) return p;
	}
	return 0;
}

void GWorld::ReScale()
{
	double outer = 0.0;
	double edge;
	GParticle* p, *q;
	q = parts;
	for ( p = parts ; p ; p = p->next )
	{
		if ( !p->inworld ) continue;
		edge = ~(p->pos) + p->r;
		if ( edge >= outer )
		{
			outer = edge;
			q = p;
		}
		p->F = GVector( 0, 0 );
	}
	
	if ( outer <= 0.1 ) outer = 0.1;
	else //Nudge outermost particle toward center.
		q->F = ( q->pos / ~(q->pos) ) * -0.0001;
	
	//Re-scale to include all particles.
	scale = ( scale + ( 3.0 / outer ) ) / 4.0 ;
}

#define RENDERED_HELP "\
 Left click: expand a node\n\
Right click: drag a node\n\
\n\
 Z: zoom in\n\
 X: zoom out\n\
 C: auto camera\n\
\n\
 P: pause physics\n\
 F: toggle friction\n\
 G: toggle high gravity\n\
\n\
 R: reset graph\n\
 T: trim to only this node\n\
\n\
 A: abolish this node\n\
 S: stow away dependents\n\
\n\
 ?: toggle this text\n\
"

void GWorld::RenderHelp()
{
	double lineheight = 28.0 / glutGet( GLUT_WINDOW_HEIGHT );
	double y = 1.0 - lineheight;
	char* c = RENDERED_HELP;

	glRasterPos2f( -1, y );
	for ( char* c = RENDERED_HELP ; *c ; c++ )
	{
		if ( *c == '\n' )
		{
			y -= lineheight;
			glRasterPos2f( -1, y );
		}
		else
		{
			glutBitmapCharacter( GLUT_BITMAP_8_BY_13, *c );
		}
	}
}