/* ************************************************************************** */
/*                                                     __ __   __             */
/*    WebServ                                         / // / /__ \            */
/*                By: skoulen, bphilago, znichola    / // /_ __/ /            */
/*                                                  /__  __// __/             */
/*                Created: 2023/06/20 17:17:11        /_/  /____/ lausanne.ch */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

// Default constructor
Location::Location()
{
}

// Copy constructor
Location::Location(const Location &other)
{
	*this = other;
}

// Destructor
Location::~Location()
{
}

// Copy assignment operator
Location &Location::operator=(const Location &other)
{
	(void)other;
	// TODO: insert return statement here
	return *this;
}


