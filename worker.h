#ifndef WORKER
#define WORKER

#include <string>
#include <vector>
#include <stdio.h>

#include "search_model.h"

struct laptop
{
	std::string brand;
	std::string model;
	int ram;
	int hdd;
	int price;
};

class Worker
{
public:
	Worker(){}
	Worker(std::string input);
	void exec(SearchModel& search_model);
	void set_input(std::string input, std::string fifo);
	
private:
	std::string file;
	std::string fifo_name;
	std::vector<laptop*> laptops;
	std::vector<laptop*> selected_laptops;
	FILE * fd;

	void extract_items(std::string line);
};
#endif