#include "BufferManager.hpp"
#include "utils.hpp"

BufferManager::BufferManager(const SuperServer& config, int fd):
	input_buffer(""),
	output_buffer(""),
	_fd(fd),
	_config(config),
	_req(),
	_resp(),
	_finished(false)
{

}

BufferManager::BufferManager(const BufferManager& h):
	input_buffer(h.input_buffer),
	output_buffer(h.output_buffer),
	_fd(h._fd),
	_config(h._config),
	_req(h._req),
	_resp(h._resp),
	_finished(h._finished)
{

}

BufferManager::~BufferManager()
{

}

BufferManager& BufferManager::operator=(const BufferManager& h)
{
	input_buffer = h.input_buffer;
	output_buffer = h.output_buffer;
	_fd = h._fd;
	/* _config is const ! */
	_req = h._req;
	_resp = h._resp;
	_finished = h._finished;

	return (*this);
}

bool	BufferManager::isFinished() const
{
	return _finished;
}

void	BufferManager::addInputBuffer(const std::string& s)
{
	std::string		host;
	const Server*	virtual_server;

	input_buffer = s; /* save the input buffer in case we read too much */
	unsigned int i = 0;
	for (i = 0; i < s.length(); i++)
	{
		_req.addChar(s[i]);
		if (_req.isParsingHeadersFinished())
		{
			if (!_req.hasValidSyntax())
			{
				std::cout << "Invalid syntax" << std::endl;
				_resp.constructErrorReply(400);
				goto phase_2;
			}
			host = _req.getHeader("Host");
			if (host == "")
			{
				std::cout << "Host not set" << std::endl;
				_resp.constructErrorReply(400);
				goto phase_2;
			}
			virtual_server = _config.getServerForHostPortAndHostName(utils::fd_to_HostPort(_fd), host);
			if (virtual_server == NULL)
			{
				std::cout << "Host not found" << std::endl;
				_resp.constructErrorReply(400);
				goto phase_2;
			}
			int content_length = atoi(_req.getHeader("content-length").c_str());
			if (content_length <= 0)
			{
				goto phase_2;
			}
			if (content_length > virtual_server->getBodyLimit())
			{
				_resp.constructErrorReply(413);
				goto phase_2;
			}
			if (_req.getMethod() != "POST")
			{
				goto phase_2;
			}
		}
		if (_req.isParsingBodyFinished())
		{
phase_2:
			if (_resp.getCode() == 0)
				requestWorker(*virtual_server, _req, _resp);
			input_buffer = input_buffer.substr(i + 1, s.length() - i - 1);
			output_buffer = _resp.serialize();
			_finished = true;
			break ;
		}
	}
	input_buffer = "";
}

const HTTPRequest&	BufferManager::getRequest() const
{
	return _req;
}

const HTTPResponse&	BufferManager::getResponse() const
{
	return _resp;
}
