#include "SudokuEdit.hpp"
#include <qevent.h>

SudokuEdit::SudokuEdit(QWidget* parent)
    : QLineEdit(parent)
{
    if (parent != nullptr) {
        QLineEdit::setFont(parent->font());
    }
    QLineEdit::setAlignment(Qt::AlignmentFlag::AlignCenter);
    QLineEdit::setInputMask("9");
}

SudokuEdit::~SudokuEdit()
{

}

void SudokuEdit::mouseReleaseEvent(QMouseEvent* event)
{
    QLineEdit::selectAll();
    return;
}