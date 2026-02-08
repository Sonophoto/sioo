## NAME

excise - Remove productions from Soar's memory 

## SYNOPSIS

**excise** _production-name_ | **-chunks** | **-default** | **-task** | **-user** | **-all**

* * *

## DESCRIPTION

This command removes productions from Soar's memory. Each option given on the command line may be either a production name or the type of the standard set of productions (indicated by a keyword).

## OPTIONS

production-name
    Remove the production named _production-name_ from memory

-chunks
    Remove all chunks (learned productions) and justifications from memory

-default
    Remove all default rules (:default) from memory

-task
    Remove all non-default productions from memory and perform an **init-soar** command

-user
    Remove all user productions (but not chunks or default rules) from memory.

-all
    Remove all productions from memory and perform an **init-soar** command

## EXAMPLES

This removes the production my*first*production and all chunks:
    
    
    **excise my*first*production -chunks**
    

This removes all productions and does an init-soar:
    
    
    **excise -all**
    

## SEE ALSO

**init-soar**
