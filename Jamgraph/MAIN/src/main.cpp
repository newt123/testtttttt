#ifdef WIN32
	#include <windows.h>
#endif //WIN32
#include <stdio.h>
#include <GL/glut.h>

#include "gvector.h"
#include "gparticle.h"
#include "gworld.h"
#include "partdict.h"

GWorld* w;
GParticle* p;
PartDict* pd;
double mx, my;
bool paused;
bool showhelp;

bool antialias;
bool blend;
bool include;

#define JAMGRAPH_HELP "\
	\n\
	Usage: \n\
		jam -ndd | jamgraph [opts]\n\
	\n\
	Jamgraph options:\n\
		a : Enable antialiasing (implies b)\n\
		b : Enable alpha blending\n\
		i : Graph include dependencies\n\
	\n\
	Push ? in Jamgraph to display controls information.\n\
"

void load()
{
	pd = new PartDict();
	char *line, *t1, *t2;
	GParticle *p1, *p2;
	bool root = false;

	//Going to use a big honking bunch of memory here that never gets
	//freed.  At some point I might fix this.
	for (	line = new char[1024];
			fgets( line, 1024, stdin );
			line = new char[1024]
		)
	{
		if ( include )
		{
			if ( memcmp( line, "Includes \"", 10 ) ) continue;
			line += 10;
		}
		else
		{
			if ( memcmp( line, "Depends \"", 9 ) ) continue;
			line += 9;
		}

		t1 = line;
        while ( *line != '"' ) line++;
		*line = '\0';

		line += 5;
		t2 = line;
		while ( *line != '"' ) line++;
		*line = '\0';

		p1 = pd->GetNode( t1 );
		p2 = pd->GetNode( t2 );
		p2->pos = p1->NearBy();

		if ( !root )
		{
			w->Add( p1 );
			root = true;
		}

		p1->AddSpring( p2 );
	}
}

void getpos( int x, int y )
{
	double modelm[16];
	glGetDoublev( GL_MODELVIEW_MATRIX, modelm );
	double projm[16];
	glGetDoublev( GL_PROJECTION_MATRIX, projm );
	double z;
	int viewport[4];
	glGetIntegerv( GL_VIEWPORT, viewport );

	gluUnProject( x, glutGet( GLUT_WINDOW_HEIGHT ) - y, 0,
		modelm, projm, viewport,
		&mx, &my, &z );
}

void key( unsigned char key, int x, int y )
{
	getpos( x, y );
	switch( key )
	{
	case 'z':
	case 'Z':
		w->autoscale = false;
		glTranslatef( mx * -0.1, my * -0.1, 0 );
		w->scale *= 1.1;
		glScalef( 1.1, 1.1, 1.1 );
		break;
	case 'x':
	case 'X':
		w->autoscale = false;
		glTranslatef( mx * 0.1, my * 0.1, 0 );
		w->scale *= 0.9;
		glScalef( 0.9, 0.9, 0.9 );
		break;
	case 'c':
	case 'C':
		w->autoscale = true;
		break;
	case 'f':
	case 'F':
		w->nofric = !w->nofric;
		break;
	case 'g':
	case 'G':
		w->heavyg = !w->heavyg;
		break;
	case 't':
	case 'T':
		p = w->ParticleAt( mx, my );
		if ( p ) w->RemoveAllBut( p );
		p = 0;
		break;
	case 'r':
	case 'R':
		w->RemoveAllBut( w->root );
		break;
	case 'a':
	case 'A':
		p = w->ParticleAt( mx, my );
		if ( p ) w->Remove( p );
		p = 0;
		break;
	case 's':
	case 'S':
		p = w->ParticleAt( mx, my );
		if ( p ) p->HideSprings( w );
		p = 0;
		break;
	case 'p':
	case 'P':
		paused = !paused;
		break;
	case '/':
	case '?':
		showhelp = !showhelp;
		break;
	}
}

void click( int button, int state, int x, int y )
{
	getpos( x, y );

	if ( state == GLUT_DOWN )
	{
		p = w->ParticleAt( mx, my );
		if ( !p ) return;
		if ( button == GLUT_LEFT )
		{
			if ( p->init ) p->Init( w );
			p = 0;
		}
		else
		{
			w->greased = true;
		}
	}
	else // state == GLUT_UP
	{
		p = 0;
		w->greased = false;
	}
}

void move( int x, int y )
{
	if ( !p ) return;

	getpos( x, y );

	p->pos = GVector( mx, my );
	glutPostRedisplay();
}

void timer( int val )
{
	w->Render();
	if ( !paused )
	{
		w->ComputeForce();
		w->Step();
	}
	glutPostRedisplay();
	glutTimerFunc( 1, timer, 0 );
}

void display()
{
	glutSwapBuffers();
	glFlush();
}

int main ( int argc, char** argv )
{
	glutInit( &argc, argv );

	p = 0;
	paused = showhelp = false;
	antialias = blend = include = false;

	//Parse arguments.  We only have three flags: "a" for antialiasing,
	//"b" for blending, and "i" for includes.  So I'm going to cheat
	//by just looking for occurrences of those characters, regardless
	//of context.
	while ( argc )
	{
		argc--;
		argv++;
		while ( argv[0] && *argv[0] )
		{
            switch ( *(argv[0]) )
			{
			case 'a':
				antialias = true;
			case 'b':
				blend = true;
				break;
			case 'i':
				include = true;
				break;
			case 'h':
			case '?':
				printf( JAMGRAPH_HELP );
				return 0;
			}
			(argv[0])++;
		}
	}

	w = new GWorld();
	load();

	glutInitDisplayMode( GLUT_DOUBLE | ( blend ? GLUT_ALPHA : 0 ) );
	glutCreateWindow( "Jamgraph" );
	glutReshapeWindow( 600, 600 );

	//Blending
	if ( blend )
	{
		glEnable( GL_BLEND );
		glBlendFunc (GL_SRC_ALPHA, GL_ONE);
	}

	//Antialiasing
	if ( antialias )
	{
		glEnable( GL_POLYGON_SMOOTH );
		glEnable( GL_LINE_SMOOTH );
		glHint( GL_POLYGON_SMOOTH_HINT, GL_DONT_CARE );
	}

	glutMouseFunc( click );
	glutMotionFunc( move );
	glutKeyboardFunc( key );
	glutDisplayFunc( display );
	glutTimerFunc( 1, timer, 0 );
	glutMainLoop();

	return 0;
}