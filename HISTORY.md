###############################################################################
This is an attempt to give credit to the probably hundreds of cognitive
scientists, programmers and others that have created the successive 
versions of Soar flavoured production systems. This story goes back into 
the 1960s and spans multiple computer languages and human generations. 

In "Concrete Mathematics"(1) it is mentioned that there is a growing 
movement in mathematics to be more consistent and generous in crediting the 
human beings that create instructive problems in mathematics. We support this 
ideal of giving credit where a source is known and hope this file will 
encourage others to do the same.

An affordable and fairly detailed history of the early work on AI is 
"Introduction to Artificial Intelligence"(2). This book is informative,
entertaining, and gives the reader a larger perspective of the early
foundations of research into machine simulation of intelligence. P.S. 
It in fact has an very good discussion of state machines.

(1) Concrete Mathematics, Graham-Knuth-Patashnik.
    -- Adison-Wesley (pp. viii) 2nd Ed. (1998 w/ updates to errata)
    
(2) Introduction to Artificial Intelligence, Philip C. Jackson Jr.
    -- Dover (reprint) 2nd, Enhanced Edition (1985)

###############################################################################
SiOO State-input Operator Output ([...]/bin/sioo)
###############################################################################
SiOO (see'-ooh) is entirely the fault of Brig Young and un-named cohorts. 
                                              
-- This is an academic exercise in system interface design beginning at a 
foark in the road for soar. This encompasses creating C language interfaces
and C Language tools for a fork of Soar 8.5.2 that has been stripped down to
the kernel and its interface headers.

The research review that led to this point began in February of 2015. Soar was
discovered in mid March 2015 after reading about GPS (Newell et. al.). 
Attempts to shoe-horn Soar/SML 9.4.0 into a small scale application led to an
abrupt halt and a temporary change in priorities. Having read "The Soar
Cognitive Architecture" (Laird, MIT) at this point, Brig proceeded to take 
apart the Soar repo on github and find the essential bits. In early Novenber 
of 2015 this ultimately led to a stripped down version of Soar 8.5.2 that 
stepped through 1.5+ million decision cycles without a crash and numerous ideas
about what to do next...

What began as an effort to cleanup an eleven year old source tree turned into 
a project to fork Soar 8.5.2, remove the old GUI system, audit and reorganize
the source tree, build a new cli, and add a test/debug harness with modern unit
testing techniques. 

###############################################################################
Future History:

In the past, Soar has been tied to some language and its idiosyncracies.
 
That will still be true in a sense but sioo will be a free standing command
processor just like python, octave, or sage. It will be distributable as a
single file static binary and "just work" on GNU/Linux. A flexible build
system will allow stable sets of options for a variety of uses from 
education to research to opensource engineering projects. This is to 
ensure availability and enhance stability, consistency and verification
of testing results. An Optimized and profiled build will give maximum
performance while ensuring verifiable stability.

SiOO will allow the user to load, edit, run, observe and script sp{-->} 
based agents. Some formatted GUI output will be added to latter versions 
including debugging productions via state watches with macros, breaks under 
modalized stepping control of the decision cycle and using the built-in 
callbacks, traces and backtraces.

We consider C to be a lengua franca of systems programming and the GNU gcc 
compiler is actively maintained and being adapted to new systems and 
architectures under the supervision of paid subject matter experts.

gcc binaries with source are available to anyone, anywhere on Earth; this
includes the entire GNU toolchain, a massive, mature (stable, heavily used)
collection of libraries and the GNU/Linux OS to run it on. The GNU/Linux
OS runs on many kinds of hardware and gcc targets essentially all of them.
Removing the conditional idiosyncracies of other systems will lead to 
greater stability, readability, (quo maintainability) and allow full use 
of the gcc compiler collection's extensive features and tool sets... 

   ...No effort will be made to accomadate non-GNU non-GCC systems

*******************************************************************************

For now our focus is all about stabilizing the 8.5.2 build, however the reality
is that Soar and SiOO will diverge in philosophy rapidly. 

Soar 9.5.0 has many powerful features for simulating human cognition including
the new smem and epmem systems and the improving svs system. The advantage for
us with Soar 8.5.2 is that we are not seeking to model human cognition, we are
seeking to create a new cogniton entirely -- as a programming language.

...suddenly all the weird gsl and octave rhs functions make sense, right?

...more as we go along, for now we get a revived Soar 8.5 with CLI examples

SiOO 9.9.2 will be our "stable release" of our version of essentially the 
soar 8.5.2 kernel with a *nix style interactive command interface, 
documentation (man pages), CLI demos with .soar knowledge files, and
a native C API with extern calls to allow interfacing to any language that 
can load and call a C library. 

At this point, our filename extension of .sioo will clarify differences 
though SiOO should be able to pull in soar productions from .soar files for
some time.

9.9.3 SiOO will have a larger number of installed rhs functions from its 
first version including debug functions, math functions and an embryonic 
plugin system. SiOO knowledge code that uses these features should immediately
adopt the .sioo filename extension because they obviously won't run on a 
regular soar build.

9.9.4 SiOO will be the first "stable" version of SiOO with the new features

9.9.5 SiOO will be addressing changes to the rete-net code and parser.

9.9.7 SiOO will be a full audit of the kernel with changes as needed.

As time and needs arise tools will be added to the system as it progresses.


  --*BY 03-DEC-2015
  --*BY 31-JAN-2016 (Updated)
###############################################################################
SOAR Versions (soar)
###############################################################################

(From: Soar 8 tutorial:)
<quote>
Soar 8 is based on changes to Soar 7 suggested by Bob Wray’s
thesis work, but also includes changes to the decision cycle
suggested by Randy Jones, and refined by Bob Wray, Karen Coulter,
and Mike van Lent. Karen Coulter brought all of the pieces of 
Soar 7 and Soar 8 together and is responsible for the final 
integration and release of the Soar 8 code
</quote>

<p>
In other words - if this works for you, you have many people to thank.
However! If this package doesn't work for you - Karen alone is to blame. (lol)
</p>


###############################################################################
GPS, Laird, Newell, Rosenbloom
###############################################################################

(From: CA 1990: Unified Theories of Cognition:)
<quote>
Special Aknowledgements
John Laird - Paul Rosenbloom - Kathleen Carley - Bonnie John - Rex Flynn
Thad Polk - Olin Shivers - David Steier - Gregg Yost
</quote>


###############################################################################
Citations, Quotes from older papers 
running back to church, turing, lovelace, crito...
###############################################################################


