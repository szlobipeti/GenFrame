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

			public: static constexpr uint32_t RIFF = 0x46464952;
			public: static constexpr uint32_t WAVE = 0x45564157;
			public: static constexpr uint32_t fmt_ = 0x20746d66;
			public: static constexpr uint32_t fact = 0x74636166;
			public: static constexpr uint32_t data = 0x61746164;

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