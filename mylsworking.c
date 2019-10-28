#include "header.h"
void print_permissions(struct stat file)
{
	printf( (S_ISDIR(file.st_mode)) ? "d" : "-");
	printf( (file.st_mode & S_IRUSR) ? "r" : "-");
	printf( (file.st_mode & S_IWUSR) ? "w" : "-");	
	printf( (file.st_mode & S_IXUSR) ? "x" : "-");
	printf( (file.st_mode & S_IRGRP) ? "r" : "-");
	printf( (file.st_mode & S_IWGRP) ? "w" : "-");
	printf( (file.st_mode & S_IXGRP) ? "x" : "-");
	printf( (file.st_mode & S_IROTH) ? "r" : "-");
	printf( (file.st_mode & S_IWOTH) ? "w" : "-");
	printf( (file.st_mode & S_IXOTH) ? "x " : "- ");
	printf("%d ", (int)file.st_nlink);

}
void ls(char **strings)
{
	DIR *directory;
	int show_hidden=0;
	int show_long=0;
	int show_home=0;
	int show_ls=0;
	int flag=0;
	if (strings[1]==NULL)
	{
		show_ls=1;
	}
	for (int i = 1; strings[i] != NULL & show_ls==0; i++) 
	{
		flag=1;
		if (strcmp(strings[i], "-a") == 0) 
		{
			show_hidden =1;
		}
		else if (strcmp(strings[i], "-l") == 0) 
		{
			show_long =1;
		}
		else if (strcmp(strings[i], "-la") == 0 || strcmp(strings[i], "-al") == 0) 
		{
			show_hidden=1;
			show_long=1;
		}
		else if (*strings[i]=='~')
		{
			show_home=1;
		}
		else
		{
			DIR *d;
			struct dirent *file;
			d=opendir(strings[1]);
			if(d==NULL)
				printf("Could not open directory");
			while((file = readdir(d)) != NULL)
			{
				if (file->d_name[0] != '.')
				{
					printf("%s\n", file->d_name);
				}
			}
			closedir(d);		
		}

	}

	if(flag==0&&show_ls==1)                                          //plain ls
	{
		char* cur_d=(char*)malloc(500*sizeof(char));
		if(!cur_d)
                perror("Allocation Error");

		DIR *d;
		struct dirent *file;
		getcwd(cur_d,500);
		d = opendir(cur_d);
		if(d==NULL)
			printf("Could not open directory");
		while((file = readdir(d)) != NULL)
		{
			if (file->d_name[0] != '.')
			{
				printf("%s\n", file->d_name);
			}
		}
		closedir(d);		
		free(cur_d);



	}
	else if(flag==1)
	{
		if(show_home==1)
		{
			DIR *d;
			struct dirent *file;
			d = opendir(home);
			if(d==NULL)
				printf("Could not open directory");
			while((file = readdir(d)) != NULL)
			{
				if (file->d_name[0] != '.')
				{
					printf("%s\n", file->d_name);
				}
			}
			closedir(d);		
		}
		else if(show_hidden==1&&show_long==0)                         //only -a flag
		{
			char* cur_d=(char*)malloc(500*sizeof(char));
			DIR *d;
			struct dirent *file;
			getcwd(cur_d,500);
			d = opendir(cur_d);
			if(d==NULL)
				printf("Could not open directory");
			while((file = readdir(d)) != NULL)
			{
				printf("%s\n", file->d_name);
			}
			closedir(d);		
			free(cur_d);
		}
		else if(show_hidden==0&&show_long==1)                                             //only -l flag
		{
			DIR* d;
			struct dirent *dir;
			char* cur=(char*)malloc(500*sizeof(char));
			getcwd(cur,500);
			char* path=(char*)malloc(500*sizeof(char));
			d=opendir(cur);
			if (d == NULL)
			{
				printf("cant open directory");
			}
			else
			{
				dir=readdir(d);                                          //points to the next directory entry
				while(dir != NULL)
				{
					if (dir->d_name[0] != '.')
					{
						char date[500];
						struct stat file;
						strcpy(path,cur);
						strcat(path,"/");
						strcat(path,dir->d_name);
						stat(path,&file);
						print_permissions(file);
						//user name and group name
						register gid_t gid = getgid();
						register uid_t uid = getuid();

						struct group *grp;
						struct passwd *pwd;

						grp = getgrgid(gid);
						printf("%s ", grp->gr_name);

						pwd = getpwuid(uid);
						printf("%s ", pwd->pw_name);
						printf("%d ",(int)file.st_size);
						struct tm *time = localtime(&file.st_mtime);
						strftime(date, 500, "%b %d %H:%M", time);
						printf(" %s %s\n", date, dir->d_name);			


					}
					dir=readdir(d); 
				}
			}


		}
		else if(show_hidden=1&&show_long==1)                     //both a and l flag
		{
			DIR* d;
			struct dirent *dir;
			char* cur=(char*)malloc(500*sizeof(char));
			getcwd(cur,500);
			char* path=(char*)malloc(500*sizeof(char));
			d=opendir(cur);
			if (d == NULL)
			{
				printf("cant open directory");
			}
			else
			{
				dir=readdir(d);                                          //points to the next directory entry
				while(dir != NULL)
				{
					char date[500];
					struct stat file;
					strcpy(path,cur);
					strcat(path,"/");
					strcat(path,dir->d_name);
					stat(path,&file);
					print_permissions(file);
					//user name and group name
					struct group *grp;
					struct passwd *pwd;
					register gid_t gid = getgid();
					register uid_t uid = getuid();

					grp = getgrgid(gid);
					printf("%s ", grp->gr_name);

					pwd = getpwuid(uid);
					printf("%s ", pwd->pw_name);
					printf("%d ",(int)file.st_size);
					struct tm *time = localtime(&file.st_mtime);
					strftime(date, 500, "%b %d %H:%M", time);
					printf(" %s %s\n", date, dir->d_name);			
					dir=readdir(d); 



				}
			}




		}

	}
}
job *first_job = NULL;
job * find_job (pid_t pgid)
{
	job *j;

	for (j = first_job; j; j = j->next)
		if (j->pgid == pgid)
			return j;
	return NULL;
}
int job_is_stopped (job *j)
{
	process *p;

	for (p = j->first_process; p; p = p->next)
		if (!p->completed && !p->stopped)
			return 0;
	return 1;
}

int job_is_completed (job *j)
{
	process *p;

	for (p = j->first_process; p; p = p->next)
		if (!p->completed)
			return 0;
	return 1;
}
int mark_process_status (pid_t pid, int status)
{
	job *j;
	process *p;

	if (pid > 0)
	{
		for (j = first_job; j; j = j->next)
			for (p = j->first_process; p; p = p->next)
				if (p->pid == pid)
				{
					p->status = status;
					if (WIFSTOPPED (status))
						p->stopped = 1;
					else
					{
						p->completed = 1;
						if (WIFSIGNALED (status))
							fprintf (stderr, "%d: Terminated by signal %d.\n",
									(int) pid, WTERMSIG (p->status));
					}
				}
		
		fprintf (stderr, "Process %d. exited normally\n", pid);
		return -1;
	}
	else if (pid == 0 || errno == ECHILD)
		return -1;
	else {
		perror ("waitpid");
		return -1;
	}
}
void update_status(void)
{
	int status;
	pid_t pid;

	do
		pid = waitpid (WAIT_ANY, &status, WUNTRACED|WNOHANG);
	while (!mark_process_status (pid, status));
}


void wait_for_job (job *j)
{
	int status;
	pid_t pid;

	do
		pid = waitpid (WAIT_ANY, &status, WUNTRACED);
	while (!mark_process_status (pid, status)
			&& !job_is_stopped (j)
			&& !job_is_completed (j));
}


void format_job_info (job *j, const char *status)
{
	fprintf (stderr, "%ld (%s): %s\n", (long)j->pgid, status, j->command);
}


void do_job_notification (void)
{
	job *j, *jlast, *jnext;
	process *p;

	update_status ();

	jlast = NULL;
	for (j = first_job; j; j = jnext)
	{
		jnext = j->next;

		if (job_is_completed (j)) {
			format_job_info (j, "completed");
			if (jlast)
				jlast->next = jnext;
			else
				first_job = jnext;
			free (j);
		}

		else if (job_is_stopped (j) && !j->notified) {
			format_job_info (j, "stopped");
			j->notified = 1;
			jlast = j;
		}

		else
			jlast = j;
	}
}
