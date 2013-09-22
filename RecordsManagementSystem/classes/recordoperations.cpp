#include "recordoperations.h"

#include <QDebug>

RecordOperations::RecordOperations(QObject *parent) :
    QObject(parent)
{
}

void RecordOperations::setFileName(QString fileName)
{
    this->fileName = fileName;
}

//---------------------------------------------Fields Information--------------------------------------------------------

int RecordOperations::getNumberOfFields()
{
    //Opening file
    RecordsFile in;

    //Checks if there is a problem while opening the file
    if ( !in.open(this->fileName.toStdString()))
    {
        return -1;
    }

    //If the file is empty
    if ( in.fileLength() == 0 )
    {
        in.close();
        return 0;
    }

    const unsigned short int size = 4; //max number of digits of the number of fields (Example: 1000 fields)
    char buffer[size] = {};

    //Reading
    in.read(buffer, size);

    in.close();

    //Stores the number of fields
    QString numberOfFields = "";

    //We move through the array of characters (buffer)
    for ( int a = 0; a < size; a++ )
    {
        //if this caracter is found, the number of fields was already readed
        if ( buffer[a] == '|' )
        {
            break;
        }

        else
        {
            //Concatenate the digit in the number of fields
            numberOfFields += buffer[a];
        }
    }

    return numberOfFields.toInt();
}

int RecordOperations::getLengthOfTheNumberOfFields()
{
    //Opening file
    RecordsFile in;

    //Checks if there is a problem while opening the file
    if ( !in.open(this->fileName.toStdString()))
    {
        return -1;
    }

    //If the number of field is zero (the file is empty)
    if ( this->getNumberOfFields() == 0 )
    {
        in.close();
        return 0;
    }

    const unsigned short int size = 4; //max number of digits of the number of fields(Example: 1000 fields)
    char buffer[size] = {};

    //Reading
    in.read(buffer, size);

    in.close();

    //Stores the number of fields
    QString numberOfFields = "";

    //We move through the array of characters (buffer)
    for ( int a = 0; a < size; a++ )
    {
        //if this caracter is found, the number of fields was already readed
        if ( buffer[a] == '|' )
        {
            break;
        }

        else
        {
            //Concatenate the digit in the number of fields
            numberOfFields += buffer[a];
        }
    }

    return numberOfFields.length();
}

int RecordOperations::getSizeOfAFieldInformation(int position)
{
    //Opening file
    RecordsFile in;

    //Checks if there is a problem while opening the file
    if ( !in.open(this->fileName.toStdString()))
    {
        return -1;
    }

    //max number of digits of the size of the field information (Example: 31 caracter of information)
    const unsigned short int size = 2;
    char buffer[size] = {};

    //Sets the read start position
    in.seek(position);

    //Reading
    in.read(buffer, size);

    in.close();

    //Stores the size of the field information
    QString sizeOfFieldInformation = "";

    //We move through the array of characters (buffer)
    for ( int a = 0; a < size; a++ )
    {
        //if this caracter is found, the size of the field information was already readed
        if ( buffer[a] == ',' )
        {
            break;
        }

        else
        {
            //Concatenate the digit
            sizeOfFieldInformation += buffer[a];
        }
    }

    //zero if the file is empty
    return sizeOfFieldInformation.toInt();
}

int RecordOperations::getLengthOfTheSizeOfAFieldInformation(int position)
{
    //Opening file
    RecordsFile in;

    //Checks if there is a problem while opening the file
    if ( !in.open(this->fileName.toStdString()))
    {
        return -1;
    }

    //max number of digits of the size of the field information (Example: 18 caracter of information)
    const unsigned short int size = 2;
    char buffer[size] = {};

    //Sets the read start position
    in.seek(position);

    //Reading
    in.read(buffer, size);

    in.close();

    //Stores the size of the field information
    QString sizeOfFieldInformation = "";

    //We move through the array of characters (buffer)
    for ( int a = 0; a < size; a++ )
    {
        //if this caracter is found, the size of the field information was already readed
        if ( buffer[a] == ',' )
        {
            break;
        }

        else
        {
            //Concatenate the digit
            sizeOfFieldInformation += buffer[a];
        }
    }

    //zero if the file is empty
    return sizeOfFieldInformation.length();
}

int RecordOperations::getLengthOfFieldsInformation()
{
    //Opening file
    RecordsFile file;

    //Checks if there is a problem while opening the file
    if ( !file.open(this->fileName.toStdString()))
    {
        return -1;
    }

    //If the number of fields is zero (the file is empty)
    if ( this->getNumberOfFields() == 0 )
    {
        file.close();
        return 0;
    }

    int numberOfFields = this->getNumberOfFields();
    int characterCounter = this->getLengthOfTheNumberOfFields() + 1; //The number of fields + the first ' | '

    //All the fields
    for ( int a = 0; a < numberOfFields; a++ )
    {
        int position = characterCounter;

        characterCounter += this->getLengthOfTheSizeOfAFieldInformation(position) +
                this->getSizeOfAFieldInformation(position);

        file.seek(characterCounter);
    }

    file.close();

    //Return the length of the fields information
    return (characterCounter - this->getLengthOfTheNumberOfFields());
}

QStringList RecordOperations::getFieldsInformation()
{
    //Read file
    RecordsFile file(this->fileName.toStdString());

    int size = this->getLengthOfFieldsInformation() - 2; // -2 because we dont need the first and last ' | '
    char buffer[size];

    file.seek(this->getLengthOfTheNumberOfFields() + 1);//put the cursor of the file after the first ' | '
    file.read(buffer, size);

    file.close();

    //Store the buffer in a QString (Bescause of the 'split' function)
    QString fieldsInformation = buffer;

    return fieldsInformation.split("|");
}



//----------------------------------------------------Records Information------------------------------------------------------



int RecordOperations::getInitialPositionOfRecordsInformation()
{
    //La longitud del numero de campos + la informacion de los campos + los dos puntos ' : '
    return this->getLengthOfTheNumberOfFields() + this->getLengthOfFieldsInformation() + 1;
}

int RecordOperations::getNumberOfRecords()
{
    //Opening file
    RecordsFile in;

    //Checks if there is a problem while opening the file
    if ( !in.open(this->fileName.toStdString()))
    {
        return -1;
    }

    //If the length of the file is equals to the initial position of records informations, no records
    if ( in.fileLength() == this->getInitialPositionOfRecordsInformation() )
    {
        in.close();
        return 0;
    }

    const unsigned short int size = 7; //max number of digits of the number of records (Example: 1000 records)
    char buffer[size] = {};

    //Sets the read start position (after ' : ')
    in.seek(this->getInitialPositionOfRecordsInformation());

    //Reading
    in.read(buffer, size);

    in.close();

    //Stores the number of records
    QString numberOfRecords = "";

    //We move through the array of characters (buffer)
    for ( int a = 0; a < size; a++ )
    {
        //if this caracter is found, the number of records was already readed
        if ( buffer[a] == '|' )
        {
            break;
        }

        else
        {
            //Concatenate the digit in the number of records
            numberOfRecords += buffer[a];
        }
    }

    return numberOfRecords.toInt();
}

int RecordOperations::getLengthOfTheNumberOfRecords()
{
    //Opening file
    RecordsFile in;

    //Checks if there is a problem while opening the file
    if ( !in.open(this->fileName.toStdString()))
    {
        return -1;
    }

    //If there are no records
    if ( this->getNumberOfRecords() == 0 )
    {
        in.close();
        return 0;
    }

    const unsigned short int size = 7; //max number of digits of the number of records (Example: 1000 records)
    char buffer[size] = {};

    //Sets the read start position (after ' : ')
    in.seek(this->getInitialPositionOfRecordsInformation());

    //Reading
    in.read(buffer, size);

    in.close();

    //Stores the number of records
    QString numberOfRecords = "";

    //We move through the array of characters (buffer)
    for ( int a = 0; a < size; a++ )
    {
        //if this caracter is found, the number of records was already readed
        if ( buffer[a] == '|' )
        {
            break;
        }

        else
        {
            //Concatenate the digit in the number of records
            numberOfRecords += buffer[a];
        }
    }

    return numberOfRecords.length();
}

int RecordOperations::getSizeOfARecordInformation(int position)
{
    //Opening file
    RecordsFile in;

    //Checks if there is a problem while opening the file
    if ( !in.open(this->fileName.toStdString()))
    {
        return -1;
    }

    //max number of digits of the size of the record information (Example: 10000 caracter of information)
    const unsigned short int size = 5;
    char buffer[size] = {};

    //Sets the read start position
    in.seek(position);

    //Reading
    in.read(buffer, size);

    in.close();

    //Stores the size of the record information
    QString sizeOfRecordInformation = "";

    //We move through the array of characters (buffer)
    for ( int a = 0; a < size; a++ )
    {
        //if this caracter is found, the size of the record information was already readed
        if ( buffer[a] == ',' )
        {
            break;
        }

        else
        {
            //Concatenate the digit
            sizeOfRecordInformation += buffer[a];
        }
    }

    //zero if the file is empty
    return sizeOfRecordInformation.toInt();
}

int RecordOperations::getLengthOfTheSizeOfARecordInformation(int position)
{
    //Opening file
    RecordsFile in;

    //Checks if there is a problem while opening the file
    if ( !in.open(this->fileName.toStdString()))
    {
        return -1;
    }

    //max number of digits of the size of the record information (Example: 10000 caracter of information)
    const unsigned short int size = 5;
    char buffer[size] = {};

    //Sets the read start position
    in.seek(position);

    //Reading
    in.read(buffer, size);

    in.close();

    //Stores the size of the record information
    QString sizeOfRecordInformation = "";

    //We move through the array of characters (buffer)
    for ( int a = 0; a < size; a++ )
    {
        //if this caracter is found, the size of the record information was already readed
        if ( buffer[a] == ',' )
        {
            break;
        }

        else
        {
            //Concatenate the digit
            sizeOfRecordInformation += buffer[a];
        }
    }

    //zero if the file is empty
    return sizeOfRecordInformation.size();
}

int RecordOperations::getLengthOfRecordsInformation()
{
    //Opening file
    RecordsFile file;

    //Checks if there is a problem while opening the file
    if ( !file.open(this->fileName.toStdString()))
    {
        return -1;
    }

    //If there are no records
    if ( this->getNumberOfRecords() == 0 )
    {
        file.close();
        return 0;
    }

    int numberOfRecords = this->getNumberOfRecords();

    //The length to ' : ' + The number of records + the first ' | '
    int characterCounter = this->getInitialPositionOfRecordsInformation() + this->getLengthOfTheNumberOfRecords() + 1;

    //All the records
    for ( int a = 0; a < numberOfRecords; a++ )
    {
        int position = characterCounter;

        characterCounter += this->getLengthOfTheSizeOfARecordInformation(position) +
                this->getSizeOfARecordInformation(position);

        file.seek(characterCounter);
    }

    file.close();

    //Total length of the file - the number of records - the length to the begining of the file
    return (characterCounter - this->getLengthOfTheNumberOfRecords() - this->getInitialPositionOfRecordsInformation());
}

int RecordOperations::getRecordPositionAt(int index)
{
    //Opening file
    RecordsFile file;

    //Checks if there is a problem while opening the file
    if ( !file.open(this->fileName.toStdString()))
    {
        return -1;
    }

    //The length to ' : ' + The number of records + the first ' | '
    int position = this->getInitialPositionOfRecordsInformation() + this->getLengthOfTheNumberOfRecords() + 1;

    for ( int a = 0; a < index; a++ )
    {
        int tempPosition = position;

        position += this->getLengthOfTheSizeOfARecordInformation(tempPosition) +
                this->getSizeOfARecordInformation(tempPosition);

        file.seek(position);
    }

    file.close();

    //Return the position of the record of the file ( after the first ' | ' )
    return position;
}

QStringList RecordOperations::getRecordInformationAt(int index)
{
    //Read file
    RecordsFile file(this->fileName.toStdString());

    //Store the initial record position (After the first ' | ' of the record)
    int position = this->getRecordPositionAt(index);

    int size = this->getLengthOfTheSizeOfARecordInformation(position) +
            this->getSizeOfARecordInformation(position) - 1;

    char buffer[size];

    file.seek(position);
    file.read( buffer, size );

    file.close();

    //Store the buffer in a Qstring because of the split function
    QString recordInformation = buffer;

    QString info;
    for( int j = 0; j < size; j++ )
    {
        info += recordInformation[j];
    }

    qDebug() << size;
    qDebug() << info;

    return info.split(",");
}

QStringList RecordOperations::getRecordsInformation()
{
    //Read file
    RecordsFile file(this->fileName.toStdString());

    int size = this->getLengthOfRecordsInformation() - 2; // -2 because we dont need first and last ' | '
    char buffer[size];

    //put the cursor of the file after the first ' | '
    file.seek(this->getInitialPositionOfRecordsInformation() + this->getLengthOfTheNumberOfRecords() + 1);
    file.read(buffer, size);

    file.close();

    //Store the buffer in a QString (Bescause of the 'split' function)
    QString recordsInformation = buffer;

    return recordsInformation.split("|");
}
