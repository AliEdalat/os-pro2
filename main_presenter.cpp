#include "presenter.h"
#include "sort_model.h"
#include <vector>
#include <string>
#include <unistd.h>
#include <iostream>
#include <fcntl.h>

SortModel extract_input_data(std::string line)
{
	line += "-";
	int state = 0;
	std::string subject;
	std::string value;
	SortModel sort_model;
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
				;
			else
				sort_model.set(subject, value);
			state = 0;
			subject = std::string();
			continue;
		}

		if (state == 0)
			subject.push_back(line[i]);
		else if (state == 1)
			value.push_back(line[i]);
	}
	return sort_model;
}

int main(int argc, char* argv[])
{
	int prc_cnt = atoi(argv[1]);
	std::vector<bool> presented(prc_cnt,false);
	int end = 0;
	Presenter presenter;
	while(true)
	{
		for (int i = 0; i < prc_cnt; ++i)
		{
			char str1[5000];
			int fd1 = open(argv[i+2],O_RDONLY); 
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
	SortModel sort_model = extract_input_data(argv[prc_cnt+2]);
	presenter.present(sort_model);
	return 0;
}