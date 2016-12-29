// JLaunch.java

package com.walkerbros.common.elaunch;

import java.awt.Graphics;

public class JLaunch
{
	static JLaunchApp m_App = null;
	
	public JLaunch() 
	{
	}

	public static void main( String argv[] ) {
		JLaunch jLaunch = new JLaunch();
		jLaunch.begin();
	}

	public void begin() {
		m_App = new JLaunchApp();
		m_App.show();
		m_App.start();
	}
}