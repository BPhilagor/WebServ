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
# include <map>

# include "HTTPHeaders.hpp"
# include "typedefs.hpp"
# include "Data.hpp"
# include "Server.hpp"

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
		void			setReason(const std::string &reason);
		void			setHeader(const std::string &name, const std::string &value);
		void			setBody(const std::string &body);
		void			setDate();

		std::string	serialize() const;

		/* constructors */
		void	constructReply(const Server &server, const std::string &body, int code);
		void	constructErrorReply(int code);

		std::string genErrorPage(int code) const;

	private:
		/* status line */
		t_version	_version;
		int			_code;
		std::string	_reason;

		/* headers */
		HTTPHeaders	_headers;

		/* body */
		std::string	_body;

		/* error codes */
		static std::map<int, std::string> _reasonMap;
		static std::map<int, std::string> _initialiseReasonMap();
};

std::ostream&	operator<<(std::ostream& o, const HTTPResponse& h);

#endif
