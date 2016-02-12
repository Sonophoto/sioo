#define _GNU_SOURCE

#include "soarapi.h"
#include "soarkernel.h"
#include "soar_core_api.h"
#include "parsing.h"
#include "commands.h"
#include "callbacks.h"
#include "utilifuncs.h"
#include "linenoise/linenoise.h"
#include "linenoise/utf8.h"

#include <stdio.h>

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


/* Linenoise command completion callbacks */
/*
*  This completion system needs fixed or replaced. If you have a habit
*  of tabbing for filename completion, you will be very disappointed
*  when it erases your command string...
*/
void completion_CB(const char *buf, linenoiseCompletions *lc) {
    if (buf[0] == 'a')
      {
        linenoiseAddCompletion(lc, "add-wme");

      } else if (buf[0] == 'b') {
        linenoiseAddCompletion(lc, "build-info");
        linenoiseAddCompletion(lc, "bye");

      } else if (buf[0] == 'c') {
        linenoiseAddCompletion(lc, "counter-demo");
        linenoiseAddCompletion(lc, "capture");

      } else if (buf[0] == 'e') {
    if (buf[1] == 'c')
       {
       linenoiseAddCompletion(lc, "echo");
       }
    if (buf[1] == 'x') 
       {
       linenoiseAddCompletion(lc, "excise -all");
       linenoiseAddCompletion(lc, "excise");
       linenoiseAddCompletion(lc, "exit");
       }

      } else if (buf[0] == 'l') {
    if (buf[1] == 'e')
       { 
       linenoiseAddCompletion(lc, "learn");
       }
    if (buf[1] == 'o')
       { 
       linenoiseAddCompletion(lc, "log");
       }

      } else if (buf[0] == 'i') {
        linenoiseAddCompletion(lc, "init-soar");
        linenoiseAddCompletion(lc, "indifferent-selection");

      } else if (buf[0] == 'p') {
    if (buf[1] == 'o')
       { 
       linenoiseAddCompletion(lc, "popd");
       }
    if (buf[1] == 'r')
       { 
       linenoiseAddCompletion(lc, "print");
       linenoiseAddCompletion(lc, "pref");
       }
    if (buf[1] =='u')
       {
       linenoiseAddCompletion(lc, "pushd");
       }
    if (buf[1] == 'f')
       {
       linenoiseAddCompletion(lc, "pfind");
       }

      } else if (buf[0] == 'q') {
        linenoiseAddCompletion(lc, "quit");

      } else if (buf[0] == 'r') {
    if (buf[1] == 'e')
       { 
       linenoiseAddCompletion(lc, "rete-net"); 
       linenoiseAddCompletion(lc, "remove-wme");
       linenoiseAddCompletion(lc, "replay");
       }
    if (buf[1] == 'u')
       {
       linenoiseAddCompletion(lc, "run 1");
       linenoiseAddCompletion(lc, "run 10");
       linenoiseAddCompletion(lc, "run 100");
       }
        
      } else if (buf[0] == 's') {
        linenoiseAddCompletion(lc, "source");
        linenoiseAddCompletion(lc, "stats");
        linenoiseAddCompletion(lc, "set");
        linenoiseAddCompletion(lc, "sp");


      } else if (buf[0] == 't') {
        linenoiseAddCompletion(lc, "toh-demo");

      } else if (buf[0] == 'v') {
      linenoiseAddCompletion(lc, "verbose");

      } else if (buf[0] == 'w') {
      linenoiseAddCompletion(lc, "watch");
/*
      } else if (buf[0] == '') {
      linenoiseAddCompletion(lc, "");
*/
      } /* END if (buf[0]) */
} /* END completion() */


