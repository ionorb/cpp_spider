#include "Spider.hpp"

// void	extract_row_from_page(PageDB &table, std::string webpage)
// {
// 	std::string	line, key, value;
// 	std::stringstream ss;
// 	ss << webpage;

// 	while (std::getline(ss, line) && line.find("show-left company-name") == line.npos)
// 		continue ;
// 	while (!line.empty())
// 	{
// 		std::string part = line;
// 		while (std::getline(ss, line) && line.find("show-right") == line.npos)
// 		{
// 			part += line;
// 		}
// 		key = strip_html(part);
// 		part = line;
// 		while (std::getline(ss, line) && line.find("show-left") == line.npos)
// 		{
// 			part += line;
// 		}
// 		value = strip_html(part);
// 		table[key].push_back(value);
// 		std::cout << "ROW:\n" << "key: " << key << "value: " << value << std::endl;
// 	}
// 	// std::cout << "WEBPAGE:\n" << webpage;

// }

struct HtmlNode
{
	std::string tag;  // e.g., "div", "p", "img"
	std::map<std::string, std::string> attributes;  // e.g., "class" => "flex-item"
	std::vector<HtmlNode> children;
};

void printTree(HtmlNode& node, int level)
{
	// Process the current node, e.g., print tag
	for (std::vector<HtmlNode>::iterator child = node.children.begin(); child != node.children.end(); child++) 
	{
		if (level == 0)
		{
			std::cout << "|-- " << "<" << node.tag << " ";
			for (std::map<std::string, std::string>::iterator attribute = node.attributes.begin(); attribute != node.attributes.end(); attribute++)
				std::cout << attribute->first << "=\'" << attribute->second << "\" ";
			std::cout << ">\n";
		}
		else
		{
			for (int i = 0; i < level; i++)
				std::cout << "   ";
			std::cout << "`-- " << "<" << node.tag << " ";
			for (std::map<std::string, std::string>::iterator attribute = node.attributes.begin(); attribute != node.attributes.end(); attribute++)
				std::cout << attribute->first << "=\'" << attribute->second << "\" ";
			std::cout << ">\n";
		}
		traverseDFS(*child);
	}
	// Process post-child logic if needed
}

typedef	std::map< std::string, std::vector<std::string> > PageDB;

std::string	strip_html(std::string part)
{
	int bracket = 0;

	std::string	stripped;
	for (size_t i = 0; i < part.size(); i++)
	{
		if (part[i] == '<') bracket++;
		else if (part[i] == '>') bracket--;
		else if (bracket == 0)
			stripped += part[i];
	}
	return stripped;
}

bool	isNL(char c)
{
	return (c == '\n');
}

void	extract_row_from_page(PageDB &table, std::string webpage)
{
	std::string	line, key, value, page;

	for (size_t i = 0; i < webpage.size(); i++)
	{
		if (webpage[i] != '\n')
			page += webpage[i];
	}
	// std::cout << "WEBPAGE:\n" << page;
	if (page.find('\n') != page.npos)
		std::cerr << "\n\nKFJDLKFJ\n\n";
	size_t i = page.find("show-offer");
	if ((i = page.find("<div class=\'container-flex\'>", i)) == page.npos)
		return ;
	// i = page.find('>', i);
	// int brackets = 1;
	// int level = 0;
	size_t j = 0;
	while (i < page.size() && j < page.size())
	{
		i = page.find('<', i);
		j = page.find('>', i) + 1;
		i = page.find("</", j);
		std::string internal = page.substr(j, i - j);
		std::cout << "INTERNAL:\n" << internal << std::endl;
		if (internal.find("show-left") != internal.npos)
		{
			key = strip_html(internal);
			std::cout << "KEY:\n" << key << std::endl;
		}
		else if (internal.find("show-right") != internal.npos)
		{
			value = strip_html(internal);
			std::cout << "VALUE:\n" << value << std::endl;
			table[key].push_back(value);
		}
	}
	(void)table;
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