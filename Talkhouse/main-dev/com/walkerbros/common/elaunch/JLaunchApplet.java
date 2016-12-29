// JLaunchApplet.java

package com.walkerbros.common.elaunch;

import java.awt.*;
import java.awt.event.*;
import java.applet.*;
import java.net.*;

import com.walkerbros.projects.walkerbros.ajava.*;
import com.walkerbros.projects.walkerbros.aclasses.*;
import com.walkerbros.common.ejava.*;
import com.walkerbros.common.ejava.eevent.*;

public class JLaunchApplet extends Applet implements EMsgPoster
{
	static AMain 	m_aMain 	= new AMain();
	
	boolean m_bIsStandalone = false;
	boolean m_bIsFirstStart = true;
	
	public JLaunchApplet() 
	{
		if( AConfig.DEBUG ) 
		{	
			// Can't call due to Security:	System.out.println( "java.home = " + System.getProperty("java.home") );
			// Can't call due to Security:	System.out.println( "java.class.path = " + System.getProperty("java.class.path") );
			System.out.println( "java.specification.version = " + System.getProperty("java.specification.version") );
			System.out.println( "java.specification.vendor = " + System.getProperty("java.specification.vendor") );
			System.out.println( "java.specification.name = " + System.getProperty("java.specification.name") );
			System.out.println( "java.version = " + System.getProperty("java.version") );
			System.out.println( "java.vendor = " + System.getProperty("java.vendor") );
			System.out.println( "java.vendor.url = " + System.getProperty("java.vendor.url") );
			System.out.println( "java.vm.specification.version = " + System.getProperty("java.vm.specification.version") );
			System.out.println( "java.vm.specification.vendor = " + System.getProperty("java.vm.specification.vendor") );
			System.out.println( "java.vm.specification.name = " + System.getProperty("java.vm.specification.name") );
			System.out.println( "java.vm.version = " + System.getProperty("java.vm.version") );
			System.out.println( "java.vm.vendor = " + System.getProperty("java.vm.vendor") );
			System.out.println( "java.vm.name = " + System.getProperty("java.vm.name") );
			System.out.println( "java.class.version = " + System.getProperty("java.class.version") );
			System.out.println( "os.name = " + System.getProperty("os.name") );
			System.out.println( "os.arch = " + System.getProperty("os.arch") );
			System.out.println( "os.version = " + System.getProperty("os.version") );
			System.out.println( "file.separator = " + System.getProperty("file.separator") );
			System.out.println( "path.separator = " + System.getProperty("path.separator") );
			System.out.println( "line.separator = " + System.getProperty("line.separator") );
			// Can't call due to Security:	System.out.println( "user.name = " + System.getProperty("user.name") );
			// Can't call due to Security:	System.out.println( "user.home = " + System.getProperty("user.home") );
			// Can't call due to Security:	System.out.println( "user.dir = " + System.getProperty("user.dir") );
		}
	}
	
	// Retrieve the value of an applet parameter
	public String getParameter(String key, String def) 
	{
		return m_bIsStandalone ? System.getProperty(key, def) :
				(getParameter(key) != null ? getParameter(key) : def);
	}
	
	// Get info on the applet parameters
	public String[][] getParameterInfo() 
	{
		return null;
	}
	
	// Get applet information
	public String getAppletInfo() 
	{
		return "Applet Information";
	}
	
	// Initialize the applet
	public void init() 
	{
		try {
		initComponents();
		}
		catch (Exception e) {
			e.printStackTrace();
		}
	
		// use anonymous inner classes to create listeners
		this.addMouseMotionListener( new MouseMotionAdapter()
		{
			public void mouseMoved( MouseEvent e )
			{
				EEvtMouseMove eEvtMouseMove = new EEvtMouseMove();
				eEvtMouseMove.x = e.getX();
				eEvtMouseMove.y = e.getY();
				
				m_aMain.onMouseMove( eEvtMouseMove );
			}
		} );
		
		this.addMouseListener( new MouseAdapter()
		{
			public void mousePressed( MouseEvent e )
			{
				EEvtMouseDown eEvtMouseDown = new EEvtMouseDown();
				eEvtMouseDown.x = e.getX();
				eEvtMouseDown.y = e.getY();
				
				m_aMain.onMouseDown( eEvtMouseDown );
			}
			public void mouseReleased( MouseEvent e )
			{
				EEvtMouseUp eEvtMouseUp = new EEvtMouseUp();
				eEvtMouseUp.x = e.getX();
				eEvtMouseUp.y = e.getY();
				m_aMain.onMouseUp( eEvtMouseUp );				
			}
		} );
		
		setSize( 640,480 );
		setVisible( true );
	}
	
	public void initComponents() throws Exception
	{
		// the following code sets the frame's initial state		setLocation( new java.awt.Point(0, 0) );
		setLayout( null );
		setBackground( Color.white );
		setSize( new java.awt.Dimension(640, 480) );
	}
	
	public void start()
	{
		if( m_bIsFirstStart )
		{
			postMsg( EMsg.E_MSG_STARTUP );
			postMsg( EMsg.E_MSG_INIT_SCENE );
			
			m_bIsFirstStart = false;
		}
		else
		{
		}
	}
	
	public void stop()
	{
	}
	
	public void postMsg( int msg )
	{
		switch( msg )
		{
			case EMsg.E_MSG_STARTUP:
			
				EEvtStartUp eEvtStartUp = new EEvtStartUp();
				eEvtStartUp.setContainer( this );
				
				m_aMain.onStartUp( eEvtStartUp );
				
				break;
			case EMsg.E_MSG_INIT_SCENE:
			
				EEvtInitializeScene eEvtInitializeScene = new EEvtInitializeScene();
				eEvtInitializeScene.nIdFileIni = AConfig.A_STR_INIT_INI;
				eEvtInitializeScene.nIdFileBit = AConfig.A_STR_INIT_BIT;
				
				m_aMain.onInitializeScene( eEvtInitializeScene );
				
				break;
			default:
				if( AConfig.DEBUG ) System.out.println( "Unknown case" );
				break;
		}
	}
	
	public void update( Graphics g )
	{
		paint( g );
	}
	public void paint( Graphics g )
	{
		EEvtPaint eEvtPaint = new EEvtPaint();
		eEvtPaint.setGraphics( g );
		
		m_aMain.onPaint( eEvtPaint );
	}
}