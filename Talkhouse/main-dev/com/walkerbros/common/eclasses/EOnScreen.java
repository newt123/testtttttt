// EOnScreen.java

package com.walkerbros.common.eclasses;

import java.awt.*;
import com.walkerbros.common.ejava.*;
import com.walkerbros.projects.walkerbros.aclasses.*;
import com.walkerbros.common.ejava.eevent.*;

public class EOnScreen extends EObject
{
	private Rectangle m_pRect = new Rectangle();
	private boolean		m_bIsVisible = true;
	
	private int				m_nWindowID;
	
	public EOnScreen() 
	{
	}
	
	public boolean isType( short type )
	{
		if( EDefines.E_OBJ_TYPE_ONSCREEN == type )
			return false;
		return true;
	}
	
	public boolean isOfType( short type )
	{
		if( EDefines.E_OBJ_TYPE_ONSCREEN == type )
			return true;
		return super.isOfType( type );
	}

	public void loadFromDisk( EFile eFile, EErr err )
	{
		super.loadFromDisk( eFile,err );
		if( err.error() )
			return;
		
		m_bIsVisible = eFile.readBool( err );
		if( err.error() )
			return;
		
		short l = eFile.readShort( err );
		if( err.error() )
			return;
		
		short t = eFile.readShort( err );
		if( err.error() )
			return;
		
		short r = eFile.readShort( err );
		if( err.error() )
			return;
		
		short b = eFile.readShort( err );
		if( err.error() )
			return;
			
		m_pRect.setBounds( l,t,r-l,b-t );
	}
	
	public void saveToDisk( EFile eFile, EErr err )
	{
		super.saveToDisk( eFile,err );
		if( err.error() )
			return;

		eFile.writeBool( m_bIsVisible,err );
		if( err.error() )
			return;
		
		eFile.writeShort( (short)m_pRect.x,err );
		if( err.error() )
			return;
		
		eFile.writeShort( (short)m_pRect.y,err );
		if( err.error() )
			return;
		
		eFile.writeShort( (short)m_pRect.width,err );
		if( err.error() )
			return;
		
		eFile.writeShort( (short)m_pRect.height,err );
		if( err.error() )
			return;
	}

	public void createObject( EErr err )
	{
		super.createObject( err );
		if( err.error() )
			return;
		
		// now that we know whether object is permanent, its safe to assign a unique id
		m_nWindowID	= getUniqueWindowID();
	}

	public void setRect(Rectangle rect )
	{
		m_pRect.setBounds( rect );
	}
	
	public Rectangle 	getRect()	{ return m_pRect;		}
	public void			 	setVisible( boolean bVisible )	{ m_bIsVisible = bVisible; }
	public boolean		isVisible()											{ return m_bIsVisible; }
	public boolean		isVisibleBasedOnData()					{ return false; }
	
	public boolean contains( Point pt )
	{
		return m_pRect.contains( pt );
	}

	public int getUniqueWindowID()
	{
		int id = EDefines.E_WND_ID_INVALID;

		AObjectController aObjCtrl 	= getObjCtrl();
		AMain 						aMain			= aObjCtrl.getAMain();
		
		if( false == isPermanent() )
		{
			if( EDefines.E_WND_ID_FINAL < aMain.getWindowIDCounter()  )
				aMain.setWindowIDCounter( EDefines.E_WND_ID_INITIAL );

			id = aMain.getWindowIDCounter();
			aMain.incrementWindowIDCounter();
		}
		else
		{
			id = aMain.getWindowIDCounterPerm();
			aMain.incrementWindowIDCounterPerm();
		}

		return id;
	}
	
	public int getWindowID() { return m_nWindowID; }
	
	public int onPaint( EEvtPaint eEvtPaint )
	{
		return EErr.errNOERR;
	}
}