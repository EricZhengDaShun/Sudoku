#include "SudokuGui.hpp"
#include <qlineedit.h>
#include <QFileDialog>
#include <QStringList>
#include <qpalette.h>
#include <QMessageBox>
#include <qicon.h>
#include "SudokuEdit.hpp"
#include "../Sudoku/FileControl.hpp"
#include "../Sudoku/SudokuFileFormat.hpp"
#include "../Sudoku/SudokuMap.hpp"

SudokuGui::SudokuGui(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    this->setWindowIcon(QIcon(":/SudokuGui/Resources/sudoku.ico"));
    initialWidgetTable();
}

SudokuGui::~SudokuGui()
{

}

void SudokuGui::initialWidgetTable()
{
    ui.tableWidget->setFont(this->font());

    for (size_t row = 0; row < 9; ++row) {
        ui.tableWidget->insertRow(row);
        ui.tableWidget->setVerticalHeaderItem(row, new QTableWidgetItem(QString('A' + row)));
    }

    for (size_t column = 0; column < 9; ++column) {
        ui.tableWidget->insertColumn(column);
        ui.tableWidget->setHorizontalHeaderItem(column, new QTableWidgetItem(QString::number(column + 1)));
    }

    ui.tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui.tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui.tableWidget->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);

    ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui.tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    const int minColumnWidth = ui.tableWidget->horizontalHeader()->minimumSectionSize() * ui.tableWidget->columnCount();
    const int minWidth = minColumnWidth + ui.tableWidget->verticalHeader()->minimumSectionSize();
    ui.tableWidget->setMinimumWidth(minWidth);

    const int minRowWidth = ui.tableWidget->verticalHeader()->minimumSectionSize() * ui.tableWidget->rowCount();
    const int minHeight = minRowWidth + ui.tableWidget->horizontalHeader()->minimumSectionSize();
    ui.tableWidget->setMinimumHeight(minHeight);

    ui.tableWidget->setStyleSheet("gridline-color: gray;");
    
    for (size_t row = 0; row < 9; ++row) {
        for (size_t column = 0; column < 9; ++column) {
            SudokuEdit* lineEdit = new SudokuEdit(ui.tableWidget);
            QString styleSheet = "border: 3px solid;\
                                  border-color: rgb(255, 255, 255);";

           if (row == 0) styleSheet += "border-top-color: rgb(0, 0, 0);";
           if (column == 0) styleSheet += "border-left-color: rgb(0, 0, 0);";
           if (((column + 1) % 3) == 0) styleSheet += "border-right-color: rgb(0, 0, 0);";
           if (((row + 1) % 3) == 0) styleSheet += "border-bottom-color: rgb(0, 0, 0);";
            
            lineEdit->setStyleSheet(styleSheet);
            ui.tableWidget->setCellWidget(row, column, lineEdit);
        }
    }
    return;
}

std::wstring SudokuGui::selectQuestionFile()
{
    QStringList filters;
    filters << tr("Text files (*.txt)");
    filters << tr("Any files (*)");

    QFileDialog qFileDialog(this);
    qFileDialog.setWindowTitle(tr("Select File"));
    qFileDialog.setFileMode(QFileDialog::ExistingFile);
    qFileDialog.setViewMode(QFileDialog::Detail);
    qFileDialog.setNameFilters(filters);
    if (qFileDialog.exec() != QFileDialog::FileName) return L"";
    const QStringList files = qFileDialog.selectedFiles();
    const QString fileName = files.front();
    return fileName.toStdWString();
}

std::wstring SudokuGui::selectSaveFile()
{
    QStringList filters;
    filters << tr("Text files (*.txt)");
    filters << tr("Any files (*)");

    QFileDialog qFileDialog(this);
    qFileDialog.setWindowTitle(tr("Select File"));
    qFileDialog.setFileMode(QFileDialog::AnyFile);
    qFileDialog.setViewMode(QFileDialog::Detail);
    qFileDialog.setNameFilters(filters);
    if (qFileDialog.exec() != QFileDialog::FileName) return L"";
    const QStringList files = qFileDialog.selectedFiles();
    const QString fileName = files.front();
    return fileName.toStdWString();
}

std::wstring SudokuGui::checkFileNameExtension(const std::wstring& fileName)
{
    const size_t lastBackslashPos = fileName.rfind(L"/");
    if (lastBackslashPos == std::wstring::npos) return fileName;
    const std::wstring name = fileName.substr(lastBackslashPos + 1);
    if (name.find(L".") != std::wstring::npos) return fileName;

    const std::wstring result = fileName + L".txt";
    return result;
}

bool SudokuGui::readQuestionFile(const std::wstring& fileName, std::vector<unsigned char>& direct)
{
    FileControl file;
    file.setFileName(fileName);
    if (!file.open()) return false;
    std::string fileBuf;
    if(!file.readAll(fileBuf)) return false;
    file.close();

    if(!SudokuFileFormat::fileDataToNumFormat(fileBuf, direct)) return false;
    return true;
}

void SudokuGui::setQuestionToWidget()
{
    for (size_t row = 0; row < 9; ++row) {
        for (size_t column = 0; column < 9; ++column) {
            QWidget* widget = ui.tableWidget->cellWidget(row, column);
            SudokuEdit* lineEdit = dynamic_cast<SudokuEdit*>(widget);
            if (lineEdit == nullptr) return;
            const size_t pos = (row * 9) + column;
            const unsigned char value = numData[pos];
            if (value != 0) {
                lineEdit->setText(QString::number(value));
                lineEdit->setReadOnly(true);
                QPalette palette;
                palette.setColor(QPalette::Text, Qt::black);
                lineEdit->setPalette(palette);
            } else {
                lineEdit->setText(tr(""));
                lineEdit->setReadOnly(false);
                QPalette palette;
                palette.setColor(QPalette::Text, Qt::blue);
                lineEdit->setPalette(palette);
            }
        }
    }
    return;
}

void SudokuGui::setSolvedToWidget()
{
    QColor green(0, 170, 0);
    for (size_t row = 0; row < 9; ++row) {
        for (size_t column = 0; column < 9; ++column) {
            QWidget* widget = ui.tableWidget->cellWidget(row, column);
            SudokuEdit* lineEdit = dynamic_cast<SudokuEdit*>(widget);
            if (lineEdit == nullptr) return;
            const size_t pos = (row * 9) + column;
            const unsigned char value = answer[pos];
            if (!lineEdit->isReadOnly()) {
                lineEdit->setText(QString::number(value));
                lineEdit->setReadOnly(true);
                QPalette palette;
                palette.setColor(QPalette::Text, green);
                lineEdit->setPalette(palette);
            } 
        }
    }
    return;
}

void SudokuGui::showInformation(const QString& title, const QString& text)
{
    QMessageBox qMessageBox(this);
    qMessageBox.setFont(this->font());
    qMessageBox.setIcon(QMessageBox::Information);
    qMessageBox.setWindowTitle(title);
    qMessageBox.setText(text);
    qMessageBox.setStandardButtons(QMessageBox::Close);
    qMessageBox.exec();
    return;
}

void SudokuGui::on_fileActionLoad_triggered(const bool checked)
{
    const std::wstring fileName = selectQuestionFile();
    if (fileName.empty()) return;

    if (!readQuestionFile(fileName, numData)) {
        showInformation(tr("Load Error"), tr("Load file fail !"));
        return;
    }

    setQuestionToWidget();
    return;
}

void SudokuGui::on_fileActionSolve_triggered(const bool checked)
{
    if (numData.size() != 81) {
        showInformation(tr("Solve Error"), tr("Load or set question first !"));
        return;
    }

    SudokuMap sudokuMap;
    if(!sudokuMap.setQuestion(numData)) {
        showInformation(tr("Solve Error"), tr("Question conflict !"));
        return;
    }
    if (!sudokuMap.getAnswer(answer)) {
        showInformation(tr("Solve Error"), tr("Solve fail !"));
        return;
    }
    setSolvedToWidget();
    showInformation(tr("Solve"), tr("Solve successful !"));
    return;
}

void SudokuGui::on_setQuestionModeReset_triggered(const bool checked)
{
    numData.clear();
    answer.clear();

    for (size_t row = 0; row < 9; ++row) {
        for (size_t column = 0; column < 9; ++column) {
            QWidget* widget = ui.tableWidget->cellWidget(row, column);
            SudokuEdit* lineEdit = dynamic_cast<SudokuEdit*>(widget);
            if (lineEdit == nullptr) return;
            lineEdit->setText("");
            lineEdit->setReadOnly(false);
            QPalette palette;
            palette.setColor(QPalette::Text, Qt::black);
            lineEdit->setPalette(palette);
        }
    }
    return;
}

void SudokuGui::on_setQuestionModeSet_triggered(const bool checked)
{
    numData.clear();

    for (size_t row = 0; row < 9; ++row) {
        for (size_t column = 0; column < 9; ++column) {
            QWidget* widget = ui.tableWidget->cellWidget(row, column);
            SudokuEdit* lineEdit = dynamic_cast<SudokuEdit*>(widget);
            if (lineEdit == nullptr) return;

            const QString text = lineEdit->text();
            if (text.isEmpty() || (text.toInt() == 0)) {
                lineEdit->setText(tr(""));
                lineEdit->setReadOnly(false);
                QPalette palette;
                palette.setColor(QPalette::Text, Qt::blue);
                lineEdit->setPalette(palette);
                numData.push_back(0);
            } else {
                lineEdit->setReadOnly(true);
                QPalette palette;
                palette.setColor(QPalette::Text, Qt::black);
                lineEdit->setPalette(palette);
                const char value = static_cast<unsigned char>(text.toInt());
                numData.push_back(value);
            }
        }
    }
    return;
}

void SudokuGui::on_setQuestionModeSave_triggered(const bool checked)
{
    if (numData.size() != 81) {
        showInformation(tr("Save file Error"), tr("Load or set question first !"));
        return;
    }

    std::wstring fileName = selectSaveFile();
    if (fileName.empty()) return;
    fileName = checkFileNameExtension(fileName);

    std::string fileBuf;
    if (!SudokuFileFormat::numToFileDataFormat(numData, fileBuf)) {
        showInformation(tr("Save"), tr("Save file fail !"));
        return;
    }

    FileControl file;
    file.setFileName(fileName);
    if (!file.create()) return;
    if (!file.writeAppend(fileBuf.c_str(), fileBuf.size())) {
        showInformation(tr("Save"), tr("Save file fail !"));
        return;
    }
    file.close();

    showInformation(tr("Save"), tr("Save file successful !"));
    return;
}

void SudokuGui::on_setQuestionModeEdit_triggered(const bool checked)
{
    numData.clear();
    answer.clear();

    for (size_t row = 0; row < 9; ++row) {
        for (size_t column = 0; column < 9; ++column) {
            QWidget* widget = ui.tableWidget->cellWidget(row, column);
            SudokuEdit* lineEdit = dynamic_cast<SudokuEdit*>(widget);
            if (lineEdit == nullptr) return;

            const QString text = lineEdit->text();
            lineEdit->setReadOnly(false);
            QPalette palette;
            palette.setColor(QPalette::Text, Qt::black);
            lineEdit->setPalette(palette);   
        }
    }
    return;
}