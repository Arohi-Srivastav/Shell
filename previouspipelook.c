#include "header.h"
void exec(char** strings)
{
	char c;
	if (strings[1]!=NULL)
	{
		int i=0;
		char *args[500];
		while(strings[i]!=NULL)
		{
			args[i]=strings[i];
			i++;
		}
		args[i]=NULL;
		pid_t  pid;
		int  status;
		printf("%s now running in background \n",strings[0]);

		if ((pid = fork()) < 0)                                                     //child process
		{
			printf(" ERROR: forking child process failed\n");
			exit(1);
		}
		else if (pid == 0)
		{
			setpgid(0,0);

			if (execvp(args[0],args) < 0)
			{
				printf(" ERROR: exec failed\n");
				exit(1);
			}

		}
		else if(pid > 0)
		{
			st new;
			new.pid=pid;
			strcpy(new.name,strings[0]);
			char *PID[] = {NULL, NULL};
			char Pid[100];
			sprintf(Pid, "%d", pid);
			PID[0] = Pid;
			char d=pinfo(PID);
			if(d=='R')
			{
				strcpy(new.state,"Running");
			}
			else if(d=='S')
			{
				strcpy(new.state,"Sleeping");
			}
			else if(d=='T')
			{
				strcpy(new.state,"Stopped");
			}
			int t=0;
			while(list[t].pid!=-1)
				t++;
			list[t]=new;

		}

	}

}
void ps(void)
{
	FILE *file = popen("ps", "w");
	char buffer[100];
	fscanf(file, "%100s", buffer);
	printf("%s\n", buffer);
	pclose(file);
}
void sigint(int n)
{
	if(n == SIGINT)
	{
		if(getpid() != shellpid)
		{
			kill(getpid(),9);
			int i=0;
			int t=getpid();
			while(list[i].pid!=-1)
			{
			if(list[i].pid==t)
				list[1].pid=-1;
			i++;
			
			}
			
		}
		else
		{
		}
	}
}


