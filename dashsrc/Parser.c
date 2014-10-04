#include "../include/Parser.h"

void parseCommand(char *line, char **argv)
{
	/* if not the end of line we have args to process */
	while (*line != '\0') {
		/* Consume whitespace until we reach an argument */
		while (*line == ' ' || *line == '\t' || *line == '\n') {
			*line++ = '\0';
		}

		/* Save the arg in it's proper position in argv */
		*argv++ = line;

		/* Continue until we find a space indicating future args */
		while (*line != '\0' && *line != ' ' && *line != '\t' && *line != '\n') {
			line++;
		}
	}

	/* Make sure we can find the end */
	*argv = '\0';
}

int parseEmptyLine(const char *s)
{
	while (*s != '\0') {
		if (!isspace(*s))
			return FALSE;
		s++;
	}
	return TRUE;
}

int parseAmp(char *s)
{
	while (*s != '\0') {
		if (*s == '&') {
			*s = '\0';
			s--;
			while (isspace(*s)) {
				*s = '\0';
				s--;
			}
			return TRUE;
		}
		s++;
	}
	return FALSE;
}

void parseOptions(int argc, char *args[])
{
	int option = 0;
	/* Handle any options input */
	if ((option = getopt(argc, args, "vh")) != -1) {
		switch (option) {
		case 'v':
			printf("Version: ");
			fflush(stdout);
			system("git rev-parse HEAD");
			exit(0);
		case 'h':
			printf("Usage: mydash [option]\nOptions:\n\t-v: Print version (git GUID)\n\t-h: Print usage\n");
			exit(0);
		default:
			printf("Please try again using the -h option for information\n");
			exit(1);
		}
	}
}
