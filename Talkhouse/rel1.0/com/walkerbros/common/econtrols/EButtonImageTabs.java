// EButtonImageTabs.java

package com.walkerbros.common.econtrols;

import java.awt.*;

import com.walkerbros.common.eclasses.*;
import com.walkerbros.common.eimage.*;
import com.walkerbros.common.ejava.*;
import com.walkerbros.common.ejava.eaction.*;
import com.walkerbros.projects.walkerbros.aclasses.*;

public class EButtonImageTabs extends EButtonImageRadio
{
	EGifImage m_eUpStateDnLf 	= new EGifImage();
	EGifImage m_eUpStateDnRt 	= new EGifImage();

	public EButtonImageTabs() 
	{
	}
	
	public boolean isType( short type )
	{
		if( EDefines.E_OBJ_TYPE_BUTTONIMAGETABS == type )
			return true;
		return false;
	}
	
	public boolean isOfType( short type )
	{
		if( EDefines.E_OBJ_TYPE_BUTTONIMAGETABS == type )
			return true;
		return super.isOfType( type );
	}

	public void loadFromDisk( EFile eFile, EErr err )
	{
		super.loadFromDisk( eFile,err );
		if( err.error() )
			return;
			
		m_eUpStateDnLf.loadFromDisk( eFile,err );
		if( err.error() )
			return;
			
		m_eUpStateDnRt.loadFromDisk( eFile,err );
		if( err.error() )
			return;
	}
	
	public void saveToDisk( EFile eFile, EErr err )
	{
		super.saveToDisk( eFile,err );
		if( err.error() )
			return;
	
		m_eUpStateDnLf.saveToDisk( eFile,err );
		if( err.error() )
			return;

		m_eUpStateDnRt.saveToDisk( eFile,err );
		if( err.error() )
			return;

	}

	public void createObject( EErr err )
	{
		super.createObject( err );
		if( err.error() )
			return;

		AObjectController aObjCtrl = getObjCtrl();
		
		m_eUpStateDnLf.createObject( err,aObjCtrl,false );
		if( err.error() )
			return;
		
		m_eUpStateDnRt.createObject( err,aObjCtrl,false );
		if( err.error() )
			return;
	}

	public Image getGif()
	{
		Image theGif = null;
		short index = super.getBtnGroupIndex();
		short indexSelected = super.getBtnGroupIndexSelected();
		
		if( index == indexSelected )
		{
			return super.getGifImageDn().getGif();
		}

		if( (index-1) == indexSelected )
		{
			if( null != (theGif = m_eUpStateDnLf.getGif()) )
				return theGif;
		}

		if( (index+1) == indexSelected )
		{
			if( null != (theGif = m_eUpStateDnRt.getGif()) )
				return theGif;
		}
		return super.getGifImageUp().getGif();
	}

	public EGifImage getBitmapUpStateDnLf()	{ return m_eUpStateDnLf; }
	public EGifImage getBitmapUpStateDnRt()	{ return m_eUpStateDnRt; }
}