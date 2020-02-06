#include "SudokuMap.hpp"

SudokuMap::SudokuMap()
{
    numBuf.resize(rowNum);
    for (std::vector<SudokuNum>& row : numBuf) {
        row.resize(columnNum);
    }
}

SudokuMap::~SudokuMap()
{

}

void SudokuMap::reset()
{
    for (std::vector<SudokuNum>& row : numBuf) {
        for (SudokuNum& column : row) {
            column.reset();
        }
    }
    return;
}

bool SudokuMap::setQuestion(const std::vector<unsigned char>& source)
{
    if (source.size() != allMapNum) return false;

    reset();
    size_t count = 0;
    while (count < source.size()) {
        const unsigned char value = source[count];
        if (value != 0) {
            const size_t row = count / columnNum;
            const size_t column = count % columnNum;
            SudokuNum& sudokuNum = numBuf[row][column];
            const bool isSuccess = sudokuNum.setValue(value);
            if (!isSuccess) break;
            sudokuNum.setFixed();
        }

        ++count;
    }

    if (count != source.size()) return false;
    if (!checkQestion()) return false;
    return true;
}

bool SudokuMap::getAnswer(std::vector<unsigned char>& answer)
{
    if (!solveQuestion()) return false;

    answer.clear();
    answer.reserve(allMapNum);
    for (size_t rowCount = 0; rowCount < rowNum; ++rowCount) {
        for (size_t columnCount = 0; columnCount < columnNum; ++columnCount) {
            const SudokuNum& sudokuNum = numBuf[rowCount][columnCount];
            const char value = sudokuNum.getValue();
            answer.push_back(value);
        }
    }

    return true;
}

bool SudokuMap::solveQuestion()
{
    SudokuMap::Direction direction = SudokuMap::Direction::forward;
    size_t currentPos = 0;
    while (currentPos < allMapNum) {
        switch (direction)
        {
        case SudokuMap::Direction::forward:
            direction = forwardStep(currentPos);
            break;

        case SudokuMap::Direction::backward:
            direction = backwardStep(currentPos);
            break;

        default:
            break;
        }

        if (direction == SudokuMap::Direction::forward) {
            ++currentPos;
        } else if (direction == SudokuMap::Direction::backward) {
            if (currentPos == 0) break;
            --currentPos;
        }
    }
    if (currentPos != allMapNum) return false;

    return true;
}

SudokuMap::Direction SudokuMap::forwardStep(const size_t currentPos)
{
    const SudokuMap::Direction result = step(SudokuMap::Direction::forward, currentPos);
    return result;
}

SudokuMap::Direction SudokuMap::backwardStep(const size_t currentPos)
{
    const SudokuMap::Direction result = step(SudokuMap::Direction::backward, currentPos);
    return result;
}

SudokuMap::Direction SudokuMap::step(const Direction& direction, const size_t currentPos)
{
    const size_t row = currentPos / columnNum;
    const size_t column = currentPos % columnNum;
    SudokuNum& sudokuNum = numBuf[row][column];
    if (sudokuNum.isFixedNum()) return direction;

    bool isPass = false;
    while (!isPass) {
        if (!sudokuNum.autoAddOne()) break;
        if (!checkRowValue(row, column)) continue;
        if (!checkColumnValue(row, column)) continue;
        if (!checkBlockValue(row, column)) continue;
        isPass = true;
    }
    if (!isPass) {
        sudokuNum.reset();
        return SudokuMap::Direction::backward;
    }

    return SudokuMap::Direction::forward;
}

bool SudokuMap::checkRowValue(const size_t row, const size_t column)
{
    const SudokuNum& currentNum = numBuf[row][column];
    const std::vector<SudokuNum>& rowBuf = numBuf[row];
    size_t count = 0;
    while (count < rowBuf.size()) {
        if (count == column) {
            ++count;
            continue;
        }

        const SudokuNum& columnNum = rowBuf[count];
        if (currentNum == columnNum) break;

        ++count;
    }
    if (count != rowBuf.size()) return false;

    return true;
}

bool SudokuMap::checkColumnValue(const size_t row, const size_t column)
{
    const SudokuNum& currentNum = numBuf[row][column];
    size_t count = 0;
    while (count < rowNum) {
        if (count == row) {
            ++count;
            continue;
        }

        const SudokuNum& rowNum = numBuf[count][column];
        if (currentNum == rowNum) break;

        ++count;
    }
    if (count != rowNum) return false;

    return true;
}

bool SudokuMap::checkBlockValue(const size_t row, const size_t column)
{
    static const size_t blockNum = blockRowSize * blockColumnSize;
    const SudokuNum& currentNum = numBuf[row][column];
    const size_t blockRow    = (row    / blockRowSize)    * blockRowSize;
    const size_t blockColumn = (column / blockColumnSize) * blockColumnSize;

    size_t count = 0;
    while (count < blockNum) {
        const size_t scanRow    = blockRow    + (count / blockRowSize);
        const size_t scanColumn = blockColumn + (count % blockColumnSize);
        if ((scanRow == row) && (scanColumn == column)) {
            ++count;
            continue;
        }

        const SudokuNum& scanNum = numBuf[scanRow][scanColumn];
        if (scanNum == currentNum) break;

        ++count;
    }
    if (count != blockNum) return false;

    return true;
}

bool SudokuMap::checkQestion()
{
    size_t currentPos = 0;
    while (currentPos < allMapNum) {
        const size_t row = currentPos / columnNum;
        const size_t column = currentPos % columnNum;
        SudokuNum& sudokuNum = numBuf[row][column];
        if (sudokuNum.isFixedNum()) {
            if (!checkRowValue(row, column)) break;
            if (!checkColumnValue(row, column)) break;
            if (!checkBlockValue(row, column)) break;
        }
        ++currentPos;
    }
    if (currentPos != allMapNum) return false;
    return true;
}