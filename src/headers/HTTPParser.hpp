#ifndef HTTP_PARSER_H
# define HTTP_PARSER_H

#include "HTTPRequest.hpp"

#include <vector>
#include <string>
#include <exception>

class HTTPParser
{
	public:
		HTTPParser();
		HTTPParser(const HTTPParser& h);
		~HTTPParser();
		HTTPParser&	operator=(const HTTPParser& h);

		bool	isFinished() const;
		void	consumeBuffer(const std::string& s);

		const std::string&	getBuffer() const;
		HTTPRequest	*getHTTPRequest() const;

		class MessageNotFinishedException: std::exception
		{
			virtual const char*	what() const throw();
		};

	private:
		bool						_finished;
		bool						_finishedHeaders;
		std::vector<std::string>	_lines;
		std::string					_completeBuffer;
};

#endif
