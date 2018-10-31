/*
 * shell.c - A Simple Shell
 *
 * CSCI503(Fall 2017) Operating Systems: Lab 2
 * Submitted by: Ayush Maharjan
 */
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

#define BUFFER_SIZE 3000
#define TOKEN_BUFFER_SIZE 1000
#define TOKEN_SIZE 512
#define COMMAND_TOKEN_SIZE 20

char *readLine();
char **getTokens(char *input);
int validateTokens(char **tokens);
void processTokens(char **tokens);
void executecommands(char **command, char **tokens);

/*
 * Function: main
 * ---------------------------
 * Asks user to input commands, parses it, validates it, and executes it
 */
int main() {
	char *input;
	char **tokens;

	while (TRUE) {
		printf("shell>");

		// Read user input
		if ((input = readLine()) == NULL) {
			return 0;
		}

		// Parse Command
		tokens = getTokens(input);

		// Check validity and execute commands
		if (validateTokens(tokens)) {
			processTokens(tokens);
		}
	}
}

/*
 * Function: readLine
 * ---------------------------
 * Reads user input from the standard input in a buffer and returns it as a string
 *
 * returns: User input as a character array (string)
 */
char *readLine() {
	int position = 0;
	int inputcharacter;

	char *buffer = (char *) malloc(sizeof(char) * BUFFER_SIZE);
	if (NULL == buffer) {
		printf("Not enough memory \n");
		return NULL;
	}

	while (TRUE) {
		inputcharacter = getchar();

		if (inputcharacter == EOF) {
			return NULL;
		} else if (inputcharacter == '\n') {
			buffer[position] = '\0';
			return buffer;
		}

		buffer[position] = inputcharacter;

		position++;

		if (position > BUFFER_SIZE) {
			printf("Not enough memory\n");
			return NULL;
		}
	}
}

/*
 * Function: getTokens
 * ---------------------------
 * Breaks the user input into tokens according to the commands line grammar.
 *
 * input: Character array of input
 * returns: Array of tokens
 */
char **getTokens(char *input) {
	char **tokenlist;
	char *token;
	char *ptr;
	int i = 0;

	// Add check
	tokenlist = (char **) malloc(sizeof(char *) * TOKEN_BUFFER_SIZE);
	if (NULL == tokenlist) {
		printf("Not enough memory \n");
		return NULL;
	}

	while (*input != '\0') {
		// Add check
		token = (char *) malloc(sizeof(char) * TOKEN_SIZE);
		if (NULL == token) {
			printf("Not enough memory\n");
			return NULL;
		}

		ptr = token;

		/* Ignore the white spaces */
		while (*input == ' ') {
			*input++;
		}

		*ptr++ = *input;

		if (*input != '>' &&
		    *input != '<' &&
		    *input != '&' &&
		    *input != '|') {

			*input++;
			while (*input != '>' &&
			    *input != '<' &&
			    *input != '&' &&
			    *input != '|' &&
			    *input != ' ' &&
			    *input != '\0') {
				*ptr++ = *input++;
			}
		} else {
			*input++;
		}

		*ptr = '\0';

		if (*token != '\0') {
			tokenlist[i++] = token;
		}
	}

	tokenlist[i] = NULL;

	return tokenlist;
}

/*
 * Function: validateTokens
 * ---------------------------
 * Validates the tokens according to the commands line grammar.
 *
 * tokens: Array of tokens
 * returns: returns TRUE (1) if valid, and FALSE (0) if invalid
 */
int validateTokens(char **tokens) {
	char **ptr = tokens;
	int expectedcommand = TRUE;
	int expectedfile = FALSE;
	int firstcommand = TRUE;
	int ret;

	while (*ptr != NULL) {
		if (strcmp(*ptr, ">") == 0) {
			if (expectedfile || expectedcommand) {
				printf("Unexpected token: '%s'\n", *ptr);
				return FALSE;
			}

			*ptr++;

			if (*ptr == NULL) {
				printf("expected output file after '>'\n");
				return FALSE;
			}

			if (strcmp(*ptr, "&") == 0 || strcmp(*ptr, ">") == 0 ||
			    strcmp(*ptr, "<") == 0 || strcmp(*ptr, "|") == 0) {
					printf("Unexpected token: '%s'\n", *ptr);
					return FALSE;
			}

			if (ptr[1] != NULL) {
				if (strcmp(ptr[1], "&") != 0) {
					printf("Unexpected token: '%s'\n", ptr[1]);
					return FALSE;
				}
			}

			expectedfile = TRUE;
		} else if (strcmp(*ptr, "<") == 0) {
			if (expectedfile || expectedcommand || !firstcommand) {
				printf("Unexpected token: '%s'\n", *ptr);
				return FALSE;
			}

			*ptr++;

			if (*ptr == NULL) {
				printf("expected input file after '<'\n");
				return FALSE;
			}

			if (strcmp(*ptr, "&") == 0 || strcmp(*ptr, ">") == 0 ||
			    strcmp(*ptr, "<") == 0 || strcmp(*ptr, "|") == 0) {
					printf("Unexpected token: '%s'\n", *ptr);
					return FALSE;
			}

			if (ptr[1] != NULL) {
				if (strcmp(ptr[1], "&") != 0 && strcmp(ptr[1], "|") != 0 && strcmp(ptr[1], ">") != 0) {
					printf("Unexpected token: '%s'\n", ptr[1]);
					return FALSE;
				}
			}

			expectedfile = TRUE;
		} else if (strcmp(*ptr, "|") == 0) {
			if (expectedfile || expectedcommand) {
				printf("Unexpected token: '%s'\n", *ptr);
				firstcommand = FALSE;
				return FALSE;
			}

			*ptr++;

			expectedcommand = TRUE;
		} else if (strcmp(*ptr, "&") == 0) {
			if (expectedcommand) {
				printf("Unexpected token: '%s'\n", *ptr);
				return FALSE;
			}
			*ptr++;
			if (*ptr != NULL) {
				printf("Unexpected token: '%s'\n", *ptr);
				return FALSE;
			}
		} else {
			if (expectedfile) {
				// Check if file is valid
				expectedfile = FALSE;
			} else if (expectedcommand) {
				ret = execvp(*ptr, *ptr);
				if (errno == ENOENT) {
					printf("%s: command not found\n", *ptr);
					return FALSE;
				}
				expectedcommand = FALSE;
			}
			*ptr++;
		}
	}

	return TRUE;
}

/*
 * Function: processTokens
 * ---------------------------
 * Processes and executes the tokens. It finds the first commands, input file,
 * output file, and whether the process is a background process. If input and
 * output
 *
 * tokens: Array of tokens
 * returns: returns TRUE (1) if valid, and FALSE (0) if invalid
 */
void processTokens(char **tokens) {
	// Find first command
	char **command = (char **) malloc(sizeof(char **) * COMMAND_TOKEN_SIZE);
	if (NULL == command) {
		printf("Not enough memory\n");
		return;
	}

	char **ptr = command;
	char *inputfile = NULL;
	char *outputfile = NULL;
	int background = FALSE;
	int inputfd;
	int outputfd;
	int stdinfd;
	int stdoutfd;
	int exitstatus;
	pid_t pid;


	while (*tokens != NULL &&
	       	strcmp(*tokens, "<") != 0 &&
			strcmp(*tokens, ">") != 0 &&
			strcmp(*tokens, "&") != 0 &&
			strcmp(*tokens, "|") != 0) {
			*ptr++ = *tokens++;
		}
	*ptr++ = NULL;

	// Check if there are input and output files defined and handle accordingly
	char **temp = tokens;
	while (*temp != NULL) {
		if (strcmp(*temp, "<") == 0) {
			*temp++;
			inputfile = *temp;
		} else if (strcmp(*temp, ">") == 0) {
			*temp++;
			outputfile = *temp;
		} else if (strcmp(*temp++, "&") == 0) {
			background = TRUE;
		}
		*temp++;
	}

	// run the last command
	pid = fork();
	if (pid == 0) {
		if (inputfile != NULL) {
			inputfd = open(inputfile, O_RDONLY, 0666);

			if (inputfd < 0) {
				perror(inputfile);
				return;
			}

			if (dup2(inputfd, 0) < 0) {
				perror("dup2");
				return;
			}
		}

		if (outputfile != NULL) {
			outputfd = open(outputfile, O_WRONLY | O_CREAT | O_TRUNC, 0666);

			if (outputfd < 0) {
				perror(outputfile);
				return;
			}

			if (dup2(outputfd, 1) < 0) {
				perror("dup2");
				return;
			}
		}

		executecommands(command, tokens);
	} else if (pid < 0) {
		perror("fork");
		return;
	} else {
		if (background) {
			printf("[Process id %d]\n", pid);
			return;
		}

		wait(&exitstatus);

		if (WEXITSTATUS(exitstatus)) {
			return;
		}
	}
}

/*
 * Function: executecommands
 * ---------------------------
 * Executes the commands recursively until they are all executed
 *
 * command: Current command and its parameters
 * tokens: Remaining tokens to be executed
 */
void executecommands(char **command, char **tokens) {
	char **ptr;
	int haspipe = FALSE;
	int exitstatus;
	int pipefd[2];
	pid_t pid;
	char **nextcommand = (char **) malloc(sizeof(char **) * COMMAND_TOKEN_SIZE);
	if (NULL == nextcommand) {
		printf("Not enough memory\n");
	}

	if (*tokens != NULL) {
		if (strcmp(*tokens, "<") == 0) {
			*tokens++;
			*tokens++;
		}

		if (*tokens != NULL && strcmp(*tokens, "|") == 0) {
			haspipe = TRUE;
			*tokens++;
		}
	}


	if (haspipe) {
		ptr = nextcommand;

		while (*tokens != NULL &&
		       	strcmp(*tokens, "<") != 0 &&
				strcmp(*tokens, ">") != 0 &&
				strcmp(*tokens, "&") != 0 &&
				strcmp(*tokens, "|") != 0) {
			*ptr++ = *tokens++;
		}
		*ptr = NULL;

		if (pipe(pipefd) < 0) {
			perror("pipe");
			return;
		}

		pid = fork();

		if (pid == 0) {
			if (dup2(pipefd[0], 0) != 0) {
				perror("dup2");
				return;
			}
			close(pipefd[1]);
			close(pipefd[0]);

			executecommands(nextcommand, tokens);

		} else if (pid < 0) {
			perror("fork");
			return;
		} else {
			if (dup2(pipefd[1], 1) != 1) {
				perror("dup2");
				return;
			}
			close(pipefd[1]);
			close(pipefd[0]);

			if (execvp(command[0], command) == -1) {
	      		perror(command[0]);
	      		return;
	    	}

			wait(&exitstatus);

			if (WEXITSTATUS(exitstatus)) {
				return;
			}
		}
	} else {
		if (execvp(command[0], command) == -1) {
      		perror(command[0]);
      		return;
		}
	}

}