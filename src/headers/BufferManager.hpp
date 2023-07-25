#ifndef HTTP_PARSER_H
# define HTTP_PARSER_H

#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"
#include "SuperServer.hpp"
#include "requestWorker.hpp"

#include <vector>
#include <string>
#include <exception>

class BufferManager
{
	public:
		BufferManager(const SuperServer& config, int fd);
		BufferManager(const BufferManager& h);
		~BufferManager();
		BufferManager&	operator=(const BufferManager& h);

		bool	isFinished() const;

		void	addInputBuffer(const std::string& s);

		const HTTPRequest&	getRequest() const;
		const HTTPResponse&	getResponse() const;

		bool						_is_cgi_message;

		/* If something remains in this buffer after parsing, we'll put it in the next request */
		std::string					input_buffer;

		/* This buffer will be modified to keep what remains to be written */
		std::string					output_buffer;

	// private:

		bool						_client_fd;

		int							_fd;

		const SuperServer&			_config;

		HTTPRequest					_req;
		HTTPResponse				_resp;

		bool						_finished;

		private:
		BufferManager();
};

#endif
