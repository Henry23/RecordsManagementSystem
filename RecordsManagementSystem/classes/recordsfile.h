#ifndef RECORDSFILE_H
#define RECORDSFILE_H

#include "adtfile.h"

#include <fstream>
using std::fstream;

class RecordsFile
{
public:
    RecordsFile();
    RecordsFile(const string&, ios_base::openmode = ios_base::in | ios_base::out);
    ~RecordsFile();

    /*readRecord();
     *writeRecord();
     *updateRecord();
     *deleteRecord();
     */

    virtual bool open(const string&, ios_base::openmode = ios_base::in | ios_base::out);
    virtual bool close();
    virtual bool trunc();
    virtual int read(char*, int);
    virtual int write(const char*, int);
    virtual bool flush();
    virtual bool seek(int);
    virtual int tell();
    virtual bool isOpen();
    virtual bool isOk();
    virtual bool isBoF();
    virtual bool isEof();

private:
    string fileName;
    fstream fileStream;
};

#endif // RECORDSFILE_H
