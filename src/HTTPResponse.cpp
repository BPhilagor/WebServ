/* ************************************************************************** */
/*                                                     __ __   __             */
/*    WebServ                                         / // / /__ \            */
/*                By: skoulen, bphilago, znichola    / // /_ __/ /            */
/*                                                  /__  __// __/             */
/*                Created: 2023/06/01 23:16:37        /_/  /____/ lausanne.ch */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPResponse.hpp"
#include "utils.hpp"

#include <iostream>
#include <sstream>

#define DEBUG_PRINT 0

std::map<int, std::string> HTTPResponse::_reasonMap = _initialiseReasonMap();

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

void	HTTPResponse::setReason(const std::string &reason)
{
	_reason = reason;
}

void	HTTPResponse::setHeader(const std::string &name, const std::string &value)
{
	_headers.insert(name, value);
}

void	HTTPResponse::setBody(const std::string &body)
{
	_body = body;
}

/*
RFC 7231 section 7.1.1.1.  Date/Time Formats
	https://datatracker.ietf.org/doc/html/rfc7231#section-7.1.1.1

An example of the preferred format is
	Sun, 06 Nov 1994 08:49:37 GMT    ; IMF-fixdate
IMF-fixdate  = day-name "," SP date1 SP time-of-day SP GMT
	; fixed length/zone/capitalization subset of the format
	; see Section 3.3 of [RFC5322]
*/
void HTTPResponse::setDate()
{
	time_t rawtime;
	struct tm * timeinfo;
	char buffer [30]; // i think it's never more than 20, .. but 10 bytes enh?

	time (&rawtime);
	timeinfo = localtime (&rawtime);

	strftime (buffer,80,"%a, %m %H:%M:%S GMT",timeinfo);

	setHeader("Date", std::string(buffer));
}

std::string HTTPResponse::genErrorPage(int code) const
{
	std::string page("<!DOCTYPE html><html><head><title>EEE RRR</title></head><body><h1>EEE RRR</h1></body></html>\n");
	page.replace(page.find("EEE"), 3, SSTR(code));
	page.replace(page.find("EEE"), 3, SSTR(code));
	page.replace(page.find("RRR"), 3, _reasonMap[code]);
	page.replace(page.find("RRR"), 3, _reasonMap[code]);
	return page;
}

void HTTPResponse::constructReply(const Data &server, const std::string &body, int code)
{
	(void) body;
	setVersion(1, 1);
	setDate();
	// if (code >= 100 && code < 200)
	// 	informationalResponse(server, code);
	// else if (code >= 200 && code < 300)
	// 	successfulResponse(server, code);
	// else if (code >= 300 && code < 400)
	// 	redirectionMessage(server, code);
	// else if (code >= 400 && code < 500)
	// 	clientErrorResponse(server, code);
	// else if (code >= 500 && code < 600)
	// 	clientErrorResponse(server, code);
	// else
	// 	std::cerr << "Stupid programmer, error code" << code << " is wack\n";
	(void)server;
	setReason(_reasonMap[code]);
	setCode(code);
	setHeader("host", "WebServ");
	// if error
	// 	if error file in config 
	// 		setBody(genErrorPage(code));
	// else

	setHeader("content-length", SSTR(getBody().size()));
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
