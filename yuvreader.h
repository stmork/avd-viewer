//
//	SPDX-License-Identifier: MIT
//	SPDX-FileCopyrightText: Copyright (C) 2026 Steffen A. Mork
//

#pragma once

#ifndef AVD_YUVREADER_H
#define AVD_YUVREADER_H

#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <vector>

class YuvReader
{
	std::vector<std::uint8_t> y;
	std::vector<std::uint8_t> uv;

public:
	static constexpr std::size_t xSize = 720;
	static constexpr std::size_t ySize = 625;

	YuvReader() = delete;
	explicit YuvReader(const char * filename_y, const char * filename_uv);

	inline const std::vector<std::uint8_t> & yData() const
	{
		return y;
	}

	inline const std::vector<std::uint8_t> & uvData() const
	{
		return uv;
	}

private:
	void read(
		std::vector<std::uint8_t> & vector,
		const char         *        filename,
		const unsigned              width);
};

#endif // AVD_YUVREADER_H
