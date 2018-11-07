#include "sort_model.h"

void SortModel::set(std::string subject, std::string value)
{
	map[subject] = value;
}

std::map<std::string, std::string> SortModel::get()
{
	return map;
}
