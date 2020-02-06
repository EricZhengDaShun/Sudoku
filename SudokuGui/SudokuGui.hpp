#pragma once
#include <string>
#include <vector>
#include <QtWidgets/QMainWindow>
#include "ui_SudokuGui.h"
#include <qstring.h>

class SudokuGui : public QMainWindow
{
    Q_OBJECT

public:
    explicit SudokuGui(QWidget* parent = Q_NULLPTR);
    virtual ~SudokuGui();

private:
    void initialWidgetTable();
    std::wstring selectQuestionFile();
    std::wstring selectSaveFile();
    std::wstring checkFileNameExtension(const std::wstring& fileName);
    bool readQuestionFile(const std::wstring& fileName, std::vector<unsigned char>& direct);
    void setQuestionToWidget();
    void setSolvedToWidget();
    void showInformation(const QString& title, const QString& text);
    
private slots:
    void on_fileActionLoad_triggered(const bool checked);
    void on_fileActionSolve_triggered(const bool checked);
    void on_setQuestionModeReset_triggered(const bool checked);
    void on_setQuestionModeSet_triggered(const bool checked);
    void on_setQuestionModeSave_triggered(const bool checked);
    void on_setQuestionModeEdit_triggered(const bool checked);

private:
    Ui::SudokuGuiClass ui;
    std::vector<unsigned char> numData;
    std::vector<unsigned char> answer;
};