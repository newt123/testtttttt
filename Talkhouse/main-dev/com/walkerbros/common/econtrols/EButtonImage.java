// EButtonImage.java

package com.walkerbros.common.econtrols;

import java.awt.*;
import java.applet.*;

import com.walkerbros.common.ejava.*;
import com.walkerbros.common.ejava.eevent.*;
import com.walkerbros.common.eimage.*;

import com.walkerbros.projects.walkerbros.aclasses.*;
import com.walkerbros.projects.walkerbros.ajava.*;

public class EButtonImage extends EButton
{
	public static final short E_BUTTONIMAGE_STATE_UP			= 0;
	public static final short E_BUTTONIMAGE_STATE_DN			= 1;
	public static final short E_BUTTONIMAGE_STATE_OFF			= 2;

	private short m_nState	= E_BUTTONIMAGE_STATE_UP;
	
	EGifImage m_eGifImageUpState 	= new EGifImage();
	EGifImage m_eGifImageDnState 	= new EGifImage();
	EGifImage m_eGifImageOffState = new EGifImage();
	
	public EButtonImage() 
	{
	}
	
	public boolean isType( short type )
	{
		if( EDefines.E_OBJ_TYPE_BUTTONIMAGE == type )
			return true;
		return false;
	}
	
	public boolean isOfType( short type )
	{
		if( EDefines.E_OBJ_TYPE_BUTTONIMAGE == type )
			return true;
		return super.isOfType( type );
	}

	public void loadFromDisk( EFile eFile, EErr err )
	{
		super.loadFromDisk( eFile,err );
		if( err.error() )
			return;
		
		m_nState = eFile.readShort( err );
		if( err.error() )
			return;
			
		m_eGifImageUpState.loadFromDisk( eFile,err );
		if( err.error() )
			return;

			
		m_eGifImageDnState.loadFromDisk( eFile,err );
		if( err.error() )
			return;

			
		m_eGifImageOffState.loadFromDisk( eFile,err );
		if( err.error() )
			return;
	}
	
	public void saveToDisk( EFile eFile, EErr err )
	{
		super.saveToDisk( eFile,err );
		if( err.error() )
			return;
			
		eFile.writeShort( m_nState,err );
		if( err.error() )
			return;
			
		m_eGifImageUpState.saveToDisk( eFile,err );
		if( err.error() )
			return;
			
		m_eGifImageDnState.saveToDisk( eFile,err );
		if( err.error() )
			return;

			
		m_eGifImageOffState.saveToDisk( eFile,err );
		if( err.error() )
			return;
	}

	public void createObject( EErr err )
	{
		AObjectController aObjCtrl = getObjCtrl();
		
		m_eGifImageUpState.createObject( err,aObjCtrl,false );
		if( err.error() )
			return;

		
		m_eGifImageDnState.createObject( err,aObjCtrl,false );
		if( err.error() )
			return;

		
		m_eGifImageOffState.createObject( err,aObjCtrl,false );
		if( err.error() )
			return;
	}

	public Image getGif()
	{
		if( false == isVisible() )
			return null;

		Image theGif = null;
		switch( m_nState )
		{
			case E_BUTTONIMAGE_STATE_UP:
				theGif = m_eGifImageUpState.getGif();
				break;
			case E_BUTTONIMAGE_STATE_DN:
				theGif = m_eGifImageDnState.getGif();
				break;
			case E_BUTTONIMAGE_STATE_OFF:
				theGif = m_eGifImageOffState.getGif();
				break;
			default:
				if( AConfig.DEBUG ) System.out.println( "Unknown case" );
				break;
		}
		return theGif;
	}

	public boolean shouldProcessOnMouseDown( EEvtMouseDown eEvtMouseDown )
	{
		if( false == isVisible() )
			return false;
			
		if( E_BUTTONIMAGE_STATE_OFF == m_nState )
			return false;

		Point pt = new Point( eEvtMouseDown.x, eEvtMouseDown.y );
		return getRect().contains( pt );
	}

	public int onMouseDown( EEvtMouseDown eEvtMouseDown )
	{
		m_nState = E_BUTTONIMAGE_STATE_DN;

		displayImage();

		return 1;
	}

	public void displayImage()
	{
		Rectangle r = getRect();
		Image imageScreen = getAMain().getScreen();
		Image theGif = getGif();
		if( null != theGif )
		{
			Graphics gScreenGif = imageScreen.getGraphics();
			gScreenGif.drawImage( theGif, r.x,r.y, getApplet() );
		}
	}
	
	public void setState( short state ) { m_nState = state; }
	public EGifImage getGifImageUp()		{ return m_eGifImageUpState; }
	public EGifImage getGifImageDn()		{ return m_eGifImageDnState; }
	public EGifImage getGifImageOff()		{ return m_eGifImageOffState; }
}