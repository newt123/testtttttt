// EPanelComponent.java

package com.walkerbros.common.econtrols;

import java.awt.*;

public class EPanelComponent extends Panel
{
	private Component m_comp = null;

	public EPanelComponent() 
	{
		super();
	}

	public void setComponent( Component comp )
	{
		m_comp = comp;
	}

	public void update( Graphics g )
	{
		paint( g );
	}
	public void paint( Graphics g )
	{
		if( null != m_comp )
		{
			m_comp.paint( g );
		}
	}
}