#define  _USE_MATH_DEFINES
#include <math.h>
#include <windows.h>
#include <GL/glut.h>
#include <stdio.h>

#include "gvector.h"
#include "gparticle.h"
#include "gworld.h"

GParticle::GParticle( double x, double y )
: pos( GVector( x, y ) ),
  F( GVector( 0.0, 0.0 ) ),
  a( GVector( 0.0, 0.0 ) ),
  v( GVector( 0.0, 0.0 ) ),
  m( 1.0 ),
  r( 0.1 ),
  Q( 1.0 ),
  springs( 0 ),
  next( 0 ),
  name( 0 ),
  inworld( false ),
  init( true )
{
}

GParticle::~GParticle(void)
{
}

void GParticle::AddSpring( GParticle* p )
{
	GSpring* s = new GSpring();
	s->K = SPRING_K;
	s->part = p;
	s->next = springs;
	springs = s;
}

bool GParticle::HasSpring( GParticle* p )
{
	for ( GSpring* s = springs ; s ; s = s->next )
	{
		if ( s->part == p ) return true;
	}
	return false;
}

void GParticle::HideSprings( GWorld* w )
{
	for ( GSpring* s = springs ; s ; s = s->next )
	{
		s->part->HideSprings( w );
		w->Remove( s->part );
	}
	init = true;
}

void GParticle::Init( GWorld* w )
{
	for ( GSpring* s = springs ; s ; s = s->next )
	{
		if ( !s->part->inworld )
		{
			s->part->pos = NearBy();
			w->Add( s->part );
		}
	}
	init = false;
	return;
	init = false;
	int n = initn;
	if ( !n ) return;

	GParticle* p;

	double dx, dy, dt;

	double l = rand() % n + 1;
	for ( int i = 0 ; i < l ; i++ )
	{
		dt = ( rand() % 628 ) / 100.0;
		dx = r * 2 * cos( dt + i * 2.0 * M_PI / l ) ;
		dy = r * 2 * sin( dt + i * 2.0 * M_PI / l ) ;

		p = new GParticle( pos.x + dx, pos.y + dy );

		if ( n > 1 )
			p->name = "some";
		else
			p->name = "none";

		w->Add( p );
		AddSpring( p );

		p->initn = n - 1;
		p->Q = Q * 0.9;
	}
}

GVector GParticle::NearBy()
{
	double dx, dy;
	int i = rand() % 2048;
	dx = r * 2 * cos( i * 2.0 * M_PI / 2048 ) ;
	dy = r * 2 * sin( i * 2.0 * M_PI / 2048 ) ;
	return pos + GVector( dx, dy );
}

void GParticle::ComputeForce( GWorld* w )
{
	if ( !inworld ) return;
	GParticle* p;
	GSpring* s;
	GVector dv( 0.0, 0.0 );
	GVector fv( 0.0, 0.0 );
	double dd;

	// Electrostatic forces
	for ( p = w->parts ; p ; p = p->next )
	{
		if ( !p->inworld ) continue;
		if ( p->pos.x == pos.x && p->pos.y == pos.y ) continue;

		dv = pos - p->pos ;		//vector pointing from p to this
		dd = ~dv + p->r + r + 0.001 ;	//add radii, so dd is never zero
		dv = dv / ~dv ;			//normalize dv into a unit vector

		//Coulomb's Law:
		fv = dv * ( CONST_k * Q * p->Q / ( dd * dd ) );

		F = F + fv;
	}
	// Spring forces

	for ( s = springs ; s ; s = s->next )
	{
		p = s->part;
		if ( !p->inworld ) continue;

		dv = pos - p->pos ;		//vector pointing from p to this

		if ( ~dv < p->r + r * 2 ) continue; //ignore close springs

		dd = ~dv + p->r + r ;	//add radii, so dd is dist between surfaces
		dv =  dv / ~dv;
		dv = dv * dd;

		// Hooke's Law:
		fv = dv * ( -1 * s->K );

		F = F + fv;
		p->F = p->F - fv;
	}

	//Gravitational force toward the center

	//fv = ( pos / dd ) * ( -1 * CONST_G * w->mass * m / ( dd * dd ) );
	//Instead of using real gravity, we'll use a version that's
	//actually useful, which is more like a spring connecting everything
	//in the world to the center.
	
	dd = ~pos + r * 10.0;
	fv = ( pos / dd ) * ( -1 * CONST_G * w->mass * m * dd );
	if ( w->heavyg ) fv = fv * 1000.0;
	F = F + fv;
}

extern GParticle* p;

void GParticle::Step( GWorld* w )
{
	init = false;
	for ( GSpring* s = springs ; s ; s = s->next )
	{
		if ( !s->part->inworld ) init = true;
	}

	if ( p == this ) return;

	double fric = ( init && initn ) ? CONST_f : CONST_f * 5.0;
	if ( w->greased ) fric = CONST_f;
	if ( w->nofric ) fric = 0.0;
	v = v * ( 1.0 - fric );

	if ( ~v < STATIC_v ) 
	{
		v = GVector( 0, 0 );
		if ( ~F < STATIC_f ) F = GVector( 0, 0 );
		else F = F - ( F / ~F ) * STATIC_f ;
	}

	a = F / m;
	v = v + a;
	pos = pos + v;
}

void GParticle::Render()
{
	if ( !inworld ) return;

	//Inefficient circle drawing.  Fix this later.
	GLint i;
	GLfloat cosine, sine;

	if ( init && springs )
		glColor3f( 1.0, 0.4, 0.4 );
	else
		glColor3f( 0.4, 0.4, 1.0 );
	glBegin(GL_POLYGON);
		for(i=0;i<100;i++)
		{
			cosine = float( pos.x + r * cos(i*2*M_PI/100.0) );
			sine = float( pos.y + r * sin(i*2*M_PI/100.0) );
			glVertex2f( cosine, sine );
		}
	glEnd();

	//Draw springs.
	GSpring* s;
	GParticle* p;
	GVector sv( 0.0, 0.0 );
	GVector ev( 0.0, 0.0 );
	GVector dv( 0.0, 0.0 );
	for ( s = springs ; s ; s = s->next )
	{
		p = s->part;
		if ( !p->inworld ) continue;

		dv = p->pos - this->pos;
		sv = pos + ( dv / ~dv ) * r;
		ev = p->pos - ( dv / ~dv ) * p->r;

		glLineWidth( 2.0 );
		glBegin( GL_LINE_STRIP );
			glColor3f( 0.2, 1, 0.2 );
			glVertex2f( sv.x, sv.y );
			glColor3f( 0, 0.2, 0 );
			glVertex2f( ev.x, ev.y );
		glEnd();
	}

	//Draw node name.
	glColor3f( 1, 1, 0 );
	glBegin( GL_LINE_STRIP );
		glVertex2f( 0, 0 );
		glVertex2f( 0, 0 );
	glEnd();
	glRasterPos2f( pos.x, pos.y );
	for ( char* c = name ; c && *c ; c++ )
		glutBitmapCharacter( GLUT_BITMAP_8_BY_13, *c );
}