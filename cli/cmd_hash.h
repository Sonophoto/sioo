/* **************************************************************** *
         _____
    GNU / ___/
       / /__  
       \___/  
  
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
Declarations
 */
hash_table *gSoarCommands;
list *gDirectoryStack = NULL;














/* ENDIF #include-lock
#endif 

