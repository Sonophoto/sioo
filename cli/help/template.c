
/* print command synopsis for: help [COMMAND_NAME] */
int cmd_help_[COMMAND_NAME](int argc, char ** argv, soarResult * res)
{
print("\
NAME\n\
[COMMAND_NAME] - [COMMAND_DESC]\n\
\n\
SYNOPSIS\n\
[COMMAND_NAME] [COMMAND_OPTS]\n\
...\n\
[COMMAND_NAME] [COMMAND_OPTS_N]\n\
LINK\n\
[URL_INTO_HTML_DIR][COMMAND_NAME}.xhtml\n\
\n\n");
return 0;
}



