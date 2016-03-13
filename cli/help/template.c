
/* print command synopsis for: sioo_cmd_[COMMAND_NAME]() */
/* our command name will be sioo_cmd_[COMMAND_NAME]_help() */
/* Keep this output well under 25 lines. */
/* If a LINK to a file:/// or http:// URL is used, a modern */
/* terminal should allow the user to click on the link and have */
/* it open in their browser... */
/*
 TODO: should be ...char ** argv, sioo_cmd_status_t * sioo_cmd_status); 
*/
int sioo_cmd_[COMMAND_NAME]_help(int argc, char ** argv, soarResult * res)
{
print("\
NAME\n\
 [COMMAND_NAME] - [COMMAND_DESC]\n\
\n\
SYNOPSIS\n\
 [COMMAND_NAME] [COMMAND_ARGS]\n\
 ...\n\
 [COMMAND_NAME] [COMMAND_ARGS_N]\n\
\n\
DESCRIPTION\n\
 [COMMAND_DESCRIPTION]\n\
\n\
SEE ALSO\n\
 [COMMAND_SEE_ALSO]\n\
\n\
LINK\n\
 [URL_INTO_HTML_DIR][COMMAND_NAME}.xhtml\n\
\n\n");
return 0;
}



