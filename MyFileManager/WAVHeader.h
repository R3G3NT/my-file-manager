#pragma once
#include <cstdint>

#pragma pack(push, 1)
struct WAVHeader
{
    char chunkId[4] = { 'R', 'I', 'F', 'F' };
    uint32_t chunkSize = 1;
    char format[4] = { 'W', 'A', 'V', 'E' };
    char subchunk1Id[4] = { 'f', 'm', 't', ' ' };
    uint32_t subchunk1Size = 16;
    uint16_t audioFormat = 1;
    uint16_t numChannels = 2;
    uint32_t sampleRate = 44100;
    uint32_t byteRate = 44100 * 2 * 2;
    uint16_t blockAlign = 2 * 2;
    uint16_t bitsPerSample = 16;
    char subchunk2Id[4] = { 'd', 'a', 't', 'a' };
    uint32_t subchunk2Size = 1;
};
#pragma pack(pop)

void SetWAVHeader(WAVHeader& header, uint32_t size);

void PrintHeaderInfo(const WAVHeader& header);
