//
//	SPDX-License-Identifier: MIT
//	SPDX-FileCopyrightText: Copyright (C) 2026 Steffen A. Mork
//

#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>

struct Signal
{
	unsigned    bit;
	std::string name;
	std::string color;
};

int main(int argc, char * argv[])
{
	if (argc < 2)
	{
		std::cerr << "Usage: timing <input.bin>\n";
		return EXIT_FAILURE;
	}

	std::ifstream file(argv[1], std::ios::binary);

	if (!file)
	{
		return EXIT_FAILURE;
	}

	const std::vector<std::uint8_t> data(std::istreambuf_iterator<char>(file), {});

	constexpr int sample_width  =  2;
	constexpr int signal_height = 50;
	constexpr int top           = 30;
	constexpr int left          = 80;

	const int width  = data.size() * sample_width + left + 20;
	const int height = top + 2 * signal_height + 30;

	std::ofstream svg("timing.svg");

	svg << "<svg xmlns=\"http://www.w3.org/2000/svg\" " <<
		"width=\"" << width << "\" height=\"" << height <<
		"\" viewBox=\"0 0 " << width << " " << height << "\">\n";

	svg << "<rect width=\"100%\" height=\"100%\" fill=\"white\"/>\n";

	std::uint8_t last = 0;
	unsigned v_count = 0;
	unsigned h_count = 0;

	for (const std::uint8_t byte : data)
	{
		if (last != byte)
		{
			const std::uint8_t diff = last ^ byte;

			if ((diff & 1) && (byte & 1))
			{
				v_count++;
				printf("%u lines.\n", h_count);
				h_count = 0;
			}
			if ((diff & 2) && (byte & 2))
			{
				h_count++;
			}
			last = byte;
		}
	}

	static const Signal signals[] =
	{
		{ 0, "VSYNC",  "#d02020"},
		{ 1, "HBLANK", "#2050d0"}
	};

	for (unsigned s = 0; s < 2; s++)
	{
		const unsigned yHigh = top + s * signal_height;
		const unsigned yLow = yHigh + 25;

		svg << "<text x=\"5\" y=\"" << yHigh + 15
			<< "\" font-size=\"13\">" << signals[s].name
			<< "</text>\n";

		svg << "<path fill=\"none\" stroke=\""
			<< signals[s].color << "\" stroke-width=\"2\" d=\"";

		const std::uint8_t mask = 1 << signals[s].bit;
		for (size_t i = 0; i < data.size(); i++)
		{
			const bool value = data[i] & mask;
			const int  x     = left + i * sample_width;
			const int  y     = value ? yHigh : yLow;

			if (i == 0)
			{
				svg << "M " << x << " " << y;
			}
			else
			{
				svg << " V " << y
					<< " H " << x;
			}
		}

		svg << " H " << left + data.size() * sample_width
			<< "\"/>\n";
	}

	svg << "</svg>\n";

	std::cout << "Created timing.svg (" << data.size() << " samples)\n";

	return EXIT_SUCCESS;
}
