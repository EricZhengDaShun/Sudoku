#pragma once
#include <cstdio>
#include <string>
#include <fstream>
#include <vector>
#include <locale>

#ifndef _MSC_VER
#include <codecvt>
#endif

class FileControl
{
public:
    enum Mode
    {
        Text = 0,
        Binary
    };

public:
    explicit FileControl(const std::wstring& name = L"");
    explicit FileControl(const std::string& name);

    virtual ~FileControl();

    void setFileName(const std::wstring& name);
    void setFileName(const std::string& name);

    std::wstring getFileName() const;
    std::string getFileNameString() const;

    bool open(const Mode mode = Mode::Text);
    bool create(const Mode mode = Mode::Text);
    bool openOrCreate(const Mode mode = Mode::Text);

    void close();
    bool isOpen() const;

    size_t getFileSize();

    bool remove();
    bool rename(const std::wstring& newName);
    bool rename(const std::string& newName);

    bool readAll(std::string& buffer, const bool removeBom = true);
    bool readAll(char* const buffer, const size_t maxSize);
    bool readLine(std::string& buffer, const size_t maxSize = 512, const char delim = '\n', const bool removeBom = false);
    bool readLine(std::string& buffer, const bool removeBom, const size_t maxSize = 512, const char delim = '\n');

    bool writeAppend(const char* data, const size_t size);
    void writeAppendBomUTF8();
    void writeAppendBigEndianBomUTF16();
    void writeAppendLittleEndianBomUTF16();

private:
    std::wstring fileName;
    std::fstream fileStream;
    Mode mode;

private:
    int bomDataSize(const char* buf, const size_t size);

private:
    static std::wstring toWstring(const std::string& str, const std::locale& loc = std::locale());
    static std::string toString(const std::wstring& str, const std::locale& loc = std::locale());
#ifndef _MSC_VER
    static void wstringToUtf8String(const std::wstring& source, std::string& direct);
    static void utf8StringToWstring(const std::string& source, std::wstring& direct);
#endif

private:
    static const unsigned char utf8Bom[3];
    static const unsigned char utf16BomBigEndian[2];
    static const unsigned char utf16BomLittleEndian[2];

};

