#ifndef RECORDOPERATIONS_H
#define RECORDOPERATIONS_H

#include <QObject>
#include <QStringList>

#include "recordsfile.h"

class RecordOperations : public QObject
{
    Q_OBJECT

public:
    RecordOperations(QObject *parent = 0);
    void setFileName(QString);

    //--------------------------Fields information----------------------------

    int getNumberOfFields(); //Return the number of fields created
    int getLengthOfTheNumberOfFields(); // Example: 10 fields created, return 2 (the number of digits)
    int getSizeOfAFieldInformation(int); //Example: 2|20,Nombre,Text,10,0,1|22,Apellido,Text,10,0,0|: -> int=24 return 22
    int getLengthOfTheSizeOfAFieldInformation(int);//Example: 2|20,Nombre,Text,10,0,1|22,Apellido,Text,10,0,0|: -> int=24 return 2
    int getLengthOfFieldsInformation();//Example: 2|20,Nombre,Text,10,0,1|22,Apellido,Text,10,0,0|: -> return 47
    QStringList getFieldsInformation(); //Example: return a list -> [ ( 20,Nombre,Text,10,0,1 ), ( 22,Apellido,Text,10,0,0 ) ]

    //--------------------------Records information---------------------------

    int getInitialPositionOfRecordsInformation(); //return the position after ' : '
    int getNumberOfRecords();
    int getLengthOfTheNumberOfRecords();
    int getSizeOfARecordInformation(int);
    int getLengthOfTheSizeOfARecordInformation(int);
    int getLengthOfRecordsInformation();
    int getRecordPositionAt(int); //Return the position of a record (from the beginnig of the file to the first ' | ' of the record
    QStringList getRecordInformationAt(int); //Return the record at the given index (start from index 0)
    QStringList getRecordsInformation(); //Return all records

private:
    QString fileName;
    
};

#endif // RECORDOPERATIONS_H
