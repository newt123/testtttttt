
package com.walkerbros.common.widget;

public class EBolt
{


	public static final short E_BOLT_STATE_SCREWED_OFF			= 0;
	public static final short E_BOLT_STATE_SCREWED_ON			= 1;
	
	// Configured for a broken bolt.
	public static final short E_BOLT_STATE_TOTALLY_SCREWED_UP		= -1;

	public short m_currState = E_BOLT_STATE_SCREWED_OFF;

	/*
 	 * Dummy construtor was introduced after a meeting with the standards committee.
 	 * It was decided that it was only good form to maintain these since there might be
         * cases in the future were objected will be created and not initialized.
         */
	public EBolt() 
	{
	}

	public EBolt(int p_state) 
	{
		m_currState = p_state;

	}
}