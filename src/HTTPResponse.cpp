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

std::string		HTTPResponse::getHeader(const std::string& name) const
{
	return _headers.get(name);
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

// TODO: rename to genPage
std::string HTTPResponse::genPage(int code) const
{
	std::string page("<!DOCTYPE html><html><head><title>EEE RRR</title></head><body><h1>EEE RRR</h1></body></html>\n");
	page.replace(page.find("EEE"), 3, SSTR(code));
	page.replace(page.find("EEE"), 3, SSTR(code));
	page.replace(page.find("RRR"), 3, _reasonMap[code]);
	page.replace(page.find("RRR"), 3, _reasonMap[code]);
	return page;
}

/*
	if body is null we don't set this property
*/
void HTTPResponse::constructReply(int code, const std::string *body)
{
	setVersion(1, 1);
	setDate();
	setReason(_reasonMap[code]);
	setCode(code);
	setHeader("Server", "WebServ");
	setHeader("Content-type", "text/html");
	setHeader("Connection", "keep-alive");
	if (body != NULL)
		setBody(*body);
	else
		setBody(genPage(code));
	setHeader("content-length", SSTR(getBody().size()));
}

/*
	construct the reply for an error, if srv is spesified we
	check it for the error page directory
*/
void	HTTPResponse::constructErrorReply(int code, const Server *srv)
{
	setVersion(1, 1);
	setCode(code);
	setReason(_reasonMap[code]);
	setDate();
	setHeader("Server", "Webserv");
	setHeader("Content-type", "text/html");

	if (code == 400 || code == 413 || code == 414) /* need to make this more elegant like have a const array */
		setHeader("Connection", "close");
	else
		setHeader("Connection", "keep-alive");

	std::cout << srv << std::endl;

	if (srv == NULL)
		setBody(genPage(code));
	else
	{
		srv->getErrorDir();
		setBody(getErrorPage(*srv, code)); // but for now we cheat it
	}

	setHeader("Content-length", SSTR(getBody().size()));
}

std::string HTTPResponse::getErrorPage(const Server &srv, int code) const
{
	std::string path = srv.getErrorDir() + SSTR(code) + ".html";
	std::string body;
	switch (utils::getFile(path, body))
	{
	case ws_file_found:
		// all good nothing special to do
		break;
	case ws_file_not_found:
		// fallthrough
	case ws_no_permission:
		// fallthrough
	default:
		body = genPage(code);
	}
	return body;
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
