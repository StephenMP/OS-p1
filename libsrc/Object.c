#include "Object.h"

ObjectPtr createObject(const int jobId, const int procId, const char *command)
{
	ObjectPtr newObject = (ObjectPtr)malloc(sizeof(Object));
	newObject->jobId = jobId;
	newObject->procId = procId;
	newObject->done = 0;
	newObject->status = -1;
	newObject->command = (char*)malloc(sizeof(char) * (strlen(command) + 1));
	strcpy(newObject->command, command);
	return newObject;
}

int compareTo(const void *obj, const void *other)
{
	ObjectPtr o1 = (ObjectPtr)obj;
	ObjectPtr o2 = (ObjectPtr)other;
	return o1->procId == o2->procId;
}

char *toString(const void *obj)
{
	char* status;
	ObjectPtr myobj = (ObjectPtr)obj;

	if (myobj->done == TRUE)
		status = "Done";
	else
		status = "Running";

	char *temp;
	temp = (char*)malloc(sizeof(char) * strlen(myobj->command) + 1 + MAXPID_DIGITS + 4);
	sprintf(temp, "[%d] %s \'%s\'", myobj->jobId, status, myobj->command);
	return temp;
}

void freeObject(const void *obj)
{
	ObjectPtr myobj = (ObjectPtr)obj;
	if (myobj == NULL) return;
	free(myobj->command);
	free(myobj);
}
