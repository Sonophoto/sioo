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

void completion(const char *buf, linenoiseCompletions *lc) {
    if (buf[0] == 'a')
      {
        linenoiseAddCompletion(lc,"add-wme");
      } else if (buf[0] == 'b') {
        linenoiseAddCompletion(lc,"bye");
      } else if (buf[0] == 'c') {
        linenoiseAddCompletion(lc,"counter-demo");
      } else if (buf[0] == 'e') {
        linenoiseAddCompletion(lc,"excise -all");
        linenoiseAddCompletion(lc,"excise");
        linenoiseAddCompletion(lc,"exit");
      } else if (buf[0] == 'i') {
        linenoiseAddCompletion(lc,"init-soar");
      } else if (buf[0] == 'p') {
        linenoiseAddCompletion(lc,"popd");
        linenoiseAddCompletion(lc,"pushd");
      } else if (buf[0] == 'q') {
        linenoiseAddCompletion(lc,"quit");
      } else if (buf[0] == 'r') {
        linenoiseAddCompletion(lc,"run 1");
        linenoiseAddCompletion(lc,"run 10");
        linenoiseAddCompletion(lc,"run 100");
        linenoiseAddCompletion(lc,"rete-net");
      } else if (buf[0] == 's') {
        linenoiseAddCompletion(lc,"source");
      } else if (buf[0] == 't') {
        linenoiseAddCompletion(lc,"toh-demo");
/*
      } else if (buf[0] == '') {
      linenoiseAddCompletion(lc,"exit");
*/
      } /* END if (buf[0]) */
} /* END completion() */
/* MAIN *************************************************************
*/

int main( int argc, char *argv[] )
   {
   /* Lets go through these carefully for any #CRUFT */
   char *progname = argv[0];    /* NEED */
   agent *agent_handle;         /* NEED */
/*   soarResult res;              NEED */
   char *line;                  /* NEED */
/*   int cmd_error;              NEED */

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
         printf("Bad command usage, please review your options:\n\n");
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
	 /* TODO: So here we want to check for an error condition and return a prompt
		  based on it. Display an -OK- in green if all is well and display
		  the error code in red if something went wrong on the last command
	 */
	 line = linenoise("\x1b[32;1m[-OK-]\x1b[31;1mSiOO --\x1b[0m> ");
	 /* Sanitize and Sanity-ize the string */
	 if (line[0] != '\0' && line[0] != '/') 
	    {

	    /* ALL OF THE ACTION GOES HERE */
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






