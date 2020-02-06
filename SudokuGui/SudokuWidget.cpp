#include "SudokuWidget.hpp"
#include <qheaderview.h>
#include "SudokuEdit.hpp"

SudokuWidget::SudokuWidget(QWidget* parent)
    : QTableWidget(parent)
{
    if (parent != nullptr) {
        QTableWidget::setFont(parent->font());
    }

    for (size_t row = 0; row < 9; ++row) {
        this->insertRow(row);
        this->setVerticalHeaderItem(row, new QTableWidgetItem(QString('A' + row)));
    }

    for (size_t column = 0; column < 9; ++column) {
        this->insertColumn(column);
        this->setHorizontalHeaderItem(column, new QTableWidgetItem(QString::number(column + 1)));
    }

    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    this->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);

    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    const int minColumnWidth = this->horizontalHeader()->minimumSectionSize() * this->columnCount();
    const int minWidth = minColumnWidth + this->verticalHeader()->minimumSectionSize();
    this->setMinimumWidth(minWidth);

    const int minRowWidth = this->verticalHeader()->minimumSectionSize() * this->rowCount();
    const int minHeight = minRowWidth + this->horizontalHeader()->minimumSectionSize();
    this->setMinimumHeight(minHeight);

    this->setStyleSheet("gridline-color: gray;");
    
    for (size_t row = 0; row < 9; ++row) {
        for (size_t column = 0; column < 9; ++column) {
            SudokuEdit* lineEdit = new SudokuEdit(this);
            QString styleSheet = "border: 3px solid;\
                                  border-color: rgb(255, 255, 255);";

            if (row == 0) styleSheet += "border-top-color: rgb(0, 0, 0);";
            if (column == 0) styleSheet += "border-left-color: rgb(0, 0, 0);";
            if (((column + 1) % 3) == 0) styleSheet += "border-right-color: rgb(0, 0, 0);";
            if (((row + 1) % 3) == 0) styleSheet += "border-bottom-color: rgb(0, 0, 0);";

            lineEdit->setStyleSheet(styleSheet);
            this->setCellWidget(row, column, lineEdit);
        }
    }
    
}

SudokuWidget::~SudokuWidget()
{

}