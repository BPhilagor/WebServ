/* ************************************************************************** */
/*                                                     __ __   __             */
/*    WebServ                                         / // / /__ \            */
/*                By: skoulen, bphilago, znichola    / // /_ __/ /            */
/*                                                  /__  __// __/             */
/*                Created: 2023/06/14 19:28:33        /_/  /____/ lausanne.ch */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPResponse.hpp"

static const std::string reason(int code);

/*
	Server error responses: 500 - 599
*/
void HTTPResponse::serverErrorResponse(const Data &server, int code)
{
	(void)server;
	setReason(reason(code));
}

static const std::string reason(int code)
{
	const static char *reason[12] = {
		"Internal Server Error",
		"Not Implemented",
		"Bad Gateway",
		"Service Unavailable",
		"Gateway Timeout",
		"HTTP Version Not Supported",
		"Variant Also Negotiates",
		"Insufficient Storage",
		"Loop Detected",
		"Not Extended",
		"Network Authentication Required"
	};
	code -= 500;
	if (code < 0 || code > 12 )
		return "";
	return reason[code];
}

// would have been nicer to use a map, but it's c++ 11 to initialise like this
// map<int,int> create_map()
// {
//   map<int,int> m;
//   m[1] = 2;
//   m[3] = 4;
//   m[5] = 6;
//   return m;
// }
// map<int,int> m = create_map();
// this could be a solution but then we need to initialise it, and we'd want to
// avoid doing that for every instance of the class, idk.

	// static const std::map<int, std::string> reason{
	// 	{500, "Internal Server Error"},
	// 	{501, "Not Implemented"},
	// 	{502, "Bad Gateway"},
	// 	{503, "Service Unavailable"},
	// 	{504, "Gateway Timeout"},
	// 	{505, "HTTP Version Not Supported"},
	// 	{506, "Variant Also Negotiates"},
	// 	{507, "Insufficient Storage"},
	// 	{508, "Loop Detected"},
	// 	{510, "Not Extended"},
	// 	{511, "Network Authentication Required"},
	// 	{500, "Internal Server Error"},
	// 	{501, "Not Implemented"},
	// 	{502, "Bad Gateway"},
	// 	{503, "Service Unavailable"},
	// 	{504, "Gateway Timeout"},
	// 	{505, "HTTP Version Not Supported"},
	// 	{506, "Variant Also Negotiates"},
	// 	{507, "Insufficient Storage"},
	// 	{508, "Loop Detected"},
	// 	{510, "Not Extended"},
	// 	{511, "Network Authentication Required"},
	// 	{500, "Internal Server Error"},
	// 	{501, "Not Implemented"},
	// 	{502, "Bad Gateway"},
	// 	{503, "Service Unavailable"},
	// 	{504, "Gateway Timeout"},
	// 	{505, "HTTP Version Not Supported"},
	// 	{506, "Variant Also Negotiates"},
	// 	{507, "Insufficient Storage"},
	// 	{508, "Loop Detected"},
	// 	{510, "Not Extended"},
	// 	{511, "Network Authentication Required"}
	// };
