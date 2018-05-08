#include <QCoreApplication>

#include <iostream>
#include <fstream>
#include "AudioFile.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    std::ofstream outfile;
    outfile.open ("example.svg", std::ofstream::binary);


    AudioFile<float> audioFile;
    bool loadedOK = audioFile.load (argv[1]);

    if (loadedOK)
    {
        int samples = audioFile.getNumSamplesPerChannel();

        std::cout << samples << std::endl;
        std::cout << audioFile.getBitDepth() << std::endl;
        std::cout << audioFile.getSampleRate() << std::endl;
        std::cout << audioFile.getNumChannels() << std::endl;


        outfile << R"(<svg width="1000" height="1000" xmlns="http://www.w3.org/2000/svg" version="1.1"><polyline stroke="black" stroke-linecap="round" stroke-linejoin="round" fill="none" points=")";

        float step = 1000.0/(float)samples;
        float x = step;
        for (int i = 0; i < samples; i++)
        {
            for (int k = 0; k < audioFile.getNumChannels(); k++)
            {
              //  int max = 50000;
               // if ((i < max) && (k == 0))
                if (k == 0)
                {
                    float sampleRaw = audioFile.samples[k][i];
                    float sample = sampleRaw * 250;
                    sample = 250.0 - sample;
                    outfile << x << ", " << sample << " ";

                    if (i < 10)
                    {
                        std::cout << sampleRaw << std::endl;
                    }
                }
            }
            x += step;
        }

        outfile << R"(" /></svg>)";
        outfile.close();
    }

    AudioFile<float> audioFileOut;

    audioFileOut.setAudioBuffer (audioFile.samples);
    audioFileOut.save("copied_audio_file.wav", AudioFileFormat::Wave);

    return a.exec();
}
