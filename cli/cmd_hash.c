/* **************************************************************** *
             _____     ____  _  ___   ___
   GNU/     / ___/    / ___|(_)/ _ \ / _ \
     /ANSI / /__      \___ \| | | | | | | |
           \___/       ___) | | |_| | |_| |
                      |____/|_|\___/ \___/
 
   FILENAME: cmd_hash.c AUTHOR: BrigYoung 
   COPYRIGHT: "2015, 2016 Brig Young" LICENSE: "GPLv3, see LICENSE"         
   PURPOSE: "Hash function maps command string to function pointer"               
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
#include "cmd_funcs.h"
#include "cmd_hash.h"

#include <unistd.h>

/*
   Initialize the hash table for our command strings

   Warning: This hash maps UNIQUE KEYS ==> ARBITRARY VALUES.
   Warning: Explicit function signature _Requires_Wrapper_.

   The hash table allows us to call a specific function in response
   to the users command string. The table requires a specific 
   function signtaure and you need to include soarkernel.h to 
   pick-up the typedefs. This function automatically handles 
   processing the command arguments to the sioo_API calls. Hence
   the requirement for a specific signature, however, it will 
   fall through gracefully if you proceed as follows:

   To avoid warnings and possible segvs you should wrap whatever custom
   function you wish to implement in this signature:
  
   int (*)(int,  const char **, struct soarResult *)

   or
   
   int cmd_YourFuncName(int i, const char** cmd, soarResult *res)

   (NOTE: see: executeCommand() in parsing.c for more information)

   Add your function (and any includes) to cmd_funcs.c 
   Add your prototype (and any declarations) to cmd_funcs.h 

   Finally, use this code block as a template to add your new (unique)
   command-string and the pointer to your function. You can also
   create hard aliases by using multiple command strings to map to 
   the same function pointer. e.g. "p", "print" and "quit", "bye", "exit"
   
   or even just to set up a hard alias for specific commands.

   // COMMENT? //
   add_to_hash_table( gSoarCommands, 
      new_soar_command( "CMD_STRING", FUNCTION_NAME ) );

   It should be obvious where this fits in below:
 */


void
init_soar_command_table( void ) {
  
  gSoarCommands = make_hash_table( 6, (hash_function)hash_soar_command );

/*
Conditionally compiled commands on top
*/  
#ifdef USE_CAPTURE_REPLAY
   add_to_hash_table( gSoarCommands,
      new_soar_command( "capture-input", soar_CaptureInput ) );
   add_to_hash_table( gSoarCommands,
      new_soar_command( "replay-input", soar_ReplayInput ) );
#endif

#ifdef USE_DEBUG_UTILS
   add_to_hash_table( gSoarCommands, 
      new_soar_command( "pool", soar_Pool ) );
#endif

/*
Deprecated DO NOT USE (BUT!!!  DO NOT DELETE  either)
*/
   add_to_hash_table( gSoarCommands, 
      new_soar_command( "soar8", soar_Operand2 ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "operand2", soar_Operand2 ) );


/*
Add your custom functions here
*/


/*
Demonstration command functions
*/
   add_to_hash_table( gSoarCommands, 
      new_soar_command( "counter-demo", interface_counter_demo ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "toh-demo", interface_toh_demo ) );

/* 
Terminal Utility functions 
*/
   add_to_hash_table( gSoarCommands, 
      new_soar_command( "print-banner", cmd_PrintBanner ) );

   /*  BUGBUG #20 reported segv on call but cant reproduce it */
   add_to_hash_table( gSoarCommands, 
      new_soar_command( "clear", linenoiseClearScreen ) );

/*
Soar file compatibility functions 
*/
   add_to_hash_table( gSoarCommands, 
      new_soar_command( "echo", interface_echo ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( ".", interface_Source ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "popd", interface_popd ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "pushd", interface_pushd ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "set", interface_Set ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "source", interface_Source ) );

 /*
soar_api() functions
*/
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

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "add-wme", soar_AddWme ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "build-info", soar_BuildInfo ) );

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
      new_soar_command( "pref", soar_Preferences ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "p", soar_Print ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "print", soar_Print ) );

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
      new_soar_command( "sp", soar_Sp ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "stats", soar_Stats ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "v", soar_Verbose ) );

   add_to_hash_table( gSoarCommands,
      new_soar_command( "verbose", soar_Verbose ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "w", soar_Watch ) );

   add_to_hash_table( gSoarCommands, 
      new_soar_command( "watch", soar_Watch ) );
   
}

/*
hash_soar_command_string()
TODO: I have no idea how this works, really.  
*/

int
hash_soar_command_string( char *name, short nbits )
{
  int val, length;
  
  /* No command string? return (error) */
  if ( name == NULL ) { return 0; }

  length = strlen( name );  
  val = length + 26 * (int)name[0];
  
  /* C magic, read carefully */
  switch (length)
  {
    default: val += (int)name[3];
     case 3: val += (int)name[2];
     case 2: val += (int)name[1];
     case 1: break;
  }

  return val & masks_for_n_low_order_bits[nbits];
}  


/* 
Return the Hash value for a soar_command structure
TODO: I have no idea how this works, really.  
*/
int
hash_soar_command ( void *item, short nbits )
{
  return hash_soar_command_string( ((soar_command *)item)->command_name, nbits );
}


/*
Create a new command structure, to be added into the global comman table
*/
soar_command*
new_soar_command( char *name, int (*cmd)(int, const char **, soarResult *) )
{

  soar_command *c;
  
  c = malloc( sizeof( soar_command ) );
  c->command_name = malloc( (strlen( name ) + 1) * sizeof(char) );
  strcpy( c->command_name, name );
  c->command = cmd;
  
  return c;
}

  
/*
Check for the existence of a command.  If the command does indeed
exist, return a structure containing its registry information
(including a pointer to the function itself)
*/
soar_command*
find_soar_command_structure( char *name )
{

  int commandHash;
  soar_command *theCommand;

  commandHash = hash_soar_command_string( name, gSoarCommands->log2size );
  theCommand = (soar_command *) (*(gSoarCommands->buckets + commandHash ));
  
  for ( ; theCommand != NIL; theCommand = theCommand->next ) {
    
    if ( !strcmp( name, theCommand->command_name ) ) {
      break; 
      
    }
  }
  return theCommand;
}

