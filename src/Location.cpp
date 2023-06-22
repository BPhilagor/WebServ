/* ************************************************************************** */
/*                                                     __ __   __             */
/*    WebServ                                         / // / /__ \            */
/*                By: skoulen, bphilago, znichola    / // /_ __/ /            */
/*                                                  /__  __// __/             */
/*                Created: 2023/06/20 17:17:11        /_/  /____/ lausanne.ch */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"
#include "utils.hpp"

#define WS_ALIAS			(1U << 1)
#define WS_METHODS			(1U << 2)
#define WS_REDIR			(1U << 3)
#define WS_DIR_LISTING		(1U << 4)
#define WS_DEFAULT_FILE		(1U << 5)
#define WS_CGI				(1U << 6)
#define WS_SAVE_LOCATION	(1U << 7)

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
	_setSaveLocation	(data);
}

Location::Location(const Location &other) :
	_alias			(other._alias),
	_methods		(other._methods),
	_redir			(other._redir),
	_dir_listing	(other._dir_listing),
	_default_file	(other._default_file),
	_cgi			(other._cgi),
	_save_location	(other._save_location)
	{}

Location &Location::operator=(const Location &other)
{
	_config_mask	= other._config_mask;
	_alias			= other._alias;
	_methods		= other._methods;
	_redir			= other._redir;
	_dir_listing	= other._dir_listing;
	_default_file	= other._default_file;
	_cgi	 		= other._cgi;
	_save_location	= other._save_location;
	return *this;
}

/* ************************************************************************** */
/* getters                                                                    */
/* ************************************************************************** */

const std::string &	Location::getAlias()		const { return _alias;         }
t_methods_mask		Location::getMethods()		const { return _methods;       }
const std::string &	Location::getRedir()		const { return _redir;         }
bool				Location::getDirListing()	const { return _dir_listing;   }
const std::string &	Location::getDefaultFile()	const { return _default_file;  }
t_cgi				Location::getCGI() 			const { return _cgi;           }
const std::string &	Location::getSaveLocation()	const { return _save_location; }

/* ************************************************************************** */
/* is property configured                                                     */
/* ************************************************************************** */

bool Location::isAliasConfigured() const
									{ return          WS_ALIAS & _config_mask; }
bool Location::isMethodsConfigured() const
									{ return        WS_METHODS & _config_mask; }
bool Location::isRedirConfigured() const
									{ return          WS_REDIR & _config_mask; }
bool Location::isDirListingConfigured() const
									{ return    WS_DIR_LISTING & _config_mask; }
bool Location::isDefaultFileConfigured() const
									{ return   WS_DEFAULT_FILE & _config_mask; }
bool Location::isCGIConfigured() const
									{ return            WS_CGI & _config_mask; }
bool Location::isSaveLocationConfigured() const
									{ return  WS_SAVE_LOCATION & _config_mask; }


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
	d.setProp("save_location", "some/dir/");
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
		_redir = "";
		return ;
	}
	_config_mask |= WS_REDIR;

	_redir = data.find("redir").getContent();
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
	if (data.count("cgi") == 0)
	{
		_cgi = ws_no_cgi;
		return;
	}
	_config_mask |= WS_CGI;

	std::string str = data.find("cgi").getContent();
	if (str == "php")
		_cgi = ws_php;
	else if (str == "python")
		_cgi = ws_python;
	else
	{
		_cgi = ws_no_cgi;
		std::cerr << "Invalid cgi: " << str << std::endl;
	}
}

void Location::_setSaveLocation(const Data &data)
{
	if (data.count("save_location") == 0)
	{
		_save_location = "";
		return ;
	}
	_config_mask |= WS_SAVE_LOCATION;

	_save_location = data.find("save_location").getContent();
}
