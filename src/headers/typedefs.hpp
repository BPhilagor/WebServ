/* ************************************************************************** */
/*                                                     __ __   __             */
/*    WebServ                                         / // / /__ \            */
/*                By: skoulen, bphilago, znichola    / // /_ __/ /            */
/*                                                  /__  __// __/             */
/*                Created: 2023/06/15 15:18:55        /_/  /____/ lausanne.ch */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPEDEFS_HPP
# define TYPEDEFS_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <stdint.h>

class Server;

typedef std::pair<uint32_t, uint32_t> pairHostPort;
typedef std::map< pairHostPort, std::vector<Server> > mapHostPort;
typedef std::map<std::string, std::string> cgiMap;

// mask of the accepted methods
# define WS_GET    (1U << 1)
# define WS_POST   (1U << 2)
# define WS_DELETE (1U << 3)
typedef unsigned int t_methods_mask;

typedef enum {
	ws_not_allowed,
	ws_allowed
} t_method_response;

typedef enum {
	ws_file_found,
	ws_file_not_found,
	ws_file_no_perm,
	ws_file_isdir
} t_getfile_response;

typedef enum {
	ws_dir_found,
	ws_dir_no_perm
} t_get_dirlist;

typedef struct
{
	int	major;
	int	minor;
}	t_version;

typedef struct
{
	std::string location;
	std::string	type;
}	t_redir;

#endif /* TYPEDEFS_HPP */
