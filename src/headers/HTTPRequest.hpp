#ifndef HTTP_REQUEST_H
# define HTTP_REQUEST_H

# include "HTTPHeaders.hpp"
# include "common-http-message.hpp"

# include <string>
# include <ostream>
# include <exception>
# include <map>

class HTTPRequest
{
	public:

		HTTPRequest(const std::string& input);
		HTTPRequest(const HTTPRequest& cpy);
		~HTTPRequest();
		HTTPRequest&	operator=(const HTTPRequest& rhs);

		/* getters */
		bool				getHasValidSyntax() const;
		t_version			getVersion() const;
		std::string			getURI() const;
		std::string			getMethod() const;
		const HTTPHeaders&	getHeaders() const;
		std::string			getBody() const;

		/* serialize */
		std::string	serialize() const;

		/* bad request exception */
		class BadRequestException: public std::exception
		{
			virtual const char	*what() const throw();
		};

	private:

		/* was there a parse error ? */
		bool		_hasValidSyntax;

		/* request line */
		std::string	_method;
		std::string	_uri;
		t_version	_version;

		/* headers */
		HTTPHeaders	_headers;

		/* body */
		std::string	_body; /* this may contain binary data! */

		/* default constructor */
		HTTPRequest();

		/* parser */
		void		parse(const std::string& input);
		void		parseRequestLine(std::string line);
		void		parseHeaders(std::vector<std::string> lines);
};

std::ostream&	operator<<(std::ostream& o, const HTTPRequest& req);

#endif /* HTTP_REQUEST_H */
