#include "soarapi.h"
#include "soar_core_api.h"
#include "soarkernel.h"
#include "parsing.h"
#include "soarInterfaceCommands.h"
/* #include "ask.h" */
#include "linenoise.h"
#include "encodings/utf8.h"
#include <stdio.h>


/* Invocation help text */
void usage_help(char* progname) 
   {
   fprintf(stderr, "Usage: %s [--multiline] [--keycodes] [--help]\n", progname);
   }

/* Linenoise command completion callbacks */
/*
*  This completion system needs fixed or replaced. If you have a habit
*  of tabbing for filename completion, you will be very disappointed
*  when it erases your command string...
*/
void completion_CB(const char *buf, linenoiseCompletions *lc) {
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
	 if (buf[1] == 'o')
	    { 
	    linenoiseAddCompletion(lc,"popd");
	    } else {
	    linenoiseAddCompletion(lc,"pushd");
	    }

      } else if (buf[0] == 'q') {
        linenoiseAddCompletion(lc,"quit");

      } else if (buf[0] == 'r') {
	 if (buf[1] == 'e')
	    { 
	    linenoiseAddCompletion(lc,"rete-net"); 
	    } else {
	    linenoiseAddCompletion(lc,"run 1");
	    linenoiseAddCompletion(lc,"run 10");
	    linenoiseAddCompletion(lc,"run 100");
	    }
        
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

/* TODO: Stray Callback for the SiOO kernel needs a new home */
void askCallback( soar_callback_agent the_agent,
		  soar_callback_data data,
		  soar_call_data call_data ) {
  
  int num_candidates;
  preference *cand;

  *((soar_apiAskCallbackData *)call_data)->selection = 
    ((soar_apiAskCallbackData *)call_data)->candidates;

  num_candidates = 0;

  for (cand = ((soar_apiAskCallbackData *)call_data)->candidates;
       cand!=NIL; cand=cand->next_candidate) {
    num_candidates++;
    print( " --> %s\n", symbol_to_string( cand->value, TRUE, NULL, 0 ) );
  }
}

/* MAIN *************************************************************
*/

int main( int argc, char *argv[] )
   {
   /* CLI System Variables 
   */
   char *progname = argv[0];	 /* Store invocation name */
   char *line;			 /* Current command line from linenoise */
   /* int cmd_error; */		 /* Error Number for CLI Errors on prompt */

   /* SiOO Kernel Variables 
   */
   agent *agent_handle;		 /* Just what it says */
   /*   soarResult res;	*/	 /* Error Number from Soar kernel */

/* CONFIGURATION OF SiOO DEFAULTS AND INVOCATION ********************
*/

   /* FIRST: open our rc file and load any options */
   /* sioo_getconfig("~/.sioorc"); */

   /* SECOND: lets load our options on the Invocation */
   while(argc > 1) {
      argc--;
      argv++; /* we stored argv[0] as (char*) progname */
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

   /* FIRST: We start the Soar kernel */
   soar_cInitializeSoar();
  
   /* SECOND: We create an agent to work with */
   soar_cCreateAgent( "theAgent" );
      /* DECL: agent* agent_handle */
   agent_handle = soar_cGetAgentByName( "theAgent" );

   /* THIRD: We register our callback functions */
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

   /* We now have an agent handle which we use to make calls into the kernel
   *  for loading productions, setting execution options, and running our
   *  our agent. */

/* COMMAND PROCESSING SETUP **************************************************
*/
   /* Setup the command table from siooInterfaceCommands.c 
   */
   init_soar_command_table();
    
   /* Setup the linenoise encoding functions for UTF8 */
    linenoiseSetEncodingFunctions( linenoiseUtf8PrevCharLen, 
                                   linenoiseUtf8NextCharLen, 
                                   linenoiseUtf8ReadCode);

    /* Set the completion callback. User triggers with <TAB> key */
    linenoiseSetCompletionCallback(completion_CB);

    /* Load history from file. Plain text, /n delimited */
    linenoiseHistoryLoad("~/.sioo_history");

   /* REPL -->
      * call to linenoise() blocks until the user invokes the command or aborts
      */
    while(1)
	 {
	 /* TODO: So here we want to check for an error condition and return a prompt
		  based on it. Display an -OK- in green if all is well and display
		  the error code in red if something went wrong on the last command
	 */
	 /* Get a line from linenoise */
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
	 } /* END: REPL LOOP */

/* TODO: Cleanup everything for an honest exit */

return 0;
}






