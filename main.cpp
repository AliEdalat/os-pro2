#include <iostream>
#include "load_balancer.h"

using namespace std;

int main(int argc, char const *argv[])
{
	LoadBalancer v(3, ".");
	v.run();
	return 0;
}