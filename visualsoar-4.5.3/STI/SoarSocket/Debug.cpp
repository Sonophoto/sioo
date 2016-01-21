// Debug.cpp: implementation of the Debug functions.
//
//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdarg.h>
#include "Debug.h"
#include "Check.h"
#include "OSspecific.h"

// Initialize our nest level to zero
int	CTDebugEnterMethod::m_nCurrentNestLevel=0;

// Initialize the current method to "No Method"
//const char*	CTDebugEnterMethod::m_pCurrentMethodName = "No Method" ;

// Decide whether method names are printed when they are entered/exited
bool CTDebugEnterMethod::m_bPrintOnEnterMethod = false ;
bool CTDebugEnterMethod::m_bPrintOnExitMethod  = false ;

// Decide whether output will be indented with each function call
bool CTDebugEnterMethod::m_bIndentOutput = true ;

// Decide whether output will show previous stack trace
bool CTDebugEnterMethod::m_bShowStackTrace = true ;

// The stack of method names
StackTrace CTDebugEnterMethod::m_stackTrace ;

// Pointer into the stack showing the last item printed
long CTDebugEnterMethod::m_stackTraceDisplayPos = 0 ;

// Static method
int CTDebugEnterMethod::GetCurrentNestLevel()
{
	return CTDebugEnterMethod::m_nCurrentNestLevel;
}


// Note: This may be Windows specific way of handling
// variable args--there are other methods.
void PrintDebugFormat(char const* pFormat, ...)
{
	va_list args;
	va_start(args, pFormat);

	char szBuffer[512];

	int nBuf = VSNSPRINTF(szBuffer, sizeof(szBuffer), pFormat, args);

	// was there an error? was the expanded string too long?
	CHECK(nBuf >= 0) ;

	CTDebugEnterMethod::PrintStackTrace() ;

	PrintDebugMethod(CTDebugEnterMethod::GetCurrentNestLevel(), CTDebugEnterMethod::GetCurrentMethodName(), szBuffer) ;

	va_end(args);
}

void PrintDebug(char const* pStr)
{
	CTDebugEnterMethod::PrintStackTrace() ;

	PrintDebugMethod(CTDebugEnterMethod::GetCurrentNestLevel(), CTDebugEnterMethod::GetCurrentMethodName(), pStr) ;
}

#ifdef _WINDOWS
#include "stdafx.h"
void PrintDebugMethod(long indent, char const* pMethodName, char const* pStr)
{
	// We want PrintDebug to be able to output to the test
	// application as well as the debug stream.
	// BADBAD: I'm not sure how we should pass an "error handler"
	// into these functions in a general way for Unix and Windows.
	// For now I'll go with the assumption that this function is
	// called soon enough that the active window is the frame of the
	// test app.  This will be true almost always (unless break at
	// the first line in the debugger for instance).
	static HWND s_OutputWnd = NULL ;

	if (s_OutputWnd == NULL)
		s_OutputWnd = ::GetActiveWindow() ;

	if (!CTDebugEnterMethod::IsOutputIndented())
		indent = 0 ;

	if (s_OutputWnd)
	{
		// Text that is our string indent
		char szIndent[1024];
		szIndent[0]='\0';

		// Indent to the correct nested level
		int i;
		for (i=0; i < indent; i++)
		{
			strcat(szIndent, "   ");
		}

		// Add the debug string
		TCHAR textDebugString[1024] ;
		wsprintf(textDebugString, "%s%s %s\r\n", szIndent, pMethodName, pStr) ;

		// Ask the view to display this string
		// Note: Must use SendMessage or text will go out of scope.
		// Also s_OutputWnd must be in the same process as us.
		SendMessage(s_OutputWnd, WM_USER+1, (WPARAM)textDebugString, NULL) ;
	}

	// We add a newline which may be O/S specific.
	OutputDebugString(pMethodName) ;
	OutputDebugString(" ") ;
	OutputDebugString(pStr) ;
	OutputDebugString("\n") ;
}
#else	// _WINDOWS
// On Linux, dump to stderr (the console)
void PrintDebugMethod(long indent, char const* pMethodName, char const* pStr)
{
	indent += 0 ;

	fprintf(stderr, pMethodName) ;
	fprintf(stderr, pStr) ;
	fprintf(stderr, "\n") ;
}
#endif	// _WINDOWS
