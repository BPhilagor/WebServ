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

class Location
{
private:

public:
// Default constructor
	Location();

// Copy constructor
	Location(const Location &other);

// Destructor
	~Location();

// Copy assignment operator
	Location & operator=(const Location &other);
};

#endif /* LOCATION_HPP */
