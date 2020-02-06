#include "SudokuNum.hpp"

SudokuNum::SudokuNum()
    : value(initialNum)
    , isFixed(false)
{

}

SudokuNum::~SudokuNum()
{

}

unsigned char SudokuNum::getValue() const
{
    return value;
}

bool SudokuNum::isFixedNum() const
{
    return isFixed;
}

bool SudokuNum::setValue(const unsigned char value)
{
    if (value < minNum) return false;
    if (value > maxNum) return false;

    this->value = value;
    return true;
}

void SudokuNum::reset()
{
    value = initialNum;
    isFixed = false;
    return;
}

void SudokuNum::setFixed()
{
    isFixed = true;
    return;
}

void SudokuNum::setVariable()
{
    isFixed = false;
    return;
}

bool SudokuNum::autoAddOne()
{
    if (isFixed) return false;

    const unsigned char result = value + 1;
    if (result > maxNum) return false;

    ++value;
    return true;
}

bool SudokuNum::operator==(const SudokuNum& rhs) const
{
    const bool result = (this->value == rhs.value);
    return result;
}