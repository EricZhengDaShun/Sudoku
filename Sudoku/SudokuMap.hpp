#pragma once
#include <vector>
#include "SudokuNum.hpp"

class SudokuMap
{
public:
    explicit SudokuMap();
    virtual ~SudokuMap();

    void reset();
    bool setQuestion(const std::vector<unsigned char>& source);
    bool getAnswer(std::vector<unsigned char>& answer);

private:
    enum class Direction 
    {
        forward = 0,
        backward
    };

private:
    bool solveQuestion();
    Direction forwardStep(const size_t currentPos);
    Direction backwardStep(const size_t currentPos);
    Direction step(const Direction& direction, const size_t currentPos);
    bool checkRowValue(const size_t row, const size_t column);
    bool checkColumnValue(const size_t row, const size_t column);
    bool checkBlockValue(const size_t row, const size_t column);
    bool checkQestion();

private:
    std::vector<std::vector<SudokuNum>> numBuf;

private:
    static const size_t rowNum = 9;
    static const size_t columnNum = 9;
    static const size_t allMapNum = 81;
    static const size_t blockRowSize = 3;
    static const size_t blockColumnSize = 3;
};

