// EEvtPaint.java

package com.walkerbros.common.ejava.eevent;

import java.awt.*;

public class EEvtPaint
{
	Graphics m_g;
	
	public EEvtPaint() 
	{
	}
	
	public void setGraphics( Graphics g )
	{
		m_g = g;
	}
	public Graphics getGraphics()
	{
		return m_g;
	}
}