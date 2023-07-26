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
#include "debugDefs.hpp"

#include <iostream>
#include <sstream>

std::map<int, std::string> HTTPResponse::_reasonMap = _initialiseReasonMap();

HTTPResponse::HTTPResponse():
	_code(0)
{
	if (DP_12 & DP_MASK) std::cout << "HTTPResponse constructor" << std::endl;
}

HTTPResponse::HTTPResponse(const HTTPResponse& h):
	_version(h._version),
	_code(h._code),
	_reason(h._reason),
	_headers(h._headers),
	_body(h._body)
{
	if (DP_12 & DP_MASK) std::cout << "HTTPResponse copy constructor" << std::endl;
}

HTTPResponse::~HTTPResponse()
{
	if (DP_12 & DP_MASK) std::cout << "HTTPResponse destructor" << std::endl;
}

HTTPResponse&	HTTPResponse::operator=(const HTTPResponse& h)
{
	if (DP_12 & DP_MASK) std::cout << "HTTPResponse assignement operator" << std::endl;

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
	construct the reply for an error, if srv is spesified we
	check it for the error page directory
*/
void	HTTPResponse::constructErrorReply(int code, const Server *srv)
{
	setCode(code);
	setHeader("Content-type", "text/html");

	if (srv == NULL)
		setBody(genPage(code));
	else
		setBody(getErrorPage(*srv, code));

	setHeader("Content-length", SSTR(getBody().size()));
}

void	HTTPResponse::constructRedirect(t_redir redir)
{
	if (redir.type == "temporary")
		setCode(302);
	else
		setCode(301);

	setHeader("Location", redir.location);

	//set a body in case auto-redirect is not activated in the browser

	std::string	body = "<html><head><title>" + SSTR(getCode()) + " " + _reasonMap[getCode()] +"</title></head><body>Please follow <a href=\"" + redir.location + "\">this link</a><body></html>";
	setBody(body);
	setHeader("Content-length", SSTR(body.size()));
	setHeader("Content-type", "text/html");
}

void	HTTPResponse::serveStaticFile(const std::string& path)
{
	std::string body;
	std::string ext = utils::getFileExtension(path);
	std::string mime = getMimeFromExtension(ext);
	utils::getFile(path, body);
	setBody(body);
	setHeader("content-type", mime);
	setHeader("content-length", SSTR(body.size()));
	setCode(200);
}

void	HTTPResponse::serveDynamicFile(const Location& location, const std::string& path, const HTTPRequest& request)
{
	std::string	cgi_response;
	if (!launchCGI(location, request, location.getCGIpath(path), path, cgi_response))
		setCode(500);
	else
		parseCGIResponse(cgi_response); /* this sets the res code */
}

void	HTTPResponse::parseCGIResponse(std::string cgi_body)
{
	std::string			line;
	std::istringstream	input(cgi_body);

	setCode(200);

	while (true)
	{
		std::getline(input, line);
		utils::sanitizeline(line);

		if (line == "")
			break ;

		/* process header here */
		std::pair<std::string, std::string> header;
		utils::parseHeader(line, header);

		if (utils::streq_ci(header.first, "content-type"))
		{
			/* i use 'replace' because i don't know yet at which stage this function will be used */
			_headers.replace("content-type", header.second);
		}
		else if (utils::streq_ci(header.first, "status"))
		{
			int code = atoi(header.second.c_str());
			setCode(code);
		}
		else if (utils::streq_ci(header.first, "location"))
		{
			/* handle Location header, this could imply reprocessing the request with the request worker!*/
		}
		else
		{
			_headers.insert(header.first, header.second);
		}
	}

	/* get what remains of the cgi_body and put it in the body */
	if (input.tellg() > 0)
	{
		_body = input.str().substr(input.tellg());
		setHeader("Content-length", SSTR(_body.size()));
	}
}

void	HTTPResponse::finalize()
{
	int code = getCode();
	setVersion(1, 1);
	setReason(_reasonMap[code]);
	setDate();
	setHeader("Server", "Webserv");
	if (code == 400 || code == 413 || code == 414 || code >= 500)
		setHeader("Connection", "close");
	else
		setHeader("Connection", "keep-alive");
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
	case ws_file_no_perm:
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
