/* ************************************************************************** */
/*                                                     __ __   __             */
/*    WebServ                                         / // / /__ \            */
/*                By: skoulen, bphilago, znichola    / // /_ __/ /            */
/*                                                  /__  __// __/             */
/*                Created: 2023/06/20 17:17:11        /_/  /____/ lausanne.ch */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

# include "typedefs.hpp"
# include "Data.hpp"

typedef enum {
	ws_no_cgi,
	ws_php,
	ws_python
} t_cgi;

typedef unsigned int t_location_cfg_mask;

class Location
{
public:
	Location();
	Location(const Data &data);
	Location(const Location &other);
	~Location();
	Location & operator=(const Location &other);

	static Data constructDefaultLocation(); /* unused as right now */

	const std::string &	getAlias()				const;
	t_methods_mask		getMethods()			const;
	const std::string &	getRedir()				const;
	bool				getDirListing()			const;
	const std::string &	getDefaultFile()		const;
	t_cgi				getCGI() 				const;
	const std::string &	getUploadDir()			const;

	bool				isAliasSet() 			const;
	bool				isMethodsSet()			const;
	bool				isRedirSet()			const;
	bool				isDirListingSet()		const;
	bool				isDefaultFileSet()		const;
	bool				isCGISet()				const;
	bool				isUploadDirSet()		const;

	t_method_response	isMethodAllowed(int m)	const;

private:
	void				_setAlias(const Data &data);
	void				_setMethods(const Data &data);
	void				_setRedir(const Data &data);
	void				_setDirListing(const Data &data);
	void				_setDefaultFile(const Data &data);
	void				_setCGI(const Data &data);
	void				_setUploadDir(const Data &data);

	std::string			_alias;
	t_methods_mask		_methods;
	std::string			_redir;
	bool				_dir_listing;
	std::string			_default_file;
	t_cgi				_cgi;
	std::string			_upload_dir;

	t_location_cfg_mask	_config_mask;

	static const Data	_defaultLocation;
};

std::ostream &operator<<(std::ostream &os, const Location &l);

#endif /* LOCATION_HPP */
