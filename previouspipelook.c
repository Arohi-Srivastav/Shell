#include "./header.h"
void lookup(char** strings)
{
	int i=0;
	int par[500];
	for(int k=0;k<500;k++)                                                          //helps in storing input properly
		par[k]=500;
	int s=0;
	par[0]=0;
	s++;
	int flag=0;
	while(strings[i]!=NULL)
	{
		if(strings[i][0]=='&')
			flag=1;
		if(strings[i][0]=='|')
		{
			par[s]=i+1;
			s++;
		}
		i++;
	}
	if (flag==1)
	{
		exec(strings);
	}
	else
	{
		int in=dup(0);
		int out=dup(1);                                            //storing initial stdout and stdin
		int read_end[1000];
		int write_end[1000];
		for(int t=0; t < s; t++)
		{
			read_end[t] = -1;
			write_end[t] = -1;
		}
		int j;
		int pipes[1000];
		for(j=0; j < s-1; j++)
		{
			pipe(pipes+2*j);
			read_end[j+1] = 2*j;
			write_end[j] = 2*j+1;
		}
		for(i=0;i<s;i++)
		{
			int in_r=0;
			int out_r=0;
			int app=0;
			char *instr[500];
			int ink=0;
			int test =0;
			char f[500],of[500];
			int fd1=0;
			int fd2=0;
			int a=0,b=0,c=0;
			for(int j=par[i];j<par[i+1]-1;j++)
			{
				if(strings[j]!=NULL)
				{
					if(strncmp(strings[j],"<",1)==0)
						in_r=1;
					else if(strncmp(strings[j],">>",2)==0)
						app=1;
					else if(strncmp(strings[j],">",1)==0 && app==0)
						out_r=1;
					else if(in_r==0&&out_r==0&&app==0)
					{
						instr[ink]=strings[j];
						ink++;
					}
				}
				if(in_r==1&&a==0)
				{
					strcpy(f,strings[j+1]);
					a=1;
				}
				if(out_r==1&&b==0)
				{
					strcpy(of,strings[j+1]);
					b=1;
				}
				if(app==1&&c==0)
				{
					strcpy(of,strings[j+1]);
					c=1;
				}
				if(strings[j]==NULL)
				{
					instr[ink]='\0';
					ink++;
					break;
				}

			}	
			if(strncmp(instr[0],"quit",4)==0)
			{
				overkill();
				exit(0);
			}
			else if(strncmp(instr[0],"setenv",6)==0)
			{
				if(instr[1]==NULL || instr[3] != NULL)
				{
					printf("Invalid number of arguments to setenv\n");
				}
				else
				{
					if (instr[2]==NULL)
					{
						int s=setenv(instr[1],"",1);                  //one for setting overwrite flag
						if(s==-1)
							perror("setenv");
					}
					else
					{
						int s=setenv(instr[1],instr[2],1);
						if(s==-1)
							perror("setenv");
					}
				}

			}
			else if(strncmp(instr[0],"unsetenv",8)==0)
			{
				if (instr[1]==NULL || instr[2]!=NULL)
				{
					printf("Invalid number of arguments to setenv\n");
				}
				else 
				{
					if (getenv(instr[1])==NULL)
					{
						printf("The environment variable does not exist\n");
					}
					else
					{
						unsetenv(instr[1]);
					}
				}
			}
			else if(strncmp(instr[0],"overkill",8)==0)
			{
				overkill();
			}
			else if(strncmp(instr[0],"jobs",4)==0)
			{
				listjobs();
			}
			else if(strncmp(instr[0],"kjob",4)==0)
			{
				kjob(instr);
			}
			else if(strncmp(instr[0],"fg",2)==0)
				fg(instr);
			else if(strncmp(instr[0],"bg",2)==0)
				bg(instr);

			else if(strncmp(instr[0],"cd",2)==0)
			{
				cd(instr,ink);
			}
			else if(strncmp(instr[0],"cronjob",7)==0)
			{
				cron(instr);
				
			}
			else
			{


				int p=fork();
				if(p==0)
				{

					if(in_r==1)
					{
						fd1=open(f, O_RDONLY);
						if(fd1==-1)
						{
							printf("The input file was not found");
							exit(0);
						}
						test = dup2(fd1,0);
						if(test == -1)
						{
							perror("dup error");
							return ;
						}

						close(fd1);
					}
					if(out_r==1)
					{
						fd2=open(of,O_CREAT | O_WRONLY | O_TRUNC, 0644);
						test = dup2(fd2,1);
						if(test == -1)
						{
							perror("dup error");
							return ;
						}

						close(fd2);
					}
					if(app==1)
					{
						fd2=open(of,O_CREAT | O_WRONLY | O_APPEND, 0644);
						test = dup2(fd2,1);
						if(test == -1)
						{
							perror("dup error");
							return ;
						}

						close(fd2);
					}

					if(app!=1 && out_r!=1 &&write_end[i] != -1)
					{
						dup2(pipes[write_end[i]],1);
					}
					if(in_r!=1 && read_end[i] != -1)
					{
						dup2(pipes[read_end[i]],0);
					}

					if(write_end[i] != -1 || read_end[i] != -1) 
					{
						for(int i_p = 0; i_p < 2*(s-1); i_p++) 
						{
							close(pipes[i_p]);
						}
					}
					if(strncmp(instr[0],"ls",2)==0)
					{
						ls(instr);
						exit(0);
					}
					else if(strncmp(instr[0],"pwd",3)==0)
					{
						pwd();
						exit(0);
					}
					else if(strncmp(instr[0],"ps",2)==0)
					{
						ps();
						exit(0);
					}
					else
					{
						int tt=0;
						signal(SIGTSTP, SIG_DFL);
						tt=execvp(instr[0],instr);
						if (tt==-1)
							perror("Execution of commmand failed");
					}



				}

				else
				{
					close(fd1);
					close(fd2);
					if(write_end[i] != -1)
						close(pipes[write_end[i]]);
					waitpid(p, NULL, WUNTRACED);
					if(getpgid(p) >= 0)
					{
						st new;
						new.pid=p;
						strcpy(new.name,strings[0]);
						char *PID[] = {NULL, NULL};
						char Pid[100];
						sprintf(Pid, "%d", p);
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
		}
		for (i=0;i<2*(s-1);i++)
		{
			close(pipes[i]);
		}

		dup2(in,0);
		dup2(out,1);
		close(in);
		close(out);

	}


}


