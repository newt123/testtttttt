// ECursor.java

package com.walkerbros.common.ejava;

import java.awt.Container;
import java.awt.Cursor;

public class ECursor
{	
	public ECursor() 
	{
	}
	public static void setCursor( Container container, int nCursorID )
	{
		if( null == container )
			return;
		container.setCursor( new Cursor(nCursorID) );
	}
}