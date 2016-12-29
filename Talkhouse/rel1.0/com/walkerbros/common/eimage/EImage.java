// EImage.java

package com.walkerbros.common.eimage;

import java.awt.*;

import com.walkerbros.common.ejava.*;
import com.walkerbros.common.eclasses.*;
import com.walkerbros.projects.walkerbros.aclasses.*;

public class EImage extends EOnScreen
{
	EGifImage m_eGifImage = new EGifImage();
	
	public EImage() 
	{
	}
	
	public boolean isType( short type )
	{
		if( EDefines.E_OBJ_TYPE_IMAGE == type )
			return true;
		return false;
	}
	
	public boolean isOfType( short type )
	{
		if( EDefines.E_OBJ_TYPE_IMAGE == type )
			return true;
		return super.isOfType( type );
	}

	public void loadFromDisk( EFile eFile, EErr err )
	{
		super.loadFromDisk( eFile,err );
		if( err.error() )
			return;
			
		m_eGifImage.loadFromDisk( eFile,err );
		if( err.error() )
			return;
	}
	
	public void saveToDisk( EFile eFile, EErr err )
	{
		super.saveToDisk( eFile,err );
		if( err.error() )
			return;
			
		m_eGifImage.saveToDisk( eFile,err );
		if( err.error() )
			return;
	}

	public void createObject( EErr err )
	{
		super.createObject( err );
		if( err.error() )
			return;
			
		AObjectController aObjCtrl = getObjCtrl();
		
		m_eGifImage.createObject( err,aObjCtrl,this.isOfType((short)EDefines.E_OBJ_TYPE_BACKGROUND) );
		if( err.error() )
			return;
	}

	public short getBitmapID()
	{
		if( null == m_eGifImage )
			return -1;

		return m_eGifImage.getGifImageID();
	}

	public Image getGif()
	{
		if( null == m_eGifImage )
			return null;

		return m_eGifImage.getGif();
	}
}