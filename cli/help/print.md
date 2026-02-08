## NAME

print - Print a Soar item from production memory or working memory 

## SYNOPSIS

**print** -stack [ **-state** | **-operator** ]*  
**print** [**-depth** _n_] [**-internal**] { _identifier | integer | pattern_}  
**print** [**-internal**] [**-name|-full**] { _prodname | -all | -chunks | -defaults | -justifications | -user_} 

* * *

## DESCRIPTION

The **print** command is used to print items from production memory or working memory. It can take several kinds of arguments. When printing items from working memory, the Soar objects are printed unless the **-internal** flag is used, in which case the wmes themselves are printed. The _item_ s to be printed may be one of the following:

production-name
    print the production named _production-name_

-all
    print the names of all productions currently loaded

-chunks
    print the names of all chunks currently loaded

-defaults
    print the names of all default productions currently loaded

-justifications
    print the names of all justifications currently loaded

-user
    print the names of all user productions currently loaded

identifier
    print the object _identifier_. _identifier_ must be a valid Soar symbol.

integer
    print the object in working memory with the timetag _integer_

pattern
    print the object whose working memory elements matching the given _pattern_. The pattern is interpreted by the Soar kernel so it must be surrounded by curly braces or double quotes to avoid being parsed by Tcl. A _pattern_ has the following form:
    
    
           **(_identifier**^_attribute value_**_** [**+**]**)**
    

The _pattern_ is surrounded by parentheses. The _identifier_ , _attribute_ , and _value_ must be valid Soar symbols or the wildcard symbol ***** which matches all occurences. The optional **+** symbol restricts pattern matches to acceptable preferences.

## OPTIONS

All options apply only to arguments which follow them on the command line.

**-depth** _n_
    This option overrides the default printing depth (see the **default-print-depth** command for more detail). Note that a **-depth 0** argument is meaningful only with accompanying integer and pattern arguments. It causes only the matching working memory elements to be printed, instead of all elements whose identifier is an identifier in one of the matching working memory elements.

-internal
    Specifies that items should be printed in their internal form. For productions, this means leaving conditions in their reordered (rete net) form. For working memory, this means printing the individual elements with their timetags, rather than the objects.

-full
    When printing productions, print the whole production. This is the default when printing a named production.

-name
    When printing productions, print only the name and not the whole production. This is the default when printing any category of productions, as opposed to a named production.

-stack
    Specifies that the Soar goal stack should be printed. By default this includes both states and operators. The stack listing can be restricted by adding the **-states** and **-operator** restrictions. Giving both options is equivalent to the default. The predefined alias 'ps' is the same as **print -stack**.

## EXAMPLES

print -internal {(s1 ^* v2)}
    Print the working memory elements (and their timetags) which have the identifier s1 as object and v2 as value.

print -stack
    Print the Soar stack which includes states and operators.

print -internal -full prodname
    Print the named production in its RETE form.

print -user
    Print the names of all user productions currently loaded.

## WARNINGS

The order of options and arguments to the **print** command do matter: options apply only to arguments which follow them. 

print -chunks -full
    prints only the names of chunks, while

print -full -chunks
    prints the entire chunk for all chunks.

## SEE ALSO

**default-wme-depth** , **predefined-aliases**
