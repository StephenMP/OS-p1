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

void execute(char **a, char *c, Boolean b);
void changeDirectory(char* p);
int initShell(void);

static ListPtr list;

/**
 * Signal handler to monitor children and
 * zombies. When a child exits, it is killed
 *
 * @param S the signal #
 */
void zombieHunter(int S)
{
	ObjectPtr job;
	NodePtr node;
	pid_t cpid;
	int status;

	/* Wait on the child to change */
	if((cpid = waitpid(-1, &status, WNOHANG)) > 0) {
		/* Get a handle into the start of the job list */
		node = getHead(list);

		/* Loop our list looking for the done job(s) */
		while(node != NULL) {
			job = ((ObjectPtr)node->obj);

			/* Update the job once found */
			if(job->procId == cpid) {
				job->done = TRUE;
				job->status = status;
				kill(cpid, SIGKILL);
				break;
			}

			/* Iterate */
			node = node->next;
		}
	}
}

/**
 * Main insertion
 */
int main(int argc, char *args[])
{
	/* Process any command line options, may exit */
	parseOptions(argc, args);

	/* Initialize our job list */
	list = createList(compareTo, toString, freeObject);

	/* Initialize the history library */
	using_history();

	/* Setup our signal to manage zombies */
	signal(SIGCHLD, zombieHunter);

	/* 'Initialize' our shell, exit off shell break */
	return initShell();
}

/**
 * Function to handle the execution of a task.
 *
 * @param argv the parsed char** manipulated by parseCommand
 * @param command a copy of the the command input
 * @param isBackground a <code>Boolean</code> True if '&' was entered; false otherwise
 */
void execute(char **argv, char *command, Boolean isBackground)
{
	/* Fork our child */
	pid_t pid = fork();

	/* Hook to track status */
	int status = 0;

	/* Check attempt to fork a child proc */
	if(pid < 0)
		err_sys("Error forking");

	/* We are the child */
	if(pid == 0) {
		/* Have the child execute the command */
		int result = execvp(*argv, argv);

		/* Handle any forking errors */
		if(result < 0)
			err_sys("Exec failed");
	}
	/* We are the parent */
	else {
		/* Handle a background job */
		if(isBackground) {
			incrJobId();

			/* Inform user of job creation */
			printf("[%d] %d \"%s\"\n", getJobId(), (int)pid, command);

			/* Add job to our job list */
			addAtRear(list, createNode(createObject(getJobId(), (int)pid, command)));
		}
		/* Otherwise, we'll just wait for job to execute */
		else
			while(wait(&status) != pid) ;
	}
}

/**
 * Function to change directories to a specified path.
 * If cd fails, an error is printed, errnor is written
 * to, user is promopted, and the shell continues.
 *
 * @param path a string representing the path entered
 */
void changeDirectory(char* path)
{
	struct passwd *pwd;

	/* If just cd, go to home directory */
	if(path == NULL || parseEmptyLine(path)) {
		pwd = getpwuid(getuid());
		path = pwd->pw_dir;
	}

	/* Otherwise, change to specified directory */
	if(chdir(path) != CHDIR_SUCCESS)
		err_ret("Error changing directory");
}

/**
 * The bread and butter, the shell code. What else to
 * say? It drives the shell!
 *
 * @return <code>int</code> exit code. 0 on sucess
 */
int initShell()
{
	/* Need to allow 2048 args */
	char *argv[2048];
	char *line;
	char *command;
	Boolean backgroundJob = FALSE;
	Boolean processedJob = FALSE;

	/* Get the prompt for our shell */
	char *prompt = getenv("DASH_PROMPT");
	if(prompt == NULL)
		prompt = "mydash> ";

	/* Collect input */
	while((line = readline(prompt))) {
		processedJob = TRUE;
		command = (char*)malloc(sizeof(char) * (strlen(line) + 1));
		strcpy(command, line);

		backgroundJob = parseAmp(line);

		/* Handle if user just pressed enter or input just whitespace */
		if(parseEmptyLine(line) == TRUE) {
			free(line);
			free(command);
			checkJobs(list);
			continue;
		}

		/* Insert line into the history for recall */
		add_history(command);

		/* Parse the user input */
		parseCommand(line, argv);

		/* Handle exit */
		if(strcmp(argv[0], "exit") == EQUAL) break;

		/* Handle cd */
		if(strcmp(argv[0], "cd") == EQUAL) {
			changeDirectory(argv[1]);
			checkJobs(list);
			free(command);
			free(line);
			continue;
		}

		/* Handle jobs command */
		if(strcmp(argv[0], "jobs") == EQUAL) {
			printJobs(list);
			cleanJobs(list);
			free(command);
			free(line);
			continue;
		}

		/* Execute a job */
		execute(argv, command, backgroundJob);
		checkJobs(list);

		/* Keep it clean */
		processedJob = FALSE;
		free(command);
		free(line);
	}

	/* Free blocks in case we exit loop */
	if(!isEmpty(list))
		killJobs(list);

    /* Free our job list */
	freeList(list);

	/* We only need to free these if we started a job but didn't finish */
	if(processedJob) {
		free(command);
		free(line);
	}
	/* We got EOF, raise SIGABRT to force core dump */
	else{
        raise(SIGABRT);
	}
	return 0;
}
