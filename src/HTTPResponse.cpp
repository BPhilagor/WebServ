/* ************************************************************************** */
/*                                                     __ __   __             */
/*    WebServ                                         / // / /__ \            */
/*                By: skoulen, bphilago, znichola    / // /_ __/ /            */
/*                                                  /__  __// __/             */
/*                Created: 2023/06/01 23:16:37        /_/  /____/ lausanne.ch */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPResponse.hpp"

#include <iostream>
#include <sstream>

#define DEBUG_PRINT 0

HTTPResponse::HTTPResponse()
{
	if (DEBUG_PRINT) std::cout << "HTTPResponse constructor" << std::endl;
}

HTTPResponse::HTTPResponse(const HTTPResponse& h):
	_version(h._version),
	_code(h._code),
	_reason(h._reason),
	_headers(h._headers),
	_body(h._body)
{
	if (DEBUG_PRINT) std::cout << "HTTPResponse copy constructor" << std::endl;
}

HTTPResponse::~HTTPResponse()
{
	if (DEBUG_PRINT) std::cout << "HTTPResponse destructor" << std::endl;
}

HTTPResponse&	HTTPResponse::operator=(const HTTPResponse& h)
{
	if (DEBUG_PRINT) std::cout << "HTTPResponse assignement operator" << std::endl;

	_version = h._version;
	_code = h._code;
	_reason = h._reason;
	_headers = h._headers;
	_body = h._body;

	return (*this);
}

/* getters */

t_version		HTTPResponse::getVersion() const
{
	return _version;
}

int				HTTPResponse::getCode() const
{
	return _code;
}

std::string		HTTPResponse::getReason() const
{
	return _reason;
}

const HTTPHeaders&	HTTPResponse::getHeaders() const
{
	return _headers;
}

std::string		HTTPResponse::getBody() const
{
	return _body;
}

/* setters */

void	HTTPResponse::setVersion(int major, int minor)
{
	_version.major = major;
	_version.minor = minor;
}

void	HTTPResponse::setCode(int code)
{
	_code = code;
}

void	HTTPResponse::setReason(std::string reason)
{
	_reason = reason;
}

void	HTTPResponse::setHeader(std::string name, std::string value)
{
	_headers.insert(name, value);
}

void	HTTPResponse::setBody(std::string body)
{
	_body = body;
}

std::string	HTTPResponse::serialize() const
{
	std::ostringstream	output;

	output << "HTTP/" << _version.major << "." << _version.minor << " " << _code << " " << _reason << "\r\n";
	output << _headers.serialize();
	output << "\r\n";
	output << _body;

	return (output.str());
}

std::ostream&	operator<<(std::ostream& o, const HTTPResponse& h)
{
	o << h.serialize();
	return (o);
}
