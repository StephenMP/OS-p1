#include "../include/JobManager.h"

static int jobId = 0;

static void processJob(ListPtr list, int process)
{
	NodePtr node = getHead(list);
	NodePtr temp;
	ObjectPtr obj;
	int pid;

	/* Setup list iteration */
	while (node != NULL) {
		temp = node;
		node = node->next;
		obj = ((ObjectPtr)temp->obj);

		/* Perform job operation based on indicated process */
		switch (process) {
		case CLEAN_JOBS:
			if (obj->done) {
				/* Remove and free the node */
				temp = removeNode(list, temp);
				freeNode(temp, freeObject);
			}

			/* Reset the jobId if necessary */
			if (getSize(list) == 0)
				jobId = 0;
			break;

		case KILL_JOBS:
			/* Send all jobs a kill signal */
			kill(obj->procId, SIGKILL);
			break;

		case CHECK_JOBS:
			/* Report finished jobs */
			if (obj->done) {
				char *output = toString(obj);
				printf("%s\n", output);
				free(output);
			}
			break;
		}
	}

	/* Just formality, will call free(NULL) with no affect */
	if (process == CLEAN_JOBS) freeNode(node, freeObject);

	/* Make sure to clean up any zombies */
	else if (process == KILL_JOBS) {
		while (!isEmpty(list)) {
			pid = waitpid(-1, NULL, WNOHANG);
			if (pid > 0)
				cleanJobs(list);
			else if (pid == 0)
				sleep(1);
			else
				break;
		}
	}
}

void cleanJobs(ListPtr list)
{
	if (getSize(list) > 0)
		processJob(list, CLEAN_JOBS);
}

void killJobs(ListPtr list)
{
	if (getSize(list) > 0)
		processJob(list, KILL_JOBS);
}

void checkJobs(ListPtr list)
{
	if (getSize(list) > 0)
		processJob(list, CHECK_JOBS);

	cleanJobs(list);
}

void incrJobId()
{
	++jobId;
}

int getJobId()
{
	return jobId;
}
