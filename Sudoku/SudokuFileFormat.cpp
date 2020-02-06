#include "SudokuFileFormat.hpp"


SudokuFileFormat::SudokuFileFormat()
{

}

SudokuFileFormat::~SudokuFileFormat()
{

}

bool SudokuFileFormat::fileDataToNumFormat(const std::string& fileData, std::vector<unsigned char>& numBuf)
{
    if (fileData.size() < allMapNum) return false;

    numBuf.clear();
    numBuf.reserve(allMapNum);

    for (size_t count = 0; count < fileData.size(); ++count) {
        const char asciiNum = fileData[count];
        if ((asciiNum < '0') || (asciiNum > '9')) continue;
        const unsigned char num = fileData[count] - '0';
        numBuf.push_back(num);
    }
    if (numBuf.size() != allMapNum) return false;

    return true;
}

bool SudokuFileFormat::numToFileDataFormat(const std::vector<unsigned char>& numBuf, std::string& fileData)
{
    if(numBuf.size() != allMapNum) return false;

    fileData.clear();
    for (size_t count = 0; count < numBuf.size(); ++count) {
        const char ch = static_cast<char>(numBuf[count] + '0');
        fileData.append(&ch, 1);
        if (((count + 1) % 9) == 0) {
            fileData.append(";\n");
        } else {
            fileData.append(",");
        }
    }
    return true;
}