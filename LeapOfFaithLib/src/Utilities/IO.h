#pragma once
#include <stdexcept>
#include <vector>
#include <fstream>

namespace Utilities::IO
{
	static std::vector<char> readFile(const std::string& filename)
	{
		// ios binary to read file as binary
		// ios ate tells stream to start reading from end of file
		std::ifstream file(filename, std::ios::binary | std::ios::ate);

		// check if file stream successfully opened
		if (!file.is_open())
		{
			throw std::runtime_error("Failed to open a file!");
		}

		// Get current read position and use to resize file buffer
		size_t fileSize = (size_t)file.tellg();
		std::vector<char> fileBuffer(fileSize);

		// Move read position to start of the file
		file.seekg(0);

		// Read the file data into the buffer (stream "fileSize" in total)
		file.read(fileBuffer.data(), fileSize);

		// Close stream
		file.close();

		return fileBuffer;
	}
}
