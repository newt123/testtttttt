// EButtonImageRadio.java

package com.walkerbros.common.econtrols;

import java.awt.*;

import com.walkerbros.common.eclasses.*;
import com.walkerbros.common.ejava.*;
import com.walkerbros.common.ejava.eaction.*;

public class EButtonImageRadio extends EButtonImage
{
	private short m_nBtnGroupID			= -1;
	private short m_nBtnGroupIndex	= -1;
	private short m_nBtnGroupIndexSelected	= -1;

	public EButtonImageRadio() 
	{
	}
	
	public boolean isType( short type )
	{
		if( EDefines.E_OBJ_TYPE_BUTTONIMAGERADIO == type )
			return true;
		return false;
	}
	
	public boolean isOfType( short type )
	{
		if( EDefines.E_OBJ_TYPE_BUTTONIMAGERADIO == type )
			return true;
		return super.isOfType( type );
	}

	public void loadFromDisk( EFile eFile, EErr err )
	{
		super.loadFromDisk( eFile,err );
		if( err.error() )
			return;
		
		m_nBtnGroupID = eFile.readShort( err );
		if( err.error() )
			return;
		
		m_nBtnGroupIndex = eFile.readShort( err );
		if( err.error() )
			return;
	}
	
	public void saveToDisk( EFile eFile, EErr err )
	{
		super.saveToDisk( eFile,err );
		if( err.error() )
			return;
			
		eFile.writeShort( m_nBtnGroupID,err );
		if( err.error() )
			return;
			
		eFile.writeShort( m_nBtnGroupIndex,err );
		if( err.error() )
			return;
	}

	public Image getGif()
	{
		Image theGif = super.getGif();

		if( null == theGif )
			return null;

		if( m_nBtnGroupIndex == m_nBtnGroupIndexSelected )
			return super.getGifImageDn().getGif();
		
		return theGif;
	}

	public short	getBtnGroupID()													{ return m_nBtnGroupID; }
	public short	getBtnGroupIndex()											{ return m_nBtnGroupIndex; }
	public void		setBtnGroupIndexSelected( short index )	{ m_nBtnGroupIndexSelected = index; }
	public short	getBtnGroupIndexSelected()							{ return m_nBtnGroupIndexSelected; }
}