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
    int getNumberOfFields(); //Return the number of fields created
    int getLengthOfTheNumberOfFields(); // Example: 10 fields created, return 2 (the number of digits)
    int getSizeOfTheFieldProperties(int); //Example: 2|20,Nombre,Text,10,0,1|22,Apellido,Text,10,0,0|: -> int=24 return 22
    int getLengthOfTheSizeOfTheFieldProperties(int);//Example: 2|20,Nombre,Text,10,0,1|22,Apellido,Text,10,0,0|: -> int=24 return 2
    int getTotalLengthOfFieldsProperties();//Example: 2|20,Nombre,Text,10,0,1|22,Apellido,Text,10,0,0|: -> return 48
    QStringList getFieldsProperties(); //Example: return list -> [ ( 20,Nombre,Text,10,0,1 ), ( 22,Apellido,Text,10,0,0 ) ]

private:
    QString fileName;
    
};

#endif // RECORDOPERATIONS_H
