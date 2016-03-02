#include <errno.h> /* errno -- libc*/
extern int errno;
char * const sys_errlist[];
int sys_nerr;

#include <unistd.h> /* getcwd() -- libc*/
char* getcwd(char *buffer, size_t size );
char* getwd(char *path_name);

#include <dirent.h> /*libc*/
int chdir(const char *path );

#include <stdlib.h>
char* realpath(const char *pathname, char *resolved_name );

#define SUCCESS 0

/* if you are using a path longer than 256 that is bizzare. */
#define MAX_PATH_LENGTH (256 - 1) 

static char *PREV[MAX_PATH_LENGTH]
static char *HOME[MAX_PATH_LENGTH]


/* SETUP to change directory */
/*
	Strip leading, doubled and trailing spaces
	Sanitize path string
	(valid path) ?  s_PreviousPath = $(PWD) : abort();
	getenv(HOME)
	getcwd();
*/

int cd_func(char *in_path)
    {

/* First we check for several special cases: */
if (strcmp(in_path, "")
	{
	cd_status = chdir((const char) HOME)
        cd_status ? abort : return SUCCESS;
	}
else if (strcmp(in_path, "~")
	{
	chdir((const char) HOME);
	return SUCCESS;
	}
else if (strcmp(in_path, ".")
	{
	return SUCCESS;
	}
else if (strcmp(in_path, "-")
	{
	chdir((const char) PREV);
	return SUCCESS;
	}


/*
The realpath() function resolves all symbolic links, extra slash (/) 
characters and references to /./ and /../ in pathname, and copies the
resulting absolute pathname into the memory referenced by resolved_name.

The chdir() function changes the current working directory to path,
which can be relative to the current working directory or an 
absolute path name.

Now we know there were no exceptions and we have a realpath() 
	chdir(realpath())
	free(everything)
	return 0 on success, -1 on failure
*/

} /* END cd_func() */

