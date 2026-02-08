## NAME

io - Manage attachment of scripts to Soar I/O 

## SYNOPSIS

**io** _action_

* * *

## DESCRIPTION

This command manages the attachment of scripts to Soar I/O. Soar has an event defined for input and another for output. Scripts can be attached to these Soar events so that they are invoked every time the Soar event occurs. These scripts can later be removed using the **io** command. The status of attachments can be listed also. If a new attachment is created, its identifier is returned as the result of this command.

The specified _action_ must have one of the following forms:

**-add** [**-input** | **-output**] _script [id]_
    Add the _script_ to the list of scripts to be invoked when the **-input** or **output** event occurs in the Soar kernel. If the _id_ is given, it is used to name the attachment. If no _id_ is given, then a unique id is generated and returned. The id is necessary if a user ever wants to delete an action.

**-delete** [**-input** | **-output**] _id_
    Remove script attachment named _id_.

**-list** [**-input** | **-output**]
    List all attachments of scripts to Soar input or output.

## EXAMPLES

A detailed example of using Soar I/O in Tcl is given in the file **demos/soar-io-using-tcl.tcl**.
