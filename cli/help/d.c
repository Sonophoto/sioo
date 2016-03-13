
/* print command synopsis for: help d */
int sioo_cmd_d_help(int argc, const char** argv, soarResult* res)
{
print("\
NAME\n\
 d - Step SiOO [integer] decision cycles\n\
\n\
SYNOPSIS\n\
 d [integer]\n\
\n\
DESCRIPTION\n\
 Runs the kernel for [integer] decision cycles. If no [integer] is\n\
 given on the command line, 1 decision cycle is run.\n\
\n\
SEE ALSO\n\
 e, run\n\
\n\
LINK\n\
 d.xhtml\n\
\n");
return 0;
}

