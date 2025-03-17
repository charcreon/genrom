#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>

int main(int argc, char* argv[])
{
    std::map<std::string, std::string> params;
    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (arg[0] == '-')
        {
            std::string key = arg.substr(1);
            if (i + 1 < argc && argv[i + 1][0] != '-')
            {
                params[key] = argv[++i];
            }
            else
            {
                params[key] = "";
            }
        }
    }

    if (params.find("out") == params.end())
    {
        std::cerr << "Error: -out parameter is required." << std::endl;
        return 1;
    }

    long long size = 0;
    if (params.find("size") != params.end())
    {
        std::string sizeArg = params["size"];
        std::stringstream ss(sizeArg);
        if (sizeArg.find("kb") != std::string::npos)
        {
            ss >> size;
            size *= 1024;
        }
        else if (sizeArg.find("mb") != std::string::npos)
        {
            ss >> size;
            size *= 1024 * 1024;
        }
        else
        {
            ss >> size;
        }
        if (ss.fail())
        {
            std::cerr << "Error: Invalid size format." << std::endl;
            return 1;
        }
    }

    std::vector<char> buffer;
    if (params.find("in") != params.end())
    {
        std::string inputFilename = params["in"];
        std::ifstream inputFile(inputFilename, std::ios::binary);
        if (inputFile)
        {
            buffer.assign(std::istreambuf_iterator<char>(inputFile), std::istreambuf_iterator<char>());
        }
        else
        {
            std::cerr << "Error: Cannot open input file " << inputFilename << std::endl;
            return 1;
        }
    }

    if (size > 0)
    {
        buffer.resize(size, 0x00);
    }

    std::string outputFilename = params["out"];
    std::ofstream outputFile(outputFilename, std::ios::binary);
    if (!outputFile)
    {
        std::cerr << "Error: Cannot open output file " << outputFilename << std::endl;
        return 1;
    }

    outputFile.write(buffer.data(), buffer.size());

    return 0;
}
