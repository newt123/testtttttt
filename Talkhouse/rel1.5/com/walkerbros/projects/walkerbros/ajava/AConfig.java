// AConfig.java

package com.walkerbros.projects.walkerbros.ajava;

public class AConfig
{
	// -- final release (or development)?
	//------------------------------------------------------------------------
	public static final boolean RELEASE = false;
	public static final boolean DEVELOPMENT = !RELEASE;
	
	// -- applet (or application)?
	//------------------------------------------------------------------------
	public static final boolean APPLET = false;
	
	// -- release version?
	//------------------------------------------------------------------------
	public static final boolean DEBUG = true;
	
	// -- initial section
	//------------------------------------------------------------------------
	public static final int A_STR_INIT_INI 		= ADefAppl.A_STR_CPRT_INI;
	public static final int A_STR_INIT_BIT		= ADefAppl.A_STR_CPRT_BIT;
	
	public AConfig() 
	{
	}
}