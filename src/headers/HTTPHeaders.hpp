/* ************************************************************************** */
/*                                                     __ __   __             */
/*    WebServ                                         / // / /__ \            */
/*                By: skoulen, bphilago, znichola    / // /_ __/ /            */
/*                                                  /__  __// __/             */
/*                Created: 2023/06/01 23:16:37        /_/  /____/ lausanne.ch */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_HEADERS_H
# define HTTP_HEADERS_H

#include <map>
#include <string>
#include <vector>
#include <ostream>

class HTTPHeaders
{
	public:
		HTTPHeaders();
		HTTPHeaders(const HTTPHeaders& h);
		~HTTPHeaders();
		HTTPHeaders&	operator=(const HTTPHeaders& h);

		std::string	get(const std::string& name) const;
		void		insert(const std::string& name, const std::string& value);
		void		replace(const std::string& name, const std::string& value);
		bool		contains(const std::string &name);
		std::string	serialize() const;

	private:
		std::map<std::string, std::string>	_headers;
};

std::ostream&	operator<<(std::ostream& o, const HTTPHeaders& h);

#endif
