#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cmath>

#include "AudioFile.h"
#include "pugixml.hpp"

static int nsvg__isspace(char c)
{
    return strchr(" \t\n\v\f\r", c) != 0;
}

static int nsvg__isdigit(char c)
{
    return c >= '0' && c <= '9';
}


// We roll our own string to float because the std library one uses locale and messes things up.
static double nsvg__atof(const char* s)
{
    char* cur = (char*)s;
    char* end = NULL;
    double res = 0.0, sign = 1.0;
    long long intPart = 0, fracPart = 0;
    char hasIntPart = 0, hasFracPart = 0;

    // Parse optional sign
    if (*cur == '+') {
        cur++;
    } else if (*cur == '-') {
        sign = -1;
        cur++;
    }

    // Parse integer part
    if (nsvg__isdigit(*cur)) {
        // Parse digit sequence
        intPart = (double)strtoll(cur, &end, 10);
        if (cur != end) {
            res = (double)intPart;
            hasIntPart = 1;
            cur = end;
        }
    }

    // Parse fractional part.
    if (*cur == '.') {
        cur++; // Skip '.'
        if (nsvg__isdigit(*cur)) {
            // Parse digit sequence
            fracPart = strtoll(cur, &end, 10);
            if (cur != end) {
                res += (double)fracPart / pow(10.0, (double)(end - cur));
                hasFracPart = 1;
                cur = end;
            }
        }
    }

    // A valid number should have integer or fractional part.
    if (!hasIntPart && !hasFracPart)
        return 0.0;

    // Parse optional exponent
    if (*cur == 'e' || *cur == 'E') {
        int expPart = 0;
        cur++; // skip 'E'
        expPart = strtol(cur, &end, 10); // Parse digit sequence with sign
        if (cur != end) {
            res *= pow(10.0, (double)expPart);
        }
    }

    return res * sign;
}


static const char* nsvg__parseNumber(const char* s, char* it, const int size)
{
    const int last = size-1;
    int i = 0;

    // sign
    if (*s == '-' || *s == '+') {
        if (i < last) it[i++] = *s;
        s++;
    }
    // integer part
    while (*s && nsvg__isdigit(*s)) {
        if (i < last) it[i++] = *s;
        s++;
    }
    if (*s == '.') {
        // decimal point
        if (i < last) it[i++] = *s;
        s++;
        // fraction part
        while (*s && nsvg__isdigit(*s)) {
            if (i < last) it[i++] = *s;
            s++;
        }
    }
    // exponent
    if (*s == 'e' || *s == 'E') {
        if (i < last) it[i++] = *s;
        s++;
        if (*s == '-' || *s == '+') {
            if (i < last) it[i++] = *s;
            s++;
        }
        while (*s && nsvg__isdigit(*s)) {
            if (i < last) it[i++] = *s;
            s++;
        }
    }
    it[i] = '\0';

    return s;
}

static const char* nsvg__getNextPathItem(const char* s, char* it)
{
    it[0] = '\0';
    // Skip white spaces and commas
    while (*s && (nsvg__isspace(*s) || *s == ',')) s++;
    if (!*s) return s;
    if (*s == '-' || *s == '+' || *s == '.' || nsvg__isdigit(*s)) {
        s = nsvg__parseNumber(s, it, 64);
    } else {
        // Parse command
        it[0] = *s++;
        it[1] = '\0';
        return s;
    }

    return s;
}

static void nsvg__parsePoly(const char* s, std::vector<double> &samples)
{
    double args[2];
    int nargs = 0;
    int npts = 0;
    char item[64];

    while (*s)
    {
        s = nsvg__getNextPathItem(s, item);
        args[nargs++] = nsvg__atof(item);
        if (nargs >= 2)
        {
            double sample = 250.0 - args[1];
            samples.push_back(sample/250.0);

            nargs = 0;
            npts++;

            if (npts < 10)
            {
                std::cout << "Point: " << args[0] << ", " << args[1] << std::endl;
            }
        }
    }
}

std::string GetSuffix(const std::string &path)
{
    return path.substr(path.find_last_of(".") + 1);
}

std::string GetFullBaseName(const std::string &path)
{
    std::string suffix = GetSuffix(path);
    std::string basename = path;
    basename.erase(path.size()-(suffix.size()+1));
    return basename;
}

void WavToSvg(const std::string &inputFileName, const std::string &outputFileName)
{
    std::ofstream outfile;
    outfile.open (outputFileName, std::ofstream::binary);

    AudioFile<double> audioFile;
    bool loadedOK = audioFile.load(inputFileName);

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
                    float sample = sampleRaw * 250.0;
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
}

void SvgToWav(const std::string &inputFileName, const std::string &outputFileName)
{

    AudioFile<double>::AudioBuffer buffer;

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(inputFileName.c_str());

     if (result.status == pugi::status_ok)
     {
        //Reading from the file
         for (pugi::xml_node tool: doc.child("svg").children("polyline"))
         {
             std::vector<double> channel;
             std::cout << "Found Polyline! " << std::endl;
             const char *allPoints = tool.attribute("points").as_string();
             nsvg__parsePoly(allPoints, channel);
             buffer.push_back(channel);
         }
/*
        do
        {
            xmlReader.readNext();
            if (xmlReader.isStartElement())
            {
                QString name = xmlReader.name().toString();
                if (name == "polyline")
                {
                    QXmlStreamAttributes attr = xmlReader.attributes();

                    if (attr.hasAttribute("points"))
                    {
                        std::cout << "Get all polyline points" << std::endl;
                        QStringRef allPoints = attr.value("points");

                        nsvg__parsePoly(allPoints.toString().toStdString().c_str(), channel);

                   // 0.00302606, 249.992 0.00605211, 250.015
                    }
                }
            }
            else if (xmlReader.isEndElement())
            {
                xmlReader.readNext();
            }

        } while (!xmlReader.isEndDocument());

        if (xmlReader.hasError())
        {
            std::cout << "XML error: " << xmlReader.errorString().data() << std::endl;
        }
        */
    }

     if (buffer.size() > 0)
     {
        AudioFile<double> audioFileOut;

        audioFileOut.setAudioBuffer (buffer);
        audioFileOut.save(outputFileName, AudioFileFormat::Wave);
    }
     else
     {
         std::cout << "No any samples found!" << std::endl;
     }
}

void PrintHelp(const char *exeName)
{
    std::cout << exeName << " - WAV from/to SVG converter" << std::endl;
    std::cout << "Usage: " << exeName << " myfile.[wav|svg]" << std::endl;
}

int main(int argc, char *argv[])
{
    int ret = 0;
    if (argc > 1)
    {
        std::cout << GetFullBaseName(argv[1]) << std::endl;

        std::string baseName = GetFullBaseName(argv[1]);
        std::string suffix = GetSuffix(argv[1]);

        if (suffix == "svg")
        {
            SvgToWav(argv[1], baseName + "_output.wav");
        }
        else if(suffix == "wav")
        {
            WavToSvg(argv[1], baseName + "_output.svg");
        }
        else
        {
            ret = -1;
        }
    }
    else
    {
        ret = -2;
    }

    if (ret != 0)
    {
        PrintHelp(argv[0]);
    }

    return ret;
}
