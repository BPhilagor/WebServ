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
# include <ostream>
# include <exception>
# include <map>
# include <cstdlib>

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
		std::string		getURI() const;
		t_methods_mask	getMethod() const;
		std::string		getHeader(const std::string& name) const;
		std::string		getBody() const;

		bool				isParsingHeadersFinished() const;
		bool				isParsingBodyFinished() const;

		/* setters */
		void					addChar(char c);

		/* serialize */
		std::string	serialize() const;

	private:

		bool		_valid_syntax;

		/* request line */
		t_methods_mask	_method;
		std::string		_uri;
		t_version		_version;

		/* headers */
		HTTPHeaders	_headers;

		/* body */
		std::string	_body; /* this may contain binary data! */

		/* used internally to parse and know what we have to do next */
		int			_state;
		std::string	_current_line;

		/* parser */
		int		parseLine(const std::string& line);
		int		parseRequestLine(const std::string& line);
		int		parseHeader(const std::string& line);
};

std::ostream&	operator<<(std::ostream& o, const HTTPRequest& req);

#endif /* HTTP_REQUEST_H */
