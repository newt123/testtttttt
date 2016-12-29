// EButton.java

package com.walkerbros.common.econtrols;

import com.walkerbros.common.eclasses.*;
import com.walkerbros.common.ejava.*;
import com.walkerbros.common.ejava.eaction.*;

public class EButton extends EOnScreen
{
	private short m_nButtonID			= -1;
	private short m_nButtonSubID	= -1;

	public EButton() 
	{
	}
	
	public boolean isType( short type )
	{
		if( EDefines.E_OBJ_TYPE_BUTTON == type )
			return true;
		return false;
	}
	
	public boolean isOfType( short type )
	{
		if( EDefines.E_OBJ_TYPE_BUTTON == type )
			return true;
		return super.isOfType( type );
	}

	public void loadFromDisk( EFile eFile, EErr err )
	{
		super.loadFromDisk( eFile,err );
		if( err.error() )
			return;
		
		m_nButtonID = eFile.readShort( err );
		if( err.error() )
			return;
		
		m_nButtonSubID = eFile.readShort( err );
		if( err.error() )
			return;
	}
	
	public void saveToDisk( EFile eFile, EErr err )
	{
		super.saveToDisk( eFile,err );
		if( err.error() )
			return;
			
		eFile.writeShort( m_nButtonID,err );
		if( err.error() )
			return;
			
		eFile.writeShort( m_nButtonSubID,err );
		if( err.error() )
			return;
	}

	public void takeAction()
	{
		EActBtn eActBtn = new EActBtn();

		eActBtn.setButtonID( m_nButtonID );
		eActBtn.setButtonSubID( m_nButtonSubID );
		getAMain().takeActionButton( eActBtn );
	}

	public short getButtonID()		{ return m_nButtonID; }
	public short getButtonSubID()	{ return m_nButtonSubID; }
}