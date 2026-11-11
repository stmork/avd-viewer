#include "yuvconverter.h"
#include "yuvreader.h"

YuvConverter::YuvConverter(FILE * output, const bool color)
{
	cinfo.err = jpeg_std_error(&jerr);
	jerr.error_exit = error;

	jpeg_create_compress(&cinfo);
	jpeg_stdio_dest(&cinfo, output);

	cinfo.image_width      = YuvReader::xSize;
	cinfo.image_height     = YuvReader::ySize & 0xfff0;

	if (color)
	{
		cinfo.input_components = 3;
		cinfo.in_color_space   = JCS_YCbCr;
	}
	else
	{
		cinfo.input_components = 1;
		cinfo.in_color_space   = JCS_GRAYSCALE;
	}

	jpeg_set_defaults(&cinfo);
	jpeg_set_quality(&cinfo, 90, TRUE);
}

void YuvConverter::prepare(
	const std::uint8_t * yPtr,
	const std::uint8_t * uvPtr)
{
	planar.clear();
	planar.reserve(YuvReader::xSize * YuvReader::ySize * cinfo.input_components);

	constexpr unsigned offset[2] { 0, (YuvReader::ySize + 1) >> 1};

	for (unsigned y = 0; y < YuvReader::ySize; y++)
	{
		unsigned yIndex = ((y / 2 + offset[y & 1]) * YuvReader::xSize);

		if (cinfo.in_color_space == JCS_YCbCr)
		{
			unsigned cIndex = yIndex >> 1;

			for (unsigned x = 0; x < YuvReader::xSize; x += 4)
			{
				const std::uint8_t u = uvPtr[cIndex++] + 0x80;
				const std::uint8_t v = uvPtr[cIndex++] + 0x80;

				planar.push_back(yPtr[yIndex++]);
				planar.push_back(u);
				planar.push_back(v);
				planar.push_back(yPtr[yIndex++]);
				planar.push_back(u);
				planar.push_back(v);

				planar.push_back(yPtr[yIndex++]);
				planar.push_back(u);
				planar.push_back(v);
				planar.push_back(yPtr[yIndex++]);
				planar.push_back(u);
				planar.push_back(v);
			}
		}
		else
		{
			for (unsigned x = 0; x < YuvReader::xSize; x += 4)
			{
				planar.push_back(yPtr[yIndex++]);
				planar.push_back(yPtr[yIndex++]);
				planar.push_back(yPtr[yIndex++]);
				planar.push_back(yPtr[yIndex++]);
			}
		}
	}
}

void YuvConverter::save()
{
	JSAMPROW       jrow[1];
	std::uint8_t * ptr = planar.data();

	// For each row...
	jpeg_start_compress(&cinfo, TRUE);
	while (cinfo.next_scanline < cinfo.image_height)
	{
		const unsigned offset =
			cinfo.image_width *
			cinfo.next_scanline *
			cinfo.input_components;

		jrow[0] = &ptr[offset];
		jpeg_write_scanlines(&cinfo, jrow, 1);
	}
	jpeg_finish_compress(&cinfo);
}

void YuvConverter::error(j_common_ptr cinfo)
{
	char message[JMSG_LENGTH_MAX];

	(*cinfo->err->format_message)(cinfo, message);
	std::fprintf(stderr, "libjpeg: %s\n", message);
	std::exit(EXIT_FAILURE);
}
