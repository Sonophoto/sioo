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

