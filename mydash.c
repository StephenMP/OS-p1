#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "include/ourhdr.h"
#include "include/common.h"
#include "include/List.h"
#include "include/Node.h"
#include "include/Object.h"
#include "include/Parser.h"
#include "include/JobManager.h"

static ListPtr list;

/**
 * Main insertion
 */
int main(int argc, char *args[])
{
	return initShell();
}

void execute(char **argv, char *command, Boolean isBackground)
{

}

void changeDirectory(char* path)
{

}

int initShell()
{
	char    *argv[2048];
	char    *line;
	char    *command;
	Boolean backgroundJob = FALSE;

	/* Get the prompt for our shell */
	char *prompt = getenv("DASH_PROMPT");
	if (prompt == NULL)
		prompt = "mydash> ";

	while ((line = readline(prompt))) {
		command = (char*)malloc(sizeof(char) * (strlen(line) + 1));
		strcpy(command, line);


		/* Keep it clean */
		free(command);
		free(line);
	}

	freeList(list);
	free(command);
	free(line);
	return 0;
}
