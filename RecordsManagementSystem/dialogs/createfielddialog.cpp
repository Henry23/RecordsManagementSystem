#include "createfielddialog.h"
#include "ui_createfielddialog.h"

#include "classes/recordsfile.h"

#include <cstdio>
using std::remove;

#include "QMessageBox"
#include <QDebug>

CreateFieldDialog::CreateFieldDialog(QString file, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateFieldDialog)
{
    ui->setupUi(this);
    this->fileName = file;
}

CreateFieldDialog::~CreateFieldDialog()
{
    delete ui;
}

void CreateFieldDialog::on_comboBoxType_currentIndexChanged(int index)
{
    //
    if ( index == 0 )
    {
        ui->lineEditName->clear();
        ui->lineEditName->setFocus();
        ui->spinBoxDecimal->setEnabled(false);
    }

    else if ( index == 1 )
    {
        ui->lineEditName->clear();
        ui->lineEditName->setFocus();
        ui->spinBoxDecimal->setEnabled(false);
    }

    else
    {
        ui->lineEditName->clear();
        ui->lineEditName->setFocus();
        ui->spinBoxDecimal->setEnabled(true);
    }
}

void CreateFieldDialog::on_pushButtonAccept_clicked()
{
    //Validate if the user has entered a name
    if ( ui->lineEditName->text().isEmpty() )
    {
        QMessageBox::critical(this, tr("Error"), tr("Specify a name"));
        ui->lineEditName->setFocus();
    }

    //Validate if the user has specified a length
    else if ( ui->spinBoxLength->value() == 0 )
    {
        QMessageBox::critical(this, tr("Error"), tr("Specify a length value"));
        ui->spinBoxLength->setFocus();
    }

    //If everything is ok
    else
    {
        //If the file is empty, this is the first field.
        if ( this->getNumberOfFields() == 0 )
        {
            //Checks if the field was not added
            if ( !this->addFirstField() )
            {
                QMessageBox::critical(this, "Error", "An error occurred while trying to add the field");
            }
        }

        //There is one or more fields in the file
        else
        {
            //Checks if the field was not added
            if ( !this->addField() )
            {
                QMessageBox::critical(this, "Error", "An error occurred while trying to add the field");
            }
        }

        ui->lineEditName->clear();
        ui->lineEditName->setFocus();
        ui->checkBoxKey->setChecked(false);
    }
}

void CreateFieldDialog::on_pushButtonCancel_clicked()
{
    this->close();
}

int CreateFieldDialog::getNumberOfFields()
{
    //Opening file
    RecordsFile in;

    //Checks if there is a problem while opening the file
    if ( !in.open(this->fileName.toStdString()))
    {
        return -1;
    }

    const unsigned short int size = 7; //max number of digits of the number of fields
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

int CreateFieldDialog::getLengthOfTheNumberOfFields()
{
    //Opening file
    RecordsFile in;

    //Checks if there is a problem while opening the file
    if ( !in.open(this->fileName.toStdString()))
    {
        return -1;
    }

    const unsigned short int size = 7; //max number of digits of the number of fields
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

int CreateFieldDialog::getTotalLengthOfFieldsProperties()
{
    //Open file
    RecordsFile file(this->fileName.toStdString());

    //Amount of existing fields in the file
    int numberOfFields = this->getNumberOfFields();

    int fieldsPropertiesApproximateSize = numberOfFields * 28;
    int fieldsPropertiesSize = 0;

    char buffer[fieldsPropertiesApproximateSize];

    //put the cursor of the file after the number of fields (after first number in the file)
    file.seek(this->getLengthOfTheNumberOfFields());

    //Read
    file.read(buffer, fieldsPropertiesApproximateSize);

    //Close
    file.close();

    //We move through the array of characters (buffer)
    while ( fieldsPropertiesSize < fieldsPropertiesApproximateSize )
    {
        //if this caracter is found, all the fields propertied are already readed
        if ( buffer[fieldsPropertiesSize] == ':' )
        {
            break;
        }

        fieldsPropertiesSize++;
    }

    return fieldsPropertiesSize;
}

bool CreateFieldDialog::addFirstField()
{
    //Opening file
    RecordsFile out;

    //Checks if there is a problem while opening the file
    if ( !out.open(this->fileName.toStdString()) )
    {
        return false;
    }

    //Save field information
    QString fieldInfo = QString::number(1) + "|" + ui->lineEditName->text() + "," + ui->comboBoxType->currentText() +
            "," + QString::number(ui->spinBoxLength->value()) + "," + QString::number(ui->spinBoxDecimal->value()) +
            "," + QString::number(ui->checkBoxKey->isChecked()) + "|:";

    //convert field information into a const char*
    const char *field = fieldInfo.toStdString().c_str();

    //Checks if there is no problem writing in the file
    if ( out.write(field, fieldInfo.size()) == -1 )
    {
        return false;
    }

    //Close the file
    out.close();

    return true;
}

bool CreateFieldDialog::addField()
{
    RecordsFile currentFile;
    RecordsFile newFile;

    if ( !currentFile.open(this->fileName.toStdString()) )
    {
        return false;
    }

    //------------------------- Read fields Properties -----------------------------------
    int fieldsPropertiesSize = this->getTotalLengthOfFieldsProperties();
    char fieldsBuffer[fieldsPropertiesSize];

    currentFile.seek(this->getLengthOfTheNumberOfFields());
    currentFile.read(fieldsBuffer, fieldsPropertiesSize);

    qDebug() << fieldsBuffer;


    //------------------------- Read records ----------------------------------
    int recordsSize = currentFile.fileLength() - this->getLengthOfTheNumberOfFields() - fieldsPropertiesSize;
    char recordsBuffer[recordsSize];

    currentFile.seek(fieldsPropertiesSize + 1);
    currentFile.read(recordsBuffer, recordsSize);

    qDebug() << recordsBuffer;

    //Close current file
    currentFile.close();


    //------------------------- New field -------------------------------------
    QString fieldInfo = ui->lineEditName->text() + "," + ui->comboBoxType->currentText() +
            "," + QString::number(ui->spinBoxLength->value()) + "," + QString::number(ui->spinBoxDecimal->value()) +
            "," + QString::number(ui->checkBoxKey->isChecked()) + "|";

    //convert field information into a const char*
    const char *newField = fieldInfo.toStdString().c_str();

    qDebug() << newField;


    //------------------------- Insert new field ------------------------------
    QString newNumberOfFieldsSize = "";
    newNumberOfFieldsSize += QString::number(this->getNumberOfFields() + 1);
    const char *newNumberOfFields = newNumberOfFieldsSize.toStdString().c_str();

    qDebug() << newNumberOfFields;

    if ( remove(this->fileName.toStdString().c_str()) != 0 )
    {
        return false;
    }

    if ( !newFile.open(this->fileName.toStdString(), ios::out) )
    {
        return false;
    }

    newFile.write(newNumberOfFields, newNumberOfFieldsSize.size());
    newFile.write(fieldsBuffer, fieldsPropertiesSize);
    newFile.write(newField, fieldInfo.size());
    newFile.write(recordsBuffer, recordsSize);

    newFile.close();

    return true;
}
