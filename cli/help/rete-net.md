## NAME

rete-net - Save and restore the Rete net 

## SYNOPSIS

**rete-net** _{-save | -load} filename_

* * *

## DESCRIPTION

The **rete-net** command saves and restores the state of the Rete network. This command provides a fast method of saving and loading productions since a binary format is used. rete-net files are portable across platforms that support Soar. The action must be one of the following:

-save
    Save the Rete network in the given _filename_. The Rete network cannot be saved while there are justifications present. These can be eliminated by using the **init-soar** command.

-load
    Load the productions stored in _filename_ into the Rete network. In order to load productions stored in binary form, working memory and production memory must both be empty. Working memory can be emptied by using the **init-soar** command. Production memory can be emptied by using the **excise -all** command.

If the _filename_ contains a suffix of ".Z", then the file is compressed automatically when it is saved and uncompressed when it is loaded.

## WARNINGS

Compressed files may not be portable to another platform is that platform does not support the same uncompress utility. 

## SEE ALSO

**excise** , **init-soar**
