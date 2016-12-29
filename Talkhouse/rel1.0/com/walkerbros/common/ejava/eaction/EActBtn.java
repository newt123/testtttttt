// EActBtn.java

package com.walkerbros.common.ejava.eaction;

public class EActBtn
{
	private short m_nButtonID;
	private short m_nButtonSubID;
	
	public EActBtn() 
	{
	}
	
	public void setButtonID( short id )
	{
		m_nButtonID = id;
	}
	public short getButtonID()
	{
		return m_nButtonID;
	}
	
	public void setButtonSubID( short subid )
	{
		m_nButtonSubID = subid;
	}
	public short getButtonSubID()
	{
		return m_nButtonSubID;
	}
}