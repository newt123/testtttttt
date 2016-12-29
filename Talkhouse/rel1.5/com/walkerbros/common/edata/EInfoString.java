// EInfoString.java

package com.walkerbros.common.edata;

import com.walkerbros.common.ejava.*;

public class EInfoString extends EInfo
{
	private String m_string;
	
	public EInfoString() 
	{
	}
	
	public boolean isType( short type )
	{
		if( EDefines.E_OBJ_TYPE_INFO_STRING == type )
			return true;
		return false;
	}
	
	public boolean isOfType( short type )
	{
		if( EDefines.E_OBJ_TYPE_INFO_STRING == type )
			return true;
		return super.isOfType( type );
	}

	public void loadFromDisk( EFile eFile, EErr err )
	{
		super.loadFromDisk( eFile,err );
		if( err.error() )
			return;

		short len = eFile.readShort( err );
		if( err.error() )
			return;
			
		if( 0 != len )
		{
			byte[] buf = new byte[len+1];
			eFile.read( buf, err);
			if( err.error() )
				return;
				
			m_string = new String( buf );
		}
	}
	
	public void saveToDisk( EFile eFile, EErr err )
	{
		super.saveToDisk( eFile,err );
		if( err.error() )
			return;
		
		short len = (short)m_string.length();
		
		eFile.writeShort( len,err );
		if( err.error() )
			return;
			
		if( 0 != len )
		{
			// NEED TO ADD 1 HERE I THINK
			eFile.write( m_string.getBytes(), err);
			if( err.error() )
				return;
		}
	}

	public String getString() { return m_string; }
}