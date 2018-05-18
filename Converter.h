#ifndef CONVERTER_H
#define CONVERTER_H

#include <string>

void WavToSvg(const std::string &inputFileName, const std::string &outputFileName);
void SvgToWav(const std::string &inputFileName, const std::string &outputFileName, uint32_t sampleRate, int bitDepth);

#endif // CONVERTER_H
