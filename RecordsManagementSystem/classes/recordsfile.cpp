#include "recordsfile.h"

RecordsFile::RecordsFile()
{
    fileName = "";
}

RecordsFile::RecordsFile(const string &file, ios_base::openmode mode)
    :fileName(file)
{
    fileStream.open(fileName, mode);
}

RecordsFile::~RecordsFile()
{
}

bool RecordsFile::open(const string &fileName, ios_base::openmode mode)
{
    this->fileName = fileName;

    fileStream.open(fileName, mode);

    //Checks if there was an error during opening
    if ( !isOpen() )
    {
        return false;
    }

    return true;
}

bool RecordsFile::close()
{
    fileStream.close();

    //Checks if the file was not closed
    if ( isOpen() )
    {
        return false;
    }

    return true;
}

bool RecordsFile::trunc()
{
}

int RecordsFile::read(char *buffer, int size)
{
}

int RecordsFile::write(const char *buffer, int size)
{
}

bool RecordsFile::flush()
{
}

bool RecordsFile::seek(int pos)
{
}

int RecordsFile::tell()
{
}

bool RecordsFile::isOpen()
{
    //Checks if the file is open
    if ( fileStream.is_open() )
    {
        return true;
    }

    return false;
}

bool RecordsFile::isOk()
{
    //Verify if there is no problem with the file
    if ( fileStream.good() )
    {
        return true;
    }

    return false;
}

bool RecordsFile::isBoF()
{
    //Verify if the position in the file is at the beginning
    if ( tell() == 0 )
    {
        return true;
    }

    return false;
}

bool RecordsFile::isEof()
{
    //Verify if the file has reached the end
    if ( fileStream.eof() )
    {
        return true;
    }

    return false;
}
