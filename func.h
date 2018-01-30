#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/uio.h>
#include <sys/wait.h>
#include <sys/types.h>
#define readsize 2048
#define toksize 100
#define comsize 1024
#define tokdelim " \t\r\n\a"
#define comdelim ";"
int kickstart(char **);
int pwd(char **);
int echo(char **);
int cd(char **);
char *lexanalyse();
char **parcom(char *);
char **parall(char *);
int execute(char **);