#!/bin/zsh


printf "generate cpp project boilerplate files\n"

help () {
	printf "usage: class  [class name]  <0..n>         # makes [.hpp/.cpp] file pairs\n"
	printf "   or: src    [file name]   <include_0..n> # makes src file with listed includes\n"
	printf "   or: header [header name] <include_0..n> # makes header file with listed includes\n"
}

################ file header ##########

std_header () {
	file_name="${1}"

	creation_date=$(date +'%Y/%m/%d')
	creation_time=$(date +'%H:%M:%S')

	printf "/* ************************************************************************** */
/*                                                     __ __   __             */
/*    WebServ                                         / // / /__ \            */
/*                By: skoulen, bphilago, znichola    / // /_ __/ /            */
/*                                                  /__  __// __/             */
/*                Created: ${creation_date} ${creation_time}        /_/  /____/ lausanne.ch */
/*                                                                            */
/* ************************************************************************** */
"
}

################# header ##################

make_header () {
	class_name="${1}"
	file_name=$(printf "%s.hpp" "${class_name}")
	shift
	header_guard_name=$(printf "%s_HPP" "${class_name}" | awk '{print toupper($0)}')
	{
		std_header $file_name

		printf "\n#ifndef %s\n" "${header_guard_name}" ;
		printf "# define %s\n\n" "${header_guard_name}";

		for var in "$@"
		do
			printf "# include \"$var.hpp\"\n"
		done

		printf "\n#endif /* %s */\n" "${header_guard_name}"
	} > src/headers/"${file_name}"
}

################# source ##################

make_source () {
	file_name=$(printf "%s.cpp" "${1}")
	shift

	{
		std_header $file_name

		printf "\n#include \"utils.hpp\"\n"

		for var in "$@"
		do
			printf "#include \"$var.hpp\"\n"
		done

	} > src/"${file_name}"
}

################## class ###################

make_class_header () {
	class_name="${1}"
	file_name=$(printf "%s.hpp" "${class_name}")

	header_guard_name=$(printf "%s_HPP" "${class_name}" | awk '{print toupper($0)}')
	{
		std_header $file_name

		printf "\n#ifndef %s\n" "${header_guard_name}" ;
		printf "# define %s\n" "${header_guard_name}";

		print "
class ${class_name}
{
private:

public:
// Default constructor
	${class_name}();

// Copy constructor
	${class_name}(const ${class_name} &other);

// Destructor
	~${class_name}();

// Copy assignment operator
	${class_name} & operator=(const ${class_name} &other);
};
"

		printf "#endif /* %s */\n" "${header_guard_name}"
	} > src/headers/"${file_name}"
}

make_class_source () {
	class_name="${1}"

	file_name=$(printf "%s.cpp" "${class_name}")

	{
		std_header $file_name

		printf "
#include \"${class_name}.hpp\"

// Default constructor
${class_name}::${class_name}()
{
}

// Copy constructor
${class_name}::${class_name}(const ${class_name} &other)
{
	*this = other;
}

// Destructor
${class_name}::~${class_name}()
{
}

// Copy assignment operator
${class_name} &${class_name}::operator=(const ${class_name} &other)
{
	(void)other;
	// TODO: insert return statement here
	return *this;
}


"
	} > src/"${file_name}"
}

make_class () {
	class_name="${1}"

	make_class_header $class_name
	make_class_source $class_name
}

# Check the number of arguments
if [ ${#} -lt 2 ]; then
	help
	exit 1
fi

if [ "$1" = "class" ]; then
	echo "make a class!"

	shift

	for var in "$@"
	do
		make_class "$var"
	done

	exit 0
fi

if [ "$1" = "src" ]; then
	echo "make a src file!"

	shift

	make_source $@

	exit 0
fi

if [ "$1" = "header" ]; then
	echo "make a header file!"

	shift

	make_header $@

	exit 0
fi

# ls -d */* | xargs -I {} bash -c "cd '{}' && make fclean"
