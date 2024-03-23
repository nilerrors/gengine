#include "lib/easy_image.h"
#include "lib/ini_configuration.h"

#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include "gengine/line.h"
#include "gengine/line3d.h"
#include "gengine/zbufwireframe.h"

using namespace gengine;

size_t total_alloc_size = 0;
size_t total_alloc_count = 0;
size_t total_dealloc_size = 0;
size_t total_dealloc_count = 0;

void *operator new(size_t size)
{
	total_alloc_size += size;
	total_alloc_count++;
	void *p = malloc(size);
	if (p == nullptr)
	{
		throw std::bad_alloc();
	}
	return p;
}

void operator delete(void *p) noexcept
{
	total_dealloc_count++;
	free(p);
}

void operator delete(void *p, size_t size) noexcept
{
	total_dealloc_count++;
	total_dealloc_size += size;
	free(p);
}

void *operator new[](size_t size)
{
	total_alloc_size += size;
	total_alloc_count++;
	void *p = malloc(size);
	if (p == nullptr)
	{
		throw std::bad_alloc();
	}
	return p;
}

void operator delete[](void *p) noexcept
{
	total_dealloc_count++;
	free(p);
}

void operator delete[](void *p, size_t size) noexcept
{
	total_dealloc_count++;
	total_dealloc_size += size;
	free(p);
}

void print_alloc_info()
{
	std::cout << "Total alloc size: " << total_alloc_size / 1000 / 1000 << "MB" << std::endl;
	std::cout << "Total alloc count: " << total_alloc_count << std::endl;
	std::cout << "Total dealloc size: " << total_dealloc_size / 1000 / 1000 << "MB" << std::endl;
	std::cout << "Total dealloc count: " << total_dealloc_count << std::endl;
	std::cout << std::endl;

	total_alloc_size = 0;
	total_alloc_count = 0;
	total_dealloc_count = 0;
	total_dealloc_size = 0;
}

img::EasyImage generate_image(const ini::Configuration &configuration)
{
    const std::string imageType = configuration["General"]["type"].as_string_or_die();

    img::EasyImage image(0, 0);

	if (imageType == "2DLSystem")
	{
		image = Draw2DLSystem(configuration["General"], configuration["2DLSystem"]).drawLSystem();
	}
	else if (imageType == "Wireframe")
	{
		image = Wireframe(configuration).drawWireframe();
	}
	else if (imageType == "ZBufferedWireframe")
	{
		image  = ZBufferedWireframe(configuration).drawWireframe();
	}

	print_alloc_info();

	return image;
}

int main(int argc, char const* argv[])
{
        int retVal = 0;
        try
        {
                std::vector<std::string> args = std::vector<std::string>(argv+1, argv+argc);
                if (args.empty()) {
                        std::ifstream fileIn("filelist");
                        std::string filelistName;
                        while (std::getline(fileIn, filelistName)) {
                                args.push_back(filelistName);
                        }
                }
                for(std::string fileName : args)
                {
                        ini::Configuration conf;
                        try
                        {
                                std::ifstream fin(fileName);
                                if (fin.peek() == std::istream::traits_type::eof()) {
                                    std::cout << "Ini file appears empty. Does '" <<
                                    fileName << "' exist?" << std::endl;
                                    continue;
                                }
                                fin >> conf;
                                fin.close();
                        }
                        catch(ini::ParseException& ex)
                        {
                                std::cerr << "Error parsing file: " << fileName << ": " << ex.what() << std::endl;
                                retVal = 1;
                                continue;
                        }

                        img::EasyImage image = generate_image(conf);
                        if(image.get_height() > 0 && image.get_width() > 0)
                        {
                                std::string::size_type pos = fileName.rfind('.');
                                if(pos == std::string::npos)
                                {
                                        //filename does not contain a '.' --> append a '.bmp' suffix
                                        fileName += ".bmp";
                                }
                                else
                                {
                                        fileName = fileName.substr(0,pos) + ".bmp";
                                }
                                try
                                {
                                        std::ofstream f_out(fileName.c_str(),std::ios::trunc | std::ios::out | std::ios::binary);
                                        f_out << image;

                                }
                                catch(std::exception& ex)
                                {
                                        std::cerr << "Failed to write image to file: " << ex.what() << std::endl;
                                        retVal = 1;
                                }
                        }
                        else
                        {
                                std::cout << "Could not generate image for " << fileName << std::endl;
                        }
                }
        }
        catch(const std::bad_alloc &exception)
        {
    		//When you run out of memory this exception is thrown. When this happens the return value of the program MUST be '100'.
    		//Basically this return value tells our automated test scripts to run your engine on a pc with more memory.
    		//(Unless of course you are already consuming the maximum allowed amount of memory)
    		//If your engine does NOT adhere to this requirement you risk losing points because then our scripts will
		//mark the test as failed while in reality it just needed a bit more memory
                std::cerr << "Error: insufficient memory" << std::endl;
                retVal = 100;
        }
        return retVal;
}
