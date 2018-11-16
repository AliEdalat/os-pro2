#include "load_balancer.h"
#include <iostream>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <fstream>
#include <streambuf>

LoadBalancer::LoadBalancer(std::string input)
{
	extract_input_data(input);
	items = new std::vector<std::string*>;
	// prc_cnt = prc_cnt_input;
	// dir = dir_input;
	step = 1;
	start_file = 0;
	std::string path = "fifos/fifo";
	mkdir("fifos", S_IRWXU|S_IRWXG|S_IROTH);
	for (int i = 0; i < prc_cnt; ++i)
	{
		FILE* fp;
		if ((fp = fopen((path + std::to_string(i)).c_str(), "ab+")) == NULL){
			std::cout << " Fifo Failed!" << std::endl; 
        	throw 1;	
		}
		fclose(fp);
		mkfifo((path + std::to_string(i)).c_str(), 0666);
		fifos.push_back(path + std::to_string(i));
		workers.push_back(new Worker());
		pipes.push_back(new int[2]);
		if (pipe(pipes[i])==-1)
		{ 
        	std::cout << " Pipe Failed!" << std::endl; 
        	throw 1;
		}
	}
}

LoadBalancer::~LoadBalancer()
{
	std::string path = "fifos/fifo";
	for (int i = 0; i < prc_cnt; ++i)
	{
		unlink((path + std::to_string(i)).c_str());
	}
	rmdir("fifos");
}

void LoadBalancer::extract_input_data(std::string line)
{
	line += "-";
	int state = 0;
	std::string subject;
	std::string value;
	for (int i = 0; i < line.length(); ++i)
	{
		if (line[i] == '=')
		{
			state = 1;
			value = std::string();
			continue;
		}

		if (line[i] == '-')
		{
			if (subject == "prc_cnt")
				prc_cnt = stoi(value);
			else if (subject == "dir")
				dir = value;
			else if (value != "descend" && value != "ascend")
				search_model.set(subject, value);
			else
				sort_model.set(subject, value);
			//std::cout << "(" << subject << "," << value << ")" << std::endl;
			state = 0;
			subject = std::string();
			continue;
		}

		if (state == 0)
			subject.push_back(line[i]);
		else if (state == 1)
			value.push_back(line[i]);
	}
}

void LoadBalancer::extract_files_data_in_dir()
{
	DIR* dir_pointer;
	struct dirent *ent;
	if ((dir_pointer = opendir (dir.c_str())) != NULL)
	{
	/* print all the files and directories within directory */
		while ((ent = readdir (dir_pointer)) != NULL) {
			files.push_back(new std::string(dir + "/" + std::string(ent->d_name)));
			//std::cout<<*files[files.size()-1]<<std::endl;
			if (files[files.size()-1]->at(dir.length()+1) == '.')
			{
				files.pop_back();
				//std::cout << "poped" << std::endl;
			}
		}
		closedir (dir_pointer);
	} else {
	/* could not open directory */
		throw 1;
	}
	step = (files.size() > prc_cnt) ? (files.size() / prc_cnt) : 1;
	for (int i = 0; i < prc_cnt; ++i)
	{
		if (start_file < files.size()){
			starts.push_back(start_file);
			start_file += step;
		}else{
			starts.push_back(-1);
		}
	}
	//std::cout << "step : " << step << std::endl;
}

void LoadBalancer::extract_header(std::string str)
{
	std::string temp;
	for (int i = 0; i < 26; ++i)
	{
		if (str[i] == ' ' || str[i] == '\n')
		{
			items->push_back(new std::string(temp));
			if (str[i] == '\n')
				break;
			temp = "";
			continue;
		}
		temp.push_back(str[i]);
	}
	for (int i = 0; i < items->size(); ++i)
	{
		std::cout << *(*items)[i] << std::endl;
	}
}

void LoadBalancer::run()
{
	bool header_ext = false;
	extract_files_data_in_dir();
	int root = getpid();
	for (int i = 0; i < prc_cnt; ++i)
	{
		int pid;
		if ((pid = fork()) > 0){
			proc_ids.push_back(pid);
			// Close reading end of first pipe
			// std::cout << pid << std::endl;
			// std::cout << "start : " << starts[i] << std::endl;
			std::string res;
			if (files.size() % prc_cnt > 0 && (files.size() - 1 - starts[i]) > 0 && (files.size() - 1 - starts[i]) <= step)
			{
				step += files.size() - 1 - starts[i];
			}
			for (int j = starts[i]; j < step+starts[i]; ++j)
			{
				std::ifstream t(*files[j]);
				std::string str((std::istreambuf_iterator<char>(t)),
	                 std::istreambuf_iterator<char>());
				if (header_ext == false)
				{
					extract_header(str);
					header_ext = true;
				}
				std::string temp (&str[26]);
				res += temp + "\n";
				//std::string s = "forgeeks.org";
				// Write input string and close writing end of first 
				// pipe. 
			}
			close(pipes[i][0]);
			write(pipes[i][1], res.c_str(), res.length());
			close(pipes[i][1]);
			// wait(NULL);
		}else{
			close(pipes[i][1]);  // Close writing end of first pipe 
			// Read a string using first pipe 
			std::string concat_str;
			concat_str.resize(5000);
			int val = read(pipes[i][0], &concat_str[0], 5000);
			concat_str[val] = '\0';
			std::string input = std::string(concat_str);
			//std::cout << input << std::endl;
			workers[i]->set_input(input, fifos[i]);
			workers[i]->exec(search_model, items);
			exit(0);
		}
	}
	std::vector<bool> presented(prc_cnt,false);
	if (fork() == 0)
	{
		int end = 0;
		while(true)
		{
			for (int i = 0; i < prc_cnt; ++i)
			{
				char str1[5000];
				int fd1 = open(fifos[i].c_str(),O_RDONLY); 
		        int val = read(fd1, str1, 5000);
		        str1[val] = '\0';
		        // Print the read string and close 
		        // printf("User%d: %s\n", i,str1);
		        if (val > 0 && presented[i] == false)
		        {
		        	presented[i] = true;
		        	end++;
		        	presenter.set_input(std::string(str1));
		        }
		        close(fd1);
			}
			if (end == prc_cnt)
				break;
		}
		presenter.present(sort_model);
		exit(0); 
	}

	if (getpid() == root)
		while(1)
		{
			std::string temp;
			std::cin >> temp;
			if (temp == "quit")
				break;
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

