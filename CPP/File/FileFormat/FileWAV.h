#pragma once

#include "..\File.h"
#include <Windows.h>
#include <mmreg.h>

namespace gen
{
	namespace file
	{
		namespace format
		{
			class FileWAV : public File
			{
			public:	struct sFactChunk
			{
				uint32_t DataSize = 0;
				char* Data = nullptr;

				~sFactChunk();
			};

			public: static constexpr uint32_t RIFF = 1179011410;
			public: static constexpr uint32_t WAVE = 1163280727;
			public: static constexpr uint32_t fmt_ = 544501094;
			public: static constexpr uint32_t fact = 1952670054;
			public: static constexpr uint32_t data = 0x64617461;

			public: tWAVEFORMATEX Waveformat;
			public: char* WaveformatEx = nullptr;
			public: char* WaveData = nullptr;
			public: size_t WaveDataSize = 0;
			public: sFactChunk FactChunk;

			public: ~FileWAV();

			private: bool oRead(std::ifstream& inFile, size_t dataBegin, size_t dataSize) override;
			private: bool oWrite(std::ofstream& outFile) override;
			};
		}
	}
}