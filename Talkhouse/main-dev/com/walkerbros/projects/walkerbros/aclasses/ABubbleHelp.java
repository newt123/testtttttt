// ABubbleHelp.java

package com.walkerbros.projects.walkerbros.aclasses;

public class ABubbleHelp
{
	public static final int	A_BUBBLEHELP_LEN_MAX					= 500;
	public static final int	A_HELPSTR_CURSOR_DEPTH					= 32;

	public static final int	A_HELPSTR_NONE						= -1;
	// general help ids
	public static final int	A_HELPSTR_MINIMIZE					= 0;
	public static final int	A_HELPSTR_ONLINE					= A_HELPSTR_MINIMIZE+1;
	public static final int	A_HELPSTR_GINFO						= A_HELPSTR_ONLINE+1;
	public static final int	A_HELPSTR_PAINT						= A_HELPSTR_GINFO+1;
	public static final int	A_HELPSTR_ADD						= A_HELPSTR_PAINT+1;
	public static final int	A_HELPSTR_SAVE						= A_HELPSTR_ADD+1;
	public static final int	A_HELPSTR_DELETE					= A_HELPSTR_SAVE+1;
	public static final int	A_HELPSTR_ITEM_LABEL					= A_HELPSTR_DELETE+1;
	public static final int	A_HELPSTR_ROOM_LABEL					= A_HELPSTR_ITEM_LABEL+1;
	// --- repairs ---
	public static final int	A_HELPSTR_REPAIRS					= A_HELPSTR_ROOM_LABEL+1;
	public static final int	A_HELPSTR_REP_ADD					= A_HELPSTR_REPAIRS+1;
	public static final int	A_HELPSTR_REP_DEL					= A_HELPSTR_REP_ADD+1;
	public static final int	A_HELPSTR_REP_HISTORY					= A_HELPSTR_REP_DEL+1;
	public static final int	A_HELPSTR_REP_PREV					= A_HELPSTR_REP_HISTORY+1;
	public static final int	A_HELPSTR_REP_NEXT					= A_HELPSTR_REP_PREV+1;
	// home screen help ids
	public static final int	A_HELPSTR_HOME_HOME_INFO				= A_HELPSTR_REP_NEXT+1;
	public static final int	A_HELPSTR_HOME_TIPS					= A_HELPSTR_HOME_HOME_INFO+1;
	public static final int	A_HELPSTR_HOME_RESO					= A_HELPSTR_HOME_TIPS+1;
	public static final int	A_HELPSTR_HOME_HELP					= A_HELPSTR_HOME_RESO+1;
	public static final int	A_HELPSTR_HOME_PRINT					= A_HELPSTR_HOME_HELP+1;
	public static final int	A_HELPSTR_HOME_FORWARD					= A_HELPSTR_HOME_PRINT+1;
	public static final int	A_HELPSTR_HOME_BACKWARD					= A_HELPSTR_HOME_FORWARD+1;
	public static final int	A_HELPSTR_HOME_HOME					= A_HELPSTR_HOME_BACKWARD+1;
	public static final int	A_HELPSTR_HOME_QUIT					= A_HELPSTR_HOME_HOME+1;
	public static final int	A_HELPSTR_HOME_INTE					= A_HELPSTR_HOME_QUIT+1;
	public static final int	A_HELPSTR_HOME_APPL					= A_HELPSTR_HOME_INTE+1;
	public static final int	A_HELPSTR_HOME_RFEX					= A_HELPSTR_HOME_APPL+1;
	public static final int	A_HELPSTR_HOME_HTCL					= A_HELPSTR_HOME_RFEX+1;
	public static final int	A_HELPSTR_HOME_ELEC					= A_HELPSTR_HOME_HTCL+1;
	public static final int	A_HELPSTR_HOME_MORT					= A_HELPSTR_HOME_ELEC+1;
	public static final int	A_HELPSTR_HOME_TITL					= A_HELPSTR_HOME_MORT+1;
	public static final int	A_HELPSTR_HOME_OTHE					= A_HELPSTR_HOME_TITL+1;
	// interior help ids
	public static final int	A_HELPSTR_INTE_PAINT					= A_HELPSTR_HOME_OTHE+1;
	public static final int	A_HELPSTR_INTE_FLOOR					= A_HELPSTR_INTE_PAINT+1;
	public static final int	A_HELPSTR_INTE_REPAIRS					= A_HELPSTR_INTE_FLOOR+1;
	public static final int	A_HELPSTR_INTE_ADD					= A_HELPSTR_INTE_REPAIRS+1;
	public static final int	A_HELPSTR_INTE_DELETE					= A_HELPSTR_INTE_ADD+1;
	public static final int	A_HELPSTR_INTE_LISTBOX					= A_HELPSTR_INTE_DELETE+1;
	// appliances help ids
	public static final int	A_HELPSTR_APPL_LISTBOX					= A_HELPSTR_INTE_LISTBOX+1;
	// roof and exterior help ids
	public static final int	A_HELPSTR_RFEX_LISTBOX					= A_HELPSTR_APPL_LISTBOX+1;
	// heating and cooling help ids
	public static final int	A_HELPSTR_HTCL_LISTBOX					= A_HELPSTR_RFEX_LISTBOX+1;
	// electrical and plumbing help ids
	public static final int	A_HELPSTR_ELEC_LISTBOX					= A_HELPSTR_HTCL_LISTBOX+1;
	// mortgage and insurance help ids
	public static final int	A_HELPSTR_MORT_LISTBOX					= A_HELPSTR_ELEC_LISTBOX+1;
	// other areas help ids
	public static final int	A_HELPSTR_OTHE_LISTBOX					= A_HELPSTR_MORT_LISTBOX+1;
	// monthly tips help ids
	public static final int	A_HELPSTR_TIPS_MOVE					= A_HELPSTR_OTHE_LISTBOX+1;
	public static final int	A_HELPSTR_TIPS						= A_HELPSTR_TIPS_MOVE+1;
	public static final int	A_HELPSTR_TIPS_MY_OWN					= A_HELPSTR_TIPS+1;
	public static final int	A_HELPSTR_TIPS_ADD					= A_HELPSTR_TIPS_MY_OWN+1;
	public static final int	A_HELPSTR_TIPS_DELETE					= A_HELPSTR_TIPS_ADD+1;
	// directory help ids
	public static final int	A_HELPSTR_RESO_LISTBOX					= A_HELPSTR_TIPS_DELETE+1;
	// escrow and title help ids
	public static final int	A_HELPSTR_TITL_LISTBOX					= A_HELPSTR_RESO_LISTBOX+1;
	// web address help id
	public static final int	A_HELPSTR_WEB_ADDRESS					= A_HELPSTR_TITL_LISTBOX+1;

	public static final int	A_HELPSTR_NUMBER					= A_HELPSTR_WEB_ADDRESS+1;

	// SPECIAL: custom help ids
	public static final int	A_HELPSTR_WEB_SITES					= A_HELPSTR_WEB_ADDRESS;

	public ABubbleHelp() 
	{
	}
}