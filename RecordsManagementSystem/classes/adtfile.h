#ifndef ADTFILE_H
#define ADTFILE_H

#include <string>
using std::string;

#include <ios>
using std::ios_base;

class ADTFile
{
public:
    virtual bool open(const string&, ios_base::openmode = ios_base::in | ios_base::out) = 0;
    virtual bool close() = 0;
    virtual bool trunc() = 0;
    virtual int read(char*, int) = 0;
    virtual int write(const char*, int) = 0;
    virtual bool flush() = 0;
    virtual bool seek(int) = 0;
    virtual int tell() = 0;
    virtual bool isOpen() = 0;
    virtual int isOk() = 0;
    virtual bool isBoF() = 0;
    virtual bool isEof() = 0;
};

#endif // ADTFILE_H
