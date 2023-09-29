#include "Spider.hpp"

int	main(int ac, char **av)
{
	(void)av;
	(void)ac;

	Spider	spider;
	spider.search_pages_in_domain("https://companies.intra.42.fr/en/offers");
	std::list<std::string> unique_subdomains = spider.extract_unique_subdomains("42.fr");

	std::cout << spider.getPages().begin()->second << std::endl;
	std::cout << "\n---------------UNIQUE SUBDOMAINS--------------\n";
	for (std::list<std::string>::iterator it = unique_subdomains.begin(); it != unique_subdomains.end(); it++)
		std::cout << *it << std::endl;
	std::cout << "----------------------------------------------\n";
	return (0);
}