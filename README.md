# sioo

## SIOO is an acronym for State Input Operator Output

SIOO is an embeddable cognitive architecture based on Soar 8.5.2
and includes that kernel and a handful of other files from the 
8.5.2 version of SoarSuite.


***

#### This code requires the GNU Compiler Collection (GCC)
#### This code will probably only run on GNU/Linux.
#### All Tcl code has been removed from the source tree. Sorry.
#### All WINDOWS and WIN32 code has been removed. Sorry.
#### ALL MACINTOSH code has been removed. Sorry.
#### All HP-UX code has been removed. Sorry.

***

**This is alpha code and is just now coming together.**

This code is NOT for beginners (at all) and is intended for experts with the C language, (gcc) *nix tools, production systems, the PSCM, FSMs, and Knowledge/Domain Modeling. There is no support other than the source code. Debugging support is primitive, oh, and there is no test suite...

**If you have not yet been scared off you should be able to to build it by changing into the root of the source tree, then try:**

`./make-sioo`

You may need to 

`chmod u+x make-sioo`

This is a simple sh script that manages the process, it will build the kernel, headers and the cli interface and dump it all into the out directory. There is no install yet. Note that at this time the kernel is built with an autotools

`./configure && make` 

process but the CLI is built with a hand-rolled make file.

**Suggested Reading** includes _Unified Theories of Cognition_ by Allen Newell (Harvard Press), and _The Soar Cognitive Architecture_ by John Laird (MIT Press). 

**Production Systems** Wikipedia has a decent article on "_Production Systems (Computer Science)_" if you need a starting point on what this type of software system is used for and how.

**Robot Systems** If you are building a Robot, google "_Jess_" and also try "_Open Cognition_". SiOO is really a research project, not a ready to use AGI system. But it does work ;-)

**Using the CLI Demos**

Note that you need to change to the build directory of the CLI system, and then run `sioo` from that directory or the system won't find the demos. The demos do work, and this hassle is a listed bug. **NOTE** the demos are named `counter-demo` and `toh-demo` 
