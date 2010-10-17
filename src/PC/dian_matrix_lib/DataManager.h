/********************************************************
 *  DataManager.h
 *  DataManager class definition file
 *
 *  Description:
 *      This class is a unsigned char matrix data manager. You can
 *      write and read some data with matrix using char or bool mode.
 *      0 based.
 *
 *  Interface list:
 *      DataManager(int width= 8, int height= 16)
 *          constructor
 *          create a data manager with width and height (default is 8 x 16).
 *      unsigned long getLength()
 *          get length of this data container.
 *      void writeChar(int x, int y, unsigned char cValue)
 *          write a unsigned char data to the position (x, y) in matrix.
 *          note that the data location is description by char count.
 *      void writeBool(int x, int y, bool bValue)
 *          write a bool data to the (x, y) in matrix.
 *          note that the data location is description by bool count.
 *      unsigned char readChar(int x, int y) const
 *          read a char data from (x, y)
 *      bool readBool(int x, int y) const
 *          read a char bool from (x, y)
 *      void clearAll()
 *          Set all data to 0.
 *
 *  Usage example:
 *      {
 *          // create a 2 x 2 char matrix
 *          DataManager* data= new DataManager(2,2);
 *          data->writeChar(0, 0, 'A');
 *          data->writeChar(1, 0, 0x42);    // write 'B'
 *
 *          // create a char '@' (0100 0000) using bool mode.
 *          for (int i= 0; i < 8; i++)
 *              data->writeBool(i, 1, false);   // write 0 at (0, 1) to (7, 1)
 *          data->writeBool(1, 1, true);        // write 1 at (1, 1)
 *          // create NULL
 *          for (int i= 8; i < 16; i++)
 *              data->writeBool(i, 1, false);
 *
 *          // after these operation, the data matrix become like this:
 *          // 01000001 01000010
 *          // 01000000 00000000
 *          // then you can use read to get those data.
 *      }
 ********************************************************/
#ifndef Tankery_DATA_MANAGER_H
#define Tankery_DATA_MANAGER_H

class DataManager
{
private:
    unsigned char *matrixData;
    int dataWidth;
    int arrayLength;

public:
    DataManager(int width, int height);
    ~DataManager(void);

    inline unsigned long getLength(void) const
    {
        return arrayLength;
    }
    // Read and write value using char type, 0 based char position
    inline unsigned char readChar(int x, int y) const
    {
        // read from matrixData[x][y]
        return *(matrixData + (dataWidth*y + x));
    }
    inline void writeChar(int x, int y, unsigned char cValue)
    {
        // write to matrixData[x][y]
        *(matrixData + (dataWidth*y + x)) = cValue;
    }

    // Read and write value using bool type, 0 based bool position
    bool readBool(int x, int y) const;
    void writeBool(int x, int y, bool bValue);

    // Set all data to 0
    inline void clearAll()
    {
        int pos= arrayLength;
        while (pos--)
            *(matrixData + pos) = 0;
    }
};


#endif // #ifndef DATA_MANAGER_H
