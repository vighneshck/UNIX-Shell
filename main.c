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
int main(int argc, char *argv[])
{
	char *line;
	char **ctok;
	char **tok;
	int stat,i;
	char username[100], system_name[100], curr_dir[100];
	do
	{
		i=0;
		gethostname(system_name, sizeof(system_name));
		getcwd(curr_dir, sizeof(curr_dir));
		getlogin_r(username, sizeof(username));
		char * printstr=curr_dir;
		if(strstr(curr_dir,"/Users/vighneshck")>0)
		{
			printstr=curr_dir+16;
			printstr[0]='~';
			//strcpy(curr_dir,"~");
		}
		printf("<%s@%s:%s> ", username, system_name,printstr);
		line=lexanalyse();
		ctok=parcom(line);
		do
		{
			tok=parall(ctok[i]);
			stat=execute(tok);
			i++;
		} while(stat&&ctok[i] != 0);
		free(line);
		free(tok);
		free(ctok);
	} while(stat);
	return 0;
}

