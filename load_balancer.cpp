#include "load_balancer.h"
#include <iostream>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <dirent.h>
#include <fstream>
#include <streambuf>

#define presenter_exe "main_presenter"
#define worker_exe "main_worker"

LoadBalancer::LoadBalancer(std::string input)
{
	extract_input_data(input);
	file = input;
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
	for (int i = 0; i < prc_cnt; ++i)
	{
		kill(proc_ids[i], SIGKILL);
	}
	kill(presenter_pid, SIGKILL);
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

void LoadBalancer::run()
{
	extract_files_data_in_dir();
	int root = getpid();
	for (int i = 0; i < prc_cnt; ++i)
	{
		bool head = false;
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
				std::string temp (&str[26]);
				res += ((head == false) ? str : temp) + "\n";
				if (head == false)
					head = true;
				//std::string s = "forgeeks.org";
				// Write input string and close writing end of first 
				// pipe. 
			}
			std::cout << res << std::endl;
			close(pipes[i][0]);
			write(pipes[i][1], res.c_str(), res.length());
			close(pipes[i][1]);
			// wait(NULL);
		}else{
			std::string a = std::to_string(pipes[i][0]);
			std::string b = std::to_string(pipes[i][1]);
			char* end = nullptr;
			char* argv_worker [] = {(char*)worker_exe,(char*)(a.c_str()),(char*)(b.c_str()),
					(char*)(fifos[i].c_str()),(char*)(file.c_str()),end};
			execv("main_worker", argv_worker);
		}
	}

	if ((presenter_pid = fork()) == 0)
	{
		std::string a = std::to_string(prc_cnt);
		std::vector<char*> argv_present;
		argv_present.push_back((char*)presenter_exe);
		argv_present.push_back((char*)(a.c_str()));
		for (int i = 0; i < prc_cnt; ++i)
			argv_present.push_back((char*)(fifos[i].c_str()));
		argv_present.push_back((char*)(file.c_str()));
		argv_present.push_back((char*)(nullptr));
		execv("main_presenter", &argv_present[0]);
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