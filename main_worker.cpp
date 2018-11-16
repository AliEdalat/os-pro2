#include "worker.h"
#include "search_model.h"
#include <string>
#include <unistd.h>
#include <iostream>

void extract_pipe(int* pipes,char** argv)
{
	pipes[0] = atoi(argv[1]);
	pipes[1] = atoi(argv[2]);
}

SearchModel extract_input_data(std::string line)
{
	line += "-";
	int state = 0;
	std::string subject;
	std::string value;
	SearchModel search_model;
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
				;
			else if (subject == "dir")
				;
			else if (value != "descend" && value != "ascend")
				search_model.set(subject, value);
			else
				;
			state = 0;
			subject = std::string();
			continue;
		}

		if (state == 0)
			subject.push_back(line[i]);
		else if (state == 1)
			value.push_back(line[i]);
	}
	return search_model;
}

int main(int argc, char* argv[])
{
	// std::cout << argv[0] << " "<< argv[1] << " "<< argv[2] << " "<< argv[3] << " "<< argv[4] << " " << std::endl;
	int pipes[2];
	extract_pipe(pipes, argv);
	// std::cout << getpid() << " " << atoi(argv[1]) << " " << atoi(argv[2]) << std::endl;
	Worker* worker = new Worker();
	close(pipes[1]);  // Close writing end of first pipe 
	// Read a string using first pipe 
	std::string concat_str;
	concat_str.resize(5000);
	int val = read(pipes[0], &concat_str[0], 5000);
	// std::cout<< "okkkkkkkeyy?????" << std::endl;
	concat_str[val] = '\0';
	std::string input = std::string(concat_str);
	// std::cout << input << std::endl;
	worker->set_input(input, std::string(argv[3]));
	SearchModel search_model = extract_input_data(std::string(argv[4]));
	worker->exec(search_model);
	//exit(0);
	return 0;
}