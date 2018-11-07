#ifndef SEARCH_MODEL
#define SEARCH_MODEL

#include <string>
#include <map>

class SearchModel
{
public:
	SearchModel(){}
	void set(std::string subject, std::string value);
	std::map<std::string, std::string> get();

private:
	std::map<std::string, std::string> map;
	
};
#endif