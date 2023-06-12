/* ************************************************************************** */
/*                                                     __ __   __             */
/*    WebServ                                         / // / /__ \            */
/*                By: skoulen, bphilago, znichola    / // /_ __/ /            */
/*                                                  /__  __// __/             */
/*                Created: 2023/06/12 19:33:04        /_/  /____/ lausanne.ch */
/*                                                                            */
/* ************************************************************************** */

#include "FooBar.hpp"

// Default constructor
FooBar::FooBar()
{
}

// Copy constructor
FooBar::FooBar(const FooBar &other)
{
	*this = other;
}

// Destructor
FooBar::~FooBar()
{
}

// Copy assignment operator
FooBar &FooBar::operator=(const FooBar &other)
{
	(void)other;
	// TODO: insert return statement here
	return *this;
}


