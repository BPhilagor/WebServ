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
	std::string				_content;
	std::vector<dataObj>	_vecObjs;
	std::string				_config_file_path; //used to resolve relative paths

	// parse in config file
	static void read_ifstream(Data &n, std::ifstream &file);

public:
	Data();
	Data(const Data &other);
	~Data();

	// parse in config file
	static void readFile(Data &n, const char *path);

	void setProp(const std::string &ident, const std::string &content);
	void pushBack(dataObj &o);
	void setContent(const std::string &s);

	// accessors
	const Data &		find(const std::string &type, int n = 0) const;
	Data				get(const std::string &type, int depth = 0) const;
	int 				count(const std::string &type) const;
	int					getInt() const;
	const std::string	getContent() const;
	const std::string	&getContentRef() const;
	const dataObj & 	getObj(size_t index) const;
	size_t				getObjSize() const;
	const std::string	&getConfigFilePath() const;
	void				setConfigFilePath(const char *path);

	// opperators and print functions
	Data & operator=(const Data &other);
	static void print(const Data &d, int level = 1);


};

std::ostream &operator<<(std::ostream &os, const dataObj &o);
std::ostream &operator<<(std::ostream &os, const Data &d);

#endif /* DATA_HPP */
