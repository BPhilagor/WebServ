/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Data.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: znichola <znichola@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 05:21:02 by znichola          #+#    #+#             */
/*   Updated: 2023/06/12 13:20:17 by znichola         ###   ########.fr       */
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

	int count(const std::string &type);
	Data & find(const std::string &type, int n = 0);
	Data get(const std::string &type);

	Data & operator=(const Data &other);
	// Data & operator[](const std::string node, size_t idx);

	static void readFile(Data &n, const std::string &path);
	static void print(const Data &d, int level = 1);
};

std::ostream &operator<<(std::ostream &os, const dataObj &o);
std::ostream &operator<<(std::ostream &os, const Data &d);

#endif /* DATA_HPP */
