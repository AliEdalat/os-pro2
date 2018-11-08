#ifndef PERESENTER
#define PERESENTER

#include <string>
#include <vector>
#include "sort_model.h"

struct presenter_laptop
{
	std::string brand;
	std::string model;
	int ram;
	int hdd;
	int price;
};

class Presenter
{
public:
	Presenter(){}
	void set_input(std::string input);
	void present(SortModel sort_model);

private:
	std::vector<presenter_laptop*> laptops;
	std::string file;

	void extract_items(std::string line);
	
};
#endif