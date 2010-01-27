
#ifndef CCUSTOMTIMER_H
#define CCUSTOMTIMER_H

#include <windows.h>

class cCustomTimer
{
	private:
		enum TIMER_COMMAND { TIMER_RESET, TIMER_START, TIMER_STOP, TIMER_ADVANCE, TIMER_GETABSOLUTETIME, TIMER_GETAPPTIME, TIMER_GETELAPSEDTIME };
		float __stdcall Timer ( TIMER_COMMAND command );

		BOOL     m_bTimerInitialized;
		BOOL     m_bUsingQPF;
        BOOL     m_bTimerStopped;
        LONGLONG m_llQPFTicksPerSec;

	public:
		void  Reset				( void );
		void  Start				( void );
		void  Stop				( void );
		void  Advance			( void );

		float GetAbsoluteTime	( void );
		float GetAppTime		( void );
		float GetElapsedTime	( void );
};

#endif CCUSTOMTIMER_H



