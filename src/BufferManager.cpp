#include "BufferManager.hpp"
#include "utils.hpp"

BufferManager::BufferManager(const SuperServer& config, int fd):
	input_buffer(""),
	output_buffer(""),
	_fd(fd),
	_config(config),
	_req(),
	_resp(),
	_state(0)
{

}

BufferManager::BufferManager(const BufferManager& h):
	input_buffer(h.input_buffer),
	output_buffer(h.output_buffer),
	_fd(h._fd),
	_config(h._config),
	_req(h._req),
	_resp(h._resp),
	_state(h._state)
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
	_state = h._state;

	return (*this);
}

bool	BufferManager::isFinished() const
{
	return _req.isParsingFinished();
}

void	BufferManager::addInputBuffer(const std::string& s)
{
	input_buffer = s; /* save the input buffer in case we read too much */
	unsigned int i = 0;
	for (i = 0; i < s.length(); i++)
	{
		_req.addChar(s[i]);
		if (_req.isParsingFinished())
		{
			constructResponse();
			break ;
		}
	}

	if (_req.isParsingFinished())
		input_buffer = input_buffer.substr(i + 1, s.length() - i - 1);
	else
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

void	BufferManager::constructResponse()
{

	std::cout << "Constructing reply for request:" << std::endl;
	std::cout << _req << std::endl;

	/* when do we process the body ? */

	const Server*	virtual_server = NULL;
	std::string		host;

	if (!_req.hasValidSyntax())
	{
		std::cout << "Parse error" << std::endl;
		_resp.constructErrorReply(400);
		goto generate_output;
	}

	host = _req.getHeader("Host");
	if (host == "")
	{
		std::cout << "Host header not set" << std::endl;
		_resp.constructErrorReply(400);
		goto generate_output;
	}
	virtual_server = _config.getServerForHostPortAndHostName(utils::fd_to_HostPort(_fd), host);
	if (virtual_server == NULL)
	{
		std::cout << "Virtual server not found" << std::endl;
		_resp.constructErrorReply(400);
		goto generate_output;
	}
	requestWorker(*virtual_server, _req, _resp);

generate_output:
	output_buffer = _resp.serialize();
}
