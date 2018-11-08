#include "worker.h"
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>


Worker::Worker(std::string input)
{
	file = input;
}

void Worker::set_input(std::string input, std::string fifo)
{
	file = input;
	fifo_name = fifo;
	fd = fopen(fifo_name.c_str(), "w"); 
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
    	for (int i = 0; i < laptops.size(); ++i){
	    	bool selecte = true;
	    	for (std::map<std::string, std::string>::iterator it = map.begin(); it != map.end(); ++it)
	    	{
	    		// std::cout << it->first << std::endl;
	    		if (it->first == "brand"){
	    			if(laptops[i]->brand == it->second && selecte){
	    				// std::cout << "match" << std::endl;
	    				selecte = true;
	    			}else{
	    				// std::cout << "not match" << std::endl;
	    				selecte = false;
	    				break;
	    			}
		    	}
	    		else if (it->first == "model"){
	    			if(laptops[i]->brand == it->second && selecte){
	    				// std::cout << "match" << std::endl;
	    				selecte = true;
	    			}else{
	    				// std::cout << "not match" << std::endl;
	    				selecte = false;
	    				break;
	    			}
	    		}
	    		else if (it->first == "hdd"){
	    			if(laptops[i]->brand == it->second && selecte){
	    				// std::cout << "match" << std::endl;
	    				selecte = true;
	    			}else{
	    				// std::cout << "not match" << std::endl;
	    				selecte = false;
	    				break;
	    			}
	    		}
	    		else if (it->first == "ram"){
	    			if(laptops[i]->brand == it->second && selecte){
	    				// std::cout << "match" << std::endl;
	    				selecte = true;
	    			}else{
	    				// std::cout << "not match" << std::endl;
	    				selecte = false;
	    				break;
	    			}
	    		}
	    		else if (it->first == "price"){
	    			if(laptops[i]->brand == it->second && selecte){
	    				// std::cout << "match" << std::endl;
	    				selecte = true;
	    			}else{
	    				// std::cout << "not match" << std::endl;
	    				selecte = false;
	    				break;
	    			}
	    		}
	    	}
	    	if (selecte == false){
	    		// std::cout << "continue" << std::endl;
	    		continue;
	    	}
	    	// std::cout << "selecte" << std::endl;
	    	selected_laptops.push_back(laptops[i]);
	    }
	    for (int i = 0; i < selected_laptops.size(); ++i)
	    {
	    	fprintf(fd, "%s %s %d %d %d\n", selected_laptops[i]->brand.c_str(), selected_laptops[i]->model.c_str(),
	    		selected_laptops[i]->ram, selected_laptops[i]->hdd, selected_laptops[i]->price);
	    }
        fclose(fd); 
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