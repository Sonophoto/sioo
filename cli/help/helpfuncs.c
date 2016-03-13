/* these are pulled in to get the kernel's plane jane print() function. */
#include "soarapi.h"
#include "soarkernel.h"


/* Each file below contains one function which prints the CLI context help */
/* for that command. These files are packed into helpfuncs.c and linked in */
/* with the command  processor code for sioo CLI. */
#include "d.c"
#include "dirs.c"
#include "e.c"
#include "excise.c"
#include "help.c"
#include "init-soar.c"
#include "io.c"
#include "learn.c"
#include "log.c"
#include "matches.c"
#include "memories.c"
#include "monitor.c"
#include "popd.c"
#include "preferences.c"
#include "print.c"
#include "pwatch.c"
#include "quit.c"
#include "rete-net.c"
#include "run.c"
#include "sioo-news.c"
#include "sp.c"
#include "topd.c"
#include "version.c"
#include "watch.c"


/* This test should print out a notice of which function it is */
/* printing */
/* followed by the body of the help message */
/* followed by a message of pass: or FAIL! based on return status */
/* interestingly, these should always print "pass:" */

int testhelpfuncs()
{
  int fail = 0;
  int argc = 2;
  char** argv;
  soarResult* res;

  print("Running cmd_help_d(): ");
  fail = cmd_help_d(argc, argv, res);
  (fail) ? print("FAIL!\n") : print("pass:\n");

  print("Running cmd_help_dirs(): ");
  fail = cmd_help_dirs(argc, argv, res);
  (fail) ? print("FAIL!\n") : print("pass:\n");

  print("Running cmd_help_e(): ");
  fail = cmd_help_e(argc, argv, res);
  (fail) ? print("FAIL!\n") : print("pass:\n");

  print("Running cmd_help_excise(): ");
  fail = cmd_help_excise(argc, argv, res);
  (fail) ? print("FAIL!\n") : print("pass:\n");

  print("Running cmd_help_help(): ");
  fail = cmd_help_help(argc, argv, res);
  (fail) ? print("FAIL!\n") : print("pass:\n");

  print("Running cmd_help_initsoar(): ");
  fail = cmd_help_initsoar(argc, argv, res);
  (fail) ? print("FAIL!\n") : print("pass:\n");

  print("Running cmd_help_io(): ");
  fail = cmd_help_io(argc, argv, res);
  (fail) ? print("FAIL!\n") : print("pass:\n");

  print("Running cmd_help_learn(): ");
  fail = cmd_help_learn(argc, argv, res);
  (fail) ? print("FAIL!\n") : print("pass:\n");

  print("Running cmd_help_log(): ");
  fail = cmd_help_log(argc, argv, res);
  (fail) ? print("FAIL!\n") : print("pass:\n");

  print("Running cmd_help_matches(): ");
  fail = cmd_help_matches(argc, argv, res);
  (fail) ? print("FAIL!\n") : print("pass:\n");

  print("Running cmd_help_memories(): ");
  fail = cmd_help_memories(argc, argv, res);
  (fail) ? print("FAIL!\n") : print("pass:\n");

  print("Running cmd_help_monitor(): ");
  fail = cmd_help_monitor(argc, argv, res);
  (fail) ? print("FAIL!\n") : print("pass:\n");

  print("Running cmd_help_popd(): ");
  fail = cmd_help_popd(argc, argv, res);
  (fail) ? print("FAIL!\n") : print("pass:\n");

  print("Running cmd_help_preferences(): ");
  fail = cmd_help_preferences(argc, argv, res);
  (fail) ? print("FAIL!\n") : print("pass:\n");

  print("Running cmd_help_print(): ");
  fail = cmd_help_print(argc, argv, res);
  (fail) ? print("FAIL!\n") : print("pass:\n");

  print("Running cmd_help_pwatch(): ");
  fail = cmd_help_pwatch(argc, argv, res);
  (fail) ? print("FAIL!\n") : print("pass:\n");

  print("Running cmd_help_quit(): ");
  fail = cmd_help_quit(argc, argv, res);
  (fail) ? print("FAIL!\n") : print("pass:\n");

  print("Running cmd_help_retenet(): ");
  fail = cmd_help_retenet(argc, argv, res);
  (fail) ? print("FAIL!\n") : print("pass:\n");

  print("Running cmd_help_run(): ");
  fail = cmd_help_run(argc, argv, res);
  (fail) ? print("FAIL!\n") : print("pass:\n");

  print("Running cmd_help_sioonews(): ");
  fail = cmd_help_sioonews(argc, argv, res);
  (fail) ? print("FAIL!\n") : print("pass:\n");

  print("Running cmd_help_sp(): ");
  fail = cmd_help_sp(argc, argv, res);
  (fail) ? print("FAIL!\n") : print("pass:\n");

  print("Running cmd_help_topd(): ");
  fail = cmd_help_topd(argc, argv, res);
  (fail) ? print("FAIL!\n") : print("pass:\n");

  print("Running cmd_help_version(): ");
  fail = cmd_help_version(argc, argv, res);
  (fail) ? print("FAIL!\n") : print("pass:\n");

  print("Running cmd_help_watch(): ");
  fail = cmd_help_watch(argc, argv, res);
  (fail) ? print("FAIL!\n") : print("pass:\n");

}


