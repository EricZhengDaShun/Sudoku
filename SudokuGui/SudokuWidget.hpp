#pragma once
#include <qtablewidget.h>

class SudokuWidget : public QTableWidget
{
public:
    explicit SudokuWidget(QWidget* parent = Q_NULLPTR);
    virtual ~SudokuWidget();
};