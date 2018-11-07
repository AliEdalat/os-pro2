#include <iostream>
#include <string>

#include "load_balancer.h"

using namespace std;

int main(int argc, char const *argv[])
{
	std::string input = std::string(argv[1]);
	LoadBalancer vlll (input);
	vlll.run();
	return 0;
}