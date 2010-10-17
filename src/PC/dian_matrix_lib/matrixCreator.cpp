/*
**  matrixCreator.cpp
**      The MatrixCreator class implementation file.
**      Using the HZK16 file to create matrix.
**
**  copyleft: Tankery.chen@gmail.com, 2010
*/
#include <fstream>
#include "matrixCreator.h"
#include "DataManager.h"

MatrixCreator::MatrixCreator(std::string const path)
: matrixLibPath(path)
{
}

MatrixCreator::~MatrixCreator(void)
{
}

bool MatrixCreator::makeStrMatrix16(char const *wstr, unsigned int const wideCharLength, DataManager *data)
{
    // hold the word string of Chinese character.
    // 16 x 2x8 = 16x16, that can make a HZ16 matrix.
    char tmpWordData[32];
    unsigned int const dataLength= data->getLength()/16;

    if (wideCharLength*2 > dataLength)
    {   // if data is too small, false
        return false;
    }

    data->clearAll();
    for (unsigned int charCount= 0; charCount < wideCharLength; charCount++)
    {
        // a wide char is 2 char length
        if(!makeCharMatrix16((unsigned char *)wstr + 2*charCount, tmpWordData))
            return false;

        // transform the data from tmpWordData to data
        for (int j= 0; j < 16; j++)
            for (int i= 0; i < 2; i++)
                // offset data to the middle of the data matrix.
                data->writeChar(2*charCount + i + int(dataLength/2 - wideCharLength),
                j, tmpWordData[j*2 + i]);
    }

    return true;
}

bool MatrixCreator::makeCharMatrix16(unsigned char const word[], char *wordData)
{
    // get the word's zone/bit code
    unsigned char zoneCode= word[0] - 0xa0;
    unsigned char bitCode = word[1] - 0xa0;
    long offset = (94*(zoneCode-1) + (bitCode-1))*32;

    std::ifstream infile(matrixLibPath.c_str());
    if (!infile.is_open())
    {
        return false;
    }

    // get 32 byte data from position 'offset'.
    infile.seekg(offset, std::ios_base::beg);
    if (!infile.good())
    {
        infile.close();
        return false;
    }
    infile.read(wordData, 32);

    infile.close();

    return true;
}
