## NAME

log - Log a Soar agent session 

## SYNOPSIS

**log** _action_

* * *

## DESCRIPTION

This command records the prompts, commands, printing, and command results that are printed to stdout for a given agent. The _action_ must be one of the following:

**-new** _pathname_
    A log file named _pathname_ is created and logging is initiated.

**-existing** _pathname_
    An existing log file named _pathname_ is opened and logging is added to the end of the file.

**-add** _string_
    Add the given _string_ to the open log file

-query
    Returns **open** if logging is active or **closed** if logging is not active.

-off
    Close the open log file.

## EXAMPLES

This initiates logging and places the record in foo.log: 
    
    
    **log -new foo.log**
    

This appends log data to an existing foo.log file:
    
    
    **log -existing foo.log**
    

This terminates logging and closes the open log file:
    
    
    **log -off**
