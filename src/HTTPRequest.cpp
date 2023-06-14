#include "HTTPRequest.hpp"
#include "utils.hpp"
#include <iostream>
#include <sstream>
#include <map>

#define DEBUG_PRINT 0

std::istream &operator>>(std::istream &is, char const *s);

HTTPRequest::HTTPRequest(const std::string &input)
{
	if (DEBUG_PRINT) std::cout << "HTTPRequest constructor" << std::endl;

	_hasValidSyntax = true;
	try
	{
		parse(input);
	}
	catch (BadRequestException& e)
	{
		_hasValidSyntax = false;
	}
}

HTTPRequest::HTTPRequest(const HTTPRequest &cpy):
	_hasValidSyntax(cpy._hasValidSyntax),
	_method(cpy._method),
	_uri(cpy._uri),
	_version(cpy._version),
	_headers(cpy._headers),
	_body(cpy._body)
{
	if (DEBUG_PRINT) std::cout << "HTTPRequest copy constructor" << std::endl;
}

HTTPRequest::~HTTPRequest()
{
	if (DEBUG_PRINT) std::cout << "HTTPRequest destructor" << std::endl;
}

HTTPRequest &HTTPRequest::operator=(const HTTPRequest &rhs)
{
	if (DEBUG_PRINT) std::cout << "HTTPRequest assignment operator" << std::endl;

	_hasValidSyntax = rhs._hasValidSyntax;
	_method = rhs._method;
	_uri = rhs._uri;
	_version = rhs._version;
	_headers = rhs._headers;
	_body = rhs._body;

	return (*this);
}

/* getters */
bool					HTTPRequest::getHasValidSyntax() const { return _hasValidSyntax; }
t_version				HTTPRequest::getVersion() const { return _version; }
std::string				HTTPRequest::getURI() const { return _uri; }
std::string				HTTPRequest::getMethod() const { return _method; }
const HTTPHeaders&		HTTPRequest::getHeaders() const { return _headers; } /* should not be like that in the future */
std::string				HTTPRequest::getBody() const { return _body; }

/* serialize */
std::string	HTTPRequest::serialize() const
{
	std::ostringstream	output;

	output << getMethod() << " " << getURI() << " HTTP/" << getVersion().major << "." <<getVersion().minor << "\r\n";
	output << getHeaders().serialize();
	output << getBody();

	return (output.str());
}


/* bad request exception */
const char *HTTPRequest::BadRequestException::what() const throw()
{
	return ("Bad HTTP request");
}

/* parser */

#include <vector>
#include <string>

void HTTPRequest::parse(const std::string &input)
{
	std::istringstream	iss(input);
	std::string			line;

	std::getline(iss, line);
	utils::sanitizeline(line); /* need to do these to operations in one line for the sake of elegance! */

	parseRequestLine(line);

	std::vector<std::string>	header_lines;
	std::getline(iss, line);
	utils::sanitizeline(line);

	while (line != "")
	{
		if (iss.eof())
			throw BadRequestException();
		header_lines.push_back(line);
		std::getline(iss, line);
		utils::sanitizeline(line);
	}
	parseHeaders(header_lines);



	/* what remains is the body */
	//std::cout<<iss.tellg()<<std::endl;
	if (!iss.fail())
		_body = std::string(iss.str().substr(iss.tellg()));
}

void	HTTPRequest::parseRequestLine(std::string line)
{
	std::istringstream input(line);

	input >> _method >> _uri >> "HTTP" >> "/" >> _version.major >> "." >> _version.minor;

	if (input.fail())
		throw BadRequestException();
}

void	HTTPRequest::parseHeaders(std::vector<std::string> lines)
{
	for (unsigned int i = 0; i < lines.size(); i++)
	{
		std::string	fieldname;
		std::string	fieldvalue;

		size_t pos = lines[i].find(':');
		if (pos == std::string::npos)
			throw BadRequestException();

		fieldname = lines[i].substr(0, pos);
		if (fieldname.find_first_of(LINEAR_WHITESPACE) != std::string::npos) /* reject if there is whitespace in the fieldname */
			throw BadRequestException();

		fieldvalue = lines[i].substr(pos + 1, lines[i].size() - pos - 1); /* trim right and left optional whitespace */
		utils::trim(fieldvalue);

		_headers.insert(fieldname, fieldvalue);
	}
}

std::ostream &operator<<(std::ostream &o, const HTTPRequest &req)
{
	o << req.serialize();
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

	if (s == nullptr)
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
