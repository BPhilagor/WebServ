/* ************************************************************************** */
/*                                                     __ __   __             */
/*    WebServ                                         / // / /__ \            */
/*                By: skoulen, bphilago, znichola    / // /_ __/ /            */
/*                                                  /__  __// __/             */
/*                Created: 2023/06/01 23:16:37        /_/  /____/ lausanne.ch */
/*                                                                            */
/* ************************************************************************** */
#ifndef DATA_HPP
# define DATA_HPP

#include <iostream>
#include <string>
// #include <unordered_map>
#include <vector>
#include <functional>
#include <fstream>
#include <utility>
#include <stack> // what does this do?!
#include <sstream>

class Data;

/*
	a pair that makes up the object and it's name,

	fist: server
		second: {
			ip = 192.168.1.1
		}

	or as a property it's
	first: ip = second: obj.content 192.168.1.1
*/
typedef std::pair<std::string, Data> dataObj;

/*
	This class throws excpetions all over the place, it's chaos.
*/
class Data // a = 1, 2, 4, 5
{
private:
	std::string _content;
	std::vector<dataObj> _vecObjs;

	static void read_ifstream(Data &n, std::ifstream &file);

public:
	Data();
	Data(const Data &other);
	~Data();

	const std::string getContent() const;

	int getInt() const;

	const dataObj & getObj(size_t index) const;
	size_t getObjSize() const;

	int count(const std::string &type) const;
	const Data & find(const std::string &type, int n = 0) const;
	Data get(const std::string &type) const;

	Data & operator=(const Data &other);
	// Data & operator[](const std::string node, size_t idx);

	static void readFile(Data &n, const std::string &path);
	static void print(const Data &d, int level = 1);
};

std::ostream &operator<<(std::ostream &os, const dataObj &o);
std::ostream &operator<<(std::ostream &os, const Data &d);

#endif /* DATA_HPP */
