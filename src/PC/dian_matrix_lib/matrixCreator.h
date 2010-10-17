/*
**  matrixCreator.h
**      The MatrixCreator class definition file.
**      Using the HZK16 file to create matrix.
**
**  copyleft: Tankery.chen@gmail.com, 2010
*/
#ifndef MATRIX_CREATOR_H
#define MATRIX_CREATOR_H
#include <string>

class DataManager;

class MatrixCreator
{
    enum    {
        WORD_COUNT  = 4
    };
private:
    std::string const matrixLibPath; // hold the matrix lib path

public:
    MatrixCreator(std::string const path);
    ~MatrixCreator(void);

    bool makeStrMatrix16(char const *wstr, unsigned int const wideCharLength, DataManager *data);
    bool makeCharMatrix16(unsigned char const word[], char *wordData);
};

#endif  // #ifndef MATRIX_CREATOR_H
