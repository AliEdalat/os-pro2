#ifndef LOAD_BALANCER
#define LOAD_BALANCER

#include <unistd.h>
#include <string>
#include <vector>

#include "worker.h"
#include "search_model.h"
#include "sort_model.h"

class LoadBalancer
{
public:
	LoadBalancer(std::string input);
	~LoadBalancer();
	void run();

private:
	std::vector<int> proc_ids;
	std::vector<int*> pipes;
	std::vector<std::string> fifos;
	std::vector<std::string*> files;
	std::vector<int> starts;
	std::vector<Worker*> workers;
	int step;
	int start_file;
	int prc_cnt;
	std::string dir;
	SearchModel search_model;
	SortModel sort_model;

	void extract_files_data_in_dir();
	void extract_input_data(std::string input);
};
#endif