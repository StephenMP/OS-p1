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
	/* Fork our child */
	pid_t pid = fork();

	/* Hook to track status */
	int status = 0;

	/* Attempt to fork a child proc */
	if (pid < 0) {
		err_sys("Error forking");
	}

	/* We are the child */
	if (pid == 0) {
		/* Have the child execute the command */
		int result = execvp(*argv, argv);

		/* Handle any forking errors */
		if (result < 0)
			err_sys("Exec failed");
	}
	/* We are the parent */
	else{
		while (wait(&status) != pid) ;
	}
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

		/* Handle if user just pressed enter or input just whitespace */
		if (parseEmptyLine(line) == TRUE) {
			free(line);
			free(command);
			continue;
		}

		/* Insert line into the history for recall */
		add_history(command);

		/* Parse the user input */
		parseCommand(line, argv);

		/* Handle exit */
		if (strcmp(argv[0], "exit") == EQUAL) break;

		/* Handle cd */
		if (strcmp(argv[0], "cd") == EQUAL) {
			changeDirectory(argv[1]);
			free(command);
			free(line);
			continue;
		}

		/* Handle jobs command */
		if (strcmp(argv[0], "jobs") == EQUAL) {
			printf("Not yet supported");
			continue;
		}

		/* Execute a job */
		execute(argv, command, backgroundJob);

		/* Keep it clean */
		free(command);
		free(line);
	}

	freeList(list);
	free(command);
	free(line);
	return 0;
}
