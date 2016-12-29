// EWidget.java

package com.walkerbros.common.widget;

public class EWidget
{



	ENut  m_eNut 	= new ENut(ENut.E_NUT_STATE_TOTALLY_SCREWED_UP);
	EBolt m_eBolt 	= new EBolt(EBolt.E_BOLT_STATE_TOTALLY_SCREWED_UP);

	/*
 	 * Dummy construtor was introduced after a meeting with the standards committee.
 	 * It was decided that it was only good form to maintain these since there might be
         * cases in the future were objected will be created and not initialized.
         */
	public EWidget() 
	{
	}
}