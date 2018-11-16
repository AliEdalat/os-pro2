#ifndef WORKER
#define WORKER

#include <string>
#include <vector>
#include <stdio.h>

#include "search_model.h"

struct laptop
{
	// std::string brand;
	// std::string model;
	// int ram;
	// int hdd;
	// int price;
	std::vector<std::string*>* vals;
	laptop(int size){
		vals = new std::vector<std::string*> (size);
	}
};

class Worker
{
public:
	Worker(){}
	Worker(std::string input);
	void exec(SearchModel& search_model, std::vector<std::string*>* items_);
	void set_input(std::string input, std::string fifo);
	
private:
	std::string file;
	std::string fifo_name;
	std::vector<laptop*> laptops;
	std::vector<laptop*> selected_laptops;
	FILE * fd;

	void extract_items(std::string line, int size);
};
#endif