// EObject.java

package com.walkerbros.common.eclasses;

import java.applet.*;
import java.awt.Container;

import com.walkerbros.projects.walkerbros.aclasses.*;
import com.walkerbros.common.ejava.*;

public class EObject extends ELinkItem
{
	private AObjectController m_pObjCtrl	= null;
	private boolean	m_bPermanent					= false;
	private boolean	m_bPermanentHelp			= false;
	private short 	m_nObjectID						= EDefines.E_OBJ_ID_DEFAULT;
	private short		m_nGroup							=	EDefines.E_GRP_ID_ALL;
	private short 	m_nStatus							= EDefines.E_OBJ_STATUS_ACTIVE;
	private short 	m_nConditions					= EDefines.E_OBJ_CONDITIONS_NONE;
	private short		m_nHelpStrID					= ABubbleHelp.A_HELPSTR_NONE;
	private boolean	m_bCreated						= false;
	private EObject m_pNext								= null;
	private EObject m_pPrev								= null;
	
	public EObject() 
	{
	}
	
	public boolean isType( short type )
	{
		return false;
	}
	
	public boolean isOfType( short type )
	{
		return false;
	}
	
	public void loadFromDisk( EFile eFile, EErr err )
	{
		m_bPermanent = eFile.readBool( err );
		if( err.error() )
			return;
		
		m_bPermanentHelp = eFile.readBool( err );
		if( err.error() )
			return;
		
		m_nObjectID = eFile.readShort( err );
		if( err.error() )
			return;
		
		m_nGroup = eFile.readShort( err );
		if( err.error() )
			return;
		
		m_nStatus = eFile.readShort( err );
		if( err.error() )
			return;
		
		m_nConditions = eFile.readShort( err );
		if( err.error() )
			return;
		
		m_nHelpStrID = eFile.readShort( err );
		if( err.error() )
			return;
	}
	
	public void saveToDisk( EFile eFile, EErr err )
	{
		eFile.writeBool( m_bPermanent,err );
		if( err.error() )
			return;
		
		eFile.writeBool( m_bPermanentHelp,err );
		if( err.error() )
			return;
		
		eFile.writeShort( m_nObjectID,err );
		if( err.error() )
			return;
		
		eFile.writeShort( m_nGroup,err );
		if( err.error() )
			return;
		
		eFile.writeShort( m_nStatus,err );
		if( err.error() )
			return;
		
		eFile.writeShort( m_nConditions,err );
		if( err.error() )
			return;
		
		eFile.writeShort( m_nHelpStrID,err );
		if( err.error() )
			return;
	}
	
	public boolean 		isPermanent() 	{ return m_bPermanent; }
	public short 			getObjectID()		{ return m_nObjectID; }
	public short 			getGroup()			{ return m_nGroup;		}
	public short 			getStatus()			{ return m_nStatus;	}
	public short 			getConditions()	{ return m_nConditions;	}
	public short 			getHelpStrID()	{ return m_nHelpStrID; }
	public void				setObjCtrl( AObjectController objCtrl )	{ m_pObjCtrl = objCtrl; }
	public AObjectController getObjCtrl()					{ return m_pObjCtrl; }
	public AMain 			getAMain()									{ return m_pObjCtrl.getAMain(); }
	public Container	getApplet()									{ return m_pObjCtrl.getContainer(); }
	public void 			createObject( EErr err)			{ return; }
	public void				setCreated( boolean bVal )	{ m_bCreated = bVal; }
	public boolean 		isCreated()									{ return m_bCreated; }
	public void				setStatus( short status )		{ m_nStatus = status; }
	public boolean 		onTimer()										{ return true; }
	public void				setNext( EObject next )	{ m_pNext = next; }
	public EObject		getNext()								{ return m_pNext;		}
	public void				setPrev( EObject prev )	{ m_pPrev = prev; }
	public EObject		getPrev()								{ return m_pPrev;		}
	public boolean 		isPermanentHelp()				{ return m_bPermanentHelp; }
}