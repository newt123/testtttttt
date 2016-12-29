package com.walkerbros.common.eclasses;

import com.walkerbros.common.ejava.*;

public class EString
{
	private String m_str							= new String();
	private StringBuffer m_strBuffer	= new StringBuffer();

	public EString() 
	{
	}

	public EString( String str ) 
	{
		m_str = str;
	}

	public void loadFromDisk( EFile eFile, EErr err )
	{
		int len = eFile.readShort( err );
		if( err.error() )
			return;

		byte[] buf = new byte[len+1];
		eFile.read( buf,err );
		if( err.error() )
			return;

		for( int i=0; i<buf.length; i++ )
		{
			m_strBuffer.append( (char)buf[i] );
		} 
		m_strBuffer.setLength( len );

		m_str = new String( m_strBuffer );
	}
	
	public void saveToDisk( EFile eFile, EErr err )
	{
/*
		int len = m_str.length();
		
		eFile.writeShort( len,err );
		if( err.error() )
			return;

		for( int i=0; i<len; i++ )
		{
			eFile.writeShort( m_str.charAt(i),err );
			if( err.error() )
				return;
		}
*/
	}

	public String getString()
	{
		return m_str;
	}
}