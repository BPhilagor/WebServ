/* ************************************************************************** */
/*                                                     __ __   __             */
/*    WebServ                                         / // / /__ \            */
/*                By: skoulen, bphilago, znichola    / // /_ __/ /            */
/*                                                  /__  __// __/             */
/*                Created: 2023/06/01 23:16:37        /_/  /____/ lausanne.ch */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_REQUEST_H
# define HTTP_REQUEST_H

# include "HTTPHeaders.hpp"
# include "typedefs.hpp"
# include <string>
# include <cstring>
# include <ostream>
# include <exception>
# include <map>
# include <cstdlib>

struct s_uri
{
	std::string	authority;
	std::string	path;
	std::string	query;
};

class HTTPRequest
{
	public:

		HTTPRequest();
		HTTPRequest(const HTTPRequest& cpy);
		~HTTPRequest();
		HTTPRequest&	operator=(const HTTPRequest& rhs);

		/* getters */
		bool			hasValidSyntax() const;
		t_version		getVersion() const;
		struct s_uri	getURI() const;
		t_methods_mask	getMethod() const;
		std::string		getHeader(const std::string& name) const;
		std::string		getBody() const;
		HTTPHeaders		&getAllHeaders();

		bool				isParsingHeadersFinished() const;
		bool				isParsingBodyFinished() const;

		bool				isBodyCGIgenerated() const;

		/* setters */
		void					addChar(char c);
		void					setBodyCGIgenerated(bool value);
		void					setBody(const std::string &body);

		/* serialize */
		std::string	serialize() const;

	private:

		bool		_valid_syntax;

		/* request line */
		t_methods_mask	_method;
		struct s_uri	_uri;
		t_version		_version;

		/* headers */
		HTTPHeaders	_headers;

		/* body */
		std::string	_body; /* this may contain binary data! */

		/* used internally to parse and know what we have to do next */
		int			_state;
		std::string	_current_line;

		/* used to know if body is gen by CGI */
		bool		_isBodyCGIgenerated;

		/* parser */
		int		parseLine(const std::string& line);
		int		parseRequestLine(const std::string& line);
		int		parseHeader(const std::string& line);
		int		parseUri(const std::string& str);
};

std::ostream&	operator<<(std::ostream& o, const HTTPRequest& req);
std::ostream&	operator<<(std::ostream& o, const struct s_uri uri);

#endif /* HTTP_REQUEST_H */
