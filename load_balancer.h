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
	int prc_cnt;
	std::string dir;
};
#endif