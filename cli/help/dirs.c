
/* print command synopsis for: help dirs*/
int cmd_help_[COMMAND_NAME](int argc, char ** argv, soarResult * res)
{
print("\
NAME\n\
dirs - List the directory stack used by pushd and popd\n\
\n\
SYNOPSIS\n\
dirs\n\
\n\n");
return 0;
}

