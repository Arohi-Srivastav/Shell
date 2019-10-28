#include "./header.h"
void lookup(char** strings)
{
	int in=dup(0);
	int out=dup(1);                                            //storing initial stdout and stdin
	int i=0;
	int par[500];
	for(int k=0;k<500;k++)
		par[k]=500;
	int s=0;
	//int newin=dup(in);
	par[0]=0;
	s++;
	while(strings[i]!=NULL)
	{
		if(strings[i][0]=='|')
		{
			par[s]=i+1;
			s++;
		}
		i++;
	}
	int reading[s];
	int writing[s];


	for(int t=0; t < s; t++)
	{
		reading[t] = -1;
		writing[t] = -1;
	}

	int j;
	int fileds[2];
	for(j=0; j < s-1; j++)
	{
		int fileds[2];
		pipe(fileds);
		reading[j+1] = fileds[0];
		writing[j] = fileds[1];
	}
	for(i=0;i<s;i++)
	{
		int piped[2];
		pipe(piped);
		char *instr[500];
		int ink=0;
		for(int j=par[i];j<par[i+1]-1;j++)
		{
			if(strings[j]!=NULL)
			{
				instr[ink]=strings[j];
				ink++;
			}
			else
			{
				instr[ink]='\0';
				ink++;
				break;
			}

		}	
		int p=fork();
		if(p==0)
		{
			//dup2(newin,0);
			//close(newin);
		/*	if(i!=s-1)
			  {
			  close(1);
			  dup2(piped[1],1);
			  if(i!=0)
			  {
			  close(0);
			  dup2(piped[0],0);

			  }
			  close(piped[0]);
			close(piped[1]);
			}
			else
			{
			if(i!=0)
			{
			close(0);
			dup2(piped[0],0);
			}
			close(1);
			dup2(out,1);
			close(piped[1]);
			close(piped[0]);
			close(out);
			}*/

			if(writing[i] != -1)
			{
				close(1);
				dup2(writing[i],1);
			}

			if(reading[i] != -1)
			{
				close(0);
				dup2(reading[i],0);
			}
			/*pr++;	
			int count=0;
			while(strings[count]!=NULL)
				count++;
			*//*if(strncmp(instr[0],"ls",2)==0)
				ls(instr);
			else if(strncmp(instr[0],"cd",2)==0)
				cd(instr,ink);
			else if(strncmp(instr[0],"pwd",3)==0)
				pwd();
			else if(strncmp(instr[0],"echo",4)==0)
				echo(instr,ink);
			else if(strncmp(instr[0],"pinfo",5)==0)
				pinfo(instr);
			else if(strncmp(instr[0],"ps",2)==0)
				ps();
			else if(strncmp(instr[0],"history",7)==0)
				hist(strings);
			else
			{*/
				execvp(instr[0],instr);
			//}

		}
		else
		{
			wait(NULL);
			//close(piped[1]);
		//	close(piped[0]);
			close(writing[i]);

			if(i > 0)
			{
				close(reading[i]);
			}
		}
	}
	dup2(in,0);
	//dup2(out,1);
	close(in);
}

