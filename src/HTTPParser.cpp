#include "HTTPParser.hpp"
#include "utils.hpp"

HTTPParser::HTTPParser():
	_finished(false),
	_finishedHeaders(false)
{

}

HTTPParser::HTTPParser(const HTTPParser& h):
	_finished(h._finished),
	_finishedHeaders(h._finishedHeaders),
	_lines(h._lines)
{

}

HTTPParser::~HTTPParser()
{

}

HTTPParser& HTTPParser::operator=(const HTTPParser& h)
{
	_finished = h._finished;
	_finishedHeaders = h._finishedHeaders;
	_lines = h._lines;

	return (*this);
}

bool	HTTPParser::isFinished() const
{
	return (_finished);
}

void	HTTPParser::consumeBuffer(const std::string& s)
{
	unsigned int i = 0;
	if ( ! _finishedHeaders)
	{
		if (_lines.size() == 0)
			_lines.push_back("");

		for (i = 0; i < s.length(); i++)
		{
			_completeBuffer += s[i];

			if (s[i] == '\n')
			{
				utils::sanitizeline(_lines.back());
				if (_lines.back() == "")
				{
					_finishedHeaders = true;
					break ;
				}
				_lines.push_back("");
			}
			else
				_lines.back() += s[i];
		}
	}
	if (_finishedHeaders)
	{
		/* process body here */
		_finished = true;
	}
}

const std::string&	HTTPParser::getBuffer() const
{
	return (_completeBuffer);
}

HTTPRequest*	HTTPParser::getHTTPRequest() const
{
	if ( ! _finished)
	{
		throw (MessageNotFinishedException());
	}

	return new HTTPRequest(_completeBuffer);
}

const char*	HTTPParser::MessageNotFinishedException::what() const throw()
{
	return ("HTTP Message not finished");
}
