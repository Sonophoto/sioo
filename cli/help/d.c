
/* print command synopsis for: help d */
int cmd_help_d(int argc, char ** argv, soarResult * res)
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
\n\n");
return 0;
}



