
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

std::string	exec(const char* cmd)
{
	char buffer[128];
	std::string result = "";
	FILE* pipe = popen(cmd, "r");
	if (!pipe)
		throw std::runtime_error("popen() failed!");
	try
	{
		while (fgets(buffer, sizeof buffer, pipe) != NULL)
			result += buffer;
	}
	catch (...)
	{
		pclose(pipe);
		throw ;
	}
	pclose(pipe);
	return result;
}

bool	isnt_domain(std::string url) {
	return (url.find("42.fr") == url.npos);
}

void	scrape_url(std::string exec_command, std::list<std::string>& url_list)
{
	std::string	webpage = exec(exec_command.c_str());
	std::string				line;
	std::stringstream		stream;
	std::string				delims("\'\")< ");
	stream << webpage;
	// std::cout << webpage;

	while (std::getline(stream, line))
	{
		size_t	current = 0;
		while (current < line.size())
		{
			size_t start = line.find("https://", current);
			size_t n = 0;
			while (delims.find(line[start + n]) == delims.npos && n != line.npos && start != line.npos)
				n++;
			if (n == line.npos || start == line.npos)
				break ;
			url_list.push_back(line.substr(start, n));
			current = start + n;
		}
	}
}

int	main(int ac, char **av)
{
	(void)av;
	if (ac == 2 || ac == 3)
	{
		std::list<std::string>		url_list;
		std::vector<std::string>	visited;
		std::list<std::string>		unique_subdomains;

		std::string	curl_command("curl --compressed ");
		std::string	url(av[1]);
		std::string	cookies(" ");
		if (ac == 3)
			cookies.append(av[2]);
		
		for (size_t i = 0; i < 100; i++)
		{
			if (url.empty() || url_list.empty())
				std::cout << "\n----------------------------\n"
						  << "ALL PAGES HAVE BEEN SCRAPED!"
						  << "\n----------------------------\n";
			scrape_url(curl_command + url + cookies, url_list);
			url_list.remove_if(isnt_domain);
			url_list.sort();
			url_list.unique();
			url_list.remove(url);
			visited.push_back(url);
			while (std::find(visited.begin(), visited.end(), url_list.front()) != visited.end())
				url_list.pop_front();
			url = url_list.front();
			srand(time(NULL));
			size_t j = 100 + (rand() % 1000000);
			std::cout << "j: " << j << std::endl;
			usleep(j);
		}

		std::cout << "\n------------------SCRAPED URLS----------------\n";
		for (size_t i = 0; i < visited.size(); i++)
			std::cout << visited[i] << std::endl;
		std::cout << "----------------------------------------------\n";

		for (size_t i = 0; i < visited.size(); i++)
		{
			std::string	subdomain;
			size_t n = visited[i].find("42.fr") + 5;
			unique_subdomains.push_back(visited[i].substr(0, n));
		}
		unique_subdomains.sort();
		unique_subdomains.unique();
		std::cout << "\n---------------UNIQUE SUBDOMAINS--------------\n";
		for (std::list<std::string>::iterator it = unique_subdomains.begin(); it != unique_subdomains.end(); it++)
			std::cout << *it << std::endl;
		std::cout << "----------------------------------------------\n";
	}
	else
		std::cerr << "please add input url to scrape as first argument\n";
	return (0);
}