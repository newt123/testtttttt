// AMain.java

package com.walkerbros.projects.walkerbros.aclasses;

import java.applet.*;
import java.awt.*;

import com.walkerbros.common.eclasses.*;
import com.walkerbros.common.econtrols.*;
import com.walkerbros.common.eimage.*;
import com.walkerbros.common.ejava.*;
import com.walkerbros.common.ejava.eevent.*;
import com.walkerbros.common.ejava.eaction.*;
import com.walkerbros.common.edata.*;
import com.walkerbros.projects.walkerbros.ajava.*;

public class AMain
{
	AObjectController m_pObjectController 	= new AObjectController();
	EEvtStartUp 			m_pEEvtStartUp 				= new EEvtStartUp();
	private boolean	m_bChangesMade					= false; 
	private boolean	m_bRepairAdded					= false; 
	private int		m_nWindowIDCounter				= EDefines.E_WND_ID_INITIAL;
	private int		m_nWindowIDCounterPerm		= EDefines.E_WND_ID_PERM_INITIAL;
	private Image	m_imageScreen							= null;
	private EButtonImage	m_pButtonCaptured	= null;
	private Cursor m_cursorDefault					= new Cursor( Cursor.DEFAULT_CURSOR );
	private Cursor m_cursorHand							= new Cursor( Cursor.HAND_CURSOR );
	
	public AMain() 
	{
	}
	
	public int onStartUp( EEvtStartUp eEvtStartUp )
	{
		m_pEEvtStartUp.setContainer( eEvtStartUp.getContainer() );
		m_pObjectController.setAMain( this );
		return 1;
	}
	
	public int onShutDown( EEvtShutDown eEvtShutDown )
	{
		return 1;
	}
	
	public int onTimer( EEvtTimer eEvtTimer )
	{
		EObject eObject = getFirstObject();
		while( null != eObject )
		{
			if( false == eObject.onTimer() )
				break;
			eObject = eObject.getNext();
		}
		return 1;
	}
																																																												
	public int onPaint( EEvtPaint eEvtPaint )
	{
		Graphics g = eEvtPaint.getGraphics();
		if( null != m_imageScreen )
		{
			g.drawImage( m_imageScreen,0,0, getContainer() );
		}
		return 1;
	}

	public int onInitializeScene( EEvtInitializeScene eEvtInitializeScene )
	{
		int err = m_pObjectController.initializeScene( eEvtInitializeScene );
		return err;
	}

	public int onMouseMove( EEvtMouseMove eEvtMouseMove )
	{
		//#ifdef MAC
		//	if( TRUE == GetInModalWindowFlag() )
		//		return errNOERR;
		//#endif // MAC
		
		//		if( NULL != GetBubbleHelp() )
		//			GetBubbleHelp()->SetPaused( FALSE );
		
		EErr err = new EErr();
		
		int x = eEvtMouseMove.x;
		int y = eEvtMouseMove.y;
		Point mousePt = new Point( x,y );
		
		// check if we are over any object
		EObject eObject = getFirstObject();
		while( null != eObject )
		{
			short group = eObject.getGroup();
			if( EDefines.E_GRP_ID_ALL == group || EDefines.E_GRP_ID_LIST_BOX == group || getCurrentGroup() == group )
			{
				if( eObject.isOfType(EDefines.E_OBJ_TYPE_BUTTON) || eObject.isOfType(EDefines.E_OBJ_TYPE_LABEL)
															|| eObject.isOfType(ADefAppl.A_OBJ_TYPE_FRAMEHOLDERFUSES)
															|| eObject.isOfType(EDefines.E_OBJ_TYPE_BUBBLEHELPAREA) )
				{
					if( true == ((EOnScreen)eObject).contains(mousePt))
					{
						//						if( true == m_bHelpIsOn || true == eObject.isPermanentHelp() )
						//							UpdateBubbleHelp( eObject, mousePt );
						//						else
						//							HideBubbleHelp();
						showHandCursor( eObject.isOfType(EDefines.E_OBJ_TYPE_BUTTON) );
						return err.getErrCode();
					}
				}
			}
			eObject = eObject.getNext();
		}
		showHandCursor( false );
/*
		EPoint& refMmousePt = *mousePt;
		
		// check if we are over any object
		EObject* eObject = GetFirstObject();
		while( NULL != eObject )
		{
			INT16 group = eObject->GetGroup();
			if( E_GRP_ID_ALL == group || E_GRP_ID_LIST_BOX == group || GetCurrentGroup() == group )
			{
				if( eObject->IsOfType(E_OBJ_TYPE_BUTTON) || eObject->IsOfType(E_OBJ_TYPE_LABEL)
									|| eObject->IsOfType(A_OBJ_TYPE_FRAMEHOLDERFUSES)
									|| eObject->IsOfType(E_OBJ_TYPE_BUBBLEHELPAREA) )
				{
					dbgAssert( eObject->IsOfType(E_OBJ_TYPE_ONSCREEN), ID_INCONSIST );
					if( TRUE == ((EOnScreen*)eObject)->Contains(refMmousePt))
					{
						if( TRUE == m_bHelpIsOn || TRUE == eObject->IsPermanentHelp() )
							UpdateBubbleHelp( eObject, mousePt );
						else
							HideBubbleHelp();
						ShowHandCursor( eObject->IsOfType(E_OBJ_TYPE_BUTTON) );
						delete mousePt;
						return errNOERR;
					}
				}
			}
		eObject = eObject->GetNext();
		}
		
		if( NULL != GetBubbleHelp() )
			GetBubbleHelp()->SetVisible( FALSE );
		ShowHandCursor( FALSE );
		
		delete mousePt;
*/
		return err.getErrCode();
	}
	
	public int onMouseDown( EEvtMouseDown eEvtMouseDown )
	{
		//#ifdef MAC
		//	if( TRUE == GetInModalWindowFlag() )
		//		return errNOERR;
		//#endif // MAC
		
		//	if( NULL != GetBubbleHelp() )
		//		GetBubbleHelp()->SetVisible( FALSE,FALSE );
		
		EErr err = new EErr();
		
		int x = eEvtMouseDown.x;
		int y = eEvtMouseDown.y;
		Point mousePt = new Point( x,y );
		
		// check if we are over a button
		EObject eObject = getFirstObject();
		while( null != eObject )
		{
			if( eObject.isOfType(EDefines.E_OBJ_TYPE_BUTTONIMAGE) )
			{
				EButtonImage eButtonImage = (EButtonImage)eObject; 
				if( true == eButtonImage.shouldProcessOnMouseDown(eEvtMouseDown) )
				{
					eButtonImage.onMouseDown(eEvtMouseDown);
					getContainer().repaint();
					m_pButtonCaptured = eButtonImage;
					return EErr.errNOERR;
				}
			}
			eObject = eObject.getNext();
		}
		return err.getErrCode();
	}

	public int onMouseUp( EEvtMouseUp eEvtMouseUp )
	{
		if( null == m_pButtonCaptured )
			return EErr.errNOERR;
		
		//	if( NULL != GetBubbleHelp() )
		//		GetBubbleHelp()->SetVisible( FALSE,FALSE );
		
		int x = eEvtMouseUp.x;
		int y = eEvtMouseUp.y;
		Point mousePt = new Point( x,y );
		
		boolean bAct = false;
		
		// check if we are over a button
		EObject eObject = getFirstObject();
		while( null != eObject && false == bAct)
		{
			if( eObject.isOfType(EDefines.E_OBJ_TYPE_BUTTONIMAGE) )
			{
				EButtonImage eButtonImage = (EButtonImage)eObject; 
				if( true == eButtonImage.isVisible() && true == eButtonImage.contains( mousePt ) )
				{
					bAct = eButtonImage == m_pButtonCaptured;
					if( true == bAct && eObject.isOfType(EDefines.E_OBJ_TYPE_BUTTONIMAGETOGGLE) )
						((EButtonImageToggle)eObject).toggle();
					//#ifndef USE_HELP_DIALOG
					//				if(  TRUE == bAct && FALSE == m_bHelpIsOn && A_HELPSTR_HOME_HELP == eButtonImage->GetHelpStrID() )
					//					UpdateBubbleHelp( eObject, mousePt );
					//#endif
					//				break;
				}
			}
			eObject = eObject.getNext();
		}
		
		m_pButtonCaptured.setState( EButtonImage.E_BUTTONIMAGE_STATE_UP );
		m_pButtonCaptured.displayImage();
		getContainer().repaint();
		
		if( true == bAct )
		{
			m_pButtonCaptured.takeAction();
		}
		m_pButtonCaptured = null;
		
		return EErr.errNOERR;
	}

	public void takeActionButton( EActBtn eActBtn )
	{
		short buttonID		= eActBtn.getButtonID();
		short buttonSubID = eActBtn.getButtonSubID();
		
		switch( buttonID )
		{
			case ADefAppl.A_BTN_ID_NEWSCENE:
				//				if( false == keepChanges(true) )
				//					return;
				m_pObjectController.changeScene( buttonSubID, ADefAppl.A_SCENE_ID_STUP != buttonSubID );
				break;
			case ADefAppl.A_BTN_ID_NEWGROUP:
				if( m_pObjectController.getCurrentGroup() == buttonSubID )
					return;
				//			if( false == keepChanges(false) )
				//				return;
				m_pObjectController.changeGroup( buttonSubID, (short)-1 );
				
				break;
			default:
			break;
		}
	}
	public void drawAllEObjects()
	{
		if( null == m_imageScreen )
		{
			m_imageScreen = getContainer().createImage( 640,480 );
		}
		
		Graphics gScreenGif = m_imageScreen.getGraphics();
		
		EBackground eBkgd = getBackground(); 
		if( null != eBkgd )
		{
			Image imageBkgd = eBkgd.getGif();
			gScreenGif.drawImage( imageBkgd,0,0, getContainer() );
			//			gScreenGif.setColor( Color.green );
			//			gScreenGif.fillRect( 0,0,640,480 );
		}
		
		EObject eObject = getFirstObject();
		while( null != eObject )
		{
			if( eObject.isOfType(EDefines.E_OBJ_TYPE_IMAGE) )
			{
				EImage eImage = (EImage)eObject;
				Rectangle r = eImage.getRect();
				Image imageObj = eImage.getGif();
				gScreenGif.drawImage( imageObj, r.x,r.y, getContainer() );
			}
/*
			else if( eObject.isOfType(EDefines.E_OBJ_TYPE_BUTTONIMAGETABS) )
			{
				System.out.println( "E_OBJ_TYPE_BUTTONIMAGETABS in DrawAllEObjects" );
				EButtonImageTabs eButtonImageTabs = (EButtonImageTabs)eObject;
				eButtonImageTabs.displayImage();
			}
*/
			else if( eObject.isOfType(EDefines.E_OBJ_TYPE_BUTTONIMAGE) )
			{
				EButtonImage eButtonImage = (EButtonImage)eObject;
				Rectangle r = eButtonImage.getRect();
				Image imageObj = eButtonImage.getGif();
				gScreenGif.drawImage( imageObj, r.x,r.y, getContainer() );
				//				EButtonImage eButtonImage = (EButtonImage)eObject;
				//				EOnScreen eOnScreen = (EOnScreen)eObject;
				//				eButtonImage.displayImage();
				/*				Rectangle r = eOnScreen.getRect();
				Image theGif = eButtonImage.getGif();
				if( null != theGif )
				{
					gScreenGif.drawImage( theGif, r.x,r.y, getApplet() );
				} */
			}
			eObject = eObject.getNext();
		}		
		getContainer().repaint();
	}
	
	public void showHandCursor( boolean bShowHand )
	{
		Container container = getContainer();
		if( null == container )
			return;
		
		if( true == bShowHand )
		{
			container.setCursor( m_cursorHand );
		}
		else
		{
			container.setCursor( m_cursorDefault );
		}
	}
	
	public EEvtStartUp 	getEEvtStartUp()	{ return m_pEEvtStartUp; }
	public Container		getContainer() 		{ return m_pEEvtStartUp.getContainer(); }
	public short 				getCurrentGroup() { return m_pObjectController.getCurrentGroup(); }
	public EObject 			getFirstObject() 	{ return m_pObjectController.getFirstObject(); }
	public Image				getScreen()				{ return m_imageScreen; }
	public EBackground 	getBackground() 	{ return m_pObjectController.getBackground(); }
	
	public void setWindowIDCounter( int cntr )	{ m_nWindowIDCounter = cntr; }
	
	public int 	getWindowIDCounter() 						{ return m_nWindowIDCounter; }
	public void	incrementWindowIDCounter() 			{ m_nWindowIDCounter++; }
	public int 	getWindowIDCounterPerm() 				{ return m_nWindowIDCounterPerm; }
	public void incrementWindowIDCounterPerm() 	{ m_nWindowIDCounterPerm++; }
	
	public void setChangesMadeFlag( boolean bChanges )		{ m_bChangesMade = bChanges; }
	public void setRepairAddedFlag( boolean bAdded )			{ m_bRepairAdded = bAdded; }
	public void setButtonCaptured( EButtonImage button )	{ m_pButtonCaptured = button; }
}