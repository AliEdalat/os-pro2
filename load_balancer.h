#ifndef LOAD_BALANCER
#define LOAD_BALANCER

#include <unistd.h>
#include <string>
#include <vector>

class LoadBalancer
{
public:
	LoadBalancer(int prc_cnt_input, std::string dir_input);
	void run();
	// ~LoadBalancer();

private:
	std::vector<int> proc_ids;
	std::vector<int*> pipes;
	std::vector<std::string*> files;
	int step;
	int start_file;
	int prc_cnt;
	std::string dir;

	void extract_files_data_in_dir();
};
#endif