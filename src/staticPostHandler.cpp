/* ************************************************************************** */
/*                                                     __ __   __             */
/*    WebServ                                         / // / /__ \            */
/*                By: skoulen, bphilago, znichola    / // /_ __/ /            */
/*                                                  /__  __// __/             */
/*                Created: 2023/06/15 12:38:14        /_/  /____/ lausanne.ch */
/*                                                                            */
/* ************************************************************************** */

#include "staticPostHandler.hpp"

std::istream &operator>>(std::istream &is, char const *s);
static int	multipartFormDataParser(const std::string boundary, const std::string data, std::vector<t_file> &files);

int	staticPostHandler(const HTTPRequest& req)
{
	// check if content-type is multipart/form-data and that it has a boundary
	std::string	contentType = utils::toUpper(req.getHeader("content-type"));
	std::string	boundary;
	std::cout <<contentType<<std::endl;
	std::istringstream iss(contentType);
	iss >> "MULTIPART/FORM-DATA; BOUNDARY=">>boundary;
	if (!iss || boundary == "")
	{
		//error
		std::cout <<"Error: content-type is not multipart/form-data or boundary is not defined"<<std::endl;
		return (400);
	}

	std::vector<t_file> files;

	int	res = multipartFormDataParser(boundary, req.getBody(), files);
	if (res < 0)
	{
		//error
		std::cout << "Error when parsing the multipart/form-data POST payload" <<std::endl;
		return (400);
	}

	// retrieve the location where the file should be uploaded

	// try to put them in the right location
	for (unsigned int i = 0; i < files.size(); i++)
	{
		std::string strFilename = "somerandomfilename_" + files[i].filename;
		const char	*filename = strFilename.c_str();
		int fd = open(filename, O_WRONLY | O_CREAT, 0644);
		if (fd < 0)
		{
			//error
			std::cout << "Error when opening file "<<filename<<" : "<<std::strerror(errno)<<std::endl;
			return (500);
		}
		close(fd);

		std::ofstream file;
		file.open(filename);
		file << files[i].content;
		if (!file)
		{
			//error
			std::cout << "Error when writing to file "<<filename<<" : "<<std::strerror(errno)<<std::endl;
			return (500);
		}
		file.close();
	}

	// generate the response
	return (204);
}

std::string copyLine(std::string input);
bool	matchBoundary(const std::string &str, const std::string &boundary);
bool	matchEndBoundary(const std::string &str, const std::string &boundary);
int	formDataParser(std::string &data, const std::string &boundary, t_file &parsed_file);

/* generate a list of files from some multipart/form-data POST request body */
static int	multipartFormDataParser(const std::string boundary, const std::string data, std::vector<t_file> &files)
{
	std::string	line;
	std::string	dataCpy = data;

	/* Ignore anything that comes before the initial boundary */
	do {
		line = copyLine(dataCpy);
		dataCpy = dataCpy.substr(line.size(), dataCpy.size());
	} while (!matchBoundary(line, boundary));


	/* start parsing the multiple parts */
	while (true)
	{

		files.push_back(t_file());
		if (formDataParser(dataCpy, boundary, files[files.size() - 1]) != 0)
			return (-1);

		line = copyLine(dataCpy);
		if (matchEndBoundary(line, boundary))
		{
			break ;
		}
		else
		{
			dataCpy = dataCpy.substr(line.size(), data.size());
		}
	}

	/* Ignore anything that comes after the end boundary */
	return (0);
}

int	formDataParser(std::string &data, const std::string &boundary, t_file &parsed_file)
{
	std::string	line;
	std::string	trimmedLine;
	int			state = 0; //header state

	/* create some random file name in case there is no filename provided */
	parsed_file.filename = "no_filename_provided_" + utils::randomString(15);

	while (true)
	{
		line = copyLine(data);

		if (matchBoundary(line, boundary))
			break;

		//are we in header or body state?
		if (state == 0)
		{
			trimmedLine = line;
			utils::trim(trimmedLine, "\n\r\t\v\f ");

			if (trimmedLine == "")
			{
				state = 1;
			}
			else
			{
				std::pair<std::string, std::string> header;
				if (utils::parseHeader(trimmedLine, header) != 0)
					return (-1);
				if (utils::streq_ci(header.first, "Content-disposition"))
				{
					std::vector<std::string> list;
					if (utils::parseList(header.second, list) != 0)
						return (-1);
					for (unsigned int i = 0; i < list.size(); i++)
					{
						std::pair<std::string, std::string> keyValue;
						if (utils::parseKeyValue(list[i], keyValue) != 0)
							return (-1);
						if (utils::streq_ci(keyValue.first, "filename"))
						{
							parsed_file.filename = keyValue.second;
						}
					}
				}
			}
		}
		else
		{
			parsed_file.content += line;
		}
		data = data.substr(line.size(), data.size());
	}
	return (0);
}

/* copies the first line, with the newline character */
std::string copyLine(std::string input)
{
	size_t pos = input.find_first_of('\n');
	if (pos == std::string::npos)
		return (std::string(input));
	else
		return (input.substr(0, pos + 1));
}

//check if start of string matches boundary
bool	matchBoundary(const std::string &str, const std::string &boundary)
{
	std::string boundaryPlus = "--" + boundary;
	unsigned int boundaryLength = boundaryPlus.size();
	return (str.substr(0, boundaryLength) == boundaryPlus);
}

bool	matchEndBoundary(const std::string &str, const std::string &boundary)
{
	std::string boundaryPlus = "--" + boundary + "--";
	unsigned int boundaryLength = boundaryPlus.size();
	return (str.substr(0, boundaryLength) == boundaryPlus);
}
