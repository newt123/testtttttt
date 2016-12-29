// ADefAppl.java

package com.walkerbros.projects.walkerbros.ajava;

import com.walkerbros.common.ejava.*;

public class ADefAppl
{
	
	// application specific objects
	//------------------------------------------------------------------------
	public static final short A_OBJ_TYPE_ITEM_INTE						= (EDefines.E_OBJ_TYPE_APPLICATION+0);
	public static final short A_OBJ_TYPE_ITEM_APPL						= (EDefines.E_OBJ_TYPE_APPLICATION+10);
	public static final short A_OBJ_TYPE_ITEM_RFEX						= (EDefines.E_OBJ_TYPE_APPLICATION+20);
	public static final short A_OBJ_TYPE_ITEM_HTCL						= (EDefines.E_OBJ_TYPE_APPLICATION+30);
	public static final short A_OBJ_TYPE_ITEM_ELEC						= (EDefines.E_OBJ_TYPE_APPLICATION+40);
	public static final short A_OBJ_TYPE_ITEM_MORT						= (EDefines.E_OBJ_TYPE_APPLICATION+50);
	public static final short A_OBJ_TYPE_ITEM_TITL						= (EDefines.E_OBJ_TYPE_APPLICATION+60);
	public static final short A_OBJ_TYPE_ITEM_OTHE						= (EDefines.E_OBJ_TYPE_APPLICATION+70);
	public static final short A_OBJ_TYPE_ITEM_TIPS						= (EDefines.E_OBJ_TYPE_APPLICATION+80);
	public static final short A_OBJ_TYPE_ITEM_RESO						= (EDefines.E_OBJ_TYPE_APPLICATION+90);
	public static final short A_OBJ_TYPE_NAV_LIST							= (EDefines.E_OBJ_TYPE_APPLICATION+100);
	public static final short A_OBJ_TYPE_NAV_RESO							= (EDefines.E_OBJ_TYPE_APPLICATION+110);
	public static final short A_OBJ_TYPE_CAT_WITH_ITEM				= (EDefines.E_OBJ_TYPE_APPLICATION+120);
	public static final short A_OBJ_TYPE_CAT_WITH_ITEM_LIST		= (EDefines.E_OBJ_TYPE_APPLICATION+130);
	public static final short A_OBJ_TYPE_SCENEPREP_TIPS				= (EDefines.E_OBJ_TYPE_APPLICATION+140);
	public static final short A_OBJ_TYPE_BUBBLEHELP						= (EDefines.E_OBJ_TYPE_APPLICATION+150);
	public static final short A_OBJ_TYPE_FRAMEHOLDERFUSES			= (EDefines.E_OBJ_TYPE_APPLICATION+160);
	
	// application specific ids
	//------------------------------------------------------------------------
	public static final int A_STR_NONE 			= 0;
	public static final int A_STR_CPRT_INI 	= 200;
	public static final int A_STR_CPRT_BIT	= 201;
	public static final int A_STR_STUP_INI 	= 202;
	public static final int A_STR_STUP_BIT 	= 203;
	public static final int A_STR_HOME_INI 	= 204;
	public static final int A_STR_HOME_BIT 	= 205;
	public static final int A_STR_INTE_INI 	= 206;
	public static final int A_STR_INTE_BIT 	= 207;
	public static final int A_STR_APPL_INI 	= 208;
	public static final int A_STR_APPL_BIT 	= 209;
	public static final int A_STR_RFEX_INI 	= 210;
	public static final int A_STR_RFEX_BIT 	= 211;
	public static final int A_STR_HTCL_INI 	= 212;
	public static final int A_STR_HTCL_BIT 	= 213;
	public static final int A_STR_ELEC_INI 	= 214;
	public static final int A_STR_ELEC_BIT 	= 215;
	public static final int A_STR_MORT_INI 	= 216;
	public static final int A_STR_MORT_BIT 	= 217;
	public static final int A_STR_TITL_INI 	= 218;
	public static final int A_STR_TITL_BIT 	= 219;
	public static final int A_STR_OTHE_INI 	= 220;
	public static final int A_STR_OTHE_BIT 	= 221;
	public static final int A_STR_TIPS_INI 	= 222;
	public static final int A_STR_TIPS_BIT 	= 223;
	public static final int A_STR_RESO_INI 	= 224;
	public static final int A_STR_RESO_BIT 	= 225;
					
	// scene ids
	//------------------------------------------------------------------------
	public static final short A_SCENE_ID_CPRT					= 1;
	public static final short A_SCENE_ID_STUP					= 2;
	public static final short A_SCENE_ID_HOME					= 3;
	public static final short A_SCENE_ID_INTE					= 4;
	public static final short A_SCENE_ID_APPL					= 5;
	public static final short A_SCENE_ID_RFEX					= 6;
	public static final short A_SCENE_ID_HTCL					= 7;
	public static final short A_SCENE_ID_ELEC					= 8;
	public static final short A_SCENE_ID_MORT					= 9;
	public static final short A_SCENE_ID_TITL					= 10;
	public static final short A_SCENE_ID_OTHE					= 11;
	public static final short A_SCENE_ID_TIPS					= 12;
	public static final short A_SCENE_ID_RESO					= 13;
	
	// button defines
	//------------------------------------------------------------------------
	public static final short A_BTN_ID_NEWSCENE						= 100;
	public static final short A_BTN_ID_NEWGROUP						= 101;
	public static final short A_BTN_ID_QUIT								= 102;
	public static final short A_BTN_ID_MINIMIZE						= 103;
	public static final short A_BTN_ID_HELP								= 104;
	public static final short A_BTN_ID_PRNT								= 105;
	public static final short A_BTN_ID_ADD								= 106;
	public static final short A_BTN_ID_SAVE								= 107;
	public static final short A_BTN_ID_DELETE							= 108;
	public static final short A_BTN_ID_BACK								= 109;
	public static final short A_BTN_ID_FRWD								= 110;
	public static final short A_BTN_ID_START_ANIM					= 111;
	public static final short A_BTN_ID_SHOW_OBJECT				= 112;
	public static final short A_BTN_ID_START_TICKER				= 113;
	public static final short A_BTN_ID_START_RTF_TICKER		= 114;
	public static final short A_BTN_ID_START_TXT_TICKER		= 115;
	public static final short A_BTN_ID_LOGO								= 116;
	public static final short A_BTN_ID_NEW_REPAIR					= 117;
	public static final short A_BTN_ID_DELETE_REPAIR			= 118;
	public static final short A_BTN_ID_PREV_REPAIR				= 119;
	public static final short A_BTN_ID_NEXT_REPAIR				= 120;
	public static final short A_BTN_ID_PREV_MONTH					= 121;
	public static final short A_BTN_ID_NEXT_MONTH					= 122;
	
	public ADefAppl() 
	{
	}
}