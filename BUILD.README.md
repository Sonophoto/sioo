#Notes on Building SiOO from Scratch

**Remember! There are prebuilt headers, libraries and executables!**

Look in the `[prefix]/sioo/out` directory. These are supposed to be
the latest stable versions, ready to use on GNU/Linux libc systems.

***

**_It is perfectly reasonable to want to make a custom build so read on:_**

###Requirements:

GNU Tool Chain including autotools, gcc, /bin/sh and the *nix userland.

We are using bash but your posix shell should work fine.

There is no porting or portability to MS Windows or Mac OSX. Sorry.

###Overview

The idea here is to use autotools for the actual building of source
trees but to use a bourne shell script for assembling and installing
the various pieces. The rationale is simply this: lots and lots of 
people can hack an sh script but few people can hack an autotools
system with any confidence.

The autotools can then be wrapped up and ignored by essentially
all users that just want to use a working production matcher in 
several different ways but the full power of autotools is under
the hood when needed by whomever.

###Default Build:

First, start a terminal, and change into the root of the project. We will refer to where you put the project directory as `[prefix]` with `[prefix]` being entirely at your discretion. The root of the project is then `[prefix]/sioo`. Go there.

To build the entire system with default settings:

The build is done by a /bin/sh script so we may need to do 

`chmod u+x make-SiOO`

Now we can say:

`./make-SiOO`

The build will progress rapidly and verbosely. The finished files 
are in the `[prefix]/sioo/out/` directory and can be run from there 
or moved wherever you wish to install them. 

In version 9.9.1 the demo programs **MUST** be run from `[prefix]/sioo/cli` 
or they will not function

We should have an automagical install target by 9.9.4

***

###System Build Notes!
For now, the make-SiOO(sh) shell script erases all intermediary files and all 
output files before beginning the build process. This is simply for testing 
sanity to ensure consistency between builds. Source trees that are controlled 
by autotools scripts are "make distclean". The source tree for the CLI is has
a distclean target but the Makefile is handmade not Autotools. Files moved or
created by make-SiOO(sh) are erased by the same.

***

###Kernel Build Notes!
Currently the build options are preset in sioo/kernel/soarBuildOptions.h
There is a custom tag #define AKA_SIOO that turns on the SiOO option set

If you need to do a custom configuration:

Remember, This is BLACK magic, you are "ON YOUR OWN" If you can't find the 
option in the source code and understand what it does, maybe you shouldn't.

`[prefix]/sioo/kernel/README.BuildOptions` (Human Readable, LateX markup)

`[prefix]/sioo/kernel/soarBuildOptions.h` (C header file)

`[prefix]/sioo/kernel/soarBuildInfo.c` (C source file)

