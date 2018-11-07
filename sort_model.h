#ifndef SORT_MODEL
#define SORT_MODEL

#include <string>
#include <map>

class SortModel
{
public:
	SortModel(){}
	void set(std::string subject, std::string value);
	std::map<std::string, std::string> get();

private:
	std::map<std::string, std::string> map;
	
};
#endif