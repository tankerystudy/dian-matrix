/*
 *  matrixCreator.h
 *      The MatrixCreator class definition file.
 *      Using the HZK16 file to create matrix.
 *
 *  bool makeStrMatrix16(char const *wstr, unsigned int const wideCharLength, DataManager *data);
 *      input a string and get the string's matrix data to data.
 *  bool makeCharMatrix16(unsigned char const word[], char *wordData);
 *      input a char array (2 char a chinese word) word and get the char's matrix data to wordData.
 *
 *  copyleft: Tankery.chen@gmail.com, 2010
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
