

#ifndef __OBJECT_H
#define __OBJECT_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "common.h"

#define MAXPID_DIGITS 20

/* Object definitions */
typedef struct object  Object;
typedef struct object *ObjectPtr;

/**
 * Struct to manage a job. It tracks a job's
 * job id, process id, status, original command,
 * and a boolean to indicate done or not.
 */
struct object {
	int jobId;
	int procId;
	int done;
	int status;
	char *command;
};

/**
 * The constructor. Creates and returns a job <code>object</code>
 * 
 * @param J the job id
 * @param P the process id
 * @param C the original command
 */
ObjectPtr createObject (const int J, const int P, const char *C);

/**
 * The toString to print a job object
 * 
 * @param O the <code>object</code> to print
 */
char *toString(const void *O);

/**
 * Frees a job object
 * 
 * @param O the <code>object</code> to free
 */
void freeObject(const void *O);

/**
 * Compares two objects to determine if they equal or
 * not.
 * 
 * @param OA the first <code>object</code>
 * @param OB the second <code>object</code>
 * @return 0 if equal, false otherwise
 */
int compareTo(const void *OA,const void *OB);


#endif /* __OBJECT_H */
