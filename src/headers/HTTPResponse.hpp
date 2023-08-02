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
# include "cgi.hpp"
# include "mimeTypes.hpp"

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
		std::string		getHeader(const std::string& name) const;
		std::string		getBody() const;

		/* setters */
		void			setVersion(int major, int minor);
		void			setCode(int code);
		void			setReason(const std::string &reason);
		void			setHeader(const std::string &name, const std::string &value);
		void			setBody(const std::string &body);
		void			setDate();
		void			set();

		std::string	serialize() const;

		void	constructErrorReply(const int code, const Server *srv=NULL);
		void	constructRedirect(t_redir redir);
		void	serveStaticFile(const std::string& path);
		bool	serveDynamicFile(const Location& location, const std::string& path, const HTTPRequest& request);
		void	parseCGIResponse(std::string cgi_body);
		void	finalize(const HTTPRequest & resp);

		std::string genPage(int code) const;
		std::string getErrorPage(const Server &srv, int code) const;

		bool	is_cgi_used;
		cgi_ret	_cgi_ret;

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
