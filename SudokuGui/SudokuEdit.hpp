#pragma once
#include <qlineedit.h>

class SudokuEdit : public QLineEdit
{
public:
    explicit SudokuEdit(QWidget* parent = Q_NULLPTR);
    virtual ~SudokuEdit();

protected:
    void mouseReleaseEvent(QMouseEvent* event);
};