
#ifndef __JOBMANAGER_H
#define __JOBMANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <ctype.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>

#include "common.h"
#include "List.h"
#include "Object.h"

#define CHECK_JOBS 0
#define KILL_JOBS 1
#define CLEAN_JOBS 2

/* Prototypes of public method */

/**
 * Cleans finished jobs from a <code>List</code>
 * 
 * @param L a pointer to a <code>List</code>
 */
void cleanJobs(ListPtr L);

/**
 * Kills all currently running jobs in a <code>List</code> and cleans
 * up zombies.
 * 
 * @param L a pointer to a <code>List</code>
 */
void killJobs(ListPtr L);

/**
 * Checks a <code>List</code> of jobs to see if any have
 * finished and reports them back to the user if so.
 * 
 * @param L a pointer to a <code>List</code>
 */
void checkJobs(ListPtr L);

/**
 * Increases and returns the job id container
 */
void incrJobId();

/**
 * Gets the current job id level
 * 
 * @return the job id
 */
int getJobId();

#endif /* __JOBMANAGER_H */
