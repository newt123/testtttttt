// EErr.java

package com.walkerbros.common.ejava;

public class EErr
{
	public static final int errNOERR 				= 0;
	public static final int errLOWMEM				= 1;
	public static final int errSYSTEM				= 2;
	public static final int errVIOLATION		= 3; 
	public static final int errDISKFULL			= 4;  
	public static final int errNOFILE				= 5;
	public static final int errNODLL				= 6;
	public static final int errWRONGVERSION	= 7;
	public static final int errNOCD					= 8;
	
	private int m_nErrCode = errNOERR;
	
	public EErr() 
	{
	}
	
	public void clear() 
	{
		setErrCode( errNOERR );
	}
	public boolean error()
	{
		return errNOERR != m_nErrCode;
	}
	public boolean errorFree()
	{
		return errNOERR == m_nErrCode;
	}
	
	public void setErrCode( int errCode ) 
	{
		m_nErrCode = errCode;
	}
	public int getErrCode() 
	{
		return m_nErrCode;
	}
}