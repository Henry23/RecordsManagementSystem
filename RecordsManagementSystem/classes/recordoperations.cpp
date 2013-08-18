#include "recordoperations.h"

RecordOperations::RecordOperations(QObject *parent) :
    QObject(parent)
{
}

void RecordOperations::setFileName(QString fileName)
{
    this->fileName = fileName;
}

int RecordOperations::getNumberOfFields()
{
    //Opening file
    RecordsFile in;

    //Checks if there is a problem while opening the file
    if ( !in.open(this->fileName.toStdString()))
    {
        return -1;
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

    //zero if the file is empty
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

int RecordOperations::getSizeOfTheFieldProperties(int position)
{
    //Opening file
    RecordsFile in;

    //Checks if there is a problem while opening the file
    if ( !in.open(this->fileName.toStdString()))
    {
        return -1;
    }

    //max number of digits of the size of the field properties (Example: 18 caracter of properties)
    const unsigned short int size = 2;
    char buffer[size] = {};

    //Sets the read start position
    in.seek(position);

    //Reading
    in.read(buffer, size);

    in.close();

    //Stores the size of the field properties
    QString sizeOfFieldProperties = "";

    //We move through the array of characters (buffer)
    for ( int a = 0; a < size; a++ )
    {
        //if this caracter is found, the size of the field properties was already readed
        if ( buffer[a] == ',' )
        {
            break;
        }

        else
        {
            //Concatenate the digit
            sizeOfFieldProperties += buffer[a];
        }
    }

    //zero if the file is empty
    return sizeOfFieldProperties.toInt();
}

int RecordOperations::getLengthOfTheSizeOfTheFieldProperties(int position)
{
    //Opening file
    RecordsFile in;

    //Checks if there is a problem while opening the file
    if ( !in.open(this->fileName.toStdString()))
    {
        return -1;
    }

    //max number of digits of the size of the field properties (Example: 18 caracter of properties)
    const unsigned short int size = 2;
    char buffer[size] = {};

    //Sets the read start position
    in.seek(position);

    //Reading
    in.read(buffer, size);

    in.close();

    //Stores the size of the field properties
    QString sizeOfFieldProperties = "";

    //We move through the array of characters (buffer)
    for ( int a = 0; a < size; a++ )
    {
        //if this caracter is found, the size of the field properties was already readed
        if ( buffer[a] == ',' )
        {
            break;
        }

        else
        {
            //Concatenate the digit
            sizeOfFieldProperties += buffer[a];
        }
    }

    //zero if the file is empty
    return sizeOfFieldProperties.length();
}

int RecordOperations::getTotalLengthOfFieldsProperties()
{
    //Opening file
    RecordsFile file;

    //Checks if there is a problem while opening the file
    if ( !file.open(this->fileName.toStdString()))
    {
        return -1;
    }

    int numberOfFields = this->getNumberOfFields();
    int characterCounter = this->getLengthOfTheNumberOfFields() + 1; //The number of fields + the first ' | '

    //All the fields
    for ( int a = 0; a < numberOfFields; a++ )
    {
        int position = characterCounter;

        characterCounter += this->getLengthOfTheSizeOfTheFieldProperties(position) +
                this->getSizeOfTheFieldProperties(position);

        file.seek(characterCounter);
    }

    file.close();

    return (characterCounter - this->getLengthOfTheNumberOfFields());
}

QStringList RecordOperations::getFieldsProperties()
{
    //Read file
    RecordsFile file(this->fileName.toStdString());

    int size = this->getTotalLengthOfFieldsProperties() - 1; // -1 because we dont need the last ' | '
    char buffer[size];

    file.seek(this->getLengthOfTheNumberOfFields() + 1);//put the cursor of the file after the first ' | '
    file.read(buffer, size);

    //Store the buffer in a QString (Bescause of the 'split' function)
    QString temp = buffer;
    QStringList fieldsProperties = temp.split("|");

    file.close();

    return fieldsProperties;
}
