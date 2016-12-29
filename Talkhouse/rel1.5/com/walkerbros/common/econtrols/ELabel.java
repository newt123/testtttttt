// ELabel.java

package com.walkerbros.common.econtrols;

import java.awt.*;
import java.applet.*;
import javax.swing.*;

import com.walkerbros.common.eclasses.*;
import com.walkerbros.common.esupport.*;
import com.walkerbros.common.ejava.*;
import com.walkerbros.common.ejava.eevent.*;

public class ELabel extends EOnScreen
{
	public static final int E_LABEL_ALIGN_LEFT 		= 0;
	public static final int E_LABEL_ALIGN_RIGHT 	= 2;
	public static final int E_LABEL_ALIGN_CENTER	= 6;

	private short m_nAlign					= E_LABEL_ALIGN_RIGHT;

	private EFontInfo m_eFontInfo		= new EFontInfo();
	private Font m_font							= null;
	private EString m_strText				= new EString();
	private JLabel m_jLabel					= new JLabel();
	private EPanelComponent m_panel	= new EPanelComponent();

	private boolean m_bAdded				= false;

	public ELabel() 
	{
	}
	
	public boolean isType( short type )
	{
		if( EDefines.E_OBJ_TYPE_LABEL == type )
			return true;
		return false;
	}
	
	public boolean isOfType( short type )
	{
		if( EDefines.E_OBJ_TYPE_LABEL == type )
			return true;
		return super.isOfType( type );
	}

	public void loadFromDisk( EFile eFile, EErr err )
	{
		super.loadFromDisk( eFile,err );
		if( err.error() )
			return;
		
		m_eFontInfo.loadFromDisk( eFile,err );
		if( err.error() )
			return;

		m_nAlign = eFile.readShort( err );
		if( err.error() )
			return;
		
		m_strText.loadFromDisk( eFile,err );
		if( err.error() )
			return;

		setText( m_strText );
//		m_panel.setComponent( m_jLabel );
		m_panel.setBounds( getX(),getY(),getRect().width,getRect().height );
		
		int size = m_eFontInfo.getSizeWin();
		size /= 10;

		int style = Font.PLAIN;
		switch( m_eFontInfo.getFaceWin() )
		{
			case EFontInfo.E_FONTINFO_FACE_NORMAL:
				style = Font.PLAIN;
				break;
			case EFontInfo.E_FONTINFO_FACE_BOLD:
				style = Font.BOLD;
				break;
			case EFontInfo.E_FONTINFO_FACE_ITALIC:
				style = Font.ITALIC;
				break;
		}

		int align = Font.PLAIN;
		switch( m_nAlign )
		{
			case E_LABEL_ALIGN_LEFT:
				align = SwingConstants.LEFT;
				break;
			case E_LABEL_ALIGN_RIGHT:
				align = SwingConstants.RIGHT;
				break;
			case E_LABEL_ALIGN_CENTER:
				align = SwingConstants.CENTER;
				break;
		}
		m_font = new Font( m_eFontInfo.getNameWin().getString(), style, size );
//		m_jLabel.setFont( m_font );
//		m_jLabel.setHorizontalAlignment( align );
	}
	
	public void saveToDisk( EFile eFile, EErr err )
	{
		super.saveToDisk( eFile,err );
		if( err.error() )
			return;
		
		m_eFontInfo.saveToDisk( eFile,err );
		if( err.error() )
			return;
			
		eFile.writeShort( m_nAlign,err );
		if( err.error() )
			return;
		
		m_strText.saveToDisk( eFile,err );
		if( err.error() )
			return;
	}

	public void setVisible( boolean bVisible )
	{
		super.setVisible( bVisible );
		
//		m_jLabel.setVisible( bVisible );
		m_panel.setVisible( bVisible );

		Container app = getApplet();

		if( bVisible )
		{
			if( !m_bAdded )
			{
				app.add( m_panel );
				m_bAdded = true;
			}
		}
		else
		{
			if( m_bAdded )
			{
				app.remove( m_panel );
				m_bAdded = false;
			}
		}
	}

	public int getX()
	{
		if( E_LABEL_ALIGN_RIGHT == m_nAlign )
			return getRect().x+getRect().width;
		if( E_LABEL_ALIGN_LEFT == m_nAlign )
			return getRect().x;

		return (getRect().width / 2) + getRect().x;
	}

	public int getY() { return getRect().y; }

	public void setText( EString eString )
	{
		m_strText	= eString;
//		m_jLabel.setText( m_strText.getString() );
//		m_jLabel.setBounds( getX(),getY(),getRect().width,getRect().height );
//		m_jLabel.setForeground( Color.black );
	}
	
	public int onPaint( EEvtPaint eEvtPaint )
	{
		m_panel.repaint();
		return EErr.errNOERR;
	}
}