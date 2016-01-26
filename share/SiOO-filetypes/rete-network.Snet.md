#Work in Progress

The Soar 8.5.2 API can do this. The CLI interface can do this using
the `rete-net [-save|-load] [pathspec]` family of commands.

However, this filetype specifically identifies a **SiOO** rete net.

This is a binary file containing the output of the api call save_rete.
The intention is that an agent can be saved as a binary file and then
loaded later dynamically or statically built into a program file.

First examples will be the demos which will have the retenets for their
knowledge compiled into their executables. The executables can be created
as dynamic or static linked, with statics fully-linked single-image files.
