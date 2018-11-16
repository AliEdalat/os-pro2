#include "presenter.h"
#include <algorithm>
#include <iostream>
#include <sstream>

bool descend_hdd(presenter_laptop* a, presenter_laptop* b){return (a->hdd > b->hdd);}
bool ascend_hdd(presenter_laptop* a, presenter_laptop* b){return (a->hdd < b->hdd);}
bool descend_ram(presenter_laptop* a, presenter_laptop* b){return (a->ram > b->ram);}
bool ascend_ram(presenter_laptop* a, presenter_laptop* b){return (a->ram < b->ram);}
bool descend_price(presenter_laptop* a, presenter_laptop* b){return (a->price > b->price);}
bool ascend_price(presenter_laptop* a, presenter_laptop* b){return (a->price < b->price);}

void Presenter::set_input(std::string input)
{
	file = input;
	std::istringstream f (file);
    std::string line;    
    while (std::getline(f, line)) {
        //std::cout << line << std::endl;
        if (line != "\n")
        	extract_items(line);
    }
}

void Presenter::present(SortModel sort_model)
{
	std::map<std::string, std::string> map = sort_model.get();
	for (std::map<std::string, std::string>::iterator it = map.begin(); it != map.end(); ++it)
	{
		if (it->first == "hdd"){
			if(it->second == "descend"){
				std::cout << "price descend!!" << std::endl;
				std::sort(laptops.begin(), laptops.end(), descend_hdd);
			}else{
				std::sort(laptops.begin(), laptops.end(), ascend_hdd);
			}
			break;
		}
		else if (it->first == "ram"){
			if(it->second == "descend"){
				std::sort(laptops.begin(), laptops.end(), descend_ram);
			}else{
				std::sort(laptops.begin(), laptops.end(), ascend_ram);
			}
			break;
		}
		else if (it->first == "price"){
			if(it->second == "descend"){
				std::cout << "price descend!!" << std::endl;
				std::sort(laptops.begin(), laptops.end(), descend_price);
			}else{
				std::sort(laptops.begin(), laptops.end(), ascend_price);
			}
			break;
		}

	}
	for (int i = 0; i < laptops.size(); ++i)
	{
		std::cout << laptops[i]->brand << " " << laptops[i]->model << " " << laptops[i]->ram << " "
			<< laptops[i]->hdd << " " << laptops[i]->price << std::endl;
	}

}

void Presenter::extract_items(std::string line)
{
	line += " ";
	laptops.push_back(new presenter_laptop());
	int state = 0;
	std::string temp;
	for (int i = 0; i < line.length(); ++i)
	{
		if (line[i] == ' ')
		{
			if (state == 2)
				laptops[laptops.size()-1]->ram = stoi(temp);
			else if (state == 3)
				laptops[laptops.size()-1]->hdd = stoi(temp);
			else if (state == 4)
				laptops[laptops.size()-1]->price = stoi(temp);
			state++;
			temp = std::string();
			continue;
		}

		if (state == 0)
			laptops[laptops.size()-1]->brand.push_back(line[i]);
		else if (state == 1)
			laptops[laptops.size()-1]->model.push_back(line[i]);
		else if (state > 1)
			temp.push_back(line[i]);
	}

	if (laptops[laptops.size()-1]->brand == "" || laptops[laptops.size()-1]->model == "")
	{
	 	laptops.pop_back();
	}
}