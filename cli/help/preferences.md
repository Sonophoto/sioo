## NAME

preferences - Print slot preferences 

## SYNOPSIS

**preferences** [_id_] [[^]_attribute_] [ _0 | 1 | 2 | 3_ ] **preferences** [_id_] [[^]_attribute_] [ _-none | -names | -timetags | -wmes_ ] 

* * *

## DESCRIPTION

This command prints all the preferences for the given slot (i.e., object and attribute). The _id_ and _attribute_ indicate the slot of interest. If they are not specified, they default to the current state and the current operator. The '^' is optional when specifying the attribute. The optional arguments indicates the level of detail to print about each preference.

## OPTIONS

The optional _detail_ must have one of the following values: 

**-none** or **0**
    Print just the preferences themselves

**-names** or **1**
    Print the preferences and the names of the productions that generated them

**-timetags** or **2**
    Print the information for the **-names** option above plus the timetags of the wmes matched by the indicated productions

**-wmes** or **3**
    Print the information for the **-timetags** option above plus the entire wme.

If no _detail_ is given, then the **-none** option is used.

## EXAMPLES

This example prints the preferences on the **S1 operator** slot and the production names which created the preferences: 
    
    
    **preferences S1 operator -names**
