#include <stdio.h>
#include <string.h> 
#include <unistd.h> //For fork and exec
#include <errno.h> //for err no in exec
#include <sys/wait.h>
#include <sys/types.h>
#include<stdlib.h> //for exit
#include <signal.h>
#include <fcntl.h> //for O_RDONLY

#define arraylength 1024

/*******************This initialization of for separation of the commands using space*******************/
char inputarray[arraylength];
char inputarray1[arraylength];
char *token[80];
char *token1[80];
/******************************************************************************************************/
int flag, stopfgbg=0, stopioop=0; //flag is for separating foreground and background
int numOfPipeSeparatedCmds=0;
int counter=0,counter1=0;
char pidstorage[100];
int pidCounter=0;

int IpPipeOp=0;//This is a flag to check if there are any i/o o/p signs along with pipe
/***************************To seperate commands******************************************************/
 int SeparateCommand(char inputarray[])
   {
     
        token[0] = strtok(inputarray," \n"); // first call returns pointer
                                           // to first part of inputarray
                                           // separated by delimiter " "
	int i=0;
        while(token[i]!=NULL)
        {
          i++;
          token[i] = strtok(NULL," \n");    // every call with NULL uses
                                        // saved user_input value and
                                         // returns next substring
        }
        counter=i;
        token[i]=NULL; 
        flag=0;    
   }


/**************************End to separate commands**************************************************/

/**************************Signal Handler************************************************************/
 void childSignalHandler(int signum) 
  {
    int status;
    pid_t pid;
     
       pid = waitpid(-1, &status, WNOHANG);
       
/*
If you pass -1 and WNOHANG, waitpid() will check 
if any zombie-children exist. 
If yes, one of them is reaped and its exit status returned. 

WNOHANG is for. It prevents wait()/waitpid() from blocking
so that your process can go on with other tasks. 
If a child died, its pid will be returned by wait()/waitpid() 
and your process can act on that. 
If nothing died, then the returned pid is 0.
*/
  }

  void CtrlCsignalHandler(int signum1)
 {
   //This is for not to do anything when Ctrl C is pressed
 }

/***************************End of signal handler**************************************************/
/****************************Start of input redirection********************************************/
int redirectInput()
 {
    pid_t pid; 
 
   //This for loop is for finding the location of < 
    int si=0;
    for(int s=0;s<counter;s++)
    {
      if(strcmp(token[s],"<")==0) 
       {
	si=s;
       	break;
       }
    }

    pid=fork();   

    if(pid<0)
      {
        perror("Fork() failed\n");
      }

    if(pid>0)
      {
                
      }
    
   if(pid==0)
      {
         int file = open(token[si+1], O_RDONLY); //si+1 will be containing the file
         int c = dup2(file,0); 
   
         if(c==0)
         {
            close(file);  
            
            token[si]=NULL;
            if((execvp(token[0],token))==-1)
	    	    {
               		fprintf(stderr, "Execvp failed during input redirection, err no -> %d\n",errno);
  	
                    }
            
         }
      
         else
         {
            perror("Error in Dup\n");
            close(file);

            exit(1);
         
         }
  	
      } //end of if for pid is zero         
    


 } //End of input redirect
/***************************************End of input redirection***********************************************/

/************************************Start of output redirection***********************************************/

int redirectOutput()
 { 

    pid_t pid;
    
    //This for loop is for finding the location of >
    int so=0;
    for(int s=0;s<counter;s++)
    {
      if(strcmp(token[s],">")==0) 
       {
	so=s;
       	break;
       }
    }

   
    pid=fork();

     

    if(pid>0)
    { 

    }

    if(pid<0)
    {
      perror("Fork() failed\n");
    }
   
    if(pid==0)
    {
     
     int file = open( token[so+1], O_WRONLY | O_TRUNC | O_CREAT, 0755 );
     int c = dup2(file,1);
     

       if(c<0)
       {
          perror("Error in Dup\n");
          close(file);   

       }
   
       else
       {
          close(file);
          token[so]=NULL;
    
          if((execvp(token[0],token))==-1)
	    	    {
               		fprintf(stderr, "Execvp failed during output redirection, err no -> %d\n",errno);
  	
                    }
  
       }
        
 }//end of if

    
 }//end of redirect ouput

/******************************************End of output redirection**************************************************/
/*****************************************To check if there are io op signs along with pipe***************************/
 int test(char *token1[])
		{
                  
			int i=0;
			while(token1[i]!=NULL)
			{
				if(strcmp(token1[i],"<")==0)
				{	
			  	return 1;				
				}
				if(strcmp(token1[i],">")==0)
				{	
			  	return 2;				
				}	
			i++;
		 	}
		return 0;	
		}

/****************************************End of checking io op signs along iwth pipe**********************************/
/******************************************Start of execution using Pipes*********************************************/

/*
  Read = 0, Write =1

  Taken flags as first_cmd, last_cmd, input


  first_cmd will be 1, if it is the first command otherwise it will be zero
  last_cmd will be 1, if it is the last command otherwise it will be zero
  input takes value from previous commands

*/

int pipeExecution(int input, int first_cmd, int last_cmd)
{
  int fileDescriptor[2];
  pid_t pid1;

     /* create a pipe */
    if (pipe(fileDescriptor) == -1) {
            perror("pipe");
            exit(1);
    }


    pid1=fork();
     
        if (pid1 < 0)
        {
	perror("Fork Failed");
	exit(1);
        }

	if (pid1 == 0)             
         {

		if (input == 0 && first_cmd == 1 && last_cmd == 0) 
			{
			// For the First command, input from previous command will be zero
			
			dup2( fileDescriptor[1], STDOUT_FILENO );
		        } 

		else if (input != 0 && first_cmd == 0 && last_cmd == 0) 
			{
			// Middle command
                       
			dup2(input, STDIN_FILENO);
			dup2(fileDescriptor[1], STDOUT_FILENO);
			} 

		else 
			{
			// Last command
                       
			dup2( input, STDIN_FILENO );
		        }

		if(IpPipeOp == 1) //this will be 1 if the input sign is found
                {
                  	   int sid=0;
   			   for(int s=0;s<counter1;s++)
    			   {
      				if(strcmp(token1[s],"<")==0) //for finding the location of <
       				{
					sid=s;
       					break;
       				}
    			  }

				int file = open(token1[sid+1], O_RDONLY);
         			int c = dup2(file,0); 
   
         			if(c==0)
         			{
            				close(file);         
            				token1[sid]=NULL;

            				if((execvp(token1[0],token1))==-1)
	    	    			{
               					fprintf(stderr, "Execvp failed during input redirection, err no -> %d\n",errno);
                        		}
            
         			}
      
         			else
         			{

            				perror("Error in Dup\n");
            				close(file);

            				exit(1); //means failure, exit(0) means success
         
         			}

		  
               }
               else if(IpPipeOp == 2)     // this will be 2 if the output sign is found            
                {
                    
    			//This for loop is for finding the location of >
    			int sod=0;
    			for(int s=0;s<counter;s++)
   			 {
      				if(strcmp(token1[s],">")==0) 
      			 	{
			 	 sod=s;
       			 	 break;
      				 }
    			 }

    				int file = open( token1[sod+1], O_WRONLY | O_TRUNC | O_CREAT, 0755 );
     				int c = dup2(file,1);
     
      				if(c<0)
      			 	{
          				perror("Error in Dup\n");
          				close(file);   
       				}
   
       				else
       				{
          				close(file);
         				token1[sod]=NULL;
    
         				if((execvp(token1[0],token1))==-1)
	    	   			 {
               					fprintf(stderr, "Execvp failed during output redirection, err no -> %d\n",errno);
  	
                   			 }

                                 }
      
		}
                else
		{
			if (execvp(token1[0],token1) == -1)
				{
               			  fprintf(stderr, "Execvp failed during output redirection, err no -> %d\n",errno);
                   		}

		}

	  }

	if (input != 0) 
		close(input); //This will get closed for middle command and last command, after duplicating the input we close it
 
	// Nothing more needs to be written
	close(fileDescriptor[1]);
 
	// If it's the last command, nothing more needs to be read
	if (last_cmd == 1)
		close(fileDescriptor[0]);
 
	return fileDescriptor[0]; // returning the read file descriptor so that it can act as an input 

    
}

/******************************************End of execution using pipes**********************************************/

int main(int argc, char *argv[])
  {
       
       
       pid_t pid;
       int status;

       signal(SIGCHLD, childSignalHandler);
       signal(SIGINT, CtrlCsignalHandler);

       while(1)
   	{
    	   printf("minish>");          

    	   if(!fgets(inputarray, arraylength, stdin))
           break;

	   strcpy(inputarray1, inputarray); //inputarray changes post separatecommand so for using it in for pipes, i copied it in other string before it change

           SeparateCommand(inputarray);

	    /******************************************For exit command ************************************************/

	   if(strcmp(inputarray,"exit")==0)
			{        
                                for(int f=0; f<pidCounter; f++)
                                {
					kill(pidstorage[pidCounter],9);
                                       
				}
 				/*
				kill -9 Meaning: The process will be killed by the kernel; this signal cannot be ignored. 
				9 means KILL signal that is not catchable or ignorable. Uses: SIGKILL singal. 
                                Kill Meaning: The kill command without any signal passes the signal 15, which terminates
				the process the normal way.
				*/
      				exit(0);
			}

	  /******************************************End of exit command**********************************************/
      
         
 /*******************************************For bifurcation of input and output******************************************/
	int change=1; // this is a flag to stop the checking for input output if there is a pipe
       
	for(int ch=0; token[ch]!=NULL; ch++)
 	{  
           if(strcmp(token[ch],"|")==0)
            { 
             change=0; 
             break;
	    }  
	}
      
       if(change==1) //For disabling a check for input output if a pipe if found
	{ 
         int parse=0;
           while(token[parse]!=NULL)
           {
               
              if(strcmp(token[parse],"<")==0)
              { 
                       
		         stopfgbg=1; stopioop=1;   
	                 redirectInput();
			 break;
              }
              
              else if(strcmp(token[parse],">")==0)
              {          
                         stopfgbg=1; stopioop=1;
                         redirectOutput();
                         break;
              }
             	          
              parse++;

           }  
	} 
	

       /*****************************For disabling fg and bg from executing if a pipe is found***********/
                     int j=0;
                    while(token[j]!=NULL)
 			{
				if(strcmp(token[j],"|")==0)
                                {
				  stopfgbg=1;
                                  break;
				}
                           j++;
			}

       /*******************************************End of fg and bg disabling*****************************/
                                  
        
/*************************************************End of bifurcation of input and output***********************************/



     
 /***********************For bifurcation of foreground and background process*************************************/

if(stopfgbg == 0)
{
	
	    int k=0;
            while(token[k]!=NULL)
              {
	        if(strcmp(token[k],"&")==0) //if & means it is a background process
		{
		   token[k]=NULL;
                   flag = 1;
		}
		k++;
 	       }

/**************************************************End of bifurcation********************************************/  
     
/*********************************************Start of foreground process*****************************************/

           if(flag==0)
            {
	      stopioop=1; //To stop entering for input output
	      pid=fork();
          
    	      if(pid<0)
	      {
                  perror("Fork() failed\n");
	      }
           
	      if(pid==0) //For the child
	      {

	    	 if((execvp(token[0],token))==-1)
	    	    {
               		fprintf(stderr, "Execvp failed, err no -> %d\n",errno);
  	
                    }
	      }
       
              if(pid>0)
		{
			if((waitpid(pid,&status,0))==-1)
			{
			  perror("Wait Failed\n");
			  exit(1);
			}   
	                 
		}    
             
	     } //End of if
/******************************************************End of foreground process*********************************/
         
/**********************************************Start of Background process**************************************/
	    if(flag==1)
		{
		  stopioop=1; //To stop entering from input output
                   pid=fork();		   	
	           
                   
		
    	     	   if(pid<0)
	      	     {
                       perror("Fork() failed\n");
	             }
           
	          if(pid==0) //For the child
	             {
                       
	    	       if((execvp(token[0],token))==-1)
	    	          {
               		    fprintf(stderr, "Execvp failed, err no -> %d\n",errno);
  	                  }
                       
                      
	             }
                   if(pid>0)
                   {
                      
                      
                      pidstorage[pidCounter]=pid;
                      pidCounter++;

                      printf("Process: %d in background mode\n",pid);
                   }
       
		} 
/*****************************************************End of Background process****************************************/

	 	
} //if stopfgbg --> to stop entering foreground background


/****************************************************Seperating the array for pipe*************************************/
if(stopioop==0)
{

             int SeparateCommand1(char * array, int input, int first_cmd, int last_cmd)
   		{      
                     
        		token1[0] = strtok(array," \n"); // first call returns pointer
                                           // to first part of inputarray
                                           // separated by delimiter " "
			int i=0;
        		while(token1[i]!=NULL)
        		{
          		i++;
          		token1[i] = strtok(NULL," \n");    // every call with NULL uses
                                                          // saved user_input value and
                                                          // returns next substring
        		}
        		
                        counter1=i;
			                                               
   			token1[i]=NULL; 

                   /*****************After finding pipe check if there are any O/P O/P signs*********/
        
			if(change == 0) //change is zero means there is pipe 
        		{
   	  			
                                IpPipeOp = test(token1);
               
        		}

                  /***************End of checking of I/P O/P sign of pipe is present*****************/
                       
                        return pipeExecution(input, first_cmd, last_cmd);
        		 
  		}

          
/************************************End of separating the array for pipe ***********************************************/       
           
/*************************************************Start of pipe execution************************************************/

                //0 for stdin in file descriptor and stdout for output in file descriptor 
                //input is the input given from previous commands before the pipe to the commands after the pipe 
                  
                  stopfgbg=1; 
                  int first_cmd=1, last_cmd=0, input=0; 
                     
                    char* array = inputarray1;
                  // if inputarray1 is cat file.txt | grep s , array will be same
                  
		  char* next_cmd = strchr(array, '|'); /* Find first occurance of'|' */

	          while (next_cmd != NULL) {
                      
			/* 'next_cmd' points to '|' */
			*next_cmd = '\0';
                        
			input = SeparateCommand1(array, input, first_cmd, last_cmd); //This is for separating my array and will remove the space and |
                        
                              //printf("Array before was:%s\n", array); O/P --> cat
                         
			array = next_cmd + 1; // now array will become grep s
                        
                             //printf("array now is %s\n", array); O/P --> grep s

			next_cmd = strchr(array, '|'); /* Find next '|' */
			first_cmd = 0; // means no longer it is the first command so disabling the flag

		}

		input = SeparateCommand1(array,input, first_cmd, 1); //sending last_cmd as 1 to enable that it is the last command
                                	  
} //End of if --> stopioop

/*******************************************************End of pipe execution************************************************************/      

stopfgbg=0; 
stopioop=0;
change=1;
     	
    } //End of while
  
   
  } //End of main
