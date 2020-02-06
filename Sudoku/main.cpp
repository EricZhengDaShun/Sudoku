#include <iostream>
#include <vector>
#include <algorithm>
#include "SudokuNum.hpp"
#include "FileControl.hpp"
#include "SudokuMap.hpp"
#include "SudokuFileFormat.hpp"

int main(int argc, char* argv[])
{
    FileControl file;
    file.setFileName("Sudoku.txt");
    if (!file.open()) {
        std::cout << "Open file : " << file.getFileNameString() << " fail !" << std::endl;
        std::cin.get();
        return 0;
    }
    std::string fileBuf;
    file.readAll(fileBuf);
    file.close();

    std::vector<unsigned char> numData;
    const bool transferToNumResult = SudokuFileFormat::fileDataToNumFormat(fileBuf, numData);
    if (!transferToNumResult) {
        std::cout << "Transfer data of file fail : " << std::endl;
        std::cin.get();
        return 0;
    }

    SudokuMap sudokuMap;
    const bool setResult = sudokuMap.setQuestion(numData);
    if (!setResult) {
        std::cout << "SetQuestion fail : " << std::endl;
        std::cin.get();
        return 0;
    }

    std::vector<unsigned char> answer;
    const bool solved = sudokuMap.getAnswer(answer);
    if (!solved) {
        std::cout << "Solve fail !" << std::endl;
        std::cin.get();
        return 0;
    }

    const bool transferToFileResult = SudokuFileFormat::numToFileDataFormat(answer, fileBuf);
    if (!transferToFileResult) {
        std::cout << "Transfer data of num fail : " << std::endl;
        std::cin.get();
        return 0;
    }

    file.setFileName("Solved.txt");
    if (!file.create()) {
        std::cout << "Open file : " << file.getFileNameString() << " fail !" << std::endl;
        std::cin.get();
        return 0;
    }
    file.writeAppend(fileBuf.c_str(), fileBuf.size());
    file.close();

    std::cout << "Successful !!" << std::endl;
    std::cin.get();
    return 0;
}