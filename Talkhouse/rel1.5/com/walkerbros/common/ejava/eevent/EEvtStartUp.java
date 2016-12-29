// EEvtStartUp.java

package com.walkerbros.common.ejava.eevent;

import java.applet.Applet;
import java.awt.Frame;
import java.awt.Container;

import com.walkerbros.projects.walkerbros.ajava.*;

public class EEvtStartUp
{
	private Container	m_container	= null;
	private Applet 		m_applet 		= null;
	private Frame 		m_frame 		= null;
	
	public EEvtStartUp() 
	
	{
	}
	
	public void setContainer( Container container )
	{
		m_container = container;
		if( container instanceof Applet )
			m_applet = (Applet)container;
		if( container instanceof Frame )
			m_frame = (Frame)container;
	}
	public Container getContainer()
	{
		return m_container;
	}
	
	public Applet getApplet()
	{
		if( false == AConfig.APPLET )
			System.out.println( "Non-Applet called EEvtStartUp.getApplet()" );
		return m_applet;
	}
	
	public Frame getFrame()
	{
		if( false == AConfig.APPLET )
			System.out.println( "Non-Frame called EEvtStartUp.getFrame()" );
		return m_frame;
	}
}