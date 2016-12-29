// EWidget.java

package com.walkerbros.common.widget;
// Joes's edit.
/*
 * EBolt.java - definition of the bolt object for the Widget component.
 *
 * 10/31/03 (jwalker) - defined file for the Talkhouse application.
 * 11/14/03 (jcoder)  - added comments to define the default construtor definition.
 * 11/14/03 (jcoder)  - added even more comments.
 */
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