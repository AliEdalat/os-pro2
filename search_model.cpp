#include "search_model.h"

void SearchModel::set(std::string subject, std::string value)
{
	map[subject] = value;
}

std::map<std::string, std::string> SearchModel::get()
{
	return map;
}
