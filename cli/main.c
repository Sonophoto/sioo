#include "soarapi.h"
#include "soar_core_api.h"
#include "soarkernel.h"
#include "parsing.h"
#include "soarInterfaceCommands.h"
#include "ask.h"
#include <stdio.h>


/* This is the entry point for the CLI */
int main( int argc, char *argv[] )
{
  int cmd_error;
  agent *agent_handle;
  bool eof_reached;
  int i;
  soarResult res;
  char **temp;

   /* FIRST We start the Soar kernel */
  soar_cInitializeSoar();
  
  /* SECOND We create an agent to work with */
  soar_cCreateAgent( "theAgent" );
  agent_handle = soar_cGetAgentByName( "theAgent" );

  /* NEXT We register a callback function to handle print statements */
  soar_cPushCallback( agent_handle,
                      PRINT_CALLBACK,
                      (soar_callback_fn) cb_print,
                      NULL, NULL );

  /* NEXT We register our destructor function */
  soar_cPushCallback( agent_handle, 
                      SYSTEM_TERMINATION_CALLBACK, 
		      (soar_callback_fn) cb_exit,
                      NULL, NULL );

  /* */
  soar_cPushCallback( agent_handle,
                      ASK_CALLBACK, 
                      (soar_callback_fn) askCallback,
                      NULL, NULL);

  /* NEXT We initialize our command table */
  init_soar_command_table();

  /*
   * WTH???
   *
   * This small block of code allows Soar to deal with command-line 
   * arguments.  At most one argument is expected.  If such an argument is
   * found, it is taken to be the name of a file which is dealt with as 
   * though the user had typed "source <filename>" at the Soar prompt.
   */
  temp = (char **)malloc( 2 * sizeof( char * ) );
  temp[0] = (char *)malloc( 7 * sizeof( char ) );

  strcpy( temp[0], "source" );
  for( i = 1; i < argc; i++ ) 
  {
   print( "Sourcing '%s'\n", argv[i] );
   init_soarResult( res );
   temp[1] = (char *)malloc( (strlen(argv[i]) + 1) * sizeof( char ) );
   strcpy( temp[1], argv[i] );
   interface_Source( 2, temp, &res );
  } 
    
    
  /* The event loop */
  for (;;)
     {
     /* (cmd_error) ? prompt_error = itoa(cmd_error) : prompt_error = "-OK-" */
     print( "\n(-OK-)SiOO--> " );
     executeCommand ( getCommandFromFile( fgetc, stdin, &eof_reached ) );
     }

}







