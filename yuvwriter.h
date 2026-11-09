//
//	SPDX-License-Identifier: MIT
//	SPDX-FileCopyrightText: Copyright (C) 2026 Steffen A. Mork
//

#pragma once

#ifndef AVD_YUVWRITER_H
#define AVD_YUVWRITER_H

#include <cstdio>

class YuvWriter
{
	FILE * output = nullptr;

public:
	YuvWriter() = delete;

	explicit YuvWriter(const char * filename);

	~YuvWriter();

	inline operator FILE * () const
	{
		return output;
	}
};

#endif // AVD_YUVWRITER_H
