#Notes on Building SiOO from Scratch

**Remember! There are prebuilt headers, libraries and executables!**

Look in the `[prefix]/sioo/out` directory. These are supposed to be
the latest stable versions, ready to use on GNU/Linux libc systems.

***

**_It is perfectly reasonable to want to make a custom build so read on:_**

###Overview

The idea here is to use autotools for the actual building of source
trees but to use a bourne shell script for assembling and installing
the various pieces. The rationale is simply this: lots and lots of 
people can hack an sh script but few people can hack an autotools
system with any confidence.

The autotools can then be wrapped up and ignored by essentially
all users that just want to use a working production matcher in 
several different ways but the full power of autotools is under
the hood when needed by whoever.
So to build SiOO, (in a terminal) change into the root of the project 
and do:

`./make-SiOO`

Currently there are kernel build options that are being ignored,
those will be pulled into the upper level configuration. If you need
these they are accessed in the following files:

`[prefix]/sioo/kernel/README.BuildOptions` (Human Readable, LateX markup)

`[prefix]/sioo/kernel/soarBuildOptions.h` (C header file)

`[prefix]/sioo/kernel/soarBuildInfo.c` (C source file)

