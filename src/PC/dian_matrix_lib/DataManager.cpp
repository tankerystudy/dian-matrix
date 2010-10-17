#include "DataManager.h"

DataManager::DataManager(int width, int height)
: matrixData(0)
, dataWidth(width)
, arrayLength(width*height)
{
    // make space for data and initialize it.
    matrixData= new unsigned char[arrayLength];
    clearAll();
}

DataManager::~DataManager(void)
{
    // free the memory for data
    if (matrixData)
    {
        delete [] matrixData;
        matrixData= 0;  // clear pointer to make memory safe
    }
}

// Read and write value using bool type
bool DataManager::readBool(int x, int y) const
{
    int chCount= x/8;               // switch to the x-index of char
    int restBit= x - chCount*8;

    // current char = matrixData[chChount][y]
    char curChar= *(matrixData + (dataWidth*y + chCount));

    // current bit is at (1000 0000 >> restBit)
    // current bit larger than 0, return true.
    return curChar & (0x80 >> restBit) ? true : false;
}

// if need write a 1 to specific bit, bValue will be true
void DataManager::writeBool(int x, int y, bool bValue)
{
    int chCount= x/8;               // switch to the x-index of char
    int restBit= x - chCount*8;
    // get current char matrixData[chChount][y]
    unsigned char &curChar= *(matrixData + (dataWidth*y + chCount));

    // set current bit to 0 and other bit have no change.
    curChar &= ~(0x80 >> restBit);
    // set current bit to 0/1 according to the bValue
    curChar |= (bValue ? (0x80 >> restBit) : 0x00);
}
