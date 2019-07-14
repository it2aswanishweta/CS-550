## Description

This program creates a mini shell "minish". Here we learn about the basic process management in a shell and we will be able to execute and control any binary executable just as we would in a normal shell.

This shell performs the following functions:
1) minish> Command
     [ OUTPUT OF COMMAND SHOWN HERE ]

   minish>

2)Execute commands with multiple arguments:
  minish> Command arg1 arg2 arg3

3)Execute commands in either foreground or background mode.
  minish> Command1 &
	Process print_process_id_1_here in background mode
  minish> Command2

	[OUTPUT OF Command1 AND Command2 MAY INTERLEAVE IN ARBITRARY ORDER]

4)Multiple processes running in background mode simultaneously. 
  minish> Command1 &
	Process print_process_id_1_here in background mode
  minish> Command2 &
	Process print_process_id_2_here in background mode
  minish> Command3 &
	Process print_process_id_3_here in background mode
  minish> 

	[OUTPUT OF Command1, Command2, AND Command3 MAY INTERLEAVE IN ARBITRARY ORDER]

5) Redirect the input of a command from a file:
   minish> Command < input_file

   Redirect the output of a command to a file. For example:
   minish> Command > output_file

6) Implement command filters, i.e., redirect the stdout of one command to stdin of another using pipes:
   minish> cat somefile | grep somestring | less

7) Terminate a foreground process by pressing [Ctrl-C]. The shell is not get killed; only the process running in foreground mode        are terminated. If executing a chain of filters (as in the above example), all processes in the filter chain must terminate.

8) Kill a process in background using the kill command.
   minish> kill give_process_id_here

9) It is able to execute any feasible combinations of filters and file I/O redirection.

10) The exit command will terminate the shell and clean up any orphan processes as the shell exits.
		
---------------------------------------------------------------------------------------------------------------
## Compile and Execute
 The program can be compiled through make file and has to be executed using he command ./minish1 with the inputs you want to give.



