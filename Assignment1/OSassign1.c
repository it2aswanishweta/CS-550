#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
int height=0, child_no=0;
char heightC_new[2], childno_new[2];


printf("\n%d :Process starting\n",getpid());
printf("%d :Parent's id = %d\n",getpid(),getppid());
printf("%d :Height in the tree = %s\n",getpid(),argv[1]);

height = atoi(argv[1]);
child_no = atoi(argv[2]);
pid_t pid[child_no];

if (height>1)
{
	int flag=0;

	while(flag<child_no)
	{
		pid[flag] = fork();

		if(pid[flag]<0)
			{
				fprintf(stderr,"Error in fork system call!");
				exit(1);
			}


		if(pid[flag]==0)
	  	 	 {
			    sprintf(heightC_new,"%d",height-1);
                	    sprintf(childno_new,"%d",child_no);

			    printf("%d :Creating %s children at height %d\n",getpid(),argv[2],height-1);

			    if(execlp(argv[0],argv[0],heightC_new,childno_new,(char *)NULL) == -1)
			    fprintf(stderr, "Execlp has failed\n");

			    exit(0);
	  	 	 }

	flag++;

         }

	while (child_no!=0)
	{
		  wait(NULL);
		  child_no--;
	}
}

printf("\n%d :Terminating at height %d \n",getpid(),height); 
return 0;
}
