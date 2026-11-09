//
//	SPDX-License-Identifier: MIT
//	SPDX-FileCopyrightText: Copyright (C) 2026 Steffen A. Mork
//

#include "yuvreader.h"
#include "yuvconverter.h"
#include "yuvwriter.h"

int main(int argc, char * argv[])
{
	YuvReader reader(
		argc >= 3 ? argv[2] : "frame_y.dat",
		argc >= 4 ? argv[3] : "frame_uv.dat");
	YuvWriter file(argc >= 2 ? argv[1] : "frame.jpg");

	if (file)
	{
		YuvConverter converter(file);

		converter.prepare(reader.yData().data(), reader.uvData().data());
		converter.save();
	}

	return EXIT_SUCCESS;
}
