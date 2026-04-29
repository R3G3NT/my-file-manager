#include <iostream>
#include <string>

#include "WAVHeader.h"
using namespace std;

void SetWAVHeader(WAVHeader& header, uint32_t size)
{
    header.subchunk1Size = 16;
    header.audioFormat = 1;
    header.numChannels = 2;
    header.sampleRate = 44100;
    header.bitsPerSample = 16;
    header.blockAlign = header.numChannels * (header.bitsPerSample / 8);
    header.byteRate = header.sampleRate * header.blockAlign;
    header.subchunk2Size = size;
    header.chunkSize = 36 + header.subchunk2Size;
}

void PrintHeaderInfo(const WAVHeader& header)
{
    cout << "========= WAV HEADER INFO =========" << endl;

    // Chunk RIFF
    cout << "RIFF Chunk ID      : " << string(header.chunkId, 4) << endl;
    cout << "Total File Size    : " << header.chunkSize << " bytes" << endl;
    cout << "Format             : " << string(header.format, 4) << endl;

    // Subchunk 1 (fmt)
    cout << "Subchunk1 ID       : " << string(header.subchunk1Id, 4) << endl;
    cout << "Subchunk1 Size     : " << header.subchunk1Size << endl;
    cout << "Audio Format (PCM) : " << header.audioFormat << " (1 = Uncompressed)" << endl;
    cout << "Num Channels       : " << header.numChannels << (header.numChannels == 1 ? " (Mono)" : " (Stereo)") << endl;
    cout << "Sample Rate        : " << header.sampleRate << " Hz" << endl;
    cout << "Byte Rate          : " << header.byteRate << " bytes/sec" << endl;
    cout << "Block Align        : " << header.blockAlign << " bytes" << endl;
    cout << "Bits Per Sample    : " << header.bitsPerSample << " bits" << endl;

    // Subchunk 2 (data)
    cout << "Subchunk2 ID       : " << string(header.subchunk2Id, 4) << endl;
    cout << "Data Size          : " << header.subchunk2Size << " bytes" << endl;

    // Продолжительность файла
    int duration = (double)header.subchunk2Size / header.byteRate;
    cout << "Duration           : " << duration << " seconds" << endl;
    cout << "===================================\n" << endl;
}