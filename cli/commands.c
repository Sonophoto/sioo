/* **************************************************************** *
             _____     ____  _  ___   ___
   GNU/     / ___/    / ___|(_)/ _ \ / _ \
     /ANSI / /__      \___ \| | | | | | | |
           \___/       ___) | | |_| | |_| |
                      |____/|_|\___/ \___/
 
   FILENAME: commands.c AUTHOR: BrigYoung 
   COPYRIGHT: "2015, 2016 Brig Young" LICENSE: "GPLv3, see LICENSE"         
   PURPOSE: "Monolithic file full of commands and helper functions."               
   DERIVED: "From code by [original CLI author]'
 * **************************************************************** */
/* We are using ANSI C with GNU extensions */
#define _GNU_SOURCE

#include "soarapi.h"
#include "soarkernel.h"
#include "parsing.h"
#include "demos/toh_demo.h"
#include "demos/counter_demo.h" 
#include "linenoise/linenoise.h"
#include "commands.h"

#include <unistd.h>
/*
 * Variable Declarations
 */
hash_table *gSoarCommands;
list *gDirectoryStack = NULL;

void
cmd_PrintBanner(void)
   {
print("\n\
 ____  _  ___   ___       ___   ___   ___    ___\n\
/ ___|(_)/ _ \\ / _ \\     / _ \\ / _ \\ |__ \\  / _ \\ \n\
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

/*  This is command is invoked by the user with the following
 *  syntax:
 *             set <what> <value>
 *
 *  It is intended to help maintain some amount of consistency with
 *  the Tcl interface but as can be seen, it has only minimal functionality
 */
int
shellnoise_set( int argc, const char **argv, soarResult *res )
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
shellnoise_pushd(int argc, const char **argv, soarResult *res)
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
shellnoise_popd (int argc, const char **argv, soarResult *res) 
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
  FILE *F_HANDLE;
  bool eof_reached;

  if (argc < 2) 
  {
    setSoarResultResult(res, "No Filename specified\n");
    return SOAR_ERROR;
  }
  
  F_HANDLE = fopen(argv[1], "r");
  
  if (!F_HANDLE) 
  {
    setSoarResultResult(res, "Could not open file '%s'\n", argv[1]);
    return SOAR_ERROR;
  }

  eof_reached = FALSE;
  while(!eof_reached)
  {
    executeCommand(getCommandFromFile(fgetc, F_HANDLE,  &eof_reached));
  }
  
  fclose(F_HANDLE);
  clearSoarResultResult(res);
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
interface_echo( int argc, const char **argv, soarResult *res ) {
  
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





/*
 * SECTION 2:  THE COMMAND TABLE
 *
 *  Within this second section, we define a number of functions
 *  used to deal with the global command table.  In essence functions
 *  which define the capabilities of the interface.
 *
 */


/*
 *  Initialize the command table.
 *
 *  The command table is a global structure (a hash table) which
 *  stores allows commands to be looked up by name, and invoked via a
 *  function pointer.  This function serves only to initilize this
 *  command table, and name each one of the commands availble to the
 *  user.  Note that commands may be added under more that one name,
 *  but that names must be unique.
 */
void
init_soar_command_table( void ) {
  
  gSoarCommands = make_hash_table( 6, (hash_function)hash_soar_command );
  
   add_to_hash_table( gSoarCommands, 
      new_soar_command( "create-agent", soar_CreateAgent ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "destroy-agent", soar_DestroyAgent ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "capture-input", soar_CaptureInput ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "replay-input", soar_ReplayInput ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "chunk-name-format", soar_ChunkNameFormat ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "max-elaborations", soar_MaxElaborations ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "max-chunks", soar_MaxChunks ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "soar8-mode", soar_Operand2 ) );

/* Deprecated DO NOT USE */
   add_to_hash_table( gSoarCommands, 
      new_soar_command( "soar8", soar_Operand2 ) );
/* Deprecated DO NOT USE */
   add_to_hash_table( gSoarCommands, 
      new_soar_command( "operand2", soar_Operand2 ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "waitsnc", soar_WaitSNC ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "input-period", soar_InputPeriod ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "multi-attributes", soar_MultiAttributes ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "numeric-indifferent-mode", soar_NumericIndifferentMode ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "o-support-mode", soar_OSupportMode ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "explain-backtraces", soar_ExplainBacktraces ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "firing-counts", soar_FiringCounts ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "format-watch", soar_FormatWatch ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "indifferent-selection", soar_IndifferentSelection ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "internal-symbols", soar_InternalSymbols ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "memories", soar_Memories ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "production-find", soar_ProductionFind ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "preferences", soar_Preferences ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "pwatch", soar_PWatch ) );

#ifdef USE_DEBUG_UTILS
   add_to_hash_table( gSoarCommands, 
      new_soar_command( "pool", soar_Pool ) );
#endif

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "stop-soar", soar_Stop ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "verbose", soar_Verbose ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "warnings", soar_Warnings ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "log", soar_Log ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "attribute-preferences-mode", soar_AttributePreferencesMode ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "default-wme-depth", soar_DefaultWmeDepth ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "ex-build-info", soar_ExcludedBuildInfo ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "interrupt", soar_Interrupt ) );
/*
   add_to_hash_table( gSoarCommands, 
      new_soar_command( "",  ) );
*/
   add_to_hash_table( gSoarCommands, 
      new_soar_command( ".", interface_Source ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "add-wme", soar_AddWme ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "build-info", soar_BuildInfo ) );

/*  BUGBUG This segfaults on call...      */
/*  rptd as bug #20 but cant reproduce it */
/*
   add_to_hash_table( gSoarCommands, 
      new_soar_command( "clear", linenoiseClearScreen ) );
*/
   add_to_hash_table( gSoarCommands, 
      new_soar_command( "counter-demo", interface_counter_demo ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "echo", interface_echo ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "excise", soar_Excise ) );

   add_to_hash_table( gSoarCommands,
      new_soar_command( "indifferent-selection", soar_IndifferentSelection ) ); 

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "init-soar", soar_ReInitSoar ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "learn", soar_Learn ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "log", soar_Log ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "matches", soar_Matches ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "multi-attributes", soar_MultiAttributes ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "pf", soar_ProductionFind ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "pfind", soar_ProductionFind ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "popd", shellnoise_popd ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "pref", soar_Preferences ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "p", soar_Print ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "print", soar_Print ) );
/*
   add_to_hash_table( gSoarCommands, 
      new_soar_command( "print-banner", cmd_PrintBanner ) );
*/
   add_to_hash_table( gSoarCommands, 
      new_soar_command( "pushd", shellnoise_pushd ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "quit", soar_Quit ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "exit", soar_Quit ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "bye", soar_Quit ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "remove-wme", soar_RemoveWme ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "rete-net", soar_ReteNet ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "run", soar_Run ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "set", shellnoise_set ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "source", interface_Source ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "sp", soar_Sp ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "stats", soar_Stats ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "toh-demo", interface_toh_demo ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "v", soar_Verbose ) );

   add_to_hash_table( gSoarCommands,
      new_soar_command( "verbose", soar_Verbose ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "w", soar_Watch ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "watch", soar_Watch ) );
   
#ifdef USE_CAPTURE_REPLAY
   add_to_hash_table( gSoarCommands,
      new_soar_command( "capture-input", soar_CaptureInput ) );
   add_to_hash_table( gSoarCommands,
      new_soar_command( "replay-input", soar_ReplayInput ) );
#endif
}

/* 
 *   Return the Hash value for a soar_command_string
 */
int
hash_soar_command_string(char *name, short nbits)
{
  int val, length;

  if ( name == NULL ) return 0;

  length = strlen( name );  
  val = length + 26 * (int)name[0];
  
  switch (length)
  {
    default:
      val += (int)name[3];
    
    case 3:
      val += (int)name[2];
    
    case 2:
      val += (int)name[1];

    case 1:
      break;
  }

  return val & masks_for_n_low_order_bits[nbits];
}  


/* 
 *   Return the Hash value for a soar_command structure 
 */
int hash_soar_command (void *item, short nbits)
{
  return hash_soar_command_string(((soar_command *)item)->command_name, nbits);
}



/*
 *  Create a new command structure, to be added into the global comman table
 */
soar_command*
new_soar_command(char *name, int (*cmd)(int, const char **, soarResult *))
{
  soar_command *c;

  c = malloc( sizeof( soar_command ) );
  c->command_name = malloc( (strlen( name ) + 1) * sizeof(char) );
  strcpy( c->command_name, name );
  c->command = cmd;
  return c;
}


  
/*
 *  Check for the existence of a command.  If the command does indeed
 *  exist, return a structure containing its registry information
 *  (including a pointer to the function itself)
 */
soar_command*
find_soar_command_structure(char *name)
{
  int commandHash;
  soar_command *theCommand;

  commandHash = hash_soar_command_string( name, gSoarCommands->log2size );
  theCommand = (soar_command *) (*(gSoarCommands->buckets + commandHash ));
  
  for ( ; theCommand != NIL; theCommand = theCommand->next )
  {
    if (!strcmp( name, theCommand->command_name))
    {
      break; 
    }
  }
  return theCommand;
}

