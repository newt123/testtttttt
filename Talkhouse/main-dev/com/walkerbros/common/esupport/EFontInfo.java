package com.walkerbros.common.esupport;

import com.walkerbros.common.eclasses.*;
import com.walkerbros.common.ejava.*;

public class EFontInfo extends EOnScreen
{
	public static final int E_FONTINFO_ID_NONE 				= -1;

	public static final int E_FONTINFO_FACE_NORMAL 		= 0;
	public static final int E_FONTINFO_FACE_BOLD			= 1;
	public static final int E_FONTINFO_FACE_ITALIC 		= 2;
	public static final int E_FONTINFO_FACE_UNDERLINE	= 4;
	public static final int E_FONTINFO_FACE_STRIKEOUT	= 8;

	private EString m_strNameWin		= new EString( "Arial Bold" );
	private EString m_strNameMac		= new EString( "Arial" );

	private short m_nIDWin		= E_FONTINFO_ID_NONE;
	private short m_nIDMac		= E_FONTINFO_ID_NONE;
	private short m_nSizeWin	= 80;
	private short m_nSizeMac	= 10;
	private short m_nFaceWin	= E_FONTINFO_FACE_NORMAL;
	private short m_nFaceMac	= E_FONTINFO_FACE_NORMAL;
	private short m_nColor		= EDefines.E_COLOR_BLACK;

	public EFontInfo() 
	{
	}

	public void loadFromDisk( EFile eFile, EErr err )
	{
		m_strNameWin.loadFromDisk( eFile,err );
		if( err.error() )
			return;
	
		m_strNameMac.loadFromDisk( eFile,err );
		if( err.error() )
			return;
		
		m_nSizeWin = eFile.readShort( err );
		if( err.error() )
			return;
		m_nSizeMac = eFile.readShort( err );
		if( err.error() )
			return;
		m_nFaceWin = eFile.readShort( err );
		if( err.error() )
			return;
		m_nFaceMac = eFile.readShort( err );
		if( err.error() )
			return;
		m_nColor = eFile.readShort( err );
		if( err.error() )
			return;
	}
	
	public void saveToDisk( EFile eFile, EErr err )
	{
		m_strNameWin.saveToDisk( eFile,err );
		if( err.error() )
			return;
	
		m_strNameMac.saveToDisk( eFile,err );
		if( err.error() )
			return;
		
		eFile.writeShort( m_nSizeWin,err );
		if( err.error() )
			return;
		eFile.writeShort( m_nSizeMac,err );
		if( err.error() )
			return;
		eFile.writeShort( m_nFaceWin,err );
		if( err.error() )
			return;
		eFile.writeShort( m_nFaceMac,err );
		if( err.error() )
			return;
		eFile.writeShort( m_nColor,err );
		if( err.error() )
			return;
	}

	public EString getNameWin()	{ return m_strNameWin; }
	public EString getNameMac()	{ return m_strNameMac; }
	public short getIDWin()		{ return m_nIDWin; }
	public short getIDMac()		{ return m_nIDMac; }
	public short getSizeWin()	{ return m_nSizeWin; }
	public short getSizeMac()	{ return m_nSizeMac; }
	public short getFaceWin()	{ return m_nFaceWin; }
	public short getFaceMac()	{ return m_nFaceMac; }
	public short getColor()		{ return m_nColor; }

/*
	public void setNameWin( EString eString ) { m_strNameWin = eString; }
	public void setNameMac( EString eString ) { m_strNameMac = eString; }
	public void setSizeWin( short	size )	{ m_nSizeWin = size; }
	public void setSizeMac( short	size )	{ m_nSizeMac = size; }
	public void setFaceWin( short	face )	{ m_nFaceWin = face; }
	public void setFaceMac( short	face )	{ m_nFaceMac = face; }
	public void setColor( short color)		{ m_nColor = color;  }
*/
}