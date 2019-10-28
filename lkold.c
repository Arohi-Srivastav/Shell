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
	int newin=dup(in);
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
		int piped[2];
	for(i=0;i<s;i++)
	{
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
			if(i!=s-1)
			{
				dup2(piped[1],1);
				if(i!=0)
				dup2(piped[0],0);
				close(piped[0]);
				close(piped[1]);
				//dup2(out,1);
			}
			else
			{
				if(i!=0)
					dup2(piped[0],0);
				dup2(out,1);
				close(piped[1]);
				close(piped[0]);
				close(out);
				//restore initial stdout
			}
			pr++;	
			int count=0;
			while(strings[count]!=NULL)
				count++;
			if(strncmp(instr[0],"ls",2)==0)
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
			{
				execvp(instr[0],instr);
			}

		}
	}
		//else if(p>0)
		//{
			close(piped[0]);
			close(piped[1]);
			for(i=0;i<s;i++)
				wait(NULL);
		//}
	dup2(in,0);
	dup2(out,1);
	close(in);
	//close(out);
}

