#include "recordsfile.h"

RecordsFile::RecordsFile()
{
}

RecordsFile::RecordsFile(const string &file, ios_base::openmode mode)
{
  this->fileName = file;
}

RecordsFile::~RecordsFile()
{
}

bool RecordsFile::open(const string &file, ios_base::openmode mode)
{
       io.open(file, mode);

       if(io.is_open())
       {
           return true;
       }else
       {
           return false;
       }
}

bool RecordsFile::close()
{
    io.close();

    return true;
}

bool RecordsFile::trunc()
{
}

int RecordsFile::read(char *buffer, int size)
{

  if(io.is_open())
  {
  
    io.read(buffer, size);
    return 0;
  
  }else
  {
  
    return -1;
  
  }
}

int RecordsFile::write(const char *buffer, int size)
{
    if(io.is_open())
    {
        io.write(buffer,size);

        return 0;

    }else
    {
        return -1;
    }
}

bool RecordsFile::flush()
{
  if(io.is_open()){

    io.flush();

    return true;
  }else
  {
    return false;
  } 

}

bool RecordsFile::seek(int pos)
{

}

int RecordsFile::tell()
{

}

bool RecordsFile::isOpen()
{
    if(io.is_open())
    {

      return true;
    }else
    {

      return false;
    }
}

int RecordsFile::isOk()
{
    if(io.good())
    {
        return 0;
    }else
    {
        return -1;
    }
}

bool RecordsFile::isBoF()
{

}

bool RecordsFile::isEof()
{
  if(io.eof())
  {

    return true;
  }else
  {
    
    return false;
  }
}
