#include "header.h"
void user(void)
{
	char* user_name;
	char* system_name;
	user_name = (char *)malloc(500*sizeof(char));
	system_name = (char *)malloc(500*sizeof(char));
	getlogin_r(user_name,500);
	gethostname(system_name,500);
	printf("<%s@%s",user_name,system_name);
	directory();
}
void directory(void)
{
	char *temp = (char*)malloc(500*sizeof(char));
	if(!temp)
                perror("Allocation Error");

	char *dir = (char*)malloc(500*sizeof(char));
	if(!dir)
                perror("Allocation Error");

	int i=0;
	int h=501;
	getcwd(dir,500);
	for(i=0;dir[i]!='\0';i++)
	{
		temp[i]=dir[i];
		temp[i+1]='\0';
		if(strcmp (temp,home) == 0)
		{
			h = i;
			break;
		}
	}
	if(h != 501)                                         //500 is the limit of my buffer
	{
		temp[0]='~';
		int j=1;
		for(i=h+1;dir[i]!='\0';i++)
		{
			temp[j]=dir[i];
			j++;
		}
		temp[j]='\0';
		printf("%s> ",temp);
	}
	else
		printf("%s> ",dir);


}
