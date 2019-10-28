#include "header.h"
void pinfo(char **strings) 
{
	char* temp = (char *) calloc(10000, sizeof(char));
	if (temp==NULL) 
	{
		perror("Allocation error\n");
	}
	int pid;
	if (strings[1] == NULL)                                                    //id of the program itself 
	{
		pid = getpid();
	} 
	else 
	{
		pid = atoi(strings[1]);                                          //convert string to integer
	}
	char str[10];
	sprintf(str, "%d", pid);

	if (temp!=NULL) 
	{
		char temp1[1000];
		strcpy(temp1,"/proc/");
		strcat(temp1,str);
		strcat(temp1,"/status");
		int fd1=open(temp1, O_RDONLY );
		if(fd1<0)
		{
			perror("Error in opening file");
		}
		else
		{
			int length=read(fd1,temp,5000);
			if ('\n' == temp[length - 1]) 
			{
				temp[length - 1] = '\0';
			}
			close(fd1);   
			char * first;
			char ** tokens = (char**)malloc(500 * sizeof(char * ));
			char delimit[] = " \t\n";
			if (tokens==NULL) 
			{
				printf("Allocation error\n");
			}
			first = strtok(temp, delimit);
			int len=0;
			while (first != NULL) 
			{
				tokens[len] = first;
				len++;
				first = strtok(NULL, delimit);
			}
			printf("pid -- %d\n", pid);
			for (int i = 0; i < len; i++) 
			{
				if (strcmp(tokens[i], "VmSize:") == 0) 
				{
					printf("Memory -- %s {Virtual Memory}\n", tokens[i + 1]);
				}
				if (strcmp(tokens[i], "State:") == 0) 
				{

					printf("State -- %s\n", tokens[i + 1]);
				}
			}
			char tmp[1000],add[1000];
			strcpy(tmp,"/proc/");
			strcat(tmp,str);
			strcat(tmp,"/exe");
			length = readlink(tmp,add,1000);
			add[length] = '\n';	
			printf("Executable Path -- %s",add);


		}
	}
}
