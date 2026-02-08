## NAME

pwatch - Trace production firings and retractions 

## SYNOPSIS

**pwatch** [**-on** | **-off**] [_production-name_ *] 

* * *

## DESCRIPTION

This command enables and disables the tracing of production activity for individual productions. Both firings and retractions are printed. If no _production-name_ s are given, this command lists the productions currently being traced. If one or more _production-name_ s are given, tracing is enabled (or disabled if **-off** is specified) for those productions. Tracing persists until disabled or until the production is **excise** d.

This command is orthogonal to the **watch productions** mechanism. See the **watch** command for more information.

## SEE ALSO

**excise** , **watch**
