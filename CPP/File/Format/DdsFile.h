#pragma once

#include "..\File.h"
#include "..\..\Dependencies\DDS.h"

namespace gen::file::format
{
	class ddsFile : public iFile
	{
	public: DDS_HEADER header;
	public: char* pixelData = nullptr;
	public: size_t pixelDataSize = 0;

	public: ~ddsFile();

	private: __forceinline size_t oType() override { return (size_t)type::dds; };
	private: bool oRead(gen::bin::reader& bin) override;
	private: bool oWrite(gen::bin::writer& bin) override;

	public: static void decompressBC1(const uint32_t width, const uint32_t height, const char* data, char*& scan0, size_t offset = 0);
	public: static void decompressBC3(const uint32_t width, const uint32_t height, const char* data, char*& scan0, size_t offset = 0);
	};
}