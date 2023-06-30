/* ************************************************************************** */
/*                                                     __ __   __             */
/*    WebServ                                         / // / /__ \            */
/*                By: skoulen, bphilago, znichola    / // /_ __/ /            */
/*                                                  /__  __// __/             */
/*                Created: 2023/06/01 23:16:37        /_/  /____/ lausanne.ch */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPRequest.hpp"
#include "utils.hpp"
#include "debugDefs.hpp"
#include <iostream>
#include <sstream>
#include <map>

std::istream &operator>>(std::istream &is, char const *s);

HTTPRequest::HTTPRequest():
	_valid_syntax(true),
	_method(0),
	_headers(),
	_body(""),
	_state(0),
	_current_line(""),
	_isBodyCGIgenerated(false)
{
	if (DP_12 & DP_MASK) std::cout << "HTTPRequest default constructor" << std::endl;
}

HTTPRequest::HTTPRequest(const HTTPRequest &cpy):
	_valid_syntax(cpy._valid_syntax),
	_method(cpy._method),
	_uri(cpy._uri),
	_version(cpy._version),
	_headers(cpy._headers),
	_body(cpy._body),
	_state(cpy._state),
	_current_line(cpy._current_line),
	_isBodyCGIgenerated(cpy._isBodyCGIgenerated)
{
	if (DP_12 & DP_MASK) std::cout << "HTTPRequest copy constructor" << std::endl;
}

HTTPRequest::~HTTPRequest()
{
	if (DP_12 & DP_MASK) std::cout << "HTTPRequest destructor" << std::endl;
}

HTTPRequest &HTTPRequest::operator=(const HTTPRequest &rhs)
{
	if (DP_12 & DP_MASK) std::cout << "HTTPRequest assignment operator" << std::endl;

	_valid_syntax = rhs._valid_syntax;
	_method = rhs._method;
	_uri = rhs._uri;
	_version = rhs._version;
	_headers = rhs._headers;
	_body = rhs._body;
	_state = rhs._state;
	_current_line = rhs._current_line;
	_isBodyCGIgenerated = rhs._isBodyCGIgenerated;

	return (*this);
}

/* getters */

bool					HTTPRequest::hasValidSyntax() const
{
	return _valid_syntax;
}

t_version				HTTPRequest::getVersion() const
{
	return _version;
}

struct s_uri			HTTPRequest::getURI() const
{
	return _uri;
}

t_methods_mask			HTTPRequest::getMethod() const
{
	return _method;
}

std::string				HTTPRequest::getHeader(const std::string& name) const
{
	return _headers.get(name);
}

std::string				HTTPRequest::getBody() const
{
	return _body;
}

bool					HTTPRequest::isParsingHeadersFinished() const
{
	return (!_valid_syntax || _state >= 2);
}

bool					HTTPRequest::isParsingBodyFinished() const
{
	return (!_valid_syntax || _state == 3);
}

bool					HTTPRequest::isBodyCGIgenerated() const
{
	return _isBodyCGIgenerated;
}

/* setters */

void					HTTPRequest::setBodyCGIgenerated(bool value)
{
	_isBodyCGIgenerated = value;
}

void	HTTPRequest::addChar(char c)
{
	if (_state == 2) /* might need to add body */
	{
		_body += c;
		int length = atoi(getHeader("Content-length").c_str());
		if (_body.length() == (unsigned int) length)
			_state = 3; /* parsing of body terminated */
	}
	else
	{
		if (c == '\n')
		{
			utils::sanitizeline(_current_line);
			if (_current_line == "") /* empty line found */
			{
				if (_state == 1) /* we have already parsed the request line, we're now in the header state */
					_state = 2; /* state 2 means headers finished, might need to add a body */
				else
					_valid_syntax = false;
			}
			else
			{
				if (parseLine(_current_line) != 0)
				{
					_valid_syntax = false;
				}
				_current_line = "";
			}
		}
		else
		{
			_current_line += c;
		}
	}
}

/* serialize */
std::string	HTTPRequest::serialize() const
{
	std::ostringstream	output;

	if (!_valid_syntax)
		return (std::string("Not a valid HTTP Request"));

	output << getMethod() << " " << getURI() << " HTTP/" << getVersion().major << "." <<getVersion().minor << "\r\n";
	output << _headers.serialize();
	output << getBody();

	return (output.str());
}

/* parser */

int	HTTPRequest::parseRequestLine(const std::string& line)
{
	std::istringstream input(line);
	std::string	m;
	std::string	u;
	input >> m >> u >> "HTTP" >> "/" >> _version.major >> "." >> _version.minor;

	if (m == "GET")
		_method = WS_GET;
	else if (m == "POST")
		_method = WS_POST;
	else if (m == "DELETE")
		_method = WS_DELETE;

	if (input.fail() || _method == 0 || u == "" || _version.major < 0 || _version.minor < 0)
		return (-1);

	if (parseUri(u) != 0)
		return (-1);

	std::string remaining;
	std::getline(input, remaining);
	utils::trim(remaining);
	if (remaining != "")
		return (-1);

	return (0);
}

int	HTTPRequest::parseHeader(const std::string& line)
{
	std::pair<std::string, std::string>	header;
	if (utils::parseHeader(line, header) != 0)
		return (-1);
	_headers.insert(header.first, header.second);
	return (0);
}

int	HTTPRequest::parseLine(const std::string& line)
{
	if (_state == 0)
	{
		_state = 1;
		return parseRequestLine(line);
	}
	else if (_state == 1)
	{
		return parseHeader(line);
	}
	else
	{
		std::cout << "Fatal error : You stupid programmer" << std::endl; /* ..ouch */
		exit(1); // TODO : Pas sur que l'on doive exit ??
		return (-1);
	}
}

int		HTTPRequest::parseUri(const std::string& str)
{
	size_t		pos;
	std::string	remaining;

	/* try absoluteURI form */
	if (std::strncmp("http://", str.c_str(), 7) == 0)
	{
		remaining = std::string(str.c_str() + 7);
		pos = remaining.find('/');
		if (pos == std::string::npos)
			return (-1);
		_uri.authority = remaining.substr(0, pos);
		remaining = remaining.substr(pos, remaining.length() - pos);
	}
	else
	{
		remaining = str;
	}
	/* not absoluteURI*/
	pos = remaining.find('?');
	if (pos != std::string::npos)
	{
		_uri.path = remaining.substr(0, pos);
		_uri.query = remaining.substr(pos + 1, remaining.length() - pos - 1);
	}
	else
	{
		_uri.path = remaining;
	}
	return (0);
}

std::ostream &operator<<(std::ostream &o, const HTTPRequest &req)
{
	o << req.serialize();
	return (o);
}

std::ostream&	operator<<(std::ostream& o, const struct s_uri uri)
{
	if (uri.authority != "") o << "http://" << uri.authority;
	o << uri.path;
	if (uri.query != "") o << "?" <<uri.query;
	return (o);
}

/*
	this code below is copied from stackoverflow. It allows to match strings
	like with scanf but better!
	Source: https://stackoverflow.com/questions/71325940/how-to-translate-scanf-exact-matching-into-modern-c-stringstream-reading
*/
#include <cctype>

std::istream &operator>>(std::istream &is, char const *s)
{

	if (s == NULL)
		return is;

	if (is.flags() & std::ios::skipws)
	{
		while (std::isspace(is.peek()))
			is.ignore(1);

		while (std::isspace((unsigned char)*s))
			++s;
	}

	while (*s && is.peek() == *s)
	{
		is.ignore(1);
		++s;
	}
	if (*s)
		is.setstate(std::ios::failbit);
	return is;
}
