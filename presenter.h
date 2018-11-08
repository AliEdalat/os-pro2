#ifndef PERESENTER
#define PERESENTER

#include <string>
#include <vector>
#include "sort_model.h"

struct laptop
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
	void present();

private:
	std::vector<laptop*> laptops;
	std::string file;

	void extract_items(std::string line);
	
};
#endif