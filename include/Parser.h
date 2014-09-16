
#ifndef __PARSER_H
#define __PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <ctype.h>
#include <errno.h>
#include "common.h"

/* Prototypes of public method */

/**
 * Method to parse the user input command. This method
 * directly manipulates the arguments passed, so be
 * sure to create copies of passed data prior to
 * call.
 * 
 * @param L a pointer to the string created by readline
 * @param A an initialized char* array
 */
void parseCommand(char *L, char **A);

/**
 * This function checks to see if a string contains
 * the '&' char or not. If so it will remove it. This
 * function manipulates the provided string, so be
 * sure to create copies of the passed data prior to
 * call if necessary. Marks the end of the string
 * with a NULL terminator.
 * 
 * @param S a pointer to the string created by readline
 * @return a <code>Boolean</code> TRUE if & exists; FALSE otherwise
 */
int parseAmp(char *S);

/**
 * Checks to see if the passed string is only whitespace.
 * This function does not operate on the string.
 * 
 * @param S a pointer to the string created by readline
 * @return a <code>Boolean</code> TRUE if only whitespace; FALSE otherwise
 */
int parseEmptyLine(const char *S);

/**
 * This funciton utulizes the getopt() function
 * to process valid options a user may have passed in
 * 
 * @param C the <code>argc</code> passed to main
 * @param A the <code>char *arg[]</code> passed to main
 */
void parseOptions(int C, char *A[]);

#endif /* __PARSER_H */
