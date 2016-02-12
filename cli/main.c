#include "soarapi.h"
#include "soar_core_api.h"
#include "soarkernel.h"
#include "parsing.h"
#include "commands.h"
#include "linenoise/linenoise.h"
#include "linenoise/utf8.h"
#include "callbacks.h"
#include "utilfuncs.h"

#include <stdio.h>
#include <string.h>

/* Invocation help text */
void usage_help(char* progname) 
   {
   fprintf(stderr, "Usage: %s [--multiline] [--keycodes] [--help]\n", progname);
   }

/* MAIN *************************************************************
*/

int main( int argc, char *argv[], char *envp[] )
   {
   /* CLI System Variables 
   */
   char *progname = argv[0];	 /* Store invocation name */
   char *line;			 /* Current command line from linenoise */
   /* int cmd_error; */		 /* Error Number for CLI Errors on prompt */

   /* SiOO Kernel Variables 
   */
   agent *agent_handle;		 /* Just what it says */
   /*   soarResult res;	*/	 /* Error result from Soar kernel */

   /* SiOO Environment Variables
   */
	char *env_TERM = getenv("TERM");
	char *env_HOME = getenv("HOME");
	char FN_dotConfig[256] = "";
	char FN_dotHistory[256] = "";

/* TODO Setup fall through commands */
/*	char *env_SIOO_ENVIRO = getenv("SIOO_ENVIRO");
   	char *env_PATH = getenv("PATH");
*/
	
	if ( !strncmp("xterm", env_TERM, 4) 
		|| !strncmp("vt-100", env_TERM, 6)
		|| !strncmp("ansi", env_TERM, 4) )
		{
       		printf("DEBUG: Terminal type set to %s.\n", env_TERM);
       		}
	else
		{
		printf("WARNING: TERM set to: %s SiOO may not function correctly.\n", env_TERM);
		}
 
	if ( env_HOME ) 
		{
		printf("DEBUG: Home Directory set to %s.\n", env_HOME);
		/* setup our history file name */
		strcat(FN_dotHistory, env_HOME);
		strcat(FN_dotHistory, "/.sioo_history");	
		printf("DEBUG: History File set to %s.\n", FN_dotHistory);
		/* setup our configuration file name */
		strcat(FN_dotConfig, env_HOME);
		strcat(FN_dotConfig, "/.sioorc");	
		printf("DEBUG Config File set to %s.\n", FN_dotConfig);
		}
	else 
		{
		print("WARNING: HOME Environment Variable is not Set!\n");
		print("WARNING: Setting HOME to Current Working Directory.\n");
		}
/*
	if ( env_SIOO_ENVIRO ) 
		{
		printf("SiOO Environment set to: %s.\n", env_SIOO_ENVIRO);
		}
		else
	       	{
		print("WARNING: SiOO Environment is not Set! No Options in affect!\n");
		}
 
	if ( env_PATH ) 
		{
		print("Using the Following PATH for non-built-ins and bang commands:\n");
		printf("%s \n", env_PATH);
		}
		else
		{
		print("WARNING: PATH Environment Variable is not Set!\n");
		print("WARNING: Setting PATH to Current Working Directory.\n");
		} 
	char *env_X = getenv(X);
	if (env_X) 
		{
		;
		}
		else { ; }
 */ 
  
/* CONFIGURATION OF SiOO DEFAULTS AND INVOCATION ********************
*/

   /* FIRST: open our rc file and load any options */
   /* sili_GetConfig("~/.sioorc"); */

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
      /* DECL: agent* agent_handle */
   soar_cCreateAgent( "theAgent" );
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

   /* We now have an agent handle which we use to make calls into 
    * the kernel for loading productions, setting execution options,
    * and running our our agent. 
    */

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

    /* Load history from file. */
    linenoiseHistoryLoad(FN_dotHistory);

   /* THIS REMAINS IMMEDIATELY BEFORE while(42) loop */
   cmd_PrintBanner();
   /* REPL -->
      * call to linenoise() blocks until the user invokes the command or aborts
      */
    while(42)
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
	    printf("\n"); /* BY,BUG#8: Flush the kernel output buffer */

            /* Update history */
            linenoiseHistoryAdd(line);

            /* Update history file */
            linenoiseHistorySave(FN_dotHistory);

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
   /* Close any SiOO logs that are open */
      /* executeCommand("log -off"); ? or call an API function*/

   /* Shutdown Cleanly the SiOO kernel */
      /* executeCommand() ? or call an API function? */

   /* Save our configuration settings */
      /* sioo_SaveConfig("~/.sioorc"); */

   /* Save application log file(s) */

return 0;
}






