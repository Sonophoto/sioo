/* **************************************************************** *
         _____
    GNU / ___/
       / /__  
       \___/  
  
   FILENAME: callbacks.h AUTHOR: BrigYoung 
   COPYRIGHT: "2015, 2016 Brig Young" LICENSE: "GPLv3, see LICENSE"         
   PURPOSE: "Callbacks used by the CLI system and kernel"               
   DERIVED: "From code by [original CLI author]'
 * **************************************************************** */
/* We are using ANSI C with GNU extensions */
#define _GNU_SOURCE

/*
SiOO kernel callbacks includes:
*/
#include "soarapi.h"
#include "soarkernel.h"

/*
Linenoise library callbacks includes:
*/
#include "linenoise/linenoise.h"



/* **************************************************************** *
   Callbacks for the SiOO kernel
 * **************************************************************** */

/*
TODO: Purpose? askCallback()
*/
void
askCallback( soar_callback_agent the_agent, soar_callback_data data, soar_call_data call_data );

/*
This function is registered with the Soar kernel, and called 
upon system termination.  Any last second clean-ups or exit
notifications should be added to this function body
*/
void
cb_exit ( agent *the_agent, soar_callback_data d, soar_call_data c );

 
/*
This funtion is registered with the Soar kernel and called when
Soar generates output.  In this simple shell-interface, we need
only print that output using standard IO function calls
*/
void
cb_print ( agent *the_agent, soar_callback_data d, soar_call_data c );



/* **************************************************************** *
   Linenoise command completion callbacks
 * **************************************************************** */

/*
This completion system needs fixed or replaced. If you have a habit
of tabbing for filename completion, you will be very disappointed
when it erases your command string...
*/
void
completion_CB(const char *buf, linenoiseCompletions *lc); 


