/*
OPERATING SYSTEMS PROGRAMMING
CASE STUDY B
DIVYAM KSHATRIYA
19231378
*/

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<errno.h>

#define MAX_LINE 4096
#define MAX_WORDS MAX_LINE/2

void tokenize(char *line, char **words, int *nwords);
/* break line into words separated by whitespace, placing them in the 
   array words, and setting the count to nwords */

void find_and_execute();
/*
create new process usig fork() and then
use execvp() to run the program	
*/

/* declaring the variables */
char line[MAX_LINE]	, *words[MAX_WORDS], message[MAX_LINE], cwd[1024];
int stop=0,nwords=0,i,status;

int main()
{
	/* run the loop till exit is triggered */
    while(1)
    {
		printf("OSP CLI $ ");

		/* fgets(array of char, maximum num of char, stdin for standard input) */
		fgets(line,MAX_LINE,stdin);

		/* tokenize the entered input into array of words seperated by whitespace */
		tokenize(line,words,&nwords);

		if (words[0] != NULL)
		{	
			/* Comparing the input to exit */
			if (strcmp(words[0], "exit") == 0)
				break;
			
			/* Comparing the input to cd */
			else if (strcmp(words[0], "cd") == 0)
			{
				/* getcwd(buffer,size) is used to get the cwd and store it in cwd[]*/
				getcwd(cwd,1024);
				printf("Current directory: %s\n",cwd);

				/* chdir(path) is used here to change direction */
				/* It returns 0 on success and -1 on error */
				i = chdir(words[1]);
				
				/* if chdir() returns -1 then print error */
				if (i < 0)
					perror("Error");
				/* Print new directory on success */
				else
					printf("Changing directory to: \"%s\"\n",words[1]);
			}
			else
			{
				/* calling the find_and_execute() function */
				find_and_execute();	
			}
		}

	}
	
	/* if loop breaks then print "exiting..." to indicate to user */
	printf("Exiting...\n");
    return 0;
}

void tokenize(char *line, char **words, int *nwords)
{
  *nwords=1; /* initialising wordcount */

  	/*for loop uses strtok() fnctn to tokenise lines,
  	Firstly strtok() is initialized 
  	then no. of words are checked and then the next word 
  	is tokenised seperated by whitespace*/
	for(words[0]=strtok(line," \t\n");
	  	(*nwords<MAX_WORDS)&&(words[*nwords]=strtok(NULL, " \t\n"));
	  	*nwords=*nwords+1
	  	);
	return;
}

void find_and_execute()
{
	/* creating a child process */
	pid_t child; 					
	
	switch(child=fork())
	{
		case -1:
			/* incase of error, print error */
			perror("fork");
			exit(1);	
			break;
		case 0:
			// printf("I am the child my PID is: %d.\n",getpid());

			/* exec() only returns in -1 incase of error */
			i = execvp(words[0],words);
			/* in case of error, print error*/
			if (i == -1)
				perror("Error");
			exit(0);
			break;
		default:
			// printf("I am the parent, my PID is: %d.\n, ",getpid());

			waitpid(child, &status, 0); /* waiting for child process */
			printf("%s exited with status %d\n",line, WEXITSTATUS(status)); /* print exit status */
			break;
	}
}