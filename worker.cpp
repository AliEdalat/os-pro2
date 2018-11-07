#include "worker.h"
#include <sstream>
#include <iostream>
#include <unistd.h>

Worker::Worker(std::string input)
{
	file = input;
}

void Worker::set_input(std::string input)
{
	file = input;
}

void Worker::exec(SearchModel& search_model)
{
	std::istringstream f (file);
    std::string line;    
    while (std::getline(f, line)) {
        //std::cout << line << std::endl;
        if (line != "\n")
        	extract_items(line);
    }

    std::map<std::string, std::string> map = search_model.get();
    std::map<std::string, std::string>::iterator it = map.begin();
    
    try{
    	// std::cout << it->first << "," << it->second << std::endl;
    	// std::cout << getpid() <<"	" << laptops[i]->brand << std::endl;
    	for (; it != map.end(); ++it)
    	{
    		if (it->first == "brand")
    			for (int i = 0; i < laptops.size(); ++i){
	    			if(laptops[i]->brand == it->second){
	    				selected_laptops.push_back(laptops[i]);
	    			}
	    		}
    		// else if (it->first == "model")
    		// 	if(laptops[i]->model != it->second){
    		// 		add = 0;
    		// 		break;
    		// 	}
    		// else if (it->first == "hdd")
    		// 	if(laptops[i]->hdd != stoi(it->second)){
    		// 		add = 0;
    		// 		break;
    		// 	}
    		// else if (it->first == "ram")
    		// 	if(laptops[i]->ram != stoi(it->second)){
    		// 		add = 0;
    		// 		break;
    		// 	}
    		// else if (it->first == "price")
    		// 	if(laptops[i]->price != stoi(it->second)){
    		// 		add = 0;
    		// 		break;
    		// 	}
    	}
    	// std::cout << getpid() << add << std::endl;
    	// if (add)
    	// 	selected_laptops.push_back(laptops[i]);
	}
	catch(...){
		std::cout << "search ???" << std::endl;
	}
	std::cout << "find : " << selected_laptops.size() << std::endl;

}

void Worker::extract_items(std::string line)
{
	line += " ";
	laptops.push_back(new laptop());
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
	/*std::cout << "extract : " << laptops[laptops.size()-1]->brand << " "
			<< laptops[laptops.size()-1]->model << " " << laptops[laptops.size()-1]->ram
			<< " " << laptops[laptops.size()-1]->hdd << " " << laptops[laptops.size()-1]->price << std::endl;*/
	if (laptops[laptops.size()-1]->brand == "" || laptops[laptops.size()-1]->model == "")
	{
	 	laptops.pop_back();
	 	//std::cout << "poped" << std::endl;
	} 
}