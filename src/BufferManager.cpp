#include "BufferManager.hpp"
#include "utils.hpp"

BufferManager::BufferManager(const SuperServer& config):
	input_buffer(""),
	output_buffer(""),
	_config(config),
	_req(),
	_resp(),
	_state(0)
{

}

BufferManager::BufferManager(const BufferManager& h):
	input_buffer(h.input_buffer),
	output_buffer(h.output_buffer),
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
	if (_req.hasValidSyntax())
	{
		_resp.setVersion(1, 1);
		_resp.setCode(200);
		_resp.setReason("Ok");
		_resp.setHeader("Content-length", "11");
		_resp.setBody("Hello world");
	}
	else
	{
		_resp.setVersion(1, 1);
		_resp.setCode(400);
		_resp.setReason("Bad request");
	}
	output_buffer = _resp.serialize();
}
