// EFile.java

package com.walkerbros.common.ejava;

import java.io.*;
import java.net.*;

public class EFile
{
	public static final int FLIP_NONE = 0;
	public static final int FLIP_16 	= 1;
	public static final int FLIP_32 	= 2;
	public static final int FLIP_BOOL = 3;
	
	URL 						m_urlPath = null;
	DataInputStream m_dIStream = null;
	
	public EFile( String strPath ) 
	{
		try
		{
			m_urlPath = new URL( strPath );
		}
		catch( MalformedURLException e )
        {
			System.out.println(e.toString());
        }
	}
	
	public void open( EErr err )
	{
		err.clear();
		
		try
		{
			m_dIStream = new DataInputStream( m_urlPath.openStream() );
		}
		catch( Exception e )
		{
			err.setErrCode( EErr.errSYSTEM );
			System.out.println(e.toString());
		}
	}
	
	public void read( byte[] buf, EErr err )
	{
		err.clear();
		
		try
		{
			m_dIStream.readFully( buf );
		}
		catch( Exception e )
		{
			err.setErrCode( EErr.errSYSTEM );
			System.out.println(e.toString());
		}
	}
	
	public short readShort( EErr err )
	{
		err.clear();
		
		short sNum = 0;
		try
		{
			byte[] buf = new byte[2];
			m_dIStream.readFully( buf );
			sNum = (short)( (buf[1] << 8) | (buf[0] & 0x00ff) );
		}
		catch( Exception e )
		{
			err.setErrCode( EErr.errSYSTEM );
			System.out.println(e.toString());
		}
		
		return sNum;
	}
	
	public long readLong( EErr err )
	{
		err.clear();
		
		long lNum = 0;
		try
		{
			byte[] buf = new byte[4];
			m_dIStream.readFully( buf );
			lNum = (long)((buf[3] << 24) | ((buf[2] << 16) & 0x00ff0000) |
												((buf[1] << 8) & 0x0000ff00) | (buf[0] & 0x000000ff) );
		}
		catch( Exception e )
		{
			err.setErrCode( EErr.errSYSTEM );
			System.out.println(e.toString());
		}
		
		return lNum;
	}
	
	public boolean readBool( EErr err )
	{
		boolean bool = false;
		
		long lNum = this.readLong( err );
		if( EErr.errNOERR == (err.getErrCode()) )
		{
			bool = 0 != lNum;
		}
		
		return bool;
	}
	
	public void write( byte[] buf, EErr err )
	{
	/*
		err.clear();
		
		try
		{
			m_dIStream.readFully( buf );
		}
		catch( Exception e )
		{
			err.setErrCode( EErr.errSYSTEM );
			System.out.println(e.toString());
		}
	*/
	}
	
	public void writeShort( short sNum,EErr err )
	{
	/*
		err.clear();
		
		short sNum = 0;
		try
		{
			byte[] buf = new byte[2];
			m_dIStream.readFully( buf );
			sNum = (short)((buf[1] * 256) + buf[0]);
		}
		catch( Exception e )
		{
			err.setErrCode( EErr.errSYSTEM );
			System.out.println(e.toString());
		}
		
		return sNum;
	*/
	}
	
	public void writeLong( long lNum,EErr err )
	{
	/*
		err.clear();
		
		long lNum = 0;
		try
		{
			byte[] buf = new byte[4];
			m_dIStream.readFully( buf );
			lNum = (long)((buf[3] * 256 * 256 * 256) +(buf[2] * 256 * 256) + (buf[1] * 256) + buf[0]);
		}
		catch( Exception e )
		{
			err.setErrCode( EErr.errSYSTEM );
			System.out.println(e.toString());
		}
		
		return lNum;
	*/
	}
	
	public void writeBool( boolean bVal, EErr err )
	{
	/*
		boolean bool = false;
		
		long lNum = this.readLong( err );
		if( EErr.errNOERR == (err.getErrCode()) )
		{
			bool = 0 != lNum;
		}
		
		return bool;
	*/
	}
}