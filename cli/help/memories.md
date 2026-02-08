## NAME

memories - Print partial match memory usage 

## SYNOPSIS

**memories** [_production-name_ | _count_ | **-chunk** | **-user** | **-default** | **-justification**] 

* * *

## DESCRIPTION

This command prints information about memory use, in tokens, of partial matches of productions. With no options, memory use of all productions is printed. Other options indicate a subset of the productions.

## OPTIONS

If given, an option must match one of the following forms: 

production-name
    print memory usage of the production named _production-name_

count
    for all specified groups (chunk, default, user, and/or justification) print usage information on only _count_ of each.

-chunk
    print memory usage of chunks

-user
    print memory usage of user productions

-default
    print memory usage of the default productions

-justification
    print memory usage of justifications
