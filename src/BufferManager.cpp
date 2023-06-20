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
	input_buffer = input_buffer.substr(i, s.length() - i);
}

void	BufferManager::constructResponse()
{
	/* when do we process the body ? */

	const std::string host = _req.getHeader("Host");
	if (host == "")
	{
		/* bad request */
		output_buffer = "Bad";
		return ;
	}

	const Server* virtual_server = _config.getServerForHostPortAndHostName(
		utils::fd_to_HostPort(_fd), host);

	if (virtual_server == NULL)
	{
		/* bad request */
		output_buffer = "BAD";
		return ;
	}

	output_buffer = requestWorker(*virtual_server, _req);
}
