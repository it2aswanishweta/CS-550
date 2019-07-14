## Description

This assignment is about recursively creating a process hierarchy tree that is H levels deep, then printing out the process tree information as the tree is being created, and then systematically terminate all the processes in the tree.
Here we are using fork(), exec() and wait() system calls.

-------------------------------------------------------------------------------------------------------------------------------------
Please read the following lines:
1) The program can be compiled through make file and has to be executed using he command ./OSassign1 with the inputs you want to give.

2) Take only 2 command line inputs of which first is the height and second is the number of children
   eg: For height 3 and chidren 2 please use the below format to execute the file
       ./OSassign1 3 2
-------------------------------------------------------------------------------------------------------------------------------------      
The output will be as follows:

(pid): Process starting
		(pid): Parent's id = (ppid)
		(pid): Height in the tree = (value_of_H_from_command_line_argument)
		(pid): Creating (value_of_C_from_command_line) children at height (H-1)
      
      (pid): Terminating at height (H).

   
