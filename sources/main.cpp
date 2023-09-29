#include "Spider.hpp"

typedef std::map< std::string, std::vector<std::string> > PageDB;

void	extract_row_from_page(PageDB &table, std::string webpage)
{
	
}

PageDB	extract_table(std::map<std::string, std::string> pages)
{
	PageDB table;

	for (std::map<std::string, std::string>::iterator it = pages.begin(); it != pages.end(); it++)
	{
		table["URL"].push_back(it->first);
		extract_row_from_page(table, it->second);
	}
	return table;
}

int	main(int ac, char **av)
{
	(void)av;
	(void)ac;

	Spider	spider;
	spider.search_pages_in_domain("https://companies.intra.42.fr/en/offers");
	std::list<std::string> unique_subdomains = spider.extract_unique_subdomains("42.fr");

	std::cout << spider.getPages().begin()->second << std::endl;
	std::map< std::string, std::vector<std::string> > table = extract_table(spider.getPages());
	std::cout << "\n---------------UNIQUE SUBDOMAINS--------------\n";
	for (std::list<std::string>::iterator it = unique_subdomains.begin(); it != unique_subdomains.end(); it++)
		std::cout << *it << std::endl;
	std::cout << "----------------------------------------------\n";
	return (0);
}