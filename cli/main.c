#include "soarapi.h"
#include "soar_core_api.h"
#include "soarkernel.h"
#include "parsing.h"
#include "soarInterfaceCommands.h"
#include "ask.h"
#include "linenoise.h"
#include "encodings/utf8.h"
#include <stdio.h>


/* Invocation help text */
void usage_help(char* progname) 
   {
   fprintf(stderr, "Usage: %s [--multiline] [--keycodes] [--help]\n", progname);
   }

/* Command line completion strings */
void completion(const char *buf, linenoiseCompletions *lc)
   {
   if (buf[0] == 'h')
      {
      linenoiseAddCompletion(lc,"SiOO");
      linenoiseAddCompletion(lc,"SiOO*");
      }
   }

/* MAIN *************************************************************
*/

int main( int argc, char *argv[] )
   {
   /* Lets go through these carefully for any #CRUFT */
   char *progname = argv[0];    /* NEED */
   agent *agent_handle;         /* NEED */
   soarResult res;              /* NEED */
   char *line;                  /* NEED */
   int cmd_error;               /* NEED */

/* CONFIGURATION OF SiOO DEFAULTS AND INVOCATION ********************
*/

   /* FIRST: open our rc file and load any options */
   /* sioo_getconfig("~/.sioorc"); */

   /* SECOND: lets load our options on the Invocation */
   while(argc > 1) {
      argc--;
      argv++;
      if (!strcmp(*argv,"--multiline")) {
	 linenoiseSetMultiLine(1);
         printf("Multi-line mode enabled.\n");

      } else if (!strcmp(*argv,"--keycodes")) {
         linenoisePrintKeyCodes();
         exit(0);

      } else if (!strcmp(*argv, "--help")) {
         usage_help(progname);
         exit(0);

      } else {
         usage_help(progname);
         exit(1);
      }
   }   

/* START THE SiOO KERNEL ********************************************
*/

   /* THIRD: We start the Soar kernel */
   soar_cInitializeSoar();
  
   /* FOURTH: We create an agent to work with */
   soar_cCreateAgent( "theAgent" );
   agent_handle = soar_cGetAgentByName( "theAgent" );

   /* FIFTH: We register our callback functions */
   soar_cPushCallback( agent_handle,
                      PRINT_CALLBACK,
                      (soar_callback_fn) cb_print,
                      NULL, NULL );

   /* Register our destructor function */
   soar_cPushCallback( agent_handle, 
                      SYSTEM_TERMINATION_CALLBACK, 
		      (soar_callback_fn) cb_exit,
                      NULL, NULL );

  /* Register our ask callback */
  soar_cPushCallback( agent_handle,
                      ASK_CALLBACK, 
                      (soar_callback_fn) askCallback,
                      NULL, NULL);

  /* SIXTH:  We initialize our command table */
  init_soar_command_table();

/* LINENOISE SETUP **************************************************
*/
    
   /* Setup the linenoise encoding functions for UTF8 */
    linenoiseSetEncodingFunctions( linenoiseUtf8PrevCharLen, 
                                   linenoiseUtf8NextCharLen, 
                                   linenoiseUtf8ReadCode);

    /* Set the completion callback. */
    /* This will be called every time the user uses the <tab> key. */
    linenoiseSetCompletionCallback(completion);

    /* Load history from file. Plain text, /n delimited */
    linenoiseHistoryLoad("~/.sioo_history");

    /* Now this is the main loop of the typical linenoise-based application.
     * The call to linenoise() will block as long as the user types something
     * and presses enter.*/
    while(1)
	 {
	 /* Get a line from linenoise */
	 line = linenoise("\x1b[32;1m[-OK-]\x1b[31;1mSiOO --\x1b[0m> ");
	 /* Sanitize and Sanity-ize the string */
	 if (line[0] != '\0' && line[0] != '/') 
	    {

	    /* ALL OF THE ACTION GOES HERE */
	    /* so here we have a (char* line) from linenoise();... */
	    /* and we need to get it to executeCommand() */
	    executeCommand ( line );

            /* Update history */
            linenoiseHistoryAdd(line);
            /* Update history file */
            linenoiseHistorySave("~/.sioo_history");
            /* Check for command to update history length */
	    } else if (!strncmp(line,"/historylen",11)) {
	       int len = atoi(line+11);
	       linenoiseHistorySetMaxLen(len);
	       /* Catch everything we don't recognize and warn user */
	    } else if (line[0] == '/') {
	       printf("Unreconized command: %s\n", line);
	    }
        /* CALLER MUST FREE EACH LINE RETURNED! */
        free(line);
	 }
return 0;
}






