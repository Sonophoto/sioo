*******************************************************************************

      SiOO: State-input Operator Output   9 . 9 . 1  C H A N G E S 

      Summary of changes between SiOO 9.9.0 and SiOO 9.9.1
      January - February, 2016 
*******************************************************************************
SiOO 9.9.1 is all about getting the system more usable for actually developing
agents and testing them. A solid foundation for this type of application is a
solid command line interface. We have also made a number of changes to the 
build system to enhance its flexibility why bringing all of the options into 
a single file for controlling builds: sioo/make-SiOO

Currently SiOO 9.9.1 can run more than 5 million Decision Cycles << 1GB

   (There is much work remaining to be done updating the kernel...)

Modified the kernel build options to reflect the need for speed and smale size

Reduced a massive memory leak in the kernel by defining NO_TOP_LEVEL_REFS
in the kernelBuildOptions.h

Completely revamped the command line interface by
```
   Adding a history system
   Adding a command completion system (clunky but finished)
   Adding a help screen for supported commands
   Fixing demos to make them easier to use
```

removed cli/ask.c and cli/ask.h and put it in cli/main.c
rearranged demo code and created a complete disaster
re-rearranged demo code and it works again with only one ugly kludge

*******************************************************************************

      SiOO: State-input Operator Output   9 . 9 . 0  C H A N G E S 

      Summary of changes between Soar 8.5.2 and SiOO 9.9.0 
      November-December, 2015    (...11 years later...)
*******************************************************************************

Soar 8.5.2 is being used as the basis for a new system called SiOO

                         See'-OO

State-input Operator Output reflects a minor change in perspective on the 
overall system design which treats state and input as the same thing yet
with input subordinate to system State. This reflects the continuing 
seperation of operator assertion and operator application and the greatly
differing time scales for feedback of Operator to Output to Input. It also
reflects using OOP techniques in C.  

           but mostly we needed a cool new name. ;-)

All of these changes are in progress with the 9.9.0 version and therefore 
this version should be considered unstable in every sense of the word.

What we need is an extensible production processor, Soar 8.5.2 is it.

Soar 8.5.2 is being used because it is a well known implementation of a 
production matcher, it is written in ANSI C, it is an older source tree,
it uses an established syntax, and it maps more or less directly to the 
work of Newell et. al.

Soar 9.5.0 is not being used because we don't need 98% of what it does
and the kernel has been mixed with its IO stack and interface. It is 
also very much an active research project (unstable). It is written in 
C++, thats OK but C is better for our needs.  

This system is designed to be used on GNU/Linux and build using the GCC
toolset. It is not portable to OSX or Windows because that introduces
bugs we don't have time to deal with and we don't use either OS anyway...


To Date: accomplished, in progress, and TODO:

6) DONE: System builds mostly clean but is in transition

1) DONE: Entire system is being converted to use gcc cross-compiler suite

4) DONE: Tcl is being removed. Sorry :-(

2) DOING: All references in source tree to WINDOWS and WIN32 are being 
   removed. This includes conditional code and headers. Sorry :-(
   
3) DOING: All references to MACINTOSH are being removed. Sorry :-(

7) DOING: ALL refernces to HPUX and that madness (??) Going, no apology.

5) DOING: CLI is being rebuilt to use standard *nix style command shell

8) TODO: Reorder numbers in todo list.

And PLEASE NOTE: This is not a better/faster version of Soar, it is a 
stripped down, gutted, broken almost useless version of Soar and you
need to be an expert with *nix and C/C++ to even do anything with it
at all. This system has no unit tests, no user base, there is no record
of active bugs and it is 11 years old... and oh yeah.. Its really just 
a library, not an application...

If you want Soar, please get the latest version:

https://github.com/SoarGroup

It is a great system, has an active mailing list and it is 11 years newer...

Brig Young, 28 November 2015

================================================================================  
                
      S O A R   8 . 5 . 2   C H A N G E S 

=================================================================================

         Summary of changes between SoarSuite 8.5.1 and SoarSuite 8.5.2
                            July, 2004


A summary of changes in SoarSuite 8.5.2 is listed immediately below. For more
details, including changes not mentioned in the summary and a list of known
issues see the Details section after the summary.

======================
New Windows Installer:
======================

	The Windows installer is now built with a different technology which
is much easier for us and results in a much smaller package.

=========================
Several kernel bug fixes:
=========================

	Almost all known bugs fixed, including all known crash bugs. See the
details section below for more information.

==================
New TankSoar Bots:
==================

	TankSoar now comes with two new bots.  Mapping-bot demonstrates how a
tank might build up an internal map of the level.  Obscure-bot is a challenge bot
for people to test their own bots against.  The code is provided in an obfuscated
binary.  See obscure-bot.txt in the TankSoar agents directory for details.

==========================
SoarDoc and DMGen updated:
==========================

	These changes were actually in the the 8.5.1 release, but we failed
to note them.
SoarDoc:
 - Fixed special character handling when UseExistingComments is used.
 - Added ShowProdSourceInline which shows production source code
   inline in the production documentation
 - Ability disable datamap node coloring
 - Fixed case-sensitivity issues for production names and list sorting.
DMGen:
 - Fixed handling of mixed-case production names.

======================
Documentation Updated:
======================

	The tutorials have had some typo fixes and screenshot updates, and also
explain substates better.

==================
Tcl Soar Debugger:
==================

	The TSDebugger has several bug fixes which may have affected some
users.

===================
VisualSoar Updated:
===================

	VisualSoar has several small bug fixes.

======================
Critical SGIO bug fix:
======================

	Our fix for bug 254 in the last release actually caused serious
problems. This has been fixed for real now.

====================================================================
                            Details
====================================================================

Below is a fairly complete list of enhancements, changes, bug fixes, and known
issues, with bug numbers for reference. For more information and up to date list
of what's been fixed please visit the Soar bugzilla server at:

    https://winter.eecs.umich.edu/soar-bugzilla/.

New bug submissions and enhancement requests are welcome!


Soar-8.5.2
==========

Bug Fixes
---------
353  stats command keeps recording between runs
389  missing error handling for set_watch_prod_group_setting

Enhancements
------------
358  change soar8 command error message

Known Issues
------------
312  agent structure isn't cleaned up properly
356  All matches get same support as last processed match

TSDebugger 4.0.1
================

Bug Fixes
---------
365  rules that contain a capital letter break productions tab
376  stop button doesn't work with Eaters, TankSoar
396  error when closing group from task bar
398  extra 0 gets printed when soar connected to environments

Known Issues
------------
336  negated conditions don't print properly in the console window
397  grouping bar in production tab not always right

SGIO 1.1.2
==========

Bug Fixes
---------
254  Some special characters in StringWME's do not transfer to Soar

Eaters 3.0.8
============

Bug Fixes
---------
391  garbage printed to debugger output when Stop After Decision Phase is on

TankSoar 3.0.8
==============

Bug Fixes
---------
map loading menu now defaults to maps directory
188  Selecting Map->Random Map causes error message when agent present on
	current map
392  garbage printed to debugger output when Stop After Decision Phase is on
393  command 'run 1' generates 'invalid command step' error

Enhancements
------------
new challenge bot (obscure-bot) and mapping bot provided

Known Issues
------------  
399  can't use reset tank command in tank control panel tanks menu

Documentation
=============

Bug Fixes
---------
349  tutorial images need to be updated
375  typo in tutorial part 1: move-north  
386  typos in tutorial part 1 (search control)  

Enhancements
------------
Added information to tutorial about substates.

Known Issues
------------
380  typo in section 5.6.3
381  o-support-mode command missing newest support modes 

VisualSoar 4.5.3
================

Bug Fixes
---------
361  adding a dir that doesn't exist to agent path on file->new project fails with no warning
362  initial rules in a project are not justified correctly
363  if project loaded then create new project, title bar does not change
364  file with same name as datamap have same window names
366  Initialize-<task> Operator not in datamap
368  dragging node onto itself makes node disappear
372  fails to detect extra whitespace after :

Known Issues
------------
Too many to list. See bugzilla for details.


*******************************************************************************
SOAR 6.0 to Soar 7.0
*******************************************************************************
Version 6.0.0:
  -- 2 coredumps
  -- infinite loop in excise-chunks.
  -- provided tilde expansion in file names.
  -- Fixed cntrl C on ibm rs/6000 causing unwanted exits.
  -- WME supported by a justification that got removed was hanging around.
     No longer.
  -- Fix for a type of chunk being built that couldn't be reordered.
  -- Fix to atof to allow it to compile on a NeXT.
  -- Added warnings for things that look like they might be mistyped variables
     or mistyped disjunctions while parsing.

Version 6.0.1:
  -- added a 'version' command to give full version information from the
     interface level.
  -- rearranged the makefile slightly to prevent it recompiling executables
     whenever no object files had changed.
  -- removed the beta test notice from the startup routine.  Why this routine
     duplicates the text in 'soarnews' I'll never understand.

Version 6.0.2:
  -- Made some simple changes to make the code compile on HP Apollos.  I don't
     know if these will work on all HP machines. Time will tell.
  -- Made a few small fixes to lexer.c and to convert.c so that the program
     would compile under straight ultrix cc as well as gcc.
  -- Added a bugfix provided by bob that fixed a problem with a rete
     optimization.  This fixes two outstanding core dumps.

Version 6.0.3:
  -- Made some changes to "recmem.c" to overcome problems with chunk-free
     problem spaces.  This was in reference to bug report #400.

Version 6.0.4:
  -- Missed a use of alloca last time around.  Removed it.
  -- In interface.c a stray use of "printf" was replaced with "print"
     so that output would go to soar managed streams rather than
     standard output.
  -- HP fixes for ISI:
    -- Commented out "struct timeval;" and added include of sys/time.h
       in soar.h
    -- Added "const" decl to predicate function "compare_firing_counts"
       formal parameters in file interface.c to match ANSI spec.
    -- Added "rm = NIL" in function "remove_wme_from_alpha_mem" in
       rete.c to placate the native HP compiler which thinks rm needs
       to be initialized.
    -- Added a cast to (int *) in the call to select in file io.c
    -- In tilde.c added a switch before the include of pwd.h to
       correct a struct decl problem.
  -- Changed makefile so that objects and binaries for multiple machines
     can be maintained in the soar directory to avoid duplicating sources.
     The is achieved by selecting the appropriate make.header.* file
     in the makefile.  Also changed makefile for multi-agent processing.
  -- Added multi-agent capability.  A number of changes were made:
    -- Moved globals to a header file (global_vars.h contains the extern 
       decls and global_vars.c the memory-allocating decls).  This also
       forced the move of associated typedefs and constants -- so the
       tail end of soar.h looks rather messy.  Soar.h really needs to
       be broken into smaller pieces!  If DFLAGS is empty (the default 
       setting in the makefile).  Then the global vars are decl'ed as in 
       earlier versions (before 6.0.4).  If MULTI_AGENT_ENABLED is set
       then the globals are placed into a record (see the end of soar.h)
       and the record is allocated once per agent.  Note that multi-agent
       processing is not fully activated unless a .init.soar.multi file
       is present when soar is run.
    -- To select the proper agent as well as accomodating backward
       compatibility, the macro "current_agent" is introduced to define
       the access path to the proper agent's global data.  This change
       alone affected ~1900 lines of source.
    -- Added commands "create-agents", "select-agent", "agent-go",
       and "schedule" for multi-agent processing.
    -- Added a new file "scheduler.c" which handles the scheduling of
       agents in multi-agent mode.
    -- Added a file "x_interface.c" which defines the X interface to
       multi-agent soar.  The implementation uses only Xlib calls so
       no dependence on a widget set (e.g., Motif) is present.
    -- Added a new file "queue.c" and its header "queue.h".  This is 
       a generic queue abstract data type.  It is used in the
       X display processing of text-io.  Since text-io is occuring in
       separate windows (instead of standard input as in single-agent
       mode) we must queue up the text-io strings until they can be
       processed by an agent.
    -- Stripped all code out of main.c and moved it to init_soar.c
       EXCEPT for the top-level function "main".  This was done to
       facilitate integration with systems which want to control
       Soar (rather than having soar control external systems).  By
       linking with the other object files and redefining main.c
       this is easily accomplished with the stripped down version of
       main.c.
    -- Added a directory under tests: multi/multi.  This directory
       uses the same format as the other test directories but is pushed
       down one level so that it is not processed when "run-all-tests"
       is invoked.  This is because it only works if MULTI_AGENT_ENABLED
       is defined and USE_X_DISPLAY is NOT.  This causes all multi-agent
       I/O to occur in standard I/O.  To run the test, compile soar
       with the above switch setting for DFLAGS in the make.body file.
       Then cd to tests/multi/multi and execute "run-multi-test".  
       This directory also serves as an example of how to setup multi-
       agent processing.

Version 6.0.5:
  -- Added an "obj" directory to hold object files.
  -- Added machine-dependent directories sun4, IRIX, hp700 to the 
     bin and obj directories.

Version 6.0.6:
  -- Fixed bug in makefile that missed where convert.o should be 
     located.
  -- Corrected another bug in makefile that handles machine-dependent
     processing of convert.

Version 6.0.7:
  -- Added missing directories lib/{sun4,IRIX,hp700,pmax_mach}
  -- Fixed a text-io bug in x_interface.c which caused commands
     to be ignored after issuing an explicit go/run command.
  -- Added Gary's better duplicate chunk detection algorithm.
  -- Fixed a bug generated by the combination of USE_X_DISPLAY
     and running soar without a .init.soar.multi file.  The
     command processor would not read commands from the window
     in this state.
  -- Fixed a bug generated by use of MULTI_AGENT_ENABLED.  When
     running with only the global agent, control-c was not
     stopping the soar process.
  -- Another HP fix.  Added more switches to the including of the
     <sys/types.h> file.  Without these, nothing gets included.
     Fixes use of X windows on HP.
  -- Moved -lX11 in library loading sequence to the end to allow
     redirecting of search for X11 library.  This is also proper
     because X11 doesn't depend on other libs.  See make.body.
  -- Added comment to make.header.hp to show how include and lib
     files can be found on HPs.  This is needed for the X11
     stuff to be compiled correctly.
  -- Made a fix so that make_blank_growable_string no longer
     assumes that sizeof(int) == sizeof(int *)

Version 6.0.8:
  -- Removed the redundancy between the two files global_vars.c
     and global_vars.h by using the macro GLOBAL.  In global_vars.h
     the GLOBAL is set to "extern" so that the global vars are
     defined as external in header files.  In global_vars.c the
     GLOBAL macro is set to the empty string so the variables can
     be declared as fields in a record or as global variables with
     memory allocation.
  -- Reordered the include of stddef.h in soar.h to after the include
     for stdio.h.  The inclusion order was breaking on some Suns.
     The doc in GNU's stddef.h clearly states that this should be
     after any includes of sys/types.h for Suns.  But there was also
     problems with stdio.h.
  -- When users entered an unterminated string in a command, Soar
     was hanging in the X interface.  Culprit routine was
     "command_is_complete" which didn't check for  matching double
     quotes.
  -- Improved use of queues by replacing queue_init with queue_create
     which includes the malloc step.
  -- Added "help all" in the interface which is equivalent to 
     list-help-topics per bug enhancement #428.
  -- Double checked convert.c code against submitted bug report containing
     compilation changes necessary for compiling convert.c under THINK_C
     (bug #431)
  -- Added 2 hqx files in the directory mac_support.  These contain project
     files and source code needed to compile Soar under ThinkC on a Mac.
  -- Changed soar from using rand to using random thus giving better selection
     for user select.
  -- Fixed memory leak that was caused by impasse id's not being GC'd
     correctly.
  -- Renamed symbol construct to Symbol to allow for Mac compilation.

Version 6.1.0:
  -- Went to version 6.1.
  -- Added new command multi-attribute.  This command is used similar to the
     way multi-attributes was in soar5.  However, you cannot specify a list
     of attribute value pairs.  Only one multi-attribute may be given with
     each multi-attributes line.  Syntax of the command is 
     (multi-attribute symbol value).  This removes the slowdown that Gary's
     reordering change introduced.
  -- Added RCS headers to all source code files.
  -- Split some of the larger source code files into more manageable peices.
     Decide.c -> decide.c, prefmem.c, wmem.c, tempmem.c
     Production.c -> production.c, reorder.c, osupport.c
     Recmem.c -> recmem.c, osupport.c, chunk.c, backtrace.c
  -- Added the examples directory of example tasks as per Aladin's request.
  -- Reorganized the task suite tests to use the tasks from the examples dir.
  -- init-soar will now reset the arguments to the 'go' command.
  -- removeing a stack-trace-format will now no longer cause a blank line
     to be printed to the display.
  -- Soar now maintains seperate working directory pointers for each of the
     agents.
  -- select-agent can now be used to return control to the global agent in
     the non-X version of the multi-agent code.
  -- A bug in agent-go has been fixed so that agent-go commands take effect
     correctly regardless of which agent they were listed in.

Version 6.1.1:
  -- Default Rules added and modified for operator subgoaling.
  -- X-Window and Multiple Agent code changes for additional compatability
     between Soar and IFOR/ModSAF.
  -- User Interface Enhancements.
  -- New RHS Functions.
  -- Simple makefile enhancements - use of -ansi switch to allow for ansii
     check.
  -- Addition of New "doc" and "pc_support" directories.
 
Version 6.2.1:
  -- removed pc_support, mac_support, convert, taql_conversion directories
  -- added NNPSCM as a compile-time option
     - made nnpscm and non-nnpscm directories under bin, lib, obj
     - made NNPSCM versions of default rules, example pgms, and tests
  -- added pscm_convert directory - an SDE (emacs) program to convert
     between old PSCM and NNPSCM
  -- added conditional command-line options: setvar, unsetvar, if, if-not
  -- added commands: alias, unalias, pushd, popd, dirs, memories
  -- added general shell escape (UNIX only)
  -- fixed commands: select-agent, destroy-agent, preferences, explain, cd
  -- fixed load-errors facility
  -- fixed erroneous printing out of prompts
  -- fixed pathname string length overflow problem
  -- modified pathnames so they can be entered w/o quotation marks
  -- modified the print and p commands
  -- added RHS functions: ifeq, strlen, user-select
  -- modified Soar to allow backtracing through prohibit preferences
  -- added 2 new example problems, safe-stack and tower-of-hanoi

Version 6.2.2:
  -- updated "conversion.el" file in pscm-convert directory.
  -- added "convert-prods.soar" to pscm-convert directory.
  -- modified "memories" command.
  -- fixed uninitialized variable "max_chunks_reached".
  -- fixed uninitialized queue element.
  -- fix for NNPSCM in reorderer, trace-format code, and o-support 
     calculations.
  -- fix for "print" command - previous functionality.
  -- fixed "interrupt" RHS function.
  -- modified make.body to correctly process "make clobber" command.
  -- modified "dispatch_command" to correctly compile under SGI IRIX.
  -- added new "pf" (production finding) user command.

  See "User.Notes" and "Release.Notes" for more detailed information.

Version 6.2.3
  -- fix for NNPSCM Default Rules.
  -- fix for NNPSCM trace-format.
  -- removal and addition of several gold-standard tests and examples for 
     NNPSCM.

Version 6.2.5 (and 6.2.4 beta versions)
  -- fix for NNPSCM and non-NNPSCM Default Rules.
  -- fix argument list for control_c_handler.
  -- de-linting Soar.
  -- apply patches for MS-Windows Soar.
  -- apply patches for MacSoar.
  -- made the "print" command list attributes alphabetically.
  -- fixes for o-support on nnpscm.
  -- modified and added several example problems for nnpscm and non-nnpscm.
  -- cleaned up a garbage pointer in decide.c which was causing core dumps.
  -- fixed nnpscm o-support problem for operator preferences.
  -- cleaned up output concerning "Firing", "Retracting", and "Building".
  -- added print-alias command.
  -- made it so that chunks get printed out in ascending order.
  -- added "last" and 'stop asking' options to user-select command.
  -- modified destroy_soar_agent() to handle destroying current agent.
  -- added "and" feature to "if" and "if-not" commands.
  -- modified "memories" command to make it more useful.
  -- fixed core dump problem in load-errors facility.
  -- added input-period command.
  -- modified "pf" command to make it more useful.
  -- added chunky-problem-spaces command.
  -- made the numbering of production types consistent.
  -- Soar will now compile with gcc -ansi on pmax_mach.
  -- added file of aliases to the default directory.
  -- each agent now has its own default print depth.
  -- fixed problem where destroy_soar_agent tries to delete an empty queue.
  -- added default limits of 20 to memories and firing-counts commands.
  -- properly initialized cond->bt.prohibits
  -- moved the DEBUG_INSTANTIATIONS compiler flag from rete.c to recmem.c

Version 6.2.6
  -- New version indicating start of TclSoar integration.
  -- subsequent interim versions will be identified by greek letters.
  -- Note that the ChangeLog entries below for all greek versions through
     7.0.0.beta are reconstructions since the merge process was not developed
     using the CMU version tracking.  Hence, there are likely to be gaps
     in the following record.

Version 6.2.6.alpha
  -- Added Tcl, Tk, and BLT to distribution.  This is primarily for folks
     who don't already have Tcl and Tk.  BLT is used to support one of the
     demo programs, but is not essential.  The version of Tcl included
     is a modified version of Tcl 7.3.  The modifications are to support
     various Soar requirements.  Likewise, the distribution also includes
     a modified version of Tk 3.6.  BLT 1.7 included a small script
     modification for the graphing tool demonstration.
  -- Added a "make-soar" Bourne shell script which builds Tcl, Tk, Soar,
     and BLT for the user as an added convenience.
  -- Restructured directories by moving original directory down a level.
  -- Added new documentation files: DEMOS, DIFFERENCES, DOCUMENTATION,
     FAQ, FEATURES, ISSUES, PHILOSOPHY, PORTING, README, TODO.
  -- Added use of configure scripts to automate Makefile tailoring on
     installed platforms.  The configure scripts are generated from the
     configure.in files using GNU autoconf (version 2.0).  The Makefile
     is created by the configure program by using the Makefile.in
     template.  The configure script also generates the Makefile in the
     subdirectory soar (a.k.a, 6.2.6).  The previous Makefile was renamed
     to non-tcl-makefile, for users who want to build the old Soar 6 user
     interface.  The configure process also has supporting files starting
     with the name "config".
  -- Files added to support the Soar/Tcl connection: command_utils.[ch],
     commands.[ch], ma_commands.[ch].  These files implement new versions
     of the Soar 6 user interface commands.  These new versions use the
     standard Tcl C API for commands.
  -- Created a demos subdirectory to hold Tcl-specific demos.
  -- Created a doc subdirectory to contain documentation on new Soar
     commands added to the base Tcl interpreter.  The sources are in
     man troff format.  Scripts are used to translate these to "cat" and
     "html" forms.  This form is more closely aligned with the Tcl 
     documentation and is intended to replace the on-line Soar help in 
     Soar 6.
  -- Created a library subdirectory to contain Tcl initialization code
     for a Soar agent.
  -- Added a script "mosaic-bg" used to run a mosaic process under 
     the remote control of Tcl.  This is used in the gui.tcl example 
     in the demos directory to show the new documentation form.  
  -- Added tclAppInit.c and tkAppInit.c which are used to initialize (in
     C code) the Tcl+Soar executable and the Tcl+Tk+Soar executable.
  -- Various modifications to the original Soar source code to interface
     Tcl.  The changes are marked with "#ifdef USE_TCL" and "#ifdef
     USE_TK":
     - callback.[ch]: Added a callback system intended to replace the
       use of hook functions.  This allows dynamic alteration of
       functionality during various events in Soar.
     - global_vars.h: Added Tcl-related variables "interpreter", to 
       indicate the interpreter associated with an agent; "window", to
       indicate the Tk window associated with an agent; and 
       "tcl_output_buffer", used to manage I/O with the Soar 6 parser.
     - init_soar.c: Added an include for the Tcl-related "commands.h"
       header.  Modified the QUIESCENCE_PHASE processing in 
       do_one_top_level_phase to unconditionally print a carriage return 
       for Tcl.  Added a default callback function for printing on
       stdout.  Added agent initialization which creates a Tcl interpreter
       and Tk main window for each agent.  Added code to delete Tcl
       interpreter when agent destroyed.  Added function Soar_Init to
       be used to initialize Soar from Tcl.
     - interface.c: Added a header include for Tcl support
       (command_utils.h).  Commented out several utility functions that
       are now found in command_utils.c.  Since commands in interface.c
       are replaced by code in commands.c, the supporting utility
       functions need to be moved out of interface.c as well.  Interface.c
       defines the Soar 6 user interface and will be removed, eventually.
     - lexer.c: Added code to examine the "alternate_input_string" buffer
       for input to the Soar parser under Tcl.  The
       "alternate_input_suffix" is examined after the buffer is exhausted
       for trailing characters.  This makes is a little more efficient
       since a new string doesn't have to be formed at the Tcl level.
       Changed default token classification to QUOTED_STRING_LEXEME for
       Tcl, to avoid quirky quoting issues with Tcl to Soar input.
       Removed prompt printing nonsense for Tcl -- all prompts are printed
       from Tcl.
     - main.c: Eliminated main function under Tcl since Tcl has its own
       main.
     - print.c: Added Tcl header.  Redirected prints to new function
       Tcl_LogAndPrint which uses the callback system instead of printing
       to stdout.  Added a function to print WMEs differently for Tcl
       (print_wme_for_tcl).
     - rhsfun.c: Added Tcl RHS function call.  Allows Soar productions to
       run commands in the agent's Tcl interpreter.
     - scheduler.c: Added commands.h header.  Added call to switch agent
       context before running an agent -- this ensures that all critical
       global variables are updated so that an agent is run as the
       "current" agent.
     - soar.h: Added decls of Tcl related functions:
         print_wme_for_tcl, print_all_trace_formats_tcl, Soar_Init.
       Added Tcl and Tk header files.
     - trace.c:  Changed output of print_tracing_rule to eliminate parens
       since Tcl doesn't use them.  Added alternative function
       print_tracing_rule_tcl that uses Tcl syntax.  Added alternative
       function print_all_trace_formats_tcl which uses the function
       print_tracing_rule_tcl to print the formats.

Version 6.2.6.beta
  -- Added "else : ;" to for loop in dirs target in
     tcl/ts/soar/Makefile.in to work around problem with Bourne shell at CMU.
  -- Replaced use of "-x" with "-f" in tcl/make-soar (some sites don't
     have this option in the "test" command).
  -- Removed config.cache (a site dependent file) from tcl/ts in
     distribution.

Version 6.2.6.gamma
  -- In tcl/ts/command_utils.h, changed decl. of count_memory_use_for_node
     to:  extern unsigned long count_rete_tokens_for_production
     (production *prod);
  -- In tcl/ts/command_utils.c, Changed print_memories() stuff to call 
     count_rete_tokens_for_production() ; Deleted
     count_memory_use_for_node() and associated routines;  Commented out 
     rete-stats command stuff (for now, need to go back and fix this later
     -- BUGBUG)
  -- In tcl/ts/commands.c, Changed call to count_memory_use_for_node()
     into call to count_rete_tokens_for_production().
  -- Chunk.c:  Minor changes for revised rhs_value and action formats.
  -- Decide.c:  Changed all occurrences of "common.decider_wme" to 
     "common.a.decider_wme".
  -- Glob_vars.h:
     - Removed ncc_subconditions_bottom_dense_nvn
     - Removed ncc_subconditions_bottom_sparse_nvn
     - Removed ncc_subconditions_bottom_node
     - Removed neg_token_pool
     - Removed dummy_matches_node_tokens
     - Changed type of ms_assertions, ms_retractions, left_ht, right_ht,
       dummy_top_node, dummy_matches_node, dummy_matches_token
     - Added decl. of dummy_top_token
     - Added decl's of max_rhs_unbound_variables, rhs_variable_bindings
     - Removed production_addition_result, production_being_added_now,
       refracted_inst_for_production_being_added_now, 
       warn_on_duplicate_production
     - Added rete_node_counts_if_no_sharing[] array
     - BUGBUG what to do about token_additions, token_deletions?
  -- Init_soar.c:  Removed initialization of dummy_matches_node_tokens.
  -- Interface.c: 
     - Changed calling sequence for p_node_to_conditions_and_nots().
     - Made "memories" command use count_rete_tokens_for_production().
     - Fixed "pf" command stuff to use new RHS format stuff.
     - Added "fastload-productions" and "fastsave-productions" commands.
  -- Mem.c:  Changed it so memory blocks now have to start on a 4-byte 
     boundary (i.e., low-order *two* bits have to be zero) instead of a 
     2-byte boundary.  This is needed because we now use the low-order 
     two bits of rhs_value's as type specifier bits.
     - #defined DEFAULT_BLOCK_SIZE constant to replace hard-coded number.
     - BUGBUG why does it keep using sizeof(int *) rather than sizeof(int)
       on growable strings?
  -- Osupport.c:
     - Minor changes for revised rhs_value and action formats.
     - BUGBUG extern decl's of calculate_compile_time_o_support().
  -- Parser.c:  Removed first_letter_from_test() -- it's now in
     production.c because it's now used not only by the parser but also by
     the Rete (for gensymming chunk variable names).
     - Minor changes for revised rhs_value and action formats.
  -- Print.c:  Minor changes for revised rhs_value and action formats.
     Changed print_production() so it gets the RHS by calling
     p_node_to_conditions_and_nots() -- need this to re-gensym chunk variable
     names.
  -- Production.c:
     - Added first_letter_of_xxx() routines.
     - Minor changes for revised rhs_value and action formats.
     - Changed make_production() -- the Rete now fills in 
       p->rhs_unbound_variables.
  -- Recmem.c:  Minor changes for revised rhs_value and action formats.
     - Changes to firer for revised rhs unbound variable handling.
  -- Reorder.c:  Minor changes for revised rhs_value and action formats.
  -- Rete.c:  Changed whole file.
     - Adds left unlinking.
     - Adds dynamic split/merge of beta memories with single child join nodes
       (previously always merged, even when more than one child -- caused
       more tokens to be created)
     - Adds tree-based removal (previously used rematch-based removal)
     - Adds "fastload-productions" and "fastsave-productions" commands.
     - Discards names of variables in chunks (re-gensyms them when you
       print the chunk).  This can be disabled (i.e., make Soar save the 
       variable names) by setting discard_chunk_varnames = FALSE.
     - Added count_rete_tokens_for_production().
  -- Soar.h:
     - Removed a bunch of Rete-related declarations
     - Removed detailed declaration of node_varnames structure
     - Removed detailed declaration of ms_change structure
     - Added common field to all symbols (unioned with decider_wme, so it 
       doesn't take up any extra space) for use in fastsave/load
     - Added rete_binding_locations field to variables
     - Added fields to wmes for tree-based removal in the Rete
     - Revised declarations for rhs_values and actions
     - Added declarations for first_letter_from_xxx() routines
     - Revised decl. of p_node_to_conditions_and_nots(), added decl. of
       get_symbol_from_rete_loc().
     - Added decl. of count_rete_tokens_for_production().
     - Added decl. of fast_remove_from_dll() -- a slightly faster version
       of the remove_from_dll() macro.
     - Added some new Rete-related declarations
  -- Symtab.c:  Added initialization of rete_binding_locations field on
     new variables.
  -- Added rete-net command to access Bob's binary rete save/load feature.
     Added a matching man page.  Added command to demos/gui.tcl menu bar.
  -- changes for ports to DEC pmax Mach and Apple Quadra MachTen platforms
  -- Added check in config.guess to consider MachTen OS on Macs.
  -- Added fix to config.sub for MachTen OS on Macs.
  -- Added check for "fmod" to Tcl configure enabling Tcl to build under 
     CMU Mach OS.  Other Mach OS changes needed: use gcc and gmake since
     native versions busted.
  -- Removed used ot "mkdir -p" since switch not universally recognized.
  -- Added "#ifdef" checks to tilde.c for dirent.h.
  -- Cleaned up and added more documentation to demos/gui.tcl file.
  -- Changed demos/graph_tool.tcl to use chunk count rather than pos nodes
     in rete since rete stats has changed.
  -- Based on rete changes, revised code which extracts individual stats
     and updated man page correspondingly.
  -- Fix to wmem command where successive invocations led to parse error.
     (Problem with buffer handling in wmem command.)
  -- Fixed GUI prompting so always occurs on a newline
  -- Fixed several GUI logging bugs
  -- Fixed problem with typed "select-agent" command failing in GUI
  -- Fixed bug in create-agents command that would switch the "current" 
     interpreter to the newly created one.
  -- Intercept "help" commands entered in GUI text window so output
     corresponds to that obtained from menus.
  -- Renamed help files to match commands
  -- GUI intercepts typed in "help" commands and displays like the menu action
  -- Help proc checks for several pagers if env var PAGER not set
  -- Added list-help-topics command and help -all
  -- All 4 modules now use gcc and gmake if available (so take these out
     of your path for the SGI build there at UMich)
  -- All 4 modules cache results of configure
  -- Merged ma_commands.c file into commands.c file

Version 6.2.6.delta
  -- Reorder initializations in init_soar.c to a more rational order.
  -- Added before/after schedule-cycle events to support the running
     of simulators.
  -- Fixed loading of init files so that a "cd" is performed first to
     properly handle relative path names.
  -- Reorganizing code so Tcl, Tk, and BLT are as originally distributed
     from the Tcl Archive site.  TclSoar is now a fairly standard Tcl 
     extension and all code changes are contained in the TclSoar
     subdirectory.
  -- Moved soargraph demo file from tcl/blt-1.7/demos to tcl/ts/demos
     restoring BLT-1.7 to its original distributed state.
  -- Moved changed files in tk3.6 (tkMain.c tkText.c tk.h) to tcl/ts
     restoring Tk 3.6 to its original distributed state.
  -- Moved callback system out of Tcl 7.3 into tcl/ts/kernel restoring
     Tcl 7.3 to its original distributed state.
  -- Fixed seg fault upon exit -- needed to destroy top level window
     before destroying Tcl interpreter.
  -- Corrected "r" and "run" so that they run "forever" without any arguments
  -- Fixed bug so that Soar can be compiled without Tcl (uses Soar 6 user
     interface).
  -- Cleaned up order of functions in file commands.c
  -- Added "pf" command.
  -- Fixed bug in printing conjunctive negations -- the -{} chars were
     being dropped.  Change to function Tk_print_to_text_widget now makes
     text widget string insertions faster by avoided nested calls to Tcl.
  -- Enhanced build so that Soar can be built without Tk.

Version 6.2.6.epsilon
  -- fixes for build on HP.  Includes addtion of _INCLUDE_POSIX_SOURCE
     flag during compilations.
  -- fix for prompt error under Tk (for Doug Pearson)
  -- creation of libraries libtclsoar.a and libtksoar.a for linking
     by user programs
  -- Split agent related code out of init_soar.c into agent.c in soar/src
  -- Renamed files in tcl/ts to conform to Tcl conventions
  -- Removed all calls from tcl/ts/soar/src into tcl/ts to avoid
     unresolved symbols during library links.  Previous functions in
     interface.c that were calling into tcl/ts were prefixed with "old_"
     and no longer excluded by #ifdef USE_TCL switches.
  -- Added files soarAgent.c and soarScheduler.c in tcl/ts to override
     definitions in libsoar.a.
  -- Added numerous fixes and cleanups from Bob (see 
     tcl/ts/soar/doc/bobschangelog2)
  -- Rete fastsave file format changed -- incompatible with earlier
     versions
  -- Backtrace.c:  Removed some unnecessary extern declarations.
  -- Chunk.c:  Minor changes for gcc -Wall, removed some unnecessary 
     extern declarations.
  -- Explain.c:  Minor changes for gcc -Wall  
  -- Glob_vars.h:
     - Added 4 decl's:   num_[null_]{left,right}_activations
     - Added decl. of token_additions_without_sharing
     - Removed decl. of token_deletions
     - Added comments, rearranged stuff to neaten it up
     - Removed decl. of placeholder_counter[] -- moved it to parser.c.
     - Removed decl. of found_undefined -- moved it to trace.c.
     - Removed decl. of format -- moved it to trace.c.
     - Removed decl. of format_string_error_message -- moved it to trace.c.
     - Removed decl. of collected_nots -- moved it to rete.c.
     - Removed index_num.
     - Removed dummy_matches_node.
  -- Init_soar.c:
     - Added initializations for token & activation count variables.
     - Moved initialization of match_cpu_time inside #ifdef 
       DETAILED_TIMING_STATS.
     - Minor changes for gcc -Wall
     - Removed useless comments for global variables no longer there
     - Removed initial assignment to index_num.
  -- Interface.c:
     - In print_memories(), removed "curr_prod" variable from print_memories().
     - Minor changes for gcc -Wall
     - Commented out old version of "$" command
  -- Io.c:  Moved typedef's of some I/O structures here from soar.h
  -- Osupport.c:  Removed some unnecessary extern declarations.
  -- Parser.c:  Minor changes for making placeholder_counter[] not a 
     per-agent thing.
  -- Prefmem.c:  Eliminated unintentional use of trigraph in comment.
  -- Production.c:  Minor changes for gcc -Wall  
  -- Recmem.c:
     - Eliminated unintentional use of trigraph in comment.
     - Minor changes for gcc -Wall  
     - Removed some unnecessary extern declarations.
  -- Rete.c:
     - Added more comments, etc.
     - Added decl. of MS_trace (moved from soar.h)
     - Changed gensymmed variables to look like <v37> instead of <v*37>.
     - Renamed current_agent(collected_nots) as nots_found_in_production.
     - Added dummy_matches_node as a not-per-agent global variable.
     - Revised format of fastsave/load files -- now on version 3.
  -- Rhsfun.c:
     - Added "#ifdef USE_TCL" around installation of "tcl" rhs function.
     - Added alternate "#ifdef __ultrix" clause to avoid warning message on
       call to localtime().
  -- Rhsfun_math.c:  Removed unused variable "arg_value" in 
     abs_rhs_function_code().
  -- Scheduler.c:  Fixed up execute_go_selection() call for non-Tcl version.
  -- Soar.h:
     - Removed decl. of MS_trace (moved to rete.c)
     - Added note about rhs_value's
     - Added comments
     - Added many declarations for stuff that was being used in one file
       but defined in another, hence needed an extern declaration.
     - Moved typedef's of some I/O structures to io.c
  -- Trace.c:
     - Minor changes for gcc -Wall  
     - Minor changes for making found_undefined not a per-agent thing.
     - Minor changes for making format not a per-agent thing.
     - Minor changes for making format_string_error_message not a per-agent
       thing.
  -- make.body:
     - Added stuff to compile and link callback.c
     - Added callback.h to list of header files

Version 6.2.6.zeta
  -- removed text I/O stuff
  -- removed non-NNPSCM option
  -- fixed several memory leaks
  -- modified RHS grammar so it allows extra commas
  -- modified RHS grammar so attributes can be numbers or function calls
  -- fixed Gary's "multi-attribue" (with very large number) bug
  -- Removed files queue.c, queue.h; updated Makefile.in, make.body accordingly
  -- soarAgent.c:
     - Removed text I/O stuff.
     - Removed NNPSCM #ifdef stuff.
  -- soarCommandUtils.c:  Removed NNPSCM #ifdef stuff.
  -- soarCommands.c:  Removed NNPSCM #ifdef stuff.
  -- Agent.c:
     - Removed text I/O stuff.
     - Removed NNPSCM #ifdef stuff.
  -- Chunk.c:  Removed NNPSCM #ifdef stuff.
  -- Decide.c:  Removed NNPSCM #ifdef stuff.
  -- Glob_vars.h:
     - Removed text I/O stuff.
     - Removed NNPSCM #ifdef stuff.
  -- Init_soar.c:
     - Removed NNPSCM #ifdef stuff.
     - Removed "NNPSCM" from startup banner printout (there's no other
       version now).
     - Added "TCL" to startup banner printout (in Tcl version only).
  -- Interface.c:
     - Removed NNPSCM #ifdef stuff.
     - Minor changes in old_actions_are_equal_with_bindings() to allow RHS 
       attributes to be general rhs_value's.
  -- Io.c:
     - Reset io_header and io_header_link to NIL when top state is removed.
     - Removed text I/O stuff.
     - Removed NNPSCM #ifdef stuff.
  -- Osupport.c:
     - Removed NNPSCM #ifdef stuff.
     - Minor changes in various places to allow RHS attributes to be general
       rhs_value's.
  -- Parser.c:
     - Added call to deallocate_test() in parse_head_of_conds_for_one_id(), to
       fix memory leak (variables didn't get deallocated).
     - Changed 'g' (for goal) to 's' (for state).
     - Removed NNPSCM #ifdef stuff.
     - Modified RHS grammar so attributes can be arbitrary rhs_value's,
       not just variables or symbolic constants.
     - Modified RHS grammar to allow extra commas.
  -- Print.c:
     - Removed NNPSCM #ifdef stuff.
     - Minor change to print_action_list() to allow RHS attributes to be 
       general rhs_value's.
  -- Production.c:
     - In deallocate_production(), changed free_list(rhs_unbound_vars) to
       deallocate_symbol_list_removing_references() to fix memory leak.
     - Changed 'g' (for goal) to 's' (for state).
     - Minor change to add_all_variables_in_action() to allow RHS attributes to 
       be general rhs_value's.
  -- Recmem.c:  Changed 'g' (for goal) to 's' (for state).
  -- Reorder.c:
     - Fixed MAX_COST and reorder_simplified_conditions() to avoid core dumps
       with very large multi-attribute values.  This fixes Gary Pelton's bug
       report of 3/15/95.
     - Fixed memory leak in remove_isa_state_tests_for_non_roots().
     - Removed NNPSCM #ifdef stuff.
  -- Rete.c:  Changed 'g' (for goal) to 's' (for state).
  -- Soar.h:
     - Removed text I/O stuff.
     - Removed NNPSCM #ifdef stuff.
  -- Symtab.c:  Removed NNPSCM #ifdef stuff.
  -- Tempmem.c:  Removed NNPSCM #ifdef stuff.
  -- Trace.c:  Removed NNPSCM #ifdef stuff.
  -- x_interface.c:  Removed text I/O stuff.
  -- Fixed bug in load command where previous "cd"s did not affect the
     Soar kernel's directory stack.
  -- Fixed bug in Makefile.in that was leaving off TclSoar scheduler
     in soarScheduler.c
  -- Added a new function Soar_AgentInit as a hook function so that
     users can copy tkAppInit.c (or tclAppInit.c) and add their own
     C extensions.  A demo is in demos/C_extension.
  -- Finished C extension example in tcl/ts/demos/C_extension
  -- Added documention to demos directories (README files) and
     reorganized directory to make more modular.  Removed non-nnpscm
     stuff in anticipation of Bob's changes.
  -- Fixed bug in "preferences" command noticed by John Laird.
  -- Fixed a bug in soar callback initialization (after-init-agent) was
     called before soar_init_callbacks).
  -- Reorganized initialization.  Sequence is now 1) init internal 
     Soar structures 2) source tclsoar.tcl 3) call Soar_AgentInit
     4) eval "init-agent" 5) call after-init-agent callback
  -- Added check to Graph Tool Demo to make sure that BLT exists.  This
     fix prevents the GUI from hanging when BLT can't be found.
  -- Fixed a scheduler problem for Mike v. and John related to tag-soar
     running global agent callbacks > once when stopping.
  -- Fixed bug in help -- env. var PAGER was being ignored.
  -- Added info. about default setting to man page for user-select.
  -- Fixed a bug in pf command -- system would hang if pattern not
     escaped with {} or "".
  -- Merged Tcl exit and Soar quit commands.
  -- Defined "?" to print message about Soar-only commands.
  -- Added numeric arguments back for wme details in matches, preferences,
     and ms commands.
  -- Added a "help-syntax" command to list syntax from man pages.
  -- Corrected alias facility so that only defined aliases can be
     unaliased.  Also, added switch to "print-alias" to list all aliases.
  -- Added warning to alias definition when replacing an exising command.
  -- Added TCL and/or TK to version string for SDE.
  -- Changed tcl/ts/soar/Makefile.in to compile objects if makefile changes
  -- Updated documentation on sp command for NNPSCM
  -- Addition of SHELL macro to tcl/ts/Makefile.in to force Bourne shell
     usage in make on SGI.
  -- queue.{c,h} files were only needed for text-io so they have been deleted.

Version 6.2.6.eta
  -- Updated trace format stuff for NNPSCM --- *** User-visible changes: ***
     - Can no longer specify special trace formats for goals & problem spaces
     - %cg (print current goal) and %cp (print current problem space) no
       longer supported as trace format string escapes
  -- Fix for Gary's core dump bug
  -- Added Doug Pearson's o-support calculation scheme (3-way option)
  -- Updated "go" command for NNPSCM --- *** User-visible changes: ***
     - Can no longer specifiy "g" and "ps" options on "go" command.
  -- Reorderer now checks for RHS function calls with unbound variables as
     arguments -- will not accept these.
  -- soarAgent.c:
     - Changed FOR_GOALS_TF to FOR_STATES_TF.
     - Added initialization of current_agent(o_support_calculation_type).
  -- soarCommands.c:
     - Minor changes for updated (NNPSCM) trace format stuff.
     - Changed comments for "go" command -- "g" and "ps" args no longer allowed.
     - Added "o-support-mode" command.
  -- soarCommandUtils.c:
     - In execute_go_selection(), removed cases for GO_GOAL and 
       GO_PROBLEM_SPACE.
     - In parse_go_command(), removed cases for GO_GOAL and GO_PROBLEM_SPACE.
     - Minor changes to actions_are_equal_with_bindings() to allow RHS
       attr's to be arbitrary rhs_value's. (I fixed this in interface.c 
       earlier but forgot to fix it here.)
  -- go.n:  Updated for NNPSCM "go" command stuff.
  -- trace-format.n:  Updated for NNPSCM trace format stuff.
  -- agent.c:
     - Changed FOR_GOALS_TF to FOR_STATES_TF.
     - Added initialization of current_agent(o_support_calculation_type).
  -- Chunk.c: Minor changes to chunk_instantiation() for Gary's core dump bug.
  -- Decide.c:  Changed FOR_GOALS_TF to FOR_STATES_TF.
  -- Explain.c:  Minor changes to explain_add_temp_to_chunk_list() for
     Gary's core dump bug.
  -- Glob_vars.h:
     - Removed context variables for goals & problem spaces (useless with 
       NNPSCM)
     - Changed trace format arrays to have 3 entries instead of 5 (gets rid of
       special entries for goals and problem spaces).
     - Added decl. of o_support_calculation_type.
  -- Init_soar.c:  Updated comments for run_for_n_selections_of_slot() for 
     NNPSCM.
  -- Interface.c:
     - Changed FOR_GOALS_TF to FOR_STATES_TF.
     - Updated help screens for (1) trace formats and (2) trace format escapes.
     - Updated "go" command stuff to no longer accept "go for n selections
       of goals" and "go for n selections of problem spaces".
     - Added "o-support-mode" command.
  -- Osupport.c:  Added routine to do Doug Pearson's scheme for o-support 
     calculations.
  -- Recmem.c:  Added code to fill_in_new_instantiation_stuff() to do 
     (optionally) Doug Pearson's scheme for o-support calculations.  3
     modes, controlled by value of global variable o_support_calculation_type 
     (0=normal, 1=warn on differences, 2=Doug's).
  -- Reorder.c:  In legal_to_execute_action(), added checks to make sure
     RHS function calls have all their arguments bound (i.e., no unbound 
     variables as args).
  -- Soar.h:
     - Change to parameters for explain_add_temp_to_chunk_list(), for
       Gary's core dump bug.
     - Minor changes for updated (NNPSCM) trace format stuff.
     - Updated comments for run_for_n_selections_of_slot() for NNPSCM.
     - Removed GO_GOAL and GO_PROBLEM_SPACE from "enum go_type_enum".
     - Removed isa_state and isa_problem_space fields from identifiers.
  -- Symtab.c:
     - Removed context variables for goals & problem spaces (useless with 
       NNPSCM)
     - Removed initialization of isa_state and isa_problem_space on 
       identifiers.
  -- Trace.c:  Revisions for updated (NNPSCM) trace format stuff.
  -- Wmem.c:  Removed update of isa_state and isa_problem_space on identifiers.
  -- Added code to catch errors from all Tcl_Eval calls.
  -- Added a procedure to demos/gui.tcl to beautify cat pages by
     converting term control sequences to Tk text tags.
  -- Added cast functions "int" and "float" to Soar kernel.
  -- Changed RHS function "tcl" to return a value so it can be used
     in RHS functional compositions.
  -- Some man page corrections based on Clare's email messages
  -- Changed all man pages to use SEE ALSO instead of KEYWORDS.
  -- "soar" command renamed "soar6".
  -- Per Karen Coulter's request, changed "make-soar" to write a
     file summarizing build settings.
  -- Removed MULTI_AGENT_ENABLED switch.  Removed multi_agent_mode
     variable.  Removed processing of ".init.soar.multi" file.
     Folded global var decls back in soar.h -- and removed global_vars.c
     and global_vars.h.
  -- Added dependency on soar.h into tcl/ts/Makefile.in
  -- Added SHELL env. var setting in tcl/ts/Makefile.in for SGI

Version 6.2.6.theta
  -- Fixed decider bug where it preferred already-in-WM values among 
     indifferent items.
  -- Added experimental "attribute-preferences-mode" command.  This is a 
     test of John's proposal for disallowing most attribute preferences. 
     There are three modes available:
      0: Just like Soar 6 now.
      1: Just like Soar 6 now, except prints a warning whenever a 
         preference other than + or - is created for a non-context slot.
      2: Whenever a preference other than + or - is created for a 
         non-context slot, it prints an error message and ignores (discards)
         that preference.  For non-context slots, the set of values
         installed in working memory is always equal to the set of 
         acceptable values minus the set of rejected values.
  -- soarAgent.c:  Added initialization of 
     current_agent(attribute_preferences_mode).
  -- soarCommands.c:  Added attribute-preferences-mode command.
  -- agent.c:  Added initialization of 
     current_agent(attribute_preferences_mode).
  -- decide.c:
     - In run_preference_semantics(), removed code to prefer already-installed
       value among mutually indifferent candidates.
     - In run_preference_semantics(), added handling for when
       attribute_preferences_mode=2.
  -- interface.c:  Added attribute-preferences-mode command.
  -- recmem.c:  In execute_action(), added handling of 
     attribute_preferences_modes 1 and 2.
  -- soar.h:  Added decl. of attribute_preferences_mode as a per-agent 
     variable.
  -- Removed global agent from Soar kernel and Tcl interface
  -- Added several command line options.  Available options are now:
     agent, path, display, file, geometry, help, noTk, sync, tclsh,
     verbose, wish
  -- New man page describing command line options and Soar startup
     (listed first in Soar HTML directory of man pages -- see
     http://www.isi.edu/soar/tclsoar/soar_man.html)
  -- Made "soar" the default startup agent
  -- Removed Soar 6 gateway (and hence, the "load" command) since
     these depended on a global Soar agent.
  -- Converted default.soar file to TclSoar syntax.  It is contained
     in .../tcl/ts/library/default.soar
  -- Converted all examples to TclSoar syntax and added them to the
     "demos" subdirectory.  There are now 4 GUI demos and 8 non-GUI demos.
  -- Tk's "send" command has been extended to work with local non-Tk 
     interpreters.  Bug fix to send so remote sends to local agents 
     works properly.  "send" now subsumes the old "send-agent" command
     which has been removed.
  -- Changed agent init procedure to look for files with names
     <agentname>.soar instead of subdirectories.
  -- Added ability to create agent, wish, or tclsh interpreter
     shells from command line or via create-interp command
  -- Replaced {create,list,select,destroy}-agent with 
     {create,list,select,destroy}-interp commands and added the original
     commands as new aliases.
  -- Added Clare's help message to help command
  -- tksoar binary now works without X.  This can be controlled
     via a command line switch (-noTk) or the absence of a DISPLAY
     environment variable setting.
  -- Replaced "help-syntax" with "help -usage".
  -- Replaced Tcl global var "agentname" with the new globals
     "interp_name" and "interp_type".
  -- Added these commands to normal tclsh/wish interpreters:
     create-interp, list-interps, select-interp, schedule-interp,
     destroy-interp, send (generalized)
  -- Added schedule-interp command to process interpreter(s) in the
     Soar scheduling process
  -- Tclsh and wish interpreters initialized using the "path" setting
     and <name>.soar files.
  -- "agentpath" command-line switch changed to simply "path" since it
     applies to all types of interpreters.
  -- TCLSOAR_OPTIONS env. var no longer processed.  Command line option
     tailoring more flexibly handled by Unix shell aliases.
  -- non-Tcl version (using the Soar 6 UI) now has a default startup
     agent called "soar" with a simpler prompt.
  -- Added ability to compile Soar when Tcl and Tk are pre-installed
     elsewhere.  To support this, renamed .../tcl/make-soar to 
     .../tcl/make-all and added .../tcl/ts/make-soar.
  -- Streamlined user extension hook to a single file (soarAppInit.c)
     and a single function (Soar_InterpInit).
  -- Added files tclBasic.c, tkConfig.h, soarArgv.c, soarInterp.c, soarMain.c
  -- Improved modularity by removing files tcl.h, tk.h, tclMain.c, tkMain.c
  -- Removed hook functions in Soar kernel -- code now uses callback system
  -- Added man2thml converter to tcl/ts/doc subdir so users can do their
     own HTML conversions
  -- Fixed numerous gcc -Wall warnings

Version 7.0.0.beta
  -- Rewrote test suite to work with Tcl and under Bourne shell
  -- Fixed problem where error not reported for "d hi".
  -- Fixed seg fault when DISPLAY set to a bogus value.
  -- Fixed seg fault when starting two tksoar's.  Added code to notice
     when interp name changed due to registration with X server.
  -- Fixed a bug where -noTk and empty DISPLAY still gave a warning
  -- Fixed a typo: watch command arg firings-wme was missing last "s".
  -- Fixed error in soarCommands.c for watch command.  In test for on/off
     base should be TRACE_FIRINGS_OF_USER_PRODS_SYSPARAM.
  -- Upgraded Soar to Tcl 7.4 and Tk 4.0.
  -- Did a major overhaul on the configure.in script and the Makefile.in
     file to more closely match Tcl's approach.  This should make the
     software more robust and able to be installed in system areas.
  -- Took new versions of tclBasic.c, tkText.c, and tkSend.c from the new
     Tcl/Tk distributions and added Soar mods.
  -- Updated Tcl_SendCmd.c in soarInterp.c to match send process in
     tkSend.c
  -- Updated demos/gui.tcl file (and other Tk demo files) with Tk 4.0 
     changes.  This included changing "geometry" switch to "width" and 
     "height" switches; adding -tearoff option to agent list menu; adding
     "postcommand" switch for dynamically generated menus; updating
     bindings to account for new hierarchical composition approach;
     removing explicit references to Bisque color scheme.
  -- soarInterp.c: Removed call to "destroy ." when removing an
     interpreter.  Bug apparently fixed in Tk 4.0.
  -- Makefile.in: Renamed executables: tclsoar -> soar, tksoar -> soartk.
     Ditto for library names.  
  -- Dropped directory link "soar", and renamed "6.2.6" to "kernel".
  -- Renamed kernel/src/soar.h to kernel/src/soarkernel.h and updated all
     files including this header file.
  -- Renamed tclsoar.h to soar.h
  -- Added $default variable to enable "source $default" to read in
     default rules (Clare's #32)
  -- Made ? synonym of "help" (Clare's #4b)
  -- Changed pager order in "help" command from "less, more, page" to
     "more, page, less" (Clare's #5)
  -- Changed "default-print-depth" to "default-wme-depth" (Clare #9)
  -- Changed "quiescence phase" to "decision phase"
  -- Changed reference in rhsfun_math.c from "strtod" to "my_strtod"
     to increase portability and consistency.
  -- Fixed "cd" when issued with no arguments
  -- Changed "ptrace" to "pwatch" (Clare #34)
  -- Changed "no-o-support" and "NO_O_SUPPORT" and "DECLARED_NO_O_SUPPORT"
     to "i-support", "I_SUPPORT", and "DECLARED_I_SUPPORT" per Clare #14.
  -- Changed "trace-format" to "format-watch" (Clare #26)
  -- In tcl/ts/kernel: Renamed README to INSTALL and added a new README
     file.
  -- Subdivided demos directory into gui and no-gui portions.  Clare #20.
  -- Renamed "print-all-symbols" to "internal-symbols" (Clare #23)
  -- Renamed "print-destination" to "output-strings-destination" (Clare #24)
  -- Added function "reset_production_firing_counts" to function
     "reset_statistics" in file tcl/ts/kernel/src/init_soar.c.  Also
     update corresponding man page for command init-soar. (Clare #12)
  -- Changed arguments to "command-to-file" Tcl proc to be "-new" and
     "-existing" (was "w" and "a").  Clare #38.
  -- Merged the "pgs" command into "print" as the option "-stack".
     Updated the "print" man page and eliminated the "pgs" man page.
     Clare #30.
  -- Enhanced "help" to consider Tcl, Tk, and Unix man pages.  "-all"
     option now includes all Tcl, Tk, and Soar commands.  All help is
     now generated via nroff (was using cat pages) and man (for Unix
     pages). (Clare #4)
  -- Merged all alias man pages into one called "aliases".
  -- Enhanced "help" to catch aliases defined on "aliases" man page
     so that cummary page can be used rather than individual man
     pages on each alias.
  -- Moved Soar 6 related aliases to tcl/ts/library/alias-soar6.tcl
     (was in tcl/ts/library/tclsoar.tcl)
  -- Renamed "tclsoar.tcl" file to "soar.tcl"
  -- Changed GUI "step" button to "step 1 dc" to clarify step size
  -- Changed GUI "Save/Load Rete" to "Save/Load Production Memory"
     to make more generic for naive users and to accomodate possible
     production network change (e.g., to TREAT vice RETE)
  -- Changed "log" command to use "-new" and "-existing" rather than
     "a" and "w" access modes.  Removed use of "-open".  Made switch
     order of "-new" and "-existing" more flexible to match related
     command "command-to-file".  (Clare #37)
  -- Moved these agent-only commands to soarInterp.c so they could be
     used by all interps: soarnews, version.
  -- Renamed "init-interp" to "init-interpreter"
  -- Renamed "eval-in-interps" (or send-to-interps) to "send-to-interpreters"
  -- Merged "matches" and "ms" commands.  Updated matches man page
     and removed "ms" man page. (Clare #8).
  -- Removed all "#if 0" code in soarCommands.c
  -- Completed changes to explain command for Soar 7.  Removed on/off
     switch, removed sole -trace switch, removed side-effect of
     remembering last chunk/justification used. (Clare #25)
  -- Updated man page for explain to match above change.
  -- Added Tcl I/O routines to match proposal.  Now functions much
     like C I/O code.  C I/O changed to use callback system to 
     accomodate adding and removing (new) I/O functions.
  -- Updated demos/C_extension/soarAppInit.c to use new I/O scheme.
  -- Added demos/soar-io-using-tcl.tcl to illustrate how to use Soar
     I/O facilities using only Tcl.
  -- Upgraded BLT to 1.8 to match use of Tcl 7.4/Tk 4.0.  Upgraded
     graph tool demo to use this new version.
  -- Fixed a bug in the send command that incorrectly acted as if
     Tk interpreters did not have send capability.
  -- Updated make-all script for new versions of Tcl/Tk/Soar/BLT.
  -- Changed all internal references to "tclsoar" to be "soar".
  -- Changed "ts" dir name to "soar-7.0.0.beta"

*******************************************************************************
Earlier Versions?
*******************************************************************************

