/* ************************************************************************** */
/*                                                     __ __   __             */
/*    WebServ                                         / // / /__ \            */
/*                By: skoulen, bphilago, znichola    / // /_ __/ /            */
/*                                                  /__  __// __/             */
/*                Created: 2023/06/01 23:16:37        /_/  /____/ lausanne.ch */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_RESPONSE_H
# define HTTP_RESPONSE_H

# include <string>

# include "HTTPHeaders.hpp"
# include "common-http-message.hpp"

class HTTPResponse
{
	public:
		HTTPResponse();
		HTTPResponse(const HTTPResponse& h);
		~HTTPResponse();
		HTTPResponse&	operator=(const HTTPResponse& h);

		/* getters */
		t_version		getVersion() const;
		int				getCode() const;
		std::string		getReason() const;
		const HTTPHeaders&	getHeaders() const;
		std::string		getBody() const;

		/* setters */
		void			setVersion(int major, int minor);
		void			setCode(int code);
		void			setReason(std::string reason);
		void			setHeader(std::string name, std::string value);
		void			setBody(std::string body);

		std::string	serialize() const;

	private:
		/* status line */
		t_version	_version;
		int			_code;
		std::string	_reason;

		/* headers */
		HTTPHeaders	_headers;

		/* body */
		std::string	_body;
};

std::ostream&	operator<<(std::ostream& o, const HTTPResponse& h);

#endif
