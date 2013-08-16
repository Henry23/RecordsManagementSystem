#include "recordsfile.h"

RecordsFile::RecordsFile()
{
    fileName = "";
}

RecordsFile::RecordsFile(const string &file, ios_base::openmode mode)
    :fileName(file)
{
    this->fileStream.open(file, mode);
}

RecordsFile::~RecordsFile()
{
}

bool RecordsFile::open(const string &fileName, ios_base::openmode mode)
{
    this->fileName = fileName;

    this->fileStream.open(fileName, mode);

    //Checks if there was an error during opening
    if ( !this->isOpen() )
    {
        return false;
    }

    return true;
}

bool RecordsFile::close()
{
    this->fileStream.close();

    //Checks if the file was not closed
    if ( this->isOpen() )
    {
        return false;
    }

    return true;
}

bool RecordsFile::trunc()
{
}

short int RecordsFile::read(char *buffer, int size)
{
    this->fileStream.read(buffer, size);

    //Checks if something happen while reading
    if ( this->isOk() != 0 )
    {
        return -1;
    }

    return 0;
}

short int RecordsFile::write(const char *buffer, int size)
{
    this->fileStream.write(buffer, size);

    //Checks if something happend while writing
    if ( this->isOk() != 0 )
    {
        return -1;
    }

    return 0;
}

bool RecordsFile::flush()
{
    this->fileStream.flush();

    //Checks if the flush was successful
    if ( this->isOk() == 0 )
    {
        return true;
    }

    return false;
}

bool RecordsFile::seek(int pos)
{
    if ( !this->isOpen() )
    {
        return false;
    }

<<<<<<< HEAD
    fileStream.seekg(0,fileStream.end);
    int lengthFile = this->fileStream.tellg();
     fileStream.seekg(0,fileStream.beg);

    if ( pos > lengthFile)
=======
    if ( pos > this->fileLength()  )
>>>>>>> 3ea93a4cba6910ea80ab828c2bab18e2d19b8088
    {
        return false;
    }

    this->fileStream.seekg(pos);
    this->fileStream.seekp(pos);

    return true;
}

int RecordsFile::tell()
{
    if ( !isOpen() )
    {
        return false;
    }

    return this->fileStream.tellg();
}

bool RecordsFile::isOpen()
{
    //Checks if the file is open
    if ( this->fileStream.is_open() )
    {
        return true;
    }

    return false;
}

unsigned short int RecordsFile::isOk()
{
    //Logical error on i/o operation
    if ( this->fileStream.rdstate() == ios::failbit )
    {
        return 1;
    }

    //Read/writing error on i/o operation
    else if ( this->fileStream.rdstate() == ios::badbit )
    {
        return 2;
    }

    //There is no problem with the file (ios::goodbit)
    return 0;
}

bool RecordsFile::isBoF()
{
    //Checks if the position in the file is at the beginning
    if ( this->tell() == 0 )
    {
        return true;
    }

    return false;
}

bool RecordsFile::isEof()
{
    //Checks if the file has reached the end
    return this->fileStream.eof();
}

int RecordsFile::fileLength()
{
    this->fileStream.seekg(0, this->fileStream.end);

    int length = this->fileStream.tellg();

    this->fileStream.seekg(0, this->fileStream.beg);

    return length;
}
