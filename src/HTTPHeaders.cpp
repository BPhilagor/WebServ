#include "HTTPHeaders.hpp"
#include <iostream>

HTTPHeaders::HTTPHeaders()
{
	std::cout<< "HTTPHeaders constructor" << std::endl;
}

HTTPHeaders::HTTPHeaders(const HTTPHeaders& h):
	_headers(h._headers)
{
	std::cout<< "HTTPHeaders copy constructor" << std::endl;
}

HTTPHeaders::~HTTPHeaders()
{
	std::cout<< "HTTPHeaders destructor" << std::endl;
}

HTTPHeaders&	HTTPHeaders::operator=(const HTTPHeaders& h)
{
	std::cout << "HTTPHeaders assignement operator" << std::endl;
	_headers = h._headers;
	return (*this);
}

void	HTTPHeaders::insert(const std::string& name, const std::string& value)
{
	/* should be case insensitive !*/
	std::string&	content = _headers[name];
	if (content == "")
		content = value;
	else
		content += ", " + value;

	/* should translate into list first! */
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
