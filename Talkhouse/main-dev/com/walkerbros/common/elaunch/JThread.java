// JThread.java

package com.walkerbros.common.elaunch;

import com.walkerbros.common.ejava.EMsgPoster;
import com.walkerbros.common.ejava.EMsg;

public class JThread extends Thread
{
	static EMsgPoster m_eMsgPoster = null;
	
	// post message 5 times per second
	static final int INCREMENT = 200;
	
	public JThread( EMsgPoster eMsgPoster ) 
	{
		m_eMsgPoster = eMsgPoster;
	}
	
	public void run()
	{
		long timeValue = System.currentTimeMillis();
		while( true )
		{
			long timeTarget = timeValue + INCREMENT;
			long timeSleep = timeTarget - System.currentTimeMillis();
			if( timeSleep > 0 )
			{
				try
				{
					Thread.sleep( timeSleep );
				}
				catch( InterruptedException e ){}
			}
			if( null != m_eMsgPoster )
			{
				m_eMsgPoster.postMsg( EMsg.E_MSG_ACT_TIMER );
				timeValue = System.currentTimeMillis();
			}
		}
	}
}