#include "Spider.hpp"

Spider::Spider()
{
	std::string	input;

	//get input from user
	std::cout << this->input_msg[0] << std::endl;
	std::getline(std::cin, input);
	this->start_url = input;
	std::cout << this->input_msg[1] << std::endl;
	std::getline(std::cin, input);
	this->cookies = input;
	std::cout << this->input_msg[2] << std::endl;
	std::getline(std::cin, input);
	this->page_limit = atoi(input.c_str());

	//process cookies
	size_t n;
	if ((n = this->cookies.find("--compressed")) == this->cookies.npos || n + 12 >= this->cookies.size())
		throw std::runtime_error("Invalid curl");
	this->cookies = this->cookies.substr(n + 12);
	std::cout << "COOKIES:\n" << this->cookies << std::endl;
}

Spider::~Spider()
{
}

Spider::Spider(Spider& rhs)
{
	this->start_url = rhs.start_url;
	this->cookies = rhs.cookies;
	// this->domain = rhs.domain;
	this->pages = rhs.pages;
	this->page_limit = rhs.page_limit;
	this->urls_to_visit = rhs.urls_to_visit;
	this->unique_subdomains = rhs.unique_subdomains;
	this->visited = rhs.visited;
}

Spider&	Spider::operator = (Spider& rhs)
{
	if (this != &rhs)
	{
		this->start_url = rhs.start_url;
		this->cookies = rhs.cookies;
		// this->domain = rhs.domain;
		this->pages = rhs.pages;
		this->page_limit = rhs.page_limit;
		this->urls_to_visit = rhs.urls_to_visit;
		this->unique_subdomains = rhs.unique_subdomains;
		this->visited = rhs.visited;
	}
	return (*this);
}

std::string	Spider::exec(const char* cmd)
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

// bool	Spider::isnt_domain(std::string url) 
// {
// 	return (url.find(this->domain) == url.npos);
// }

struct isnt_domain
{
	isnt_domain(const std::string& a_domain) : domain(a_domain) {}
	std::string domain;
	bool operator()(const std::string& url)
	{
		return (url.find(domain) == url.npos);
	}
};

void	Spider::scrape_urls(std::string page_url, std::string domain, std::list<std::string>& urls_to_visit)
{
	std::string	webpage = exec((this->curl_command + page_url + this->cookies).c_str());
	std::string				line;
	std::stringstream		stream;
	std::string				delims("\'\")< ");
	stream << webpage;
	// std::cout << webpage;

	this->pages[page_url] = webpage;
	while (std::getline(stream, line))
	{
		size_t	current = 0;
		while (current < line.size())
		{
			size_t start = line.find("href=", current);
			if (start != line.npos)
				start += 6;
			size_t n = 0;
			while (delims.find(line[start + n]) == delims.npos 
					&& n != line.npos 
					&& start != line.npos)
				n++;
			if (n == line.npos || start == line.npos)
				break ;
			std::string to_visit = line.substr(start, n);
			if (to_visit.find("/") == 0)
			{
				size_t i = page_url.find(domain);
				if (i != page_url.npos)
					i += domain.size();
				urls_to_visit.push_back(domain.substr(0, i) + to_visit);
			}
			else
				urls_to_visit.push_back(to_visit);
			current = start + n;
		}
	}
	urls_to_visit.unique();
}

std::list<std::string>	Spider::extract_unique_subdomains(std::string domain)
{
	for (size_t i = 0; i < this->visited.size(); i++)
	{
		std::string	subdomain;
		size_t		n = this->visited[i].find(domain) + domain.size();
		this->unique_subdomains.push_back(this->visited[i].substr(0, n));
	}
	this->unique_subdomains.sort();
	this->unique_subdomains.unique();
	return this->unique_subdomains;
}

void	Spider::search_pages_in_domain(std::string domain)
{
	std::string url = this->start_url;

	for (size_t i = 0; i < this->page_limit; i++)
	{
		if (i > 0 && (url.empty() || this->urls_to_visit.empty()))
		{
			std::cout << "\n----------------------------\n"
						<< "ALL PAGES HAVE BEEN SCRAPED!"
						<< "\n----------------------------\n";
			break ;
		}
		scrape_urls(url, domain, this->urls_to_visit);
		this->urls_to_visit.remove_if(isnt_domain(domain));
		this->urls_to_visit.sort();
		this->urls_to_visit.unique();
		this->urls_to_visit.remove(url);
		this->visited.push_back(url);
		while (std::find(this->visited.begin(), this->visited.end(), this->urls_to_visit.front()) != this->visited.end())
			this->urls_to_visit.pop_front();
		// if (this->urls_to_visit.empty())
		// {
		// 	std::cerr << "\nBAD: {" << this->curl_command + this->url + this->cookies << "}\n";
		// 	exit(1);
		// }
		if (!this->urls_to_visit.empty())
			url = this->urls_to_visit.front();
		srand(time(NULL));
		// size_t j = 100 + (rand() % 1000000);
		// std::cout << "j: " << j << std::endl;
		// usleep(j);
	}

	std::cout << "\n------------------SCRAPED URLS----------------\n";
	for (size_t i = 0; i < visited.size(); i++)
		std::cout << visited[i] << std::endl;
	std::cout << "----------------------------------------------\n";
}

std::map<std::string, std::string>	Spider::getPages()
{
	return (this->pages);
}