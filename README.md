# sioo

SIOO is an acronym for State Input Operator Output

SIOO is an embeddable cognitive architecture based on Soar 8.5.2
and includes that kernel and a handful of other files from the 
8.5.2 version of SoarSuite.

This code uses the GNU Compiler Collection (GCC)

All Tcl code has been removed from the source tree. Sorry.
All WINDOWS and WIN32 code has been removed. Sorry.
ALL MACINTOSH code has been removed. Sorry.
All HP-UX code has been removed. Sorry.

This is alpha code and is just now coming together.

This code is NOT for beginners at all and is intended for
experts with the C language, *nix tools, and production rules
there is no support here other than the source code. You will
need to use gdb or ddd to do debugging...

to build it, change to the root of wherever you put the code
and try

./make-sioo

This is a simple sh script that manages the process, it will 
build the kernel, headers and the cli interface and
dump it all into the out directory. There is no install yet.
Note that at this time the kernel is built with an autotools
./configure && make process but the cli is built with a 
hand-rolled make file.

note that the demo codes counter-demo and toh-demo must be 
run from the cli directory by calling sioo while you are
in that directory --> but they do work
