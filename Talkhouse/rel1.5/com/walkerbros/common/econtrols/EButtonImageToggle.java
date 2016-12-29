// EButtonImageToggle.java

package com.walkerbros.common.econtrols;

import java.awt.*;

import com.walkerbros.common.ejava.*;
import com.walkerbros.common.ejava.eevent.*;
import com.walkerbros.common.eimage.*;
//import com.walkerbros.projects.walkerbros.aclasses.*;
//import com.walkerbros.projects.walkerbros.ajava.*;

public class EButtonImageToggle extends EButtonImage
{
	private boolean m_bSelected	= false;

	public EButtonImageToggle() 
	{
	}
	
	public boolean isType( short type )
	{
		if( EDefines.E_OBJ_TYPE_BUTTONIMAGETOGGLE == type )
			return true;
		return false;
	}
	
	public boolean isOfType( short type )
	{
		if( EDefines.E_OBJ_TYPE_BUTTONIMAGETOGGLE == type )
			return true;
		return super.isOfType( type );
	}

	public void loadFromDisk( EFile eFile, EErr err )
	{
		super.loadFromDisk( eFile,err );
		if( err.error() )
			return;
		
		m_bSelected = eFile.readBool( err );
		if( err.error() )
			return;
	}
	
	public void saveToDisk( EFile eFile, EErr err )
	{
		super.saveToDisk( eFile,err );
		if( err.error() )
			return;
			
		eFile.writeBool( m_bSelected,err );
		if( err.error() )
			return;
	}

	public Image getGif()
	{
		Image theGif = super.getGif();

		if( null == theGif )
			return null;

		if( true == m_bSelected )
			return super.getGifImageDn().getGif();
		
		return theGif;
	}
	
	public void toggle() { m_bSelected = m_bSelected ? false : true; }
}