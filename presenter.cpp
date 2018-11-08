#include "presenter.h"

void Presenter::set_input(std::string input)
{
	file = input;
}

void Presenter::present()
{

}

void Presenter::extract_items(std::string line)
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