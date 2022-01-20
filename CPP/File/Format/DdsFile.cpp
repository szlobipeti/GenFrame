#include "DdsFile.h"
#include "..\..\Binary\GenBin.h"

gen::file::format::ddsFile::~ddsFile()
{
	if (pixelData != nullptr)
	{
		free(pixelData);
	}
	return;
}

bool gen::file::format::ddsFile::oRead(gen::bin::reader& bin)
{
	std::streamsize fileSize = bin.getSize();
	if (fileSize < 128)
	{
		return false;
	}

	uint32_t Magic = 0;
	bin.read(Magic);

	if (Magic != DDS_MAGIC)
	{
		return false;
	}

	bin.read(header, 124);

	pixelDataSize = (size_t)fileSize - 128;
	pixelData = (char*)malloc(pixelDataSize);
	bin.read(pixelData, pixelDataSize);

	return true;
}

bool gen::file::format::ddsFile::oWrite(gen::bin::writer& bin)
{
	uint32_t Magic = DDS_MAGIC;
	
	bin.write(Magic);

	bin.write(header, 124);
	bin.write(pixelData, pixelDataSize);

	return true;
}

void gen::file::format::ddsFile::decompressBC1(const uint32_t width, const uint32_t height, const char* data, char*& scan0, size_t offset)
{
	free(scan0);
	scan0 = (char*)malloc(width * height * 3);

	for (size_t blockRow = 0; blockRow < size_t(height / 4); blockRow++)
	{
		for (size_t blockColumn = 0; blockColumn < size_t(width / 4); blockColumn++)
		{
			uint32_t colors = *(uint32_t*)((char*)data + blockRow * width / 4 * 8 + blockColumn * 8 + offset);

			int color_r[4], color_g[4], color_b[4];

			color_b[0] = (colors & 0x0000001F) << 3;			// mask the last 5 bits, bit shift 3 to the left
			color_b[0] |= color_b[0] >> 5;

			color_g[0] = (colors & 0x000007E0) >> (5 - 2);
			color_g[0] |= color_g[0] >> 6;

			color_r[0] = (colors & 0x0000F800) >> (11 - 3);
			color_r[0] |= color_r[0] >> 5;

			color_b[1] = (colors & 0x001F0000) >> (16 - 3);
			color_b[1] |= color_b[1] >> 5;

			color_g[1] = (colors & 0x07E00000) >> (21 - 2);
			color_g[1] |= color_g[1] >> 6;

			color_r[1] = (colors & 0xF8000000) >> (27 - 3);		// mask the first 5 bits, bit shift right 24
			color_r[1] |= color_r[1] >> 5;

			if ((colors & 0xFFFF) > ((colors & 0xFFFF0000) >> 16))
			{	// not using 1 bit alpha
				int test = color_r[0] * (2 / 3);
				color_r[2] = 2 * color_r[0] / 3 + color_r[1] / 3;
				color_g[2] = 2 * color_g[0] / 3 + color_g[1] / 3;
				color_b[2] = 2 * color_b[0] / 3 + color_b[1] / 3;

				color_r[3] = color_r[0] / 3 + 2 * color_r[1] / 3;
				color_g[3] = color_g[0] / 3 + 2 * color_g[1] / 3;
				color_b[3] = color_b[0] / 3 + 2 * color_b[1] / 3;
			}
			else
			{	// using 1 bit alpha
				color_r[2] = (color_r[0] + color_r[1]) / 2;
				color_g[2] = (color_g[0] + color_g[1]) / 2;
				color_b[2] = (color_b[0] + color_b[1]) / 2;

				color_r[3] = color_g[3] = color_b[3] = 0;
			}
			// at this point we have all our colors computed

			// another 32 bits storing the indexes
			uint32_t pixels = *(uint32_t*)((char*)data + blockRow * width / 4 * 8 + blockColumn * 8 + offset + sizeof(uint32_t));
			for (int row = 0; row < 4; row++)
			{
				for (int column = 0; column < 4; column++)
				{
					int pixel = (pixels >> ((column * 4 + row) * 2)) & 0x3;

					*(uint8_t*)((char*)scan0 + ((blockRow * 4 + column) * width * 3) + (blockColumn * 12 + (row) * 3)) = color_b[pixel];
					*(uint8_t*)((char*)scan0 + ((blockRow * 4 + column) * width * 3) + (blockColumn * 12 + (row) * 3 + 1)) = color_g[pixel];
					*(uint8_t*)((char*)scan0 + ((blockRow * 4 + column) * width * 3) + (blockColumn * 12 + (row) * 3 + 2)) = color_r[pixel];
				}
			}
		}
	}
}

void gen::file::format::ddsFile::decompressBC3(const uint32_t width, const uint32_t height, const char* data, char*& scan0, size_t offset)
{
	free(scan0);
	scan0 = (char*)malloc(width * height * 4);

	for (size_t blockRow = 0; blockRow < size_t(height / 4); blockRow++)
	{
		for (size_t blockColumn = 0; blockColumn < size_t(width / 4); blockColumn++)
		{
			uint64_t alphaData = *(uint64_t*)((char*)data + blockRow * width / 4 * 16 + blockColumn * 16 + offset);
			uint32_t colors = *(uint32_t*)((char*)data + blockRow * width / 4 * 16 + blockColumn * 16 + offset + sizeof(uint64_t));
			uint32_t pixels = *(uint32_t*)((char*)data + blockRow * width / 4 * 16 + blockColumn * 16 + offset + sizeof(uint64_t) + sizeof(uint32_t));

			int alpha[8];

			alpha[0] = (alphaData & 0x00000000000000FF);
			alpha[1] = (alphaData & 0x000000000000FF00) >> 8;

			if (alpha[0] > alpha[1])
			{
				// 6 interpolated alpha values.
				alpha[2] = 6 * alpha[0] / 7 + 1 * alpha[1] / 7; // bit code 010
				alpha[3] = 5 * alpha[0] / 7 + 2 * alpha[1] / 7; // bit code 011
				alpha[4] = 4 * alpha[0] / 7 + 3 * alpha[1] / 7; // bit code 100
				alpha[5] = 3 * alpha[0] / 7 + 4 * alpha[1] / 7; // bit code 101
				alpha[6] = 2 * alpha[0] / 7 + 5 * alpha[1] / 7; // bit code 110
				alpha[7] = 1 * alpha[0] / 7 + 6 * alpha[1] / 7; // bit code 111
			}
			else
			{
				// 4 interpolated alpha values.
				alpha[2] = 4 * alpha[0] / 5 + 1 * alpha[1] / 5; // bit code 010
				alpha[3] = 3 * alpha[0] / 5 + 2 * alpha[1] / 5; // bit code 011
				alpha[4] = 2 * alpha[0] / 5 + 3 * alpha[1] / 5; // bit code 100
				alpha[5] = 1 * alpha[0] / 5 + 4 * alpha[1] / 5; // bit code 101
				alpha[6] = 0;                         // bit code 110
				alpha[7] = 255;                       // bit code 111
			}

			int color_r[4], color_g[4], color_b[4];

			color_b[0] = (colors & 0x0000001F) << 3;			// mask the last 5 bits, bit shift 3 to the left
			color_b[0] |= color_b[0] >> 5;

			color_g[0] = (colors & 0x000007E0) >> (5 - 2);
			color_g[0] |= color_g[0] >> 6;

			color_r[0] = (colors & 0x0000F800) >> (11 - 3);
			color_r[0] |= color_r[0] >> 5;

			color_b[1] = (colors & 0x001F0000) >> (16 - 3);
			color_b[1] |= color_b[1] >> 5;

			color_g[1] = (colors & 0x07E00000) >> (21 - 2);
			color_g[1] |= color_g[1] >> 6;

			color_r[1] = (colors & 0xF8000000) >> (27 - 3);		// mask the first 5 bits, bit shift right 24
			color_r[1] |= color_r[1] >> 5;

			if ((colors & 0xFFFF) > ((colors & 0xFFFF0000) >> 16))
			{	// not using 1 bit alpha
				int test = color_r[0] * (2 / 3);
				color_r[2] = 2 * color_r[0] / 3 + color_r[1] / 3;
				color_g[2] = 2 * color_g[0] / 3 + color_g[1] / 3;
				color_b[2] = 2 * color_b[0] / 3 + color_b[1] / 3;

				color_r[3] = color_r[0] / 3 + 2 * color_r[1] / 3;
				color_g[3] = color_g[0] / 3 + 2 * color_g[1] / 3;
				color_b[3] = color_b[0] / 3 + 2 * color_b[1] / 3;
			}
			else
			{	// using 1 bit alpha
				color_r[2] = (color_r[0] + color_r[1]) / 2;
				color_g[2] = (color_g[0] + color_g[1]) / 2;
				color_b[2] = (color_b[0] + color_b[1]) / 2;

				color_r[3] = color_g[3] = color_b[3] = 0;
			}
			// at this point we have all our colors computed

			// another 32 bits storing the indexes


			for (int row = 0; row < 4; row++)
			{
				for (int column = 0; column < 4; column++)
				{
					int pixel;

					pixel = (pixels >> ((column * 4 + row) * 2)) & 0x3;

					*(uint8_t*)((char*)scan0 + ((blockRow * 4 + column) * width * 4) + (blockColumn * 16 + (row) * 4)) = color_b[pixel];
					*(uint8_t*)((char*)scan0 + ((blockRow * 4 + column) * width * 4) + (blockColumn * 16 + (row) * 4 + 1)) = color_g[pixel];
					*(uint8_t*)((char*)scan0 + ((blockRow * 4 + column) * width * 4) + (blockColumn * 16 + (row) * 4 + 2)) = color_r[pixel];

					pixel = (alphaData >> ((column * 4 + row) * 3 + 16)) & 0x7;
					*(uint8_t*)((char*)scan0 + ((blockRow * 4 + column) * width * 4) + (blockColumn * 16 + (row) * 4 + 3)) = alpha[pixel];
				}
			}
		}
	}
}
