#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/uio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "func.h"
#define readsize 2048
#define toksize 100
#define comsize 1024
#define tokdelim " \t\r\n\a"
#define comdelim ";"
char *bstr[]={"pwd", "echo", "cd"};
int (*bfunc[])(char **)={&pwd, &echo, &cd};
int kickstart(char **tok)
{
	int i, j, flag=0, l;
	for (i=0; tok[i]!= '\0'; i++)
	{
		char *b;
		b=tok[i];
		l=strlen(b);
		if(b[l-1]=='&')
		{
			flag=1;
			b[l-1]='\0';
		}
	}
	pid_t pid, wpid;
	int stat;
	/*signal(SIGCHLD, SIG_IGN);
	if(flag)
		signal(SIGCHLD, handler);*/
	pid=fork();
	if(pid==0)
	{
		if(execvp(tok[0],tok) == -1)
		{
			perror("~bash");
		}
		exit(1);
	}
	else if (pid<0)
	{
		perror("~bash");
	}
	else
	{
		if(flag==0)
		{
			do
			{
				wpid=waitpid(pid, &stat, WUNTRACED);
			} while(!WIFEXITED(stat) && !WIFSIGNALED(stat));
		}
		else
		{
			printf("[1] %d\n", pid);
		}
	}
	return 1;
}
int pwd(char **tok)
{
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));
	printf("%s\n",cwd);
	return 1;
}
int echo(char **tok)
{
	int i;
	for(i=1; tok[i]!='\0';i++)
	{
		int j=0;
		char *b;
		b=tok[i];
		while(b[j] != '\0')
		{
			if(b[j]=='\"')
			{
				printf("");
			}
			else
			{
				printf("%c", b[j]);
			}
			j++;
		}
		printf(" ");
		//printf("%s ", tok[i]);
	}
	printf("\n");
	return 1;
}
int cd(char **tok)
{
	if (tok[1]==NULL)
	{
		chdir("/Users/vighneshck");
	}
	else
	{
		if(chdir(tok[1]) != 0)
		{
			perror("~bash");
		}
	}
	return 1;
}
char *lexanalyse()
{
	int linesize=readsize, pos=0, c;
	char *command=malloc(sizeof(char)*linesize);
	if(!command)
	{
		write(2, "~bash: Error while allocating memory\n", 37);
		exit(1);
	}
	while(1)
	{
		c=getchar();
		if(c=='\n'||c==EOF)
		{
			command[pos]='\0';
			return command;
		}
		else
		{
			command[pos]=c;
		}
		pos++;
		if(pos>=linesize)
		{
			linesize=linesize+readsize;
			command=realloc(command,sizeof(char)*linesize);
			if(!command)
			{
				write(2, "~bash: Error while allocating memory\n", 37);
				exit(1);
			}

		}
	}
}
char **parcom(char *line)
{
	int argsize=comsize, pos=0;
	char **ctokens=malloc(sizeof(char *)*argsize);
	char *ctoken;
	if(!ctokens)
	{
		write(2, "~bash: Error while allocating memory\n", 37);
		exit(1);
	}
	ctoken=strtok(line, comdelim);
	while(ctoken != NULL)
	{
		ctokens[pos]=ctoken;
		pos++;
		if(pos>=argsize)
		{
			argsize=argsize+comsize;
			ctokens=realloc(ctokens, sizeof(char *)*argsize);
			if(!ctokens)
			{
				write(2, "~bash: Error while allocating memory\n", 37);
				exit(1);
			}
		}
		ctoken = strtok(NULL, comdelim);
	}
	ctokens[pos]='\0';
	return ctokens;
}
char **parall(char *line)
{
	int argsize=toksize, pos=0;
	char **tokens=malloc(sizeof(char *)*argsize);
	char *token;
	if(!tokens)
	{
		write(2, "~bash: Error while allocating memory\n", 37);
		exit(1);
	}
	token=strtok(line, tokdelim);
	while(token != NULL)
	{
		tokens[pos]=token;
		pos++;
		if(pos>=argsize)
		{
			argsize=argsize+toksize;
			tokens=realloc(tokens, sizeof(char *)*argsize);
			if(!tokens)
			{
				write(2, "~bash: Error while allocating memory\n", 37);
				exit(1);
			}
		}
		token = strtok(NULL, tokdelim);
	}
	tokens[pos]='\0';
	return tokens;
}
int execute(char **tok)
{
	int j=3;
	if(tok[0]==NULL)
	{
		return 1;
	}
	while(j--)
	{
		if(strcmp(tok[0],bstr[j])==0)
		{
			return (*bfunc[j])(tok);
		}
	}
	return kickstart(tok);
}