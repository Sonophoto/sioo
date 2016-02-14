/* **************************************************************** *
             _____     ____  _  ___   ___
   GNU/     / ___/    / ___|(_)/ _ \ / _ \
     /ANSI / /__      \___ \| | | | | | | |
           \___/       ___) | | |_| | |_| |
                      |____/|_|\___/ \___/
 
   FILENAME: cmd_hash.h AUTHOR: BrigYoung 
   COPYRIGHT: "2015, 2016 Brig Young" LICENSE: "GPLv3, see LICENSE"         
   PURPOSE: "Hash function maps command string to function pointer"               
   DERIVED: "From code by [original CLI author]'
 * **************************************************************** */
#ifndef SIOO_CMD_HASH_H
#define SIOO_CMD_HASH_H
/* We are using ANSI C with GNU extensions */
#define _GNU_SOURCE

#include "soarkernel.h"
#include "soarapi.h"

/*
DECLARATIONS
*/
hash_table *gSoarCommands;
list *gDirectoryStack = NULL;



/*
PROTOTYPES
*/


/*
This sets up the hash table for command names
*/
void
init_soar_command_table( void );

/*
hash_soar_command_string()
TODO: I have no idea how this works, really.  
*/
int
hash_soar_command_string( char *name, short nbits );

/* 
Return the Hash value for a soar_command structure
TODO: I have no idea how this works, really.  
*/
int
hash_soar_command ( void *item, short nbits );

/*
Create a new command structure, to be added into the global comman table
*/
soar_command*
new_soar_command( char *name, int (*cmd)(int, const char **, soarResult *) );

/*
Check for the existence of a command.  If the command does indeed
exist, return a structure containing its registry information
(including a pointer to the function itself)
*/
soar_command*
find_soar_command_structure( char *name )


/* ENDIF #include-lock */
#endif 

