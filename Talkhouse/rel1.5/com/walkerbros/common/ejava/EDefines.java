// EDefines.java

package com.walkerbros.common.ejava;

public class EDefines
{
	// Object types
	//------------------------------------------------------------------------
	public static final short	E_OBJ_TYPE_UNKNOWN							= 1000;
	public static final short	E_OBJ_TYPE_BACKUP								= 1010;
	public static final short	E_OBJ_TYPE_ONSCREEN							= 1020;
	public static final short	E_OBJ_TYPE_WINDOW								= 1030;
	public static final short	E_OBJ_TYPE_INFO									= 1040;
	public static final short	E_OBJ_TYPE_INFO_GROUP						= 1050;
	public static final short	E_OBJ_TYPE_INFO_STRING					= 1060;
	public static final short E_OBJ_TYPE_INFO_STRING_TXT			= 1070;
	public static final short E_OBJ_TYPE_INFO_GIF_FILENAME		= 1075;
	public static final short	E_OBJ_TYPE_SCENEPREP						= 1080;
	public static final short E_OBJ_TYPE_SCENEPREP_TIMEBOMB		= 1081;
	public static final short	E_OBJ_TYPE_BUBBLEHELPAREA				= 1090;
	public static final short E_OBJ_TYPE_BACKGROUND						= 1100;
	public static final short E_OBJ_TYPE_IMAGE								= 1110;
	public static final short E_OBJ_TYPE_FRAME								= 1120;
	public static final short E_OBJ_TYPE_FRAMEHOLDER					= 1130;
	public static final short E_OBJ_TYPE_ANIMATION						= 1140;
	public static final short E_OBJ_TYPE_TICKER								= 1150;
	public static final short	E_OBJ_TYPE_BUTTON								= 1160;
	public static final short	E_OBJ_TYPE_BUTTONPUSH						= 1170;
	public static final short E_OBJ_TYPE_BUTTONIMAGE					= 1180;
	public static final short E_OBJ_TYPE_BUTTONIMAGETOGGLE		= 1190;
	public static final short E_OBJ_TYPE_BUTTONIMAGERADIO			= 1200;
	public static final short E_OBJ_TYPE_BUTTONIMAGETIMER			= 1210;
	public static final short E_OBJ_TYPE_BUTTONIMAGETABS			= 1220;
	public static final short E_OBJ_TYPE_LABEL								= 1230;
	public static final short E_OBJ_TYPE_LABELTRIGGER					= 1240;
	public static final short E_OBJ_TYPE_LABEL_TXT						= 1250;
	public static final short E_OBJ_TYPE_LABEL_TXT_TICKER			= 1260;
	public static final short	E_OBJ_TYPE_LABELPRINTHEADER			= 1270;
	public static final short	E_OBJ_TYPE_DATE									= 1280;
	public static final short E_OBJ_TYPE_TEXTFIELD						= 1290;
	public static final short E_OBJ_TYPE_TEXTFIELD_DATE				= 1300;
	public static final short E_OBJ_TYPE_TEXTFIELD_COST				= 1310;
	public static final short E_OBJ_TYPE_LISTBOX							= 1320;
	public static final short E_OBJ_TYPE_CHECKBOX							= 1330;
	public static final short E_OBJ_TYPE_SCROLLBAR						= 1340;
	public static final short E_OBJ_TYPE_RTF									= 1350;
	public static final short E_OBJ_TYPE_RTF_TICKER						= 1360;
	public static final short E_OBJ_TYPE_NAV									= 1370;
	public static final short E_OBJ_TYPE_NAV_RESO							= 1380;
	public static final short E_OBJ_TYPE_NAV_RESO_CUST				= 1390;
	public static final short E_OBJ_TYPE_DATA_RESO_ID					= 1400;
	public static final short E_OBJ_TYPE_ONLINE_SERVICES			= 1410;
	public static final short	E_OBJ_TYPE_CAT									= 1420;
	public static final short	E_OBJ_TYPE_CAT_WITH_ITEM				= 1430;
	public static final short	E_OBJ_TYPE_CAT_WITH_ITEM_LIST		= 1440;
	public static final short	E_OBJ_TYPE_ITEM									= 1450;
	public static final short	E_OBJ_TYPE_ITEM_RESO						= 1460;
	public static final short	E_OBJ_TYPE_LAST									= 1460;
																							
// Application-specific obj IDs start here...
	public static final short	E_OBJ_TYPE_APPLICATION					= 8000;

	// for identifying individual objects
	//------------------------------------------------------------------------
	public static final int	E_OBJ_ID_DEFAULT								= -1;
	public static final int	E_OBJ_ID_INITIAL								= 0;

	// for identifying groups of data
	//------------------------------------------------------------------------
	public static final int	E_GRP_ID_ALL										= -1;
	public static final int	E_GRP_ID_LIST_BOX								= -2;

	// window ids
	//------------------------------------------------------------------------
	public static final int	E_WND_ID_INVALID								= 999;
	public static final int	E_WND_ID_AVAILABLE							= 10000;
	public static final int	E_WND_ID_PERM_AVAILABLE					= 10000;

	public static final int	E_WND_ID_INITIAL								= (E_WND_ID_INVALID+1);
	public static final int	E_WND_ID_FINAL									= (E_WND_ID_INITIAL+E_WND_ID_AVAILABLE-1);

	public static final int	E_WND_ID_PERM_INITIAL						= (E_WND_ID_FINAL+1);
	public static final int	E_WND_ID_PERM_FINAL							= (E_WND_ID_PERM_INITIAL+E_WND_ID_PERM_AVAILABLE-1);

	// object status
	//------------------------------------------------------------------------
	public static final int	E_OBJ_STATUS_NONE								= -1;
	public static final int	E_OBJ_STATUS_INACTIVE						= 0;
	public static final int	E_OBJ_STATUS_ACTIVE							= 1;
	public static final int	E_OBJ_STATUS_HIDDEN							= 2;

	// object conditions
	//------------------------------------------------------------------------
	public static final int	E_OBJ_CONDITIONS_NONE						= 0;

	// Common defines
	//------------------------------------------------------------------------
	public static final int	E_LESS_THAN											= -1;
	public static final int	E_EQUAL_TO											= 0;
	public static final int	E_GREATER_THAN									= 1;

	public static final int	E_COLOR_BLACK										= 0;
	public static final int	E_COLOR_WHITE										= 1;
	public static final int	E_COLOR_RED											= 2;
	public static final int	E_COLOR_GREEN										= 3;
	public static final int	E_COLOR_BLUE										= 4;
	public static final int	E_COLOR_YELLOW									= 5;
	
	public EDefines() 
	{
	}
}