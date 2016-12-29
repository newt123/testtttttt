// ELabelPanel.java

package com.walkerbros.common.econtrols;

import java.awt.*;

public class ELabelPanel extends Panel
{
	private Component m_comp = null;

	public ELabelPanel() 
	{
	}

	public void setComponent( Component comp )
	{
		m_comp = comp;
	}

	public void update( Graphics g )
	{
System.out.println( "update for EPanelComponent!!" );
		paint( g );
	}
	public void paint( Graphics g )
	{
		if( null != m_comp )
		{
System.out.println( "paint for EPanelComponent!!" );
			m_comp.paint( g );
		}
	}
}