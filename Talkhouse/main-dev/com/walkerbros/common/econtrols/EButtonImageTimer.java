// EButtonImageTimer.java

package com.walkerbros.common.econtrols;

import java.awt.*;

import com.walkerbros.common.ejava.*;
import com.walkerbros.common.ejava.eevent.*;
import com.walkerbros.common.eimage.*;
//import com.walkerbros.projects.walkerbros.aclasses.*;
//import com.walkerbros.projects.walkerbros.ajava.*;

public class EButtonImageTimer extends EButtonImage
{
	private long m_lTimerLimit	= 10L;
	private long m_lTimerCount	= 0;

	public EButtonImageTimer() 
	{
	}
	
	public boolean isType( short type )
	{
		if( EDefines.E_OBJ_TYPE_BUTTONIMAGETIMER == type )
			return true;
		return false;
	}
	
	public boolean isOfType( short type )
	{
		if( EDefines.E_OBJ_TYPE_BUTTONIMAGETIMER == type )
			return true;
		return super.isOfType( type );
	}

	public void loadFromDisk( EFile eFile, EErr err )
	{
		super.loadFromDisk( eFile,err );
		if( err.error() )
			return;
		
		m_lTimerLimit = eFile.readLong( err );
		if( err.error() )
			return;
	}
	
	public void saveToDisk( EFile eFile, EErr err )
	{
		super.saveToDisk( eFile,err );
		if( err.error() )
			return;
			
		eFile.writeLong( m_lTimerLimit,err );
		if( err.error() )
			return;
	}

	public boolean onTimer()
	{
		m_lTimerCount += 1;

		if( m_lTimerCount >= m_lTimerLimit )
		{
			takeAction();
			return false;
		}
		return true;
	}
}