#include "FileControl.hpp"

const unsigned char FileControl::utf8Bom[3] = {0xEF, 0xBB, 0xBF};
const unsigned char FileControl::utf16BomBigEndian[2] = {0xFE, 0xFF};
const unsigned char FileControl::utf16BomLittleEndian[2] = {0xFF, 0xFE};

FileControl::FileControl(const std::wstring& name)
    : fileName(name)
    , mode(Mode::Text)
{

}

FileControl::FileControl(const std::string& name)
    : FileControl(toWstring(name))
{

}

FileControl::~FileControl()
{

}

void FileControl::setFileName(const std::wstring& name)
{
    close();
    fileName = name;
    return;
}

void FileControl::setFileName(const std::string& name)
{
    setFileName(toWstring(name));
    return;
}

std::wstring FileControl::getFileName() const
{
    return fileName;
}

std::string FileControl::getFileNameString() const
{
    const std::string result = toString(fileName);
    return result;
}

bool FileControl::open(const Mode mode)
{
    std::ios_base::openmode openMode = std::fstream::in | std::fstream::out;
    if (mode == Mode::Binary) openMode |= std::fstream::binary;

    #ifdef _MSC_VER
        fileStream.open(fileName, openMode);
    #else
        std::string uft8FileName;
        wstringToUtf8String(fileName, uft8FileName);
        fileStream.open(uft8FileName, openMode);
    #endif

    const bool isOpen = fileStream.is_open();
    return isOpen;
}

bool FileControl::create(const Mode mode)
{
    std::ios_base::openmode openMode = std::fstream::in |std::fstream::out | std::fstream::trunc;
    if (mode == Mode::Binary) openMode |= std::fstream::binary;

    #ifdef _MSC_VER
        fileStream.open(fileName, openMode);
    #else
        std::string uft8FileName;
        wstringToUtf8String(fileName, uft8FileName);
        fileStream.open(uft8FileName, openMode);
    #endif

    const bool isOpen = fileStream.is_open();
    return isOpen;
}

bool FileControl::openOrCreate(const Mode mode)
{
    bool result = false;
    result = open(mode);
    if (!result) {
        result = create(mode);
    }

    return result;
}

void FileControl::close()
{
    fileStream.close();
    fileStream.clear();
    return;
}

bool FileControl::isOpen() const
{
    return fileStream.is_open();
}

size_t FileControl::getFileSize()
{
    if (!isOpen()) return 0;

    const std::streamoff position = fileStream.tellg();
    fileStream.seekg(0, fileStream.end);
    const std::streamoff length = fileStream.tellg();
    fileStream.seekg(position);
    return static_cast<size_t>(length);
}

bool FileControl::remove()
{
    close();

#ifdef _MSC_VER
    const int result = ::_wremove(fileName.c_str());
#else
    std::string uft8FileName;
    wstringToUtf8String(fileName, uft8FileName);
    const int result = std::remove(uft8FileName.c_str());
#endif

    if (result != 0) return false;

    return true;
}

bool FileControl::rename(const std::wstring& newName)
{
    close();

#ifdef _MSC_VER
    const int result = ::_wrename(fileName.c_str(), newName.c_str());
    if (result != 0) return false;
#else
    std::string uft8FileName;
    std::string utf8NewName;
    wstringToUtf8String(fileName, uft8FileName);
    wstringToUtf8String(newName, utf8NewName);
    const int result = std::rename(uft8FileName.c_str(), utf8NewName.c_str());
#endif

    if (result != 0) return false;

    fileName = newName;
    return true;
}

bool FileControl::rename(const std::string& newName)
{
    const std::wstring wNewName = toWstring(newName);
    const bool result = rename(wNewName.c_str());
    return result;
}

bool FileControl::readAll(std::string& buffer, const bool removeBom)
{
    if (!isOpen()) return false;
    if (mode == Mode::Binary) return false;

    const std::streamoff position = fileStream.tellg();
    const std::ios_base::iostate state = fileStream.rdstate();

    fileStream.seekg(0, fileStream.beg);
    std::vector<char> dataBuf;
    char tmp = ' ';
    while (fileStream.get(tmp)) {
        if (fileStream.eof()) break;
        dataBuf.push_back(tmp);
    }

    fileStream.seekg(position);
    fileStream.clear();
    fileStream.setstate(state);
    
    if (removeBom) {
        const int bomSize = bomDataSize(dataBuf.data(), dataBuf.size());
        dataBuf.erase(dataBuf.begin(), dataBuf.begin() + bomSize);
    }
    buffer.assign(dataBuf.data(), dataBuf.size());
    
    return true;
}

bool FileControl::readAll(char* const buffer, const size_t maxSize)
{
    if (!isOpen()) return false;
    const size_t fileSize = getFileSize();
    if (fileSize > maxSize) return false;

    const std::streamoff position = fileStream.tellg();
    const std::ios_base::iostate state = fileStream.rdstate();

    fileStream.seekg(0, fileStream.beg);

    size_t count = 0;
    while (fileStream.get(buffer[count])) {
        if (fileStream.eof()) break;
        ++count;
    }

    fileStream.seekg(position);
    fileStream.clear();
    fileStream.setstate(state);

    if (count != fileSize) return false;

    return true;
}

bool FileControl::readLine(std::string& buffer, const size_t maxSize, const char delim, const bool removeBom)
{
    if (!isOpen()) return false;
    if (mode == Mode::Binary) return false;

    std::vector<char> dataBuf(maxSize);
    if (!fileStream.getline(dataBuf.data(), dataBuf.size(), delim)) return false;
    if (removeBom) {
        const int bomSize = bomDataSize(dataBuf.data(), dataBuf.size());
        dataBuf.erase(dataBuf.begin(), dataBuf.begin() + bomSize);
    }
    buffer.assign(dataBuf.data());
    return true;
}

bool FileControl::readLine(std::string& buffer, const bool removeBom, const size_t maxSize, const char delim)
{
    return readLine(buffer, maxSize, delim, removeBom);
}

bool FileControl::writeAppend(const char* data, const size_t size)
{
    const std::streamoff position = fileStream.tellp();
    const std::ios_base::iostate state = fileStream.rdstate();

    fileStream.seekp(0, fileStream.end);

    fileStream.write(data, size);
    fileStream.flush();

    fileStream.seekp(position);
    fileStream.clear();
    fileStream.setstate(state);

    return true;
}

void FileControl::writeAppendBomUTF8()
{
    writeAppend(reinterpret_cast<const char*>(utf8Bom), sizeof(utf8Bom));
    return;
}

void FileControl::writeAppendBigEndianBomUTF16()
{
    writeAppend(reinterpret_cast<const char*>(utf16BomBigEndian), sizeof(utf16BomBigEndian));
    return;
}

void FileControl::writeAppendLittleEndianBomUTF16()
{
    writeAppend(reinterpret_cast<const char*>(utf16BomLittleEndian), sizeof(utf16BomLittleEndian));
    return;
}

int FileControl::bomDataSize(const char* buf, const size_t size)
{
    if (size < sizeof(utf8Bom)) return 0;

    bool result = true;
    result &= (utf8Bom[0] == static_cast<unsigned char>(buf[0]));
    result &= (utf8Bom[1] == static_cast<unsigned char>(buf[1]));
    result &= (utf8Bom[2] == static_cast<unsigned char>(buf[2]));
    if (result == true) return sizeof(utf8Bom);

    result = true;
    result &= (utf16BomBigEndian[0] == static_cast<unsigned char>(buf[0]));
    result &= (utf16BomBigEndian[1] == static_cast<unsigned char>(buf[1]));
    if (result == true) return sizeof(utf16BomBigEndian);

    result = true;
    result &= (utf16BomLittleEndian[0] == static_cast<unsigned char>(buf[0]));
    result &= (utf16BomLittleEndian[1] == static_cast<unsigned char>(buf[1]));
    if (result == true) return sizeof(utf16BomLittleEndian);

    return 0;
}

std::wstring FileControl::toWstring(const std::string& str, const std::locale& loc)
{
    std::vector<wchar_t> buf(str.size());
    std::use_facet<std::ctype<wchar_t>>(loc).widen(str.data(), str.data() + str.size(), buf.data());
    const std::wstring result(buf.data(), buf.size());
    return result;
}

std::string FileControl::toString(const std::wstring& str, const std::locale& loc)
{
    std::vector<char> buf(str.size());
    std::use_facet<std::ctype<wchar_t>>(loc).narrow(str.data(), str.data() + str.size(), '?', buf.data());
    return std::string(buf.data(), buf.size());
}

#ifndef _MSC_VER
void FileControl::wstringToUtf8String(const std::wstring& source, std::string& direct)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
    direct = myconv.to_bytes(source);
    return;
}

void FileControl::utf8StringToWstring(const std::string& source, std::wstring& direct)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
    direct = myconv.from_bytes(source);
    return;
}
#endif
