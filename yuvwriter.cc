//
//	SPDX-License-Identifier: MIT
//	SPDX-FileCopyrightText: Copyright (C) 2026 Steffen A. Mork
//

#include <cstdlib>

#include "yuvwriter.h"

YuvWriter::YuvWriter(const char * filename)
{
	output = std::fopen(filename, "wb");
	if (!output)
	{
		std::perror("Cannot create JPEG file");
		std::exit(EXIT_FAILURE);
	}
}

YuvWriter::~YuvWriter()
{
	std::fclose(output);
}
