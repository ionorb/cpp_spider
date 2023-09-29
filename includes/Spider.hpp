#ifndef SPIDER_HPP
# define SPIDER_HPP

#include <iostream>
#include <map>
#include <vector>
#include <list>
#include <iostream>
#include <sstream>
#include <fstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <algorithm>
#include <unistd.h>

class Spider
{
private:
	//attributes
	const std::string	input_msg[3] = {"enter starting url:", \
										"enter curl from the network monitor on your browser (leave blank if you don\'t know what this is):", \
										"enter the number of pages you want to crawl:"};
	const std::string					curl_command = "curl --compressed ";
	std::string							start_url, cookies;
	size_t								page_limit;
	std::list<std::string>				urls_to_visit, unique_subdomains;
	std::vector<std::string>			visited;
	std::map<std::string, std::string>	pages;

	//methods
	std::string	exec(const char* cmd);
	// static bool	isnt_domain(std::string url);
	void		scrape_urls(std::string page_urls, std::string domain, std::list<std::string>& urls_to_visit);
public:
	//attributes

	//methods
	Spider();
	~Spider();
	Spider(Spider& rhs);
	Spider&	operator = (Spider& rhs);

	std::list<std::string>				extract_unique_subdomains(std::string domain);
	void								search_pages_in_domain(std::string domain);
	std::map<std::string, std::string>	getPages();
};

#endif