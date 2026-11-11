//
//	SPDX-License-Identifier: MIT
//	SPDX-FileCopyrightText: Copyright (C) 2026 Steffen A. Mork
//

#include "yuvreader.h"

namespace fs = std::filesystem;

YuvReader::YuvReader(const char * filename_y, const char * filename_uv)
{
	read(y,  filename_y,  YuvReader::xSize);
	read(uv, filename_uv, YuvReader::xSize >> 1);
}

inline void YuvReader::read(
	std::vector<uint8_t> & vector,
	const char      *      filename,
	const unsigned         width)
{
	if (fs::exists(filename))
	{
		const std::size_t size  = fs::file_size(filename);
		FILE       *      input = std::fopen(filename, "rb");

		if (input != nullptr)
		{
			vector.clear();
			vector.reserve(size);
			vector.resize(width * YuvReader::ySize);

			std::fread(vector.data(), width, ySize, input);

			printf("size:  %zu/%zu %s\n", vector.size(), size, filename);
			std::fclose(input);
		}
	}
}
