
/* print command synopsis for: help dirs*/
int sioo_cmd_dirs_help(int argc, char ** argv, soarResult * res)
{
print("\
NAME\n\
 dirs - List the directory stack used by pushd and popd\n\
\n\
SYNOPSIS\n\
 dirs\n\
\n\
DESCRIPTION\n\
\n\
SEE_ALSO\n\
 pushd, popd, topd, botd\n\
\n\
LINK\n\
\n\
\n");
return 0;
}

