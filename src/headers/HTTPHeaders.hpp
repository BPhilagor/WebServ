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

		void		insert(const std::string& name, const std::string& value);
		std::string	serialize() const;

	private:
		std::map<std::string, std::string>	_headers;
};

std::ostream&	operator<<(std::ostream& o, const HTTPHeaders& h);

#endif
