// EInfo.java

package com.walkerbros.common.edata;

import com.walkerbros.common.ejava.*;
import com.walkerbros.common.eclasses.*;

public class EInfo extends EObject
{
	public EInfo() 
	{
	}
	
	public boolean isType( short type )
	{
		if( EDefines.E_OBJ_TYPE_INFO == type )
			return true;
		return false;
	}
	
	public boolean isOfType( short type )
	{
		if( EDefines.E_OBJ_TYPE_INFO == type )
			return true;
		return super.isOfType( type );
	}
}