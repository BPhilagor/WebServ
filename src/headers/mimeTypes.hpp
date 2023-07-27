/* ************************************************************************** */
/*                                                     __ __   __             */
/*    WebServ                                         / // / /__ \            */
/*                By: skoulen, bphilago, znichola    / // /_ __/ /            */
/*                                                  /__  __// __/             */
/*                Created: 2023/06/27 15:53:42        /_/  /____/ lausanne.ch */
/*                                                                            */
/* ************************************************************************** */

#ifndef MIME_TYPES_H
# define MIME_TYPES_H

#include <string>

const std::string &getMimeFromExtension(std::string& ext);

#endif /* MIME_TYPES_H */
