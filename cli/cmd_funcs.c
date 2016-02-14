/* **************************************************************** *
         _____
    GNU / ___/
       / /__  
       \___/  
  
   FILENAME: cmd_funcs.c AUTHOR: BrigYoung 
   COPYRIGHT: "2015, 2016 Brig Young" LICENSE: "GPLv3, see LICENSE"         
   PURPOSE: "Built-in cmd_functions and sioo_API CLI functions"               
   DERIVED: "From code by [original CLI author]'
 * **************************************************************** */
/* We are using ANSI C with GNU extensions */

#define _GNU_SOURCE
#include "cmd_funcs.h"

#include "soarapi.h"
#include "soarkernel.h"
#include "parsing.h"
#include "callbacks.h"
#include "demos/toh_demo.h"
#include "demos/counter_demo.h" 
#include "linenoise/linenoise.h"

#include <unistd.h>

/*
 * TODO: THESE GO IN cmd_funcs.h ! Variable Declarations
 */
hash_table *gSoarCommands;
list *gDirectoryStack = NULL;


/*
cmd_PrintBanner()
*/
int
cmd_PrintBanner( int argc, const char **argv, soarResult *res )
{
  clearSoarResultResult( res );

print("\n\
 ____  _  ___   ___       ___   ___   ___    ___   \n\
/ ___|(_)/ _ \\ / _ \\     / _ \\ / _ \\ |__ \\  / _ \\  \n\
\\___ \\| | | | | | | |   | (_) | (_) |  _) || | | | \n\
 ___) | | |_| | |_| |    \\__, |\\__, | / _/ | |_| | \n\
|____/|_|\\___/ \\___/       /_(_) /_(_)___(_)____/  beta\n\
\n\
State<input> Operator Output 9.9.2.0 beta\n\
\n\
\n\
Copyright (C) 2016 Brig Young and many others. See HISTORY.\n\
GNU GPLv3 -- https://www.gnu.org/licenses/gpl-3.0.html See LICENSE.\n\
Development of SiOO sponsored by the generous support of:\n\
Module Master LLC. http://modulemaster.com/rebuilds/about-us/\n\
***************************************************************************\n\
                    This is Liberated Software\n\
    Type 'license' for details of your Rights and Responsibilities.\n\
                    There is ABSOLUTELY NO WARRANTY!\n\
   not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n\ 
***************************************************************************\n\
useful commands: help, help help, license, build-info, version\n\
\n");
if (argc > 5) {printf("Wow! %d is a lot of arguments!\n", argc);}
if (!strncmp(argv[0], "print-banner", 12)) {printf("42 --> %d\n", argc);}

/* setSoarResultResult( res, "Too few arguments to 'set'\n" ); */
return SOAR_ERROR;
}

/*
 * SECTION 1:  REINVENTING TICKLE ALL OVER AGAIN
 *
 *  Within this first section, we define a number of commands
 *  particular to this shell-interface to Soar.  These include
 *  callbacks which are registered with the Soar kernel at startup
 *  as well a few commands which may be invoked directly by the 
 *  user.
 *
 */

/*
 *  This is command is invoked by the user with the following
 *  syntax:
 *             set <what> <value>
 *
 *  It is intended to help maintain some amount of consistency with
 *  the Tcl interface but as can be seen, it has only minimal functionality
 */
int
interface_Set( int argc, const char **argv, soarResult *res )
{
  if ( argc < 2 )
  { 
    setSoarResultResult( res, "Too few arguments to 'set'\n" );
    return SOAR_ERROR;
  }

  if ( !strcmp( argv[1], "max_elaborations" ) )
  {
    return soar_MaxElaborations( argc-1, &argv[1], res );
  }

  setSoarResultResult( res, "Don't know how to set '%s'", argv[1] );
  return SOAR_ERROR;
}


/*
 *  This command is invoked by the user with the following
 *  syntax:
 *             pushd <dir>
 *
 *  It is intended to help maintain some amount of consistency with
 *  the Tcl interface, this function is not a part of the api because
 *  the particualr implementation may vary based upon OS and interface
 *  details.
 */
int
interface_pushd( int argc, const char **argv, soarResult *res )
{
  cons *c;
  char *cdir;
  
   if ( argc < 1 )
   {
      return SOAR_ERROR;
   }
  
  c = (cons *) malloc( sizeof( cons ) );
  cdir = (char *) malloc( 1024 * sizeof( char ) );
  sys_getwd( cdir, 1024 );
  c->first = cdir;
  c->rest = gDirectoryStack;
  gDirectoryStack = c;
  
  sys_chdir( argv[1] );
  print( "Changing Directory to '%s'\n", argv[1] );
  
  return SOAR_OK;
}



/*
 *  This is command is invoked by the user with the following
 *  syntax:
 *             popd
 *
 *  It is intended to help maintain some amount of consistency with
 *  the Tcl interface, this function is not a part of the api because
 *  the particualr implementation may vary based upon OS and interface
 *  details.
 */
int
interface_popd ( int argc, const char **argv, soarResult *res ) 
{
  
  cons *c;

  if ( gDirectoryStack == NULL ) {
    print ( "Directory Stack is empty!\n" );
    return SOAR_ERROR;
  }
  sys_chdir( (char *)(gDirectoryStack->first) );
  c = gDirectoryStack;
  gDirectoryStack = gDirectoryStack->rest;
  
  free( c );
  return SOAR_OK;
}
  


/*
 *  This is command is invoked by the user with the following
 *  syntax:
 *             source <filename>
 *
 *  It is intended to help maintain some amount of consistency with
 *  the Tcl interface, this function is not a part of the api because
 *  the particualr implementation may vary based upon OS and interface
 *  details.
 */
int
interface_Source( int argc, const char **argv, soarResult *res ) 
{
  FILE *f;
  bool eof_reached;

  if ( argc < 2 ) {
    setSoarResultResult( res, "No Filename specified\n" );
    return SOAR_ERROR;
  }
  
  f = fopen( argv[1], "r" );
  
  if ( !f ) {
    setSoarResultResult( res, "Could not open file '%s'\n", argv[1] );
    return SOAR_ERROR;
  }

  eof_reached = FALSE;
  while( !eof_reached ) {
    executeCommand( getCommandFromFile( fgetc,f,  &eof_reached ) );
  }
  
  fclose( f );
  clearSoarResultResult( res );
  return SOAR_OK;
}
  


/*
 *  This is command is invoked by the user with the following
 *  syntax:
 *             echo <what>
 *
 *  It is intended to help maintain some amount of consistency with
 *  the Tcl interface, this function is not a part of the api because
 *  the particualr implementation may vary based upon OS and interface
 *  details.  We use the soar "print" function to create output that 
 *  is treated in the same manner as that which is generated by the
 *  soar kernel.
 */
int
interface_echo( int argc, const char **argv, soarResult *res )
{
  int i;
  
  for( i = 1; i < argc; i++ )
    {
      print( "%s ", argv[i] );
    }
  print( "\n" );

  clearSoarResultResult( res );
  return SOAR_OK;
}



/*
DEMONOSTRATIONS
*/

/*
 *  This is command is invoked by the user with the following
 *  syntax:
 *             counter-demo
 *
 *  It loads the agent, and initializes soar for running the demo
 * 
 *  This agent reads off the input-link and writes to the 
 *  output-link providing a simple example of how to create a 
 *  closed loop connection with any external environment.
 */
int
interface_counter_demo( int argc, const char **argv, soarResult *res )
   {
   /* ON LOAD we excise all existing productions and (re)init-soar */
   soar_cExciseAllProductions();
   soar_cReInitSoar();

   /* BUGBUG #8 This is where the agent file name is hardcoded*/
   executeCommand( "source ./demos/counter.soar" );

   /* Remove all Input Phase Callbacks */
   soar_cRemoveAllCallbacksForEvent( soar_agent, INPUT_PHASE_CALLBACK );

   /* Remove all Output Phase Callbacks */
   soar_cRemoveAllCallbacksForEvent( soar_agent, OUTPUT_PHASE_CALLBACK );

   /* Initialize global variables used by the IO functions */
   last_tt = -1;
   number_received = -1;

   /* Now we Add an Input Function for our agent */
   soar_cAddInputFunction( soar_cGetCurrentAgent(),
			  (soar_callback_fn) io_input_fn ,
			  NULL, NULL,"input-link" );

   /* Now we Add an Output Function for our agent */
   soar_cAddOutputFunction( soar_cGetCurrentAgent(),
			   (soar_callback_fn) io_output_fn ,
			   NULL, NULL, "output-link" );

   /* output status to CLI and return */
   setSoarResultResult( res, "counter demo ready" );
   return SOAR_OK;
}


/*
 *  This is command is invoked by the user with the following
 *  syntax:
 *             toh-demo
 *
 *  It loads the agent, and initializes soar for running the demo
 */
int
interface_toh_demo( int argc, const char **argv, soarResult *res ) 
   {
   /* ON LOAD we excise all existing productions and (re)init-soar */
   soar_cExciseAllProductions();
   soar_cReInitSoar();
   /* BUGBUG #8 This is where the agent file name is hardcoded*/
   executeCommand( "source ./demos/toh.soar" );
 
   soar_cSetWaitSNC( TRUE );

   /* Remove all Input Phase Callbacks */
   soar_cRemoveAllCallbacksForEvent( soar_agent, INPUT_PHASE_CALLBACK );

   /* Remove all Output Phase Callbacks */
   soar_cRemoveAllCallbacksForEvent( soar_agent, OUTPUT_PHASE_CALLBACK );

   /* Now we Add an Input Function for our agent */
   soar_cAddInputFunction( soar_cGetCurrentAgent(),
			  (soar_callback_fn) toh_input_fn ,
			  (soar_callback_data) NULL, NULL,"input-link" );
  
   /* Now we Add an Output Function for our agent */
   soar_cAddOutputFunction( soar_cGetCurrentAgent(),
			   (soar_callback_fn) toh_output_fn ,
			   (soar_callback_data) NULL, NULL, "output-link" );

   /* output status to CLI and return */
   setSoarResultResult( res, "toh demo ready (non-pausing-style)" );
   return SOAR_OK;
}

