// AObjectController.java

package com.walkerbros.projects.walkerbros.aclasses;

import java.io.*;
import java.net.*;
import java.util.*;
import java.applet.*;
import java.awt.Container;
import java.awt.Cursor;

import com.walkerbros.common.*;
import com.walkerbros.common.eclasses.*;
import com.walkerbros.common.econtrols.*;
import com.walkerbros.common.eimage.*;
import com.walkerbros.common.ejava.*;
import com.walkerbros.common.ejava.eevent.*;
import com.walkerbros.common.edata.*;
import com.walkerbros.projects.walkerbros.ajava.*;

public class AObjectController implements EEnvironment
{
	AMain 	m_aMain = null;
	
	short m_nCurrentScene	= ESceneData.E_SCENEDATA_INVALID_SCENE;
	short m_nCurrentGroup	= ESceneData.E_SCENEDATA_INVALID_GROUP;
	boolean m_bMainScreenObjects = true;
	EObject m_pFirstObject;
	EObject m_pLastObject;
	EBackground m_pBackground = null;
	
	public AObjectController() 
	{
	}
	
	public void 				setAMain( AMain aMain ) { m_aMain = aMain; }
	public AMain 				getAMain() 							{ return m_aMain; }
	public Container		getContainer() 					{ return m_aMain.getContainer(); }
	public short				getCurrentScene()				{ return m_nCurrentScene; }
	public short				getCurrentGroup()				{ return m_nCurrentGroup; }
	public EBackground 	getBackground() 				{ return m_pBackground; }
	public EObject 			getFirstObject()				{ return m_pFirstObject; }
	
	public int initializeScene( EEvtInitializeScene eEvtInitializeScene )
	
	{
		EErr err = new EErr();
		
		if( true == m_bMainScreenObjects )
		{
			// new scene, so obviously no changes yet
			//------------------------------------------------------
			m_aMain.setChangesMadeFlag( false );
			m_aMain.setRepairAddedFlag( false );
			
			// and no captured button yet
			//------------------------------------------------------
			m_aMain.setButtonCaptured( null );
		}
		
		// dispose of the current (non-permanent) objects
		//------------------------------------------------------
		disposeNonPermanentObjects();
	
		EObject eObject = null;
																																												
		String strINI = getPathStringFromID( eEvtInitializeScene.nIdFileIni );
		
		EFile eFileINI = new EFile( strINI );
		
		eFileINI.open( err );
		if( err.error() )
			return err.getErrCode();
		
		long lVersion = eFileINI.readLong( err );
		if( err.error() )
			return err.getErrCode();
		
		short nNumObjects = eFileINI.readShort( err );
		if( err.error() )
			return err.getErrCode();
		
		for( int i=0; i<nNumObjects && err.errorFree(); i++ )
		{
			short id = eFileINI.readShort( err );
			if( err.error() )
				return err.getErrCode();
		
			switch( id )
			{
				case EDefines.E_OBJ_TYPE_BACKGROUND:
					eObject = new EBackground();
					m_pBackground = (EBackground)eObject;
					break;
				case EDefines.E_OBJ_TYPE_IMAGE:
					eObject = new EImage();
					break;
				case EDefines.E_OBJ_TYPE_INFO_GIF_FILENAME:
					eObject = new EInfoGifFilename();
					break;
				case EDefines.E_OBJ_TYPE_BUTTONIMAGE:
					eObject = new EButtonImage();
					break;
				case EDefines.E_OBJ_TYPE_BUTTONIMAGERADIO:
					eObject = new EButtonImageRadio();
					break;
				case EDefines.E_OBJ_TYPE_BUTTONIMAGETABS:
					eObject = new EButtonImageTabs();
					break;
				case EDefines.E_OBJ_TYPE_BUTTONIMAGETOGGLE:
					eObject = new EButtonImageToggle();
					break;
				case EDefines.E_OBJ_TYPE_BUTTONIMAGETIMER:
					eObject = new EButtonImageTimer();
					break;
				default:
					break;
			}

			boolean bSeenBefore = false; // for now
			// now 'customize' the object by reading data from the INI file
			if( null != eObject )
			{
				eObject.setObjCtrl( this );
				eObject.loadFromDisk( eFileINI,err );

				if( err.errorFree() )
				{
					if( true == bSeenBefore && true == eObject.isPermanent() )
					{
					// no need to recreate this object--
					//	it is a permanent object from an earlier scene
					}
					else
					{
						// otherwise add it to the list of objects
						if( null == m_pFirstObject )
						{
							m_pFirstObject = eObject;
							m_pLastObject = eObject;
						}
						else
						{
							m_pLastObject.setNext( eObject );
							eObject.setPrev( m_pLastObject );
							m_pLastObject = eObject;
						}
					}
				}
			}
		}

		// 'create' the objects (windows, bitmaps, etc.)
		//------------------------------------------------------
		if( err.errorFree() )
		{
			eObject = m_pFirstObject;
			while( null != eObject && err.errorFree() )
			{
				if( false == eObject.isCreated() )
				{
					eObject.createObject( err );
				}
				eObject.setCreated( err.errorFree() );
				eObject = eObject.getNext();
			}
		}
/*
		// if resource data exists, prepare to update fields appropriately
		//------------------------------------------------------
		ANavigatorReso* eNavigatorReso = (ANavigatorReso*)FindObject( A_OBJ_TYPE_NAV_RESO );
		if( NULL != eNavigatorReso )
		{
			eNavigatorReso->Init( m_pFirstObject );
		}
*/
		// determine the initial group for this scene
		//------------------------------------------------------
		m_nCurrentGroup = 1;

/*
		m_pllScenesData->Reset();
		ESceneData* eSceneData = NULL;
		while( NULL != (eSceneData = (ESceneData*)m_pllScenesData->GetCurrentLinkItem()) )
		{
			if( m_nCurrentScene == eSceneData->GetSceneID() )
			{
				EString* strCatName = eSceneData->GetCatName();
				if( NULL != strCatName )
				{
					ANavigatorList* navList = (ANavigatorList*)FindObject( A_OBJ_TYPE_NAV_LIST );
					if( NULL != navList )
					{
						if( errNOITEMNAME != navList->SelectItemFromName(strCatName) )
						{
							ECategory* cat = navList->GetCategory();
							if( NULL != cat )
							{
								if( A_OBJ_TYPE_CAT_WITH_ITEM_LIST == cat->GetType() )
								{
									EString* strItemName = eSceneData->GetItemName();
									if( NULL != strItemName )
									{
										if( errNOITEMNAME != navList->SelectItemFromName((ACategoryWithItemList*)cat,strItemName) )
										{
											m_nCurrentGroup = eSceneData->GetLastGroup();
										}
									}
								}
								else
								{
									m_nCurrentGroup = eSceneData->GetLastGroup();
								}
							}
						}
					}
				}
				break;
			}
			m_pllScenesData->Next();
		}
*/
		// may need to adjust the initial group based on the list box item
		//------------------------------------------------------
		boolean bGroupSet = false;
/*
		ANavigatorList* aNavigatorList = (ANavigatorList*)FindObject( A_OBJ_TYPE_NAV_LIST );
		if( NULL != aNavigatorList )
		{
			INT16 newGroup = aNavigatorList->GetGroupFromCategory( m_nCurrentGroup );
			if( -1 != newGroup && m_nCurrentGroup != newGroup )
			{
				ChangeGroup( newGroup );
				bGroupSet = TRUE;
			}
		}
*/
		// make the proper objects visible
		//------------------------------------------------------
		if( false == bGroupSet )
		{
			//		InitializeGroup( newGroup );
			initializeGroup( m_nCurrentGroup );
		}

		// call PrepareScene() for any EScenePrep classes
		//------------------------------------------------------
		if( true == m_bMainScreenObjects )
		{
/*
			INT16 instance = 1;
			EScenePrep* scenePrep = NULL;
			while( NULL != (scenePrep = (EScenePrep*)FindObject(E_OBJ_TYPE_SCENEPREP,FALSE,instance)) )
			{
				scenePrep->PrepareScene();
				instance++;
			}
*/
		}

		if( true == bGroupSet )
			m_aMain.drawAllEObjects();

		// done with the files now...
		//------------------------------------------------------
		eFileINI = null;

		return err.getErrCode();
	}

	public int initializeGroup( short newGroup )
	{
		EErr err = new EErr();
		
		EObject eObject = (EObject)m_pFirstObject;
		while( null != eObject )
		{
			if( true == eObject.isOfType(EDefines.E_OBJ_TYPE_ONSCREEN) )
			{
				short group = eObject.getGroup();
				// ugly line of code follows...
				if( (ADefAppl.A_SCENE_ID_TIPS != m_nCurrentScene) || false == ((EOnScreen)eObject).isVisibleBasedOnData() )
						((EOnScreen)eObject).setVisible( EDefines.E_GRP_ID_ALL == group || EDefines.E_GRP_ID_LIST_BOX == group || m_nCurrentGroup == group );
			}
			eObject = eObject.getNext();
		}
	
		if( true == m_bMainScreenObjects )
		{
/*
			// give the proper object focus
			//------------------------------------------------------
			EActFocus eActFocus;
			eActFocus.nWindowID = (short)EDefines.E_WND_ID_INVALID;
			eActFocus.bForward = true; 
			m_aMain.takeActionFocus( &eActFocus );
*/
			// give the proper button focus
			//------------------------------------------------------
			setButtonGroup( (short)1, newGroup );

//		GetMain().DrawAllEObjects();
		}

		// if data exists, populate the text fields with that data
		//------------------------------------------------------
/*
		ANavigatorList* aNavigatorList = (ANavigatorList*)FindObject( A_OBJ_TYPE_NAV_LIST );
		if( null != aNavigatorList )
		{
			aNavigatorList->Populate( m_pFirstObject );
		}
*/
		if( true == m_bMainScreenObjects )
		{
			m_aMain.drawAllEObjects();
		}

		return err.getErrCode();
	}

	public void setButtonGroup( short theGroupID, short theBtnIndex )
	{
		EObject eObject = (EObject)m_pFirstObject;
		while( null != eObject )
		{
			if( eObject.isOfType(EDefines.E_OBJ_TYPE_BUTTONIMAGERADIO) )
			{
				EButtonImageRadio eRadio = (EButtonImageRadio)eObject;
				if( theGroupID == eRadio.getBtnGroupID() )
				{
					eRadio.setBtnGroupIndexSelected( theBtnIndex );
					eRadio.displayImage();
				}
			}
			eObject = eObject.getNext();
		}
	}

	public int changeScene( short sceneID, boolean bAddToList )
	{
		EErr err = new EErr();
		
		EEvtInitializeScene eEvtInitializeScene = new EEvtInitializeScene();
		
		if( m_nCurrentScene == sceneID )
			return EErr.errNOERR;
		
		ECursor.setCursor( getContainer(),Cursor.WAIT_CURSOR );

/*
		// store the final group for the previous scene
		//------------------------------------------------------
		if( E_SCENEDATA_INVALID_SCENE != m_nCurrentScene )
		{
			BOOL bSceneFound = false;
			m_pllScenesData->Reset();
			ESceneData* eSceneData = null;
			while( NULL != (eSceneData = (ESceneData*)m_pllScenesData->GetCurrentLinkItem()) )
			{
				if( m_nCurrentScene == eSceneData->GetSceneID() )
				{				
					bSceneFound = true;
					break;
				}
				m_pllScenesData->Next();
			}
			if( FALSE == bSceneFound )
			{
				eSceneData = new ESceneData();
				m_pllScenesData->Add( eSceneData );
			}
			EString* strCatName = null;
			EString* strItemName = null;
			ANavigatorList* navList = (ANavigatorList*)FindObject( A_OBJ_TYPE_NAV_LIST );
			if( NULL != navList )
			{
				ECategory* cat = navList->GetCategory();
				if( NULL != cat )
				{
					strCatName = cat->GetName();
					if( A_OBJ_TYPE_CAT_WITH_ITEM_LIST == cat->GetType() )
					{
						EItem* eItem = navList->GetActiveItem();
						if( NULL != eItem )
						{
							strItemName = eItem->GetName();
						}
					}
				}
			}
			
			eSceneData->SetCatName( null != strCatName ? new EString(strCatName->GetString()) : null );
			eSceneData->SetItemName( null != strItemName ? new EString(strItemName->GetString()) : null );
			eSceneData->SetSceneID( m_nCurrentScene );
			eSceneData->SetLastGroup( m_nCurrentGroup );
		}
*/

		m_nCurrentScene = sceneID;

/*
		if( true == bAddToList )
		{
		// remove everything in the remainder of the list
			if( m_pllScenesVisited->NextExists() )
			{
				m_pllScenesVisited->Next();
				ELink* eLink = NULL;
				while( NULL != (eLink = (ELink*)m_pllScenesVisited->GetCurrentLink()) )
				{
					ETypeINT16* eTypeINT16 = (ETypeINT16*)eLink->GetEItem();
					if( NULL != eTypeINT16 )
						delete eTypeINT16;
					BOOL bContinue = m_pllScenesVisited->NextExists();
					m_pllScenesVisited->Remove();
					//				delete eLink;
					if( FALSE == bContinue )
						break;
				}
			}
		
			// then add the new link
			ETypeINT16* int16typeSceneID = new ETypeINT16( sceneID );
			m_pllScenesVisited->Tail();
			m_pllScenesVisited->Add( int16typeSceneID );
			if( SCENE_MEMORY_LIMIT < m_pllScenesVisited->Size() )
			{
				m_pllScenesVisited->Reset();
				ETypeINT16* eTypeINT16 = (ETypeINT16*)m_pllScenesVisited->GetCurrentLinkItem();
				if( NULL != eTypeINT16 )
					delete eTypeINT16;
				m_pllScenesVisited->Remove();
			}
			m_pllScenesVisited->Tail();
		}
*/
		switch( sceneID )
		{
			case ADefAppl.A_SCENE_ID_CPRT:
				eEvtInitializeScene.nIdFileIni = ADefAppl.A_STR_CPRT_INI;
				eEvtInitializeScene.nIdFileBit = ADefAppl.A_STR_CPRT_BIT;
				break;
			case ADefAppl.A_SCENE_ID_STUP:
				eEvtInitializeScene.nIdFileIni = ADefAppl.A_STR_STUP_INI;
				eEvtInitializeScene.nIdFileBit = ADefAppl.A_STR_STUP_BIT;
				break;
			case ADefAppl.A_SCENE_ID_HOME:
				eEvtInitializeScene.nIdFileIni = ADefAppl.A_STR_HOME_INI;
				eEvtInitializeScene.nIdFileBit = ADefAppl.A_STR_HOME_BIT;
				break;
			case ADefAppl.A_SCENE_ID_INTE:
				eEvtInitializeScene.nIdFileIni = ADefAppl.A_STR_INTE_INI;
				eEvtInitializeScene.nIdFileBit = ADefAppl.A_STR_INTE_BIT;
				break;
			case ADefAppl.A_SCENE_ID_APPL:
				eEvtInitializeScene.nIdFileIni = ADefAppl.A_STR_APPL_INI;
				eEvtInitializeScene.nIdFileBit = ADefAppl.A_STR_APPL_BIT;
				break;
			case ADefAppl.A_SCENE_ID_RFEX:
				eEvtInitializeScene.nIdFileIni = ADefAppl.A_STR_RFEX_INI;
				eEvtInitializeScene.nIdFileBit = ADefAppl.A_STR_RFEX_BIT;
				break;
			case ADefAppl.A_SCENE_ID_HTCL:
				eEvtInitializeScene.nIdFileIni = ADefAppl.A_STR_HTCL_INI;
				eEvtInitializeScene.nIdFileBit = ADefAppl.A_STR_HTCL_BIT;
				break;
			case ADefAppl.A_SCENE_ID_ELEC:
				eEvtInitializeScene.nIdFileIni = ADefAppl.A_STR_ELEC_INI;
				eEvtInitializeScene.nIdFileBit = ADefAppl.A_STR_ELEC_BIT;
				break;
			case ADefAppl.A_SCENE_ID_MORT:
				eEvtInitializeScene.nIdFileIni = ADefAppl.A_STR_MORT_INI;
				eEvtInitializeScene.nIdFileBit = ADefAppl.A_STR_MORT_BIT;
				break;
			case ADefAppl.A_SCENE_ID_TITL:
				eEvtInitializeScene.nIdFileIni = ADefAppl.A_STR_TITL_INI;
				eEvtInitializeScene.nIdFileBit = ADefAppl.A_STR_TITL_BIT;
				break;
			case ADefAppl.A_SCENE_ID_OTHE:
				eEvtInitializeScene.nIdFileIni = ADefAppl.A_STR_OTHE_INI;
				eEvtInitializeScene.nIdFileBit = ADefAppl.A_STR_OTHE_BIT;
				break;
			case ADefAppl.A_SCENE_ID_TIPS:
				eEvtInitializeScene.nIdFileIni = ADefAppl.A_STR_TIPS_INI;
				eEvtInitializeScene.nIdFileBit = ADefAppl.A_STR_TIPS_BIT;
				break;
			case ADefAppl.A_SCENE_ID_RESO:
				eEvtInitializeScene.nIdFileIni = ADefAppl.A_STR_RESO_INI;
				eEvtInitializeScene.nIdFileBit = ADefAppl.A_STR_RESO_BIT;
				break;
			default:
				break;
		}

		initializeScene( eEvtInitializeScene );

//	SetVineStatus( m_nCurrentScene );
		ECursor.setCursor( getContainer(),Cursor.DEFAULT_CURSOR );

		return err.getErrCode();
	}

	public int changeGroup( short groupID, short previousListBoxIdx )
	{
		if( m_nCurrentGroup == groupID )
			return EErr.errNOERR;
/*
		ANavigatorList aNavigatorList = (ANavigatorList)FindObject( ADefAppl.A_OBJ_TYPE_NAV_LIST );
		if( null != aNavigatorList )
		{
			if( true == aNavigatorList.isDialogGroup(groupID) )
			{
				// show the dialog
				m_aMain.wordProc();
				// then go back to previous group
				groupID = m_nCurrentGroup;
				EListBox eListBox = (EListBox)FindObject( EDefines.E_OBJ_TYPE_LISTBOX );
				if( null != eListBox )
				{
					//				eListBox->SetSelection( previousListBoxIdx );
					aNavigatorList.setClickedActiveIndexes( previousListBoxIdx,false );
				}
			}
		}
*/
		m_nCurrentGroup = groupID;
		
		return initializeGroup( m_nCurrentGroup );
	}

	private String getPathStringFromID( int id )
	{
		if( ADefAppl.A_STR_NONE == id )
			return null;
		
		String	strStartOfPath = "file:" + getPathToData();
		
		String 	strPath = null;
		switch( id )
		{
			case ADefAppl.A_STR_CPRT_INI:
				strPath = "/DATA/INI/SectCprt.ini";
				break;
			case ADefAppl.A_STR_STUP_INI:
				strPath = "/DATA/INI/SectStup.ini";
				break;
			case ADefAppl.A_STR_HOME_INI:
				strPath = "/DATA/INI/SectHome.ini";
				break;
			case ADefAppl.A_STR_INTE_INI:
				strPath = "/DATA/INI/SectInte.ini";
				break;
			case ADefAppl.A_STR_APPL_INI:
				strPath = "/DATA/INI/SectAppl.ini";
				break;
			case ADefAppl.A_STR_RFEX_INI:
				strPath = "/DATA/INI/SectRfex.ini";
				break;
			case ADefAppl.A_STR_HTCL_INI:
				strPath = "/DATA/INI/SectHtcl.ini";
				break;
			case ADefAppl.A_STR_ELEC_INI:
				strPath = "/DATA/INI/SectElec.ini";
				break;
			case ADefAppl.A_STR_MORT_INI:
				strPath = "/DATA/INI/SectMort.ini";
				break;
			case ADefAppl.A_STR_TITL_INI:
				strPath = "/DATA/INI/SectTitl.ini";
				break;
			case ADefAppl.A_STR_OTHE_INI:
				strPath = "/DATA/INI/SectOthe.ini";
				break;
			case ADefAppl.A_STR_TIPS_INI:
				strPath = "/DATA/INI/SectTips.ini";
				break;
			case ADefAppl.A_STR_RESO_INI:
				strPath = "/DATA/INI/SectReso.ini";
				break;
			default:
				if( AConfig.DEBUG ) System.out.println( "Unknown case" );
				break;
		}
		strPath = strStartOfPath + strPath;
		
		return strPath;
	}
	
	public String getPathToData()
	{
		if( true == AConfig.APPLET )
		{
			Applet applet = m_aMain.getEEvtStartUp().getApplet();
			
			URL 		urlDocument 	= applet.getDocumentBase();
			String	strDocument 	= urlDocument.toString();
			int 		nLastIdxOf 		= strDocument.lastIndexOf( '/' );
			
			return strDocument.substring( 0,nLastIdxOf );
		}
		else
		{
			if( AConfig.DEVELOPMENT )
				return new String( E_LAUNCH_PATH );	// use app path, not forte's path
			else
				return System.getProperty( "user.dir" );
		}
	}
	
	public EObject findObject( short typeId, boolean exactMatch, short instance, short group )
	{
		EObject eObject = m_pFirstObject;
		while( null != eObject )
		{
			boolean bMatch = exactMatch ? eObject.isType(typeId) : eObject.isOfType(typeId);
			if( bMatch )
			{
				if( EDefines.E_GRP_ID_ALL == group || eObject.getGroup() == group )
				{
					if( 1 == instance )
						break;
					instance--;
				}
			}
			eObject = eObject.getNext();
		}
		return eObject;
	}
	
	public EObject findObjectOfTypeWithID( short typeId, short objId, short instance )
	{
		EObject eObject = m_pFirstObject;
		while( null != eObject )
		{
			if( true == eObject.isType(typeId) )
			{
				if( objId == eObject.getObjectID() )
				{
					if( 1 == instance )
						break;
					instance--;
				}
			}
			eObject = eObject.getNext();
		}
		return eObject;
	}
	
	public EObject findObjectWithWindowID( short windowID )
	{
		if( EDefines.E_WND_ID_INVALID == windowID )
			return null;
		
		EObject eObject = m_pFirstObject;
		while( null != eObject )
		{
			if( true == eObject.isOfType((short)EDefines.E_OBJ_TYPE_ONSCREEN) )
			{
				EOnScreen eOnScreen = (EOnScreen)eObject;
				if( windowID == eOnScreen.getWindowID() )
				{
					break;
				}
			}
			eObject = eObject.getNext();
		}
		return eObject;
	}
	
	public EObject findObjectWithID( short id )
	{
		EObject eObject = m_pFirstObject;
		while( null != eObject )
		{
			if( id == eObject.getObjectID() )
			{
				break;
			}
			eObject = eObject.getNext();
		}
		return eObject;
	}
	
	private void disposeNonPermanentObjects()
	{
		EObject eObject = m_pFirstObject;
		while( null != eObject )
		{
			EObject eObjectToDispose = eObject;
			eObject = eObject.getNext();
			if( false == eObjectToDispose.isPermanent() )
			{
				EObject prevObject = eObjectToDispose.getPrev();
				EObject nextObject = eObjectToDispose.getNext();
				if( null == prevObject )
				{
					// removing the first object;
					m_pFirstObject = eObjectToDispose.getNext();
				}
				else
				{
					prevObject.setNext( nextObject );
				}
				if( null == nextObject )
				{
					// removing the last object
					m_pLastObject = eObjectToDispose.getPrev();
				}
				else
				{
					nextObject.setPrev( prevObject );
				}
			
				eObjectToDispose = null;
			}
		}
	}
}