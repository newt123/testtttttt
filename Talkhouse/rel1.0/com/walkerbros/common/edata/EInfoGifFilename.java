// EInfoGifFilename.java

package com.walkerbros.common.edata;

import com.walkerbros.common.ejava.*;

public class EInfoGifFilename extends EInfoString
{
	public EInfoGifFilename() 
	{
	}
	
	public boolean isType( short type )
	{
		if( EDefines.E_OBJ_TYPE_INFO_GIF_FILENAME == type )
			return true;
		return false;
	}
	
	public boolean isOfType( short type )
	{
		if( EDefines.E_OBJ_TYPE_INFO_GIF_FILENAME == type )
			return true;
		return super.isOfType( type );
	}
}