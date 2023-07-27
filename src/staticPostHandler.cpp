/* ************************************************************************** */
/*                                                     __ __   __             */
/*    WebServ                                         / // / /__ \            */
/*                By: skoulen, bphilago, znichola    / // /_ __/ /            */
/*                                                  /__  __// __/             */
/*                Created: 2023/06/15 12:38:14        /_/  /____/ lausanne.ch */
/*                                                                            */
/* ************************************************************************** */

#include "staticPostHandler.hpp"
#include <iostream>

static int	multipartFormDataParser(const std::string boundary, const std::string data, std::vector<t_file> files);

int	staticPostHandler(const HTTPRequest& req)
{
	// check if content-type is multipart/form-data and that it has a boundary
	std::string	contentType = req.getHeader("content-type");
	std::string	boundary;
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
		std::string strFilename = "somerandomfilename_" + files[0].filename;
		const char	*filename = strFilename.c_str();
		int fd = open(filename, OWRONLY | OCREAT, 0644);
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

/* generate a list of files from some multipart/form-data POST request body */
static int	multipartFormDataParser(const std::string boundary, const std::string data, std::vector<t_file> files)
{
	/* placeholder code, no actual parsing is done yet */
	(void)boundary;
	(void)data;
	t_file uploaded_file;
	uploaded_file.filename = "stupidTestFile.txt";
	uploaded_file.content = "Hello, world!";
	files.push_back(uploaded_file);
	return (0);
}
