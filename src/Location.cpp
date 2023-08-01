/* ************************************************************************** */
/*                                                     __ __   __             */
/*    WebServ                                         / // / /__ \            */
/*                By: skoulen, bphilago, znichola    / // /_ __/ /            */
/*                                                  /__  __// __/             */
/*                Created: 2023/06/20 17:17:11        /_/  /____/ lausanne.ch */
/*                                                                            */
/* ************************************************************************** */

#include <cstdlib>

#include "Location.hpp"
#include "utils.hpp"
#include "HTTPRequest.hpp"
#include "mimeTypes.hpp"
#include "debugDefs.hpp"
#include "cgi.hpp"

#define WS_ALIAS		(1U << 1)
#define WS_METHODS		(1U << 2)
#define WS_REDIR		(1U << 3)
#define WS_DIR_LISTING	(1U << 4)
#define WS_DEFAULT_FILE	(1U << 5)
#define WS_CGI			(1U << 6)
#define WS_UPLOAD_DIR	(1U << 7)

const Data Location::_defaultLocation = Location::constructDefaultLocation();

Location::Location() {}
Location::~Location() {}

Location::Location(const Data &data)
{
	_config_mask = 0;
	_setAlias			(data);
	_setMethods			(data);
	_setRedir			(data);
	_setDirListing		(data);
	_setDefaultFile		(data);
	_setCGI				(data);
	_setUploadDir		(data);
	if (_config_mask == 0)
		std::cerr << "Invalid location: set at least one property" << std::endl;
}

Location::Location(const Location &other) :
	_alias			(other._alias),
	_methods		(other._methods),
	_redir			(other._redir),
	_dir_listing	(other._dir_listing),
	_default_file	(other._default_file),
	_cgi			(other._cgi),
	_upload_dir		(other._upload_dir),
	_config_mask	(other._config_mask)
	{}

Location &Location::operator=(const Location &other)
{
	_alias			= other._alias;
	_methods		= other._methods;
	_redir			= other._redir;
	_dir_listing	= other._dir_listing;
	_default_file	= other._default_file;
	_cgi	 		= other._cgi;
	_upload_dir		= other._upload_dir;
	_config_mask	= other._config_mask;
	return *this;
}

/* ************************************************************************** */
/* getters                                                                    */
/* ************************************************************************** */

const std::string &	Location::getAlias()		const { return _alias;         }
t_methods_mask		Location::getMethods()		const { return _methods;       }
t_redir				Location::getRedir()		const { return _redir;         }
bool				Location::getDirListing()	const { return _dir_listing;   }
const std::string &	Location::getDefaultFile()	const { return _default_file;  }
const std::string &	Location::getUploadDir()	const { return _upload_dir;    }
const cgiMap	  &	Location::getCGIMap()		const { return _cgi;           }

std::string			Location::getRealPath(const std::string& path) const
{
	char		cwd_buf[100];
	size_t		s = sizeof(cwd_buf);

	if (_alias.size() == 0 || _alias[0] != '/')
	{
		if (getcwd(cwd_buf, s) == NULL)
		{
			PERR2("getcwd", std::strerror(errno));
			exit(1);
		}
		return cwd_buf + std::string("/") + _alias + std::string("/") + path;
	}
	else
	{
		return _alias + std::string("/") + path;
	}
}

std::string Location::getCGI(const std::string &key) const
{
	cgiMap::const_iterator value;

	if ((value = _cgi.find(key)) == _cgi.end())
		return "";
	return value->second;
}

t_getfile_response	Location::getBody(const HTTPRequest &request,
						const std::string &path,
						std::string &body,
						bool &isCGIgenerated,
						std::string &mime)	const
{
	(void)request;
	std::string real_path = getRealPath(path);
	if (DP_11 & DP_MASK)
		std::cout << "Path : " << real_path << std::endl;
	t_getfile_response return_val = utils::getFile(real_path, body); // TODO juste verifier si existe

	if (return_val != ws_file_found)
		return return_val;

	if (isCGIrequired(real_path))
	{
		std::cout << "DELETE ME WHEN DONE: cgi should not be launched from here!\n";
		cgi_ret cgi_return = launchCGI(*this, request, getCGIpath(real_path), real_path);
		if (cgi_return.fd == -1)
		{
			//should return internal server error
		}
		else
		{
			isCGIgenerated = true;
		}
	}

	/* set mime type here! */
	std::string ext = utils::getFileExtension(real_path);
	mime = getMimeFromExtension(ext);
	return ws_file_found;
}

/*
	find the cgi path required to process the request
*/
std::string Location::getCGIpath(const std::string &real_path) const
{
	if (isCGISet() == true)
		FOREACH_MAP(std::string, _cgi)
		{
			size_t x, y;

			if ((x = it->first.size()) > (y = real_path.size()))
				continue;

			if (real_path.find(it->first, y - x) != std::string::npos)
				return it->second;
		}
	return "";
}

/*
	check if file should be processed by CGI
*/
bool Location::isCGIrequired(const std::string &real_path) const
{
	return getCGIpath(real_path) == "" ? false : true;
}

/* ************************************************************************** */
/* is property configured                                                     */
/* ************************************************************************** */

bool Location::isAliasSet() const
									{ return          WS_ALIAS & _config_mask; }
bool Location::isMethodsSet() const
									{ return        WS_METHODS & _config_mask; }
bool Location::isRedirSet() const
									{ return          WS_REDIR & _config_mask; }
bool Location::isDirListingSet() const
									{ return    WS_DIR_LISTING & _config_mask; }
bool Location::isDefaultFileSet() const
									{ return   WS_DEFAULT_FILE & _config_mask; }
bool Location::isCGISet() const
									{ return            WS_CGI & _config_mask; }
bool Location::isCGISet(const std::string & key) const
									{ return                  _cgi.count(key); }
bool Location::isUploadDirSet() const
									{ return     WS_UPLOAD_DIR & _config_mask; }

t_method_response	Location::isMethodAllowed(int m)	const
{

	if (m & _methods)
		return ws_allowed;
	else
		return ws_not_allowed;
}

/* ************************************************************************** */
/* setters                                                                    */
/* ************************************************************************** */

Data Location::constructDefaultLocation()
{
	Data d;

	d.setProp("alias", "basic_site/");
	d.setProp("methods", "GET POST DELETE");
	d.setProp("redir", "something/dir/here");
	d.setProp("dir_listing", "true");
	d.setProp("default_file", "index.html");
	d.setProp("cgi", "php");
	d.setProp("upload_dir", "some/dir/");
	return d;
}

void Location::_setAlias(const Data &data)
{
	if (data.count("alias") == 0)
	{
		_alias = "";
		return ;
	}
	_config_mask |= WS_ALIAS;

	_alias = data.find("alias").getContent();
}

void Location::_setMethods(const Data &data)
{
	_methods = 0;
	if (data.count("methods") == 0)
	{
		return;
	}
	_config_mask |= WS_METHODS;

	std::string str = data.find("methods").getContent();
	while(!str.empty())
	{
		std::string s1;
		std::string s2;
		utils::split_around_first_c(' ', str, s1, s2);
		trim_outside_whitespace(s1);
		if (s1 == "GET")
			_methods |= WS_GET;
		else if (s1 == "POST")
			_methods |= WS_POST;
		else if (s1 == "DELETE")
			_methods |= WS_DELETE;
		else
			std::cerr << "Invalid method: " << s1 << std::endl;
		str = s2;
	}
}

void Location::_setRedir(const Data &data)
{
	if (data.count("redir") == 0)
	{
		return ;
	}

	std::string	location;
	std::string	type;

	std::string	redir_content = data.find("redir").getContent();
	std::istringstream iss(redir_content);
	iss >> location >> type;
	if (iss && (type == "permanent" || type == "temporary"))
	{
		_config_mask |= WS_REDIR;
		_redir.location = location;
		_redir.type = type;
	}
}

void Location::_setDirListing(const Data &data)
{
	if (data.count("dir_listing") == 0)
	{
		_dir_listing = false;
		return ;
	}
	_config_mask |= WS_DIR_LISTING;

	std::string str = data.find("dir_listing").getContent();
	if (str == "false")
		_dir_listing = false;
	else if (str == "true")
		_dir_listing = true;
	else
	{
		_dir_listing = false;
		std::cerr << "Invalid dir listing bool: " << str << std::endl;
	}
}

void Location::_setDefaultFile(const Data &data)
{
	if (data.count("default_file") == 0)
	{
		_default_file = "";
		return ;
	}
	_config_mask |= WS_DEFAULT_FILE;
	_default_file = data.find("default_file").getContent();
}

void Location::_setCGI(const Data &data)
{
	int count = 0;
	if ((count = data.count("cgi")) == 0)
		return;
	_config_mask |= WS_CGI;

	for (int i = 0; i < count; i++)
	{
		std::string str(data.find("cgi", i).getContent());
		std::string s1, s2;
		utils::split_around_first_c(' ', str, s1, s2);
		trim_outside_whitespace(s1);
		trim_outside_whitespace(s2);

		if (s2.size() > 2 && s2[0] == '$')
		{
			s2.erase(s2.begin());
			char* new_cgi_path(std::getenv(s2.c_str()));
			if (new_cgi_path == NULL)
			{
				std::cerr << COL(ESC_COLOR_RED, "Cannot resolve cgi env variable:")
				<< " $" << s2 << std::endl;
				s2 = "";
			}
			else
				s2 = new_cgi_path;
		}
		_cgi[s1] = s2;
	}
}

void Location::_setUploadDir(const Data &data)
{
	if (data.count("upload_dir") == 0)
	{
		_upload_dir = "";
		return ;
	}
	_config_mask |= WS_UPLOAD_DIR;

	_upload_dir = data.find("upload_dir").getContent();
}

/* is set print */
static const std::string isp(bool value)
{
	return value ? "set  " : "unset";
}

std::ostream & operator<<(std::ostream &os, const Location &l)
{
	os
<< "  {"
<< "\n       alias " << isp(l.isAliasSet())			<< " = " << l.getAlias()
<< "\n     methods " << isp(l.isMethodsSet())		<< " = " << l.getMethods()
<< "\n       redir " << isp(l.isRedirSet())			<< " = " << l.getRedir()
<< "\n dir_listing " << isp(l.isDirListingSet())	<< " = " << l.getDirListing()
<< "\ndefault_file " << isp(l.isDefaultFileSet())	<< " = " << l.getDefaultFile()
<< "\n         cgi " << isp(l.isCGISet())			<< " = " << l.getCGIMap()
<< "\n  upload_dir " << isp(l.isUploadDirSet())		<< " = " << l.getUploadDir()
<< "\n  }";
	return os;
}

std::ostream &operator<<(std::ostream &os, const cgiMap &map)
{
	os << "[";
	FOREACH_MAP(std::string, map)
	{
		if (it != map.begin())
			os << ", ";
		os << "(";
		os << it->first;
		os << " => ";
		os << it->second;
		os << ")";
	}
	os << "]";
	return os;
}

std::ostream &operator<<(std::ostream &os, const t_redir redir)
{
	os << redir.type << " redirect to " << redir.location;
	return (os);
}
