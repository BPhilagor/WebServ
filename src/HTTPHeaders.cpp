/* ************************************************************************** */
/*                                                     __ __   __             */
/*    WebServ                                         / // / /__ \            */
/*                By: skoulen, bphilago, znichola    / // /_ __/ /            */
/*                                                  /__  __// __/             */
/*                Created: 2023/06/01 23:16:37        /_/  /____/ lausanne.ch */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "HTTPHeaders.hpp"
#include "debugDefs.hpp"

HTTPHeaders::HTTPHeaders()
{
	if (DP_12 & DP_MASK) std::cout<< "HTTPHeaders constructor" << std::endl;
}

HTTPHeaders::HTTPHeaders(const HTTPHeaders& h):
	_headers(h._headers)
{
	if (DP_12 & DP_MASK) std::cout<< "HTTPHeaders copy constructor" << std::endl;
}

HTTPHeaders::~HTTPHeaders()
{
	if (DP_12 & DP_MASK) std::cout<< "HTTPHeaders destructor" << std::endl;
}

HTTPHeaders&	HTTPHeaders::operator=(const HTTPHeaders& h)
{
	if (DP_12 & DP_MASK) std::cout << "HTTPHeaders assignement operator" << std::endl;
	_headers = h._headers;
	return (*this);
}

std::string	HTTPHeaders::get(const std::string& name) const
{
	/* name is case-insensitive */
	std::string key = name;
	for (std::string::iterator it = key.begin(); it != key.end(); it++)
	{
		if (std::isalpha(*it))
			*it = std::toupper(*it);
	}

	std::map<std::string, std::string>::const_iterator it = _headers.find(key);
	if (it == _headers.end())
		return ("");
	else
		return (it->second);
}

void	HTTPHeaders::insert(const std::string& name, const std::string& value)
{
	/* name is case-insensitive */
	std::string key = name;
	for (std::string::iterator it = key.begin(); it != key.end(); it++)
	{
		if (std::isalpha(*it))
			*it = std::toupper(*it);
	}

	std::string&	content = _headers[key];
	if (content == "")
		content = value;
	else
		content += ", " + value;

	/* should translate into list first! */
}

void	HTTPHeaders::replace(const std::string& name, const std::string& value)
{
	/* name is case-insensitive */
	std::string key = name;
	for (std::string::iterator it = key.begin(); it != key.end(); it++)
	{
		if (std::isalpha(*it))
			*it = std::toupper(*it);
	}

	_headers[key] = value;
}

std::string	HTTPHeaders::serialize() const
{
	std::string	res;
	for (std::map<std::string, std::string>::const_iterator it = _headers.begin(); it != _headers.end(); it++)
	{
		res += it->first + ": " + it->second + "\r\n";
	}
	return (res);
}

std::ostream&	operator<<(std::ostream& o, const HTTPHeaders& h)
{
	o << h.serialize();
	return o;
}
