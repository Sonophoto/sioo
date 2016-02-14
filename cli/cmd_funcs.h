/* **************************************************************** *
             _____     ____  _  ___   ___
   GNU/     / ___/    / ___|(_)/ _ \ / _ \
     /ANSI / /__      \___ \| | | | | | | |
           \___/       ___) | | |_| | |_| |
                      |____/|_|\___/ \___/
 
   FILENAME: cmd_funcs.h AUTHOR: BrigYoung 
   COPYRIGHT: "2015, 2016 Brig Young" LICENSE: "GPLv3, see LICENSE"         
   PURPOSE: "Built-in cmd_functions and sioo_API CLI functions"               
   DERIVED: "From code by [original CLI author]'
 * **************************************************************** */
#ifndef SIOO_CMD_FUNCS_H
#define SIOO_CMD_FUNCS_H
/* We are using ANSI C with GNU extensions */
#define _GNU_SOURCE

#include "soarapi.h"
#include "soarkernel.h"

/*
DECLARATIONS
 */

/*
This is our command struct setup as a type for convenience 
*/
typedef struct soar_command_struct 
{
 struct soar_command_struct *next; /* Warning this MUST be the first field */
                                   /* for the ht routines                  */
 char *command_name;
 int (*command)( int, const char **, soarResult * );
} soar_command;

/*
Low order bit mask
*/
extern unsigned long masks_for_n_low_order_bits[];


/*
PROTOTYPES
*/
/* TODO: CRUFTY
int
cmd_PrintBanner( int argc, const char **argv, soarResult *res );
*/

void
cmd_PrintBanner(void);

int
interface_Set( int argc, const char **argv, soarResult *res );

int
interface_pushd( int argc, const char **argv, soarResult *res );

int
interface_popd ( int argc, const char **argv, soarResult *res );

int
interface_Source( int argc, const char **argv, soarResult *res );

int
interface_echo( int argc, const char **argv, soarResult *res );

int
interface_counter_demo( int argc, const char **argv, soarResult *res );

int
interface_toh_demo( int argc, const char **argv, soarResult *res );

/* ENDIF #include-lock */
#endif 

