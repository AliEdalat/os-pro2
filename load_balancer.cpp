#include "load_balancer.h"
#include <iostream>
#include <sys/wait.h>

LoadBalancer::LoadBalancer(int prc_cnt_input, std::string dir_input)
{
	prc_cnt = prc_cnt_input;
	dir = dir_input;
	for (int i = 0; i < prc_cnt; ++i)
	{
		pipes.push_back(new int[2]);
		if (pipe(pipes[i])==-1)
		{ 
        	std::cout << " Pipe Failed!" << std::endl; 
        	throw 1;
		}
	}
}

void LoadBalancer::run()
{
	for (int i = 0; i < prc_cnt; ++i)
	{
		int pid;
		if ((pid = fork()) > 0){
			proc_ids.push_back(pid);
			std::string s = "forgeeks.org"; 
			close(pipes[i][0]);  // Close reading end of first pipe
			// Write input string and close writing end of first 
			// pipe. 
			write(pipes[i][1], s.c_str(), s.length());
			close(pipes[i][1]);
			// wait(NULL);
		}else{
			close(pipes[i][1]);  // Close writing end of first pipe 
			// Read a string using first pipe 
			char concat_str[100];
			int val = read(pipes[i][0], concat_str, 100);
			concat_str[val] = '\0';
			std::cout << std::string(concat_str) << std::endl;
			exit(0);
		}
	}
}

// char fixed_str[] = "forgeeks.org"; 
//     char input_str[100]; 
//     pid_t p; 
  
//     if (pipe(fd1)==-1) 
//     { 
//         fprintf(stderr, "Pipe Failed" ); 
//         return 1; 
//     } 
//     if (pipe(fd2)==-1) 
//     { 
//         fprintf(stderr, "Pipe Failed" ); 
//         return 1; 
//     } 
  
//     scanf("%s", input_str); 
//     p = fork(); 
  
//     if (p < 0) 
//     { 
//         fprintf(stderr, "fork Failed" ); 
//         return 1; 
//     } 
  
//     // Parent process 
//     else if (p > 0) 
//     { 
//         char concat_str[100]; 
  
//         close(fd1[0]);  // Close reading end of first pipe 
  
//         // Write input string and close writing end of first 
//         // pipe. 
//         write(fd1[1], input_str, strlen(input_str)+1); 
//         close(fd1[1]); 
  
//         // Wait for child to send a string 
//         wait(NULL); 
  
//         close(fd2[1]); // Close writing end of second pipe 
  
//         // Read string from child, print it and close 
//         // reading end. 
//         read(fd2[0], concat_str, 100); 
//         printf("Concatenated string %s\n", concat_str); 
//         close(fd2[0]); 
//     } 
  
//     // child process 
//     else
//     { 
//         close(fd1[1]);  // Close writing end of first pipe 
  
//         // Read a string using first pipe 
//         char concat_str[100]; 
//         read(fd1[0], concat_str, 100);
//         exit(0); 
//     } 

