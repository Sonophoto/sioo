/* **************************************************************** *
             _____     ____  _  ___   ___
   GNU/     / ___/    / ___|(_)/ _ \ / _ \
     /ANSI / /__      \___ \| | | | | | | |
           \___/       ___) | | |_| | |_| |
                      |____/|_|\___/ \___/
 
   FILENAME: parsing.h AUTHOR: "Brig Young" 
   COPYRIGHT: "2015, 2016 Brig Young" LICENSE: "GPLv3, see LICENSE"         
   PURPOSE: "Miscellaneous functions and definitions"               
 * **************************************************************** */
#define _GNU_SOURCE




#include "soarkernel.h"
#include "soarapi.h"


#define MAX_TOKENS 25
#define LINE_BUFFER_LENGTH 512

int tokenizeString( char *input, int *ntokens, char ***tokens, 
		    char *token_terminator, soarResult *res );


growable_string getCommandFromFile( int (*readCharacter)(FILE *), FILE *f, 
			  bool *eof_reached );

int executeCommand( char *command );

