#pragma once

#include "..\File.h"
#include <Windows.h>
#include <mmreg.h>

namespace gen::file::format
{
	class wavFile : public iFile
	{
	public:	struct factChunk
	{
		uint32_t dataSize = 0;
		char* data = nullptr;
	
		~factChunk();
	};
	
	public: static constexpr uint32_t RIFF = 0x46464952;
	public: static constexpr uint32_t WAVE = 0x45564157;
	public: static constexpr uint32_t fmt_ = 0x20746d66;
	public: static constexpr uint32_t fact = 0x74636166;
	public: static constexpr uint32_t data = 0x61746164;
	
	public: tWAVEFORMATEX waveformat;
	public: char* waveformatEx = nullptr;
	public: char* waveData = nullptr;
	public: size_t waveDataSize = 0;
	public: factChunk factChunk;
	
	public: ~wavFile();
	
	private: __forceinline size_t oType() override { return (size_t)type::wav; };
	private: bool oRead(gen::bin::reader& bin) override;
	private: bool oWrite(gen::bin::writer& bin) override;
	
	public: bool toByteStream(char*& stream);
	};
}