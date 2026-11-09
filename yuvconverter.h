//
//	SPDX-License-Identifier: MIT
//	SPDX-FileCopyrightText: Copyright (C) 2026 Steffen A. Mork
//

#pragma once

#ifndef AVD_YUVCONVERTER_H
#define AVD_YUVCONVERTER_H

#include <cstdio>
#include <cstdint>
#include <vector>

#include <jpeglib.h>

class YuvConverter
{
	std::vector<std::uint8_t> planar;
	jpeg_compress_struct      cinfo{};
	jpeg_error_mgr            jerr{};

public:
	YuvConverter() = delete;
	explicit YuvConverter(FILE * output);

	~YuvConverter()
	{
		jpeg_destroy_compress(&cinfo);
	}

	void prepare(const std::uint8_t * yPtr, const std::uint8_t * uvPtr);

	void save();

private:
	static void error(j_common_ptr cinfo);
};

#endif // AVD_YUVCONVERTER_H
