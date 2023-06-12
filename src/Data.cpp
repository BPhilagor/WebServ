/* ************************************************************************** */
/*                                                     __ __   __             */
/*    WebServ                                         / // / /__ \            */
/*                By: skoulen, bphilago, znichola    / // /_ __/ /            */
/*                                                  /__  __// __/             */
/*                Created: 2023/06/01 23:16:37        /_/  /____/ lausanne.ch */
/*                                                                            */
/* ************************************************************************** */

#include <cstdlib>
#include <sstream>

#include "utils.hpp"
#include "Data.hpp"

static void trim_outside_whitespace(std::string &line);
static void split_around_first_space(const std::string src, std::string &s1, std::string &s2);
static void print_Data(const Data &d, int level);
static void print_dataObj(const dataObj &o, int level);

Data::Data() {}

Data::Data(const Data &other)
	: _content(other._content), _vecObjs(other._vecObjs) {}

Data::~Data() {}

Data &Data::operator=(const Data &other)
{
	_content = other._content;
	_vecObjs = other._vecObjs;
	return *this;
}

void Data::read_ifstream(Data &n, std::ifstream &file)
{
	while(!file.eof())
	{
		size_t x;
		std::string line;
		std::getline(file, line); //needs protection?
		trim_outside_whitespace(line);

		// # to ignore comments
		if (line.empty() || line.at(0) == '#')
			continue;

		// check if it's an assigment ie. ip = 192.168.1.1
		if ((x = line.find_first_of('=')) != std::string::npos)
		{
			dataObj node;
			node.first = line.substr(0, x);
			trim_outside_whitespace(node.first);
			node.second._content = line.substr(x + 1, line.length());
			trim_outside_whitespace(node.second._content);
			n._vecObjs.push_back(node);
			continue;
		}
		// check if it's an object ie. location /home {
		else if ((x = line.find_first_of('{')) != std::string::npos)
		{
			dataObj node;
			split_around_first_space(line.substr(0, x), node.first, node.second._content);
			trim_outside_whitespace(node.first);           // location
			trim_outside_whitespace(node.second._content); // /home
			read_ifstream(node.second, file);
			n._vecObjs.push_back(node);
			continue;
		}
		// check if it's the end of an object ie. }
		else if ((x = line.find_first_of('}')) != std::string::npos)
			return ;
	}
}

void Data::readFile(Data &n, const std::string &path)
{
	(void)n;
	std::ifstream file(path.c_str());
	if (!file.is_open())
	{
		std::cerr << "can't read file: " << path << std::endl;
		return ;
	}

	while(!file.eof())
	{
		read_ifstream(n, file);
	}
}

#pragma region Getters

int Data::count(const std::string &type) const
{
	int count = 0;
	for (size_t i = 0; i < getObjSize(); i++)
		if (_vecObjs.at(i).first == type)
			count++;
	return count;
}

const Data & Data::find(const std::string &type, int n) const
{
	int count = -1;
	for (size_t i = 0; i < getObjSize(); i++)
	{
		if (_vecObjs.at(i).first == type)
			count++;
		if (count == n)
			return _vecObjs.at(i).second;
	}
	throw(500);
}

Data Data::get(const std::string &type) const
{
	Data ret;
	for (size_t i = 0; i < getObjSize(); i++)
		if (_vecObjs.at(i).first == type)
			ret._vecObjs.push_back(_vecObjs.at(i));
	return ret;
}


const std::string Data::getContent() const { return _content; }

int Data::getInt() const
{
	int ret = 0;
	// std::cout << "herewr" << ret << "\n";
	std::stringstream ss;
	ss << getContent();
	ss >> ret;
	if (ss.fail())
		return 8080;
	return ret;
}

// returns the object at index or throw excpetion
const dataObj &Data::getObj(size_t index) const { return _vecObjs.at(index); }

size_t Data::getObjSize() const { return _vecObjs.size(); }

#pragma endregion
#pragma region os operator overloads and print functions


std::ostream &operator<<(std::ostream &os, const dataObj &o)
{
	std::string glue = o.second.getObjSize() ? " " : " = ";
	os << o.first << glue << o.second;
	return os;
}

std::ostream &operator<<(std::ostream &os, const Data &d)
{
	size_t n = d.getObjSize();

	os << d.getContent();
	if (n != 0)
		os << " {";
	for (size_t i = 0; i < n; i++)
		os << "\n" << d.getObj(i);
	if (n != 0)
		os << "\n}";
	return os;
}

void Data::print(const Data &d, int level)
{
	if (level < 1)
		level = 1;
	print_Data(d, level);
	std::cout << "\n this that\n";
	std::cout << std::endl;
}

static void print_dataObj(const dataObj &o, int level)
{
	std::string indent(level, ' ');
	std::string glue = o.second.getObjSize() ? " " : " = ";
	std::cout << indent << o.first << glue;
	print_Data(o.second, level + 1);
}

static void print_Data(const Data &d, int level)
{
	std::string indent(level -1, ' ');
	size_t n = d.getObjSize();

	std::cout << d.getContent();
	if (n != 0)
		std::cout << " {";
	for (size_t i = 0; i < n; i++)
	{
		std::cout << "\n";
		print_dataObj(d.getObj(i), level + 1);
	}
	if (n != 0)
		std::cout << "\n" << indent << "}";
}

#pragma endregion
#pragma region Static helper functions

static void trim_outside_whitespace(std::string &line)
{
	size_t start = line.find_first_not_of("\t\n\v\f\r ");
	size_t end = line.find_last_not_of("\t\n\v\f\r ");
	if (start == std::string::npos || end == std::string::npos)
		return ;
	line = line.substr(start, end - start + 1);
	// std::cout << "\"" << line << "\"\n";
}

static void split_around_first_space(const std::string src, std::string &s1, std::string &s2)
{
	size_t x = src.find_first_of(' ');
	if ( x == std::string::npos)
	{
		s1 = src;
		s2 = "";
	}
	else
	{
		s1 = src.substr(0, x);
		s2 = src.substr(x + 1, src.length());
	}
}

#pragma endregion
