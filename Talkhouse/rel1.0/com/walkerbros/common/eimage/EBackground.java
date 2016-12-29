// EBackground.java

package com.walkerbros.common.eimage;

import com.walkerbros.common.ejava.*;

public class EBackground extends EImage
{
	public EBackground() 
	{
	}
	
	public boolean isType( short type )
	{
		if( EDefines.E_OBJ_TYPE_BACKGROUND == type )
			return true;
		return false;
	}
	
	public boolean isOfType( short type )
	{
		if( EDefines.E_OBJ_TYPE_BACKGROUND == type )
			return true;
		return super.isOfType( type );
	}
}