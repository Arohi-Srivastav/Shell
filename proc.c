#include "header.h"
void exec(char** strings)
{
	char c;
	if (strings[1]!=NULL)
	{
		c=strings[1][0];
		if(c!='&')
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

			if ((pid = fork()) < 0)                                                     //child process
			{     
				printf(" ERROR: forking child process failed\n");
				exit(1);
			}
			else if (pid == 0) 
			{          
				if (execvp(args[0],args) < 0) 
				{     
					printf(" ERROR: exec failed\n");
					exit(1);
				}
			}
			else 
			{                                  
				wait(NULL);       

			}
		}
		else
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



		}
	}
	if(strings[1]==NULL)
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

		if ((pid = fork()) < 0)                                                     //child process
		{
			printf(" ERROR: forking child process failed\n");
			exit(1);
		}
		else if (pid == 0)
		{

			if (execvp(args[0],args) < 0)
			{
				printf(" ERROR: exec failed\n");
				exit(1);
			}
		}
		else
		{
			wait(NULL);
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
