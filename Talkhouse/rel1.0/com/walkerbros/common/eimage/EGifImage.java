// EGifImage.java

package com.walkerbros.common.eimage;

import java.awt.*;
import java.net.*;
import java.io.*;

//import com.walkerbros.common.*;
import com.walkerbros.common.ejava.*;
import com.walkerbros.common.edata.*;

import com.walkerbros.projects.walkerbros.aclasses.*;
import com.walkerbros.projects.walkerbros.ajava.AConfig;

public class EGifImage // implements EEnvironment
{
	public static final short E_GIF_EMPTY_ID						= -1;
	// animations re-use certain bitmaps,
	//	consequently we use ids above 15000 to indicate that the bitmap doesn't need to be re-created
	public static final int 	E_GIF_DUPLICATED_IDS			= 15000;
				
	private short m_nID 			= E_GIF_EMPTY_ID;
	private Image	m_imageGif	= null;
	
	public EGifImage() 
	{
	}
		
	public void loadFromDisk( EFile eFile, EErr err )
	{
		m_nID = eFile.readShort( err );
		if( err.error() )
			return;
	}
	
	public void saveToDisk( EFile eFile, EErr err )
	{
		eFile.writeShort( m_nID,err );
		if( err.error() )
			return;
	}
	
	public void createObject( EErr err,AObjectController aObjCtrl,boolean bIsBackground )
	{		
		if( bIsBackground )
		{
			m_imageGif = aObjCtrl.getContainer().createImage( 640,480 );
			return;
		}
	
		if( E_GIF_EMPTY_ID == m_nID )
			return;
	
		EInfoGifFilename gif =
				(EInfoGifFilename)aObjCtrl.findObject( (short)EDefines.E_OBJ_TYPE_INFO_GIF_FILENAME, true,
																										(short)(m_nID+1), (short)EDefines.E_GRP_ID_ALL );
		if( null == gif )
			return;
	
		String strPathToData = aObjCtrl.getPathToData();
		String strImage = strPathToData + gif.getString();
	
		if( true == AConfig.APPLET )
		{
			URL urlImage = null;
			try
			{
				urlImage = new URL( "file", "", strImage );
			}
			catch( MalformedURLException e )
			{
				err.setErrCode( EErr.errSYSTEM );
				System.out.println(e.toString());
			}
			strImage = urlImage.toString();
		}
		
		strImage = strImage.replace( '\\',File.separatorChar );
		strImage = strImage.trim();
		
		if( err.errorFree() )
		{
			Container container = aObjCtrl.getContainer();
			Toolkit tk = container.getToolkit();
			MediaTracker mt = new MediaTracker( container );
			m_imageGif = tk.getImage( strImage );
			
			mt.addImage( m_imageGif,0 );
			try
			{
				mt.waitForID(0);
			}
			catch( InterruptedException e )
			{
				System.out.println( "Tracker was interrupted!" );
			}
		}
	}
	
	public short getGifImageID() 	{ return m_nID; }
	public Image getGif()					{ return m_imageGif; }
}