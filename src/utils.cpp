/* ************************************************************************** */
/*                                                     __ __   __             */
/*    WebServ                                         / // / /__ \            */
/*                By: skoulen, bphilago, znichola    / // /_ __/ /            */
/*                                                  /__  __// __/             */
/*                Created: 2023/06/01 23:16:37        /_/  /____/ lausanne.ch */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"
#include "Server.hpp"
#include <arpa/inet.h>

#define LOCALHOST_UI32 16777343

int send_to_socket(const std::string &message, int socket_fd) {
	return write(socket_fd, message.c_str(), message.length());
}

int send_file_to_socket(const std::string &filename, int socket_fd) {
	std::ifstream file;
	file.open(filename.c_str());

	std::string line;
	std::string message;
	while (std::getline(file, line)) {
		message += line + "\r\n";
	}
	return send_to_socket(message, socket_fd);
}

void trim_outside_whitespace(std::string &line)
{
	size_t start = line.find_first_not_of("\t\n\v\f\r ");
	size_t end = line.find_last_not_of("\t\n\v\f\r ");
	if (start == std::string::npos || end == std::string::npos)
		return ;
	line = line.substr(start, end - start + 1);
}

/*
	from stackoverflow: https://stackoverflow.com/questions/216823/how-to-trim-an-stdstring
	trim right and left whitespace from string
*/

static std::string &rtrim(std::string &s, const char *t = LINEAR_WHITESPACE);
static std::string &ltrim(std::string &s, const char *t = LINEAR_WHITESPACE);

std::string&	utils::trim(std::string &s, const char *t)
{
	return (ltrim(rtrim(s, t), t));
}

static std::string&	ltrim(std::string &s, const char *t)
{
	s.erase(0, s.find_first_not_of(t));
	return s;
}

static std::string&	rtrim(std::string &s, const char *t)
{
	s.erase(s.find_last_not_of(t) + 1);
	return s;
}

/*
	remove ending cr if present. replace bare cr with sp

	This is to comply with the http specification.
	It says we must either reject bare CRs or replace them with SP.
	The specification also says we can use LF as line delimiter and ignore the CR that comes before.
*/
std::string&	utils::sanitizeline(std::string& s)
{
	if (s.length() > 0 && s[s.length() - 1] == '\r')
		s.resize(s.length() - 1);
	while (size_t pos = s.find("\r") != std::string::npos)
		s.replace(pos, 1, " ");
	return (s);
}

std::string utils::addrIntToString(u_int32_t addr_int)
{
	struct in_addr tmp;

	tmp.s_addr = addr_int;
	return std::string(inet_ntoa(tmp));
}

u_int32_t utils::addrStringToInt(const std::string &addr_string)
{
	struct in_addr tmp;

	if (addr_string == "localhost")
		return LOCALHOST_UI32;

	if (!inet_aton(addr_string.c_str(), &tmp))
		std::cout << "Warning, wrong adress was converted to 0" << std::endl;
	return tmp.s_addr;
}

pairHostPort utils::getHostPort(const std::string &str)
{
	size_t tmp = str.find(':');

	u_int32_t host, port;
	if (tmp == std::string::npos)
	{
		host = 0;
		tmp = 0;
	}
	else
	{
		std::string sHost = str.substr(0, tmp);
		std::cout << "Origine : " << sHost;
		host = addrStringToInt(sHost);
		tmp += 1;
	}

	std::stringstream sPort(str.substr(tmp, str.size() - tmp));
	sPort >> port;

	std::string addr_string = addrIntToString(host);
	std::cout << " Conversion : " << host << " Reconversion : " << addr_string << std::endl;

	if (sPort.fail())
		port = 8080;

	port = htons(port);
	return pairHostPort(host, port);
}

/*
	this needs to be the most complete config possible,
	not values should be missing, we will access  with
	the assumption that everything needed is here.
	if segv or we throw some thing.
*/
Data utils::constructDefaultServer()
{
	Data d;

	d.setProp("listen", "0:8080");
	d.setProp("listen", "0:4242");
	d.setProp("server_name", "WebServ foo bar");
	d.setProp("host_name", "foo www.foo.com");
	d.setProp("methods", "GET POST DELETE");
	d.setProp("body_limit", "1048576");
	d.setProp("error_pages", "");
	d.setProp("error_dir", "");
	d.setProp("upload_dir", "");
	d.setProp("directory_listing", "true");

	dataObj location;
	location.first = "location";
	location.second.setProp("alias", "basic_site");
	location.second.setContent("/basic_site");
	d.pushBack(location);
	/* etc */
	return d;
}

int utils::toInt(const std::string &s)
{
	int ret = 0;
	std::stringstream ss(s);

	ss >> ret;
	if (ss.fail())
		return 0;
	return ret;
}

void utils::split_around_first_c(char c, const std::string src, std::string &s1, std::string &s2)
{
	size_t x = src.find_first_of(c);
	if ( x == std::string::npos)
	{
		s1 = src;
		s2 = "";
	}
	else
	{
		s1 = src.substr(0, x);
		s2 = src.substr(x + 1, src.length());
	}
}


pairHostPort utils::fd_to_HostPort(int fd)
{
	pairHostPort ret;
	struct sockaddr_in addr;
	socklen_t addrlen = sizeof(addr);
	getsockname(fd, (struct sockaddr *)&addr, &addrlen);
	// std::cout << "Received connection from: " << inet_ntoa(addr.sin_addr) << ":" << ntohs(addr.sin_port) << std::endl;
	ret.first = addr.sin_addr.s_addr;
	ret.second = addr.sin_port;
	return ret;
}

std::string utils::ifstreamToString(std::ifstream &stream)
{
	std::stringstream	buffer;

	buffer << stream.rdbuf();
	return buffer.str();
}

std::string utils::fdToString(int fd)
{
	std::string result;
	char buffer[1000];
	int byte_nbr;

	while ((byte_nbr = read(fd, buffer, 1000))) {
		if (byte_nbr != 1000 && byte_nbr > 0) {
			buffer[byte_nbr] = 0;
		}
		result.append(buffer);
	}
	return result;
}

t_getfile_response utils::getFile(const std::string &path, std::string &body)
{
	std::ifstream stream;

	struct stat path_stat;
	stat(path.c_str(), &path_stat);
	if (S_ISDIR(path_stat.st_mode))
		return ws_file_isdir;

	stream.open(path);

	if (!stream)
	{
		std::cout << "Error when opening : " << path << " : "
			<< strerror(errno) << " (n: " << errno << ")" << std::endl;
		return ws_file_not_found;
	}
	
	body = utils::ifstreamToString(stream);
	std::cout << "file {" << body << "}" << std::endl;
	stream.close();

	return ws_file_found;
}

std::string utils::getMethodStr(const HTTPRequest &req)
{
	int mask = req.getMethod();

	if (mask & WS_GET)
		return std::string("GET");
	else if (mask & WS_POST)
		return std::string("POST");
	else if (mask & WS_DELETE)
		return std::string("DELETE");
	return "";
}


