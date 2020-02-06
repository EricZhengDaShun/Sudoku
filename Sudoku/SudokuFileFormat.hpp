#pragma once
#include <string>
#include <vector>

class SudokuFileFormat
{
public:
    explicit SudokuFileFormat();
    virtual ~SudokuFileFormat();

public:
    static bool fileDataToNumFormat(const std::string& fileData, std::vector<unsigned char>& numBuf);
    static bool numToFileDataFormat(const std::vector<unsigned char>& numBuf, std::string& fileData);

public:
    static const size_t allMapNum = 81;
};