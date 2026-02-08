## NAME

quit - Quit Soar 

## SYNOPSIS

**quit**

* * *

## DESCRIPTION

This command quits Soar. It calls any registered Soar termination callbacks, closes any open log files, and exits the process. This also performs any Tcl related cleanup as would be done for the Tcl **exit** command. The Tcl **exit** command is redefined in Soar to use this command.
