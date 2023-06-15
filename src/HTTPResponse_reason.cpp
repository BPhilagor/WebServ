/* ************************************************************************** */
/*                                                     __ __   __             */
/*    WebServ                                         / // / /__ \            */
/*                By: skoulen, bphilago, znichola    / // /_ __/ /            */
/*                                                  /__  __// __/             */
/*                Created: 2023/06/15 09:32:25        /_/  /____/ lausanne.ch */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPResponse.hpp"

std::map<int, std::string> HTTPResponse::_initialiseReasonMap()
{
	std::map<int, std::string> code;

	code[100] = "Continue";
	code[101] = "Switching Protocols";
	code[102] = "Processing";
	code[103] = "Early Hints Experimental";
	code[200] = "OK";
	code[201] = "Created";
	code[202] = "Accepted";
	code[203] = "Non-Authoritative Information";
	code[204] = "No Content";
	code[205] = "Reset Content";
	code[206] = "Partial Content";
	code[207] = "Multi-Status";
	code[208] = "Already Reported";
	code[226] = "IM Used";
	code[300] = "Multiple Choices";
	code[301] = "Moved Permanently";
	code[302] = "Found";
	code[303] = "See Other";
	code[304] = "Not Modified";
	code[305] = "Use Proxy Deprecated";
	code[306] = "unused";
	code[307] = "Temporary Redirect";
	code[308] = "Permanent Redirect";
	code[400] = "Bad Request";
	code[401] = "Unauthorized";
	code[402] = "Payment Required Experimental";
	code[403] = "Forbidden";
	code[404] = "Not Found";
	code[405] = "Method Not Allowed";
	code[406] = "Not Acceptable";
	code[407] = "Proxy Authentication Required";
	code[408] = "Request Timeout";
	code[409] = "Conflict";
	code[410] = "Gone";
	code[411] = "Length Required";
	code[412] = "Precondition Failed";
	code[413] = "Payload Too Large";
	code[414] = "URI Too Long";
	code[415] = "Unsupported Media Type";
	code[416] = "Range Not Satisfiable";
	code[417] = "Expectation Failed";
	code[418] = "I'm a teapot";
	code[421] = "Misdirected Request";
	code[422] = "Unprocessable Content";
	code[423] = "Locked";
	code[424] = "Failed Dependency";
	code[425] = "Too Early Experimental";
	code[426] = "Upgrade Required";
	code[428] = "Precondition Required";
	code[429] = "Too Many Requests";
	code[431] = "Request Header Fields Too Large";
	code[451] = "Unavailable For Legal Reasons";
	code[500] = "Internal Server Error";
	code[501] = "Not Implemented";
	code[502] = "Bad Gateway";
	code[503] = "Service Unavailable";
	code[504] = "Gateway Timeout";
	code[505] = "HTTP Version Not Supported";
	code[506] = "Variant Also Negotiates";
	code[507] = "Insufficient Storage";
	code[508] = "Loop Detected";
	code[510] = "Not Extended";
	code[511] = "Network Authentication Required";
	return code;
}
