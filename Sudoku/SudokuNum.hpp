#pragma once
class SudokuNum
{
public:
    explicit SudokuNum();
    virtual ~SudokuNum();

    unsigned char getValue() const;
    bool isFixedNum() const;

    bool setValue(const unsigned char value);
    void reset();

    void setFixed();
    void setVariable();

    bool autoAddOne();

    bool operator==(const SudokuNum& rhs) const;

public:
    static const unsigned char initialNum = 0;
    static const unsigned char minNum = 1;
    static const unsigned char maxNum = 9;

private:
    unsigned char value;
    bool isFixed;
};