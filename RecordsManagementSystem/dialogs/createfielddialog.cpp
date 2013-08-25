#include "createfielddialog.h"
#include "ui_createfielddialog.h"

#include "classes/recordsfile.h"

#include <cstdio>
using std::remove;

#include <QMessageBox>
#include <QDebug>

CreateFieldDialog::CreateFieldDialog(QString fileName, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateFieldDialog)
{
    ui->setupUi(this);

    //Set the fileName
    this->fileName = fileName;

    this->recordOperations.setFileName(this->fileName);
}

CreateFieldDialog::~CreateFieldDialog()
{
    delete ui;
}

void CreateFieldDialog::on_comboBoxType_currentIndexChanged(int index)
{
    if ( index == 0 )
    {
        ui->spinBoxDecimal->setEnabled(false);
    }

    else if ( index == 1 )
    {
        ui->spinBoxDecimal->setEnabled(false);
    }

    else
    {
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

    else if ( this->fieldAlreadyExist() )
    {
        QMessageBox::critical(this, tr("Error"), tr("The field already exist"));
        ui->lineEditName->setFocus();
    }

    else if ( ui->checkBoxKey->isChecked() && this->keyAlreadyExist() )
    {
        QMessageBox::critical(this, tr("Error"), tr("A key already exist"));
        ui->checkBoxKey->setChecked(false);
    }


    //If everything is ok
    else
    {
        //Checks if the field was not added
        if ( !this->addField() )
        {
            QMessageBox::critical(this, "Error", "An error occurred while trying to add the field");
        }

        else
        {
            QMessageBox::information(this, "Success", "The field has been added successfully");
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

bool CreateFieldDialog::fieldAlreadyExist()
{
    //If there are fields (the file is not empty)
    if ( recordOperations.getNumberOfFields() > 0 )
    {
        //All the fields information
        QStringList fieldsInformation = this->recordOperations.getFieldsInformation();

        for ( int a = 0; a < fieldsInformation.size(); a++ )
        {
            //A single field information
            QStringList fieldInformation = fieldsInformation.at(a).split(",");

            //Compare if they are equals
            if ( fieldInformation.at(1) == ui->lineEditName->text() )
            {
                return true;
            }
        }
    }

    return false;
}

bool CreateFieldDialog::keyAlreadyExist()
{
    //If there are fields (the file is not empty)
    if ( recordOperations.getNumberOfFields() > 0 )
    {
        //All the fields information
        QStringList fieldsInformation = this->recordOperations.getFieldsInformation();

        for (int a = 0; a < fieldsInformation.size(); a++)
        {
            QStringList fieldInformation = fieldsInformation.at(a).split(",");

            //If a field already has a key
            if ( fieldInformation.at(fieldInformation.size() - 1) == "1" )
            {
                return true;
            }
        }
    }

    return false;
}

bool CreateFieldDialog::addField()
{
    RecordsFile currentFile;
    RecordsFile newFile;

    if ( !currentFile.open(this->fileName.toStdString()) )
    {
        return false;
    }

    //------------------------ New number of fields ---------------------------------------

    //Current number of fields + 1
    QString newNumberOfFields = "";
    newNumberOfFields += QString::number(this->recordOperations.getNumberOfFields() + 1);

    //If this is the first field (the file is empty)
    if ( this->recordOperations.getNumberOfFields() == 0 )
    {
        newNumberOfFields.append("|");
    }


    //------------------------- Read fields Information -----------------------------------

    //start at ' | ' end at ' | '
    int currentFieldsInformationSize = this->recordOperations.getLengthOfFieldsInformation();
    char currentFieldsInformationBuffer[currentFieldsInformationSize];

    //Set the cursor of the file after the number of fields
    currentFile.seek(this->recordOperations.getLengthOfTheNumberOfFields());
    currentFile.read(currentFieldsInformationBuffer, currentFieldsInformationSize);


    //------------------------- New field -------------------------------------

    //Field information size
    int fieldInformationSize = 8; // 5(,)  1(|)  1(Decimal)  1(Key)
    fieldInformationSize += ui->lineEditName->text().size(); //Text size field name
    fieldInformationSize += ui->comboBoxType->currentText().size(); //Text size fiel type
    fieldInformationSize += QString::number(ui->spinBoxLength->value()).size(); //Digits of the field name length

    QString newField = QString::number(fieldInformationSize) + "," + ui->lineEditName->text() + "," + ui->comboBoxType->currentText() +
            "," + QString::number(ui->spinBoxLength->value()) + "," + QString::number(ui->spinBoxDecimal->value()) +
            "," + QString::number(ui->checkBoxKey->isChecked()) + "|:";



    //------------------------- Read records ----------------------------------

    int recordsSize = this->recordOperations.getLengthOfTheNumberOfRecords() + this->recordOperations.getLengthOfRecordsInformation();
    char recordsBuffer[recordsSize];

    currentFile.seek(this->recordOperations.getInitialPositionOfRecordsInformation());
    currentFile.read(recordsBuffer, recordsSize);

    //Close current file
    currentFile.close();



    //------------------------- Insert new field ------------------------------


    if ( remove(this->fileName.toStdString().c_str()) != 0 )
    {
        return false;
    }

    if ( !newFile.open(this->fileName.toStdString(), ios::out) )
    {
        return false;
    }

    newFile.write(newNumberOfFields.toStdString().c_str(), newNumberOfFields.size());
    newFile.write(currentFieldsInformationBuffer, currentFieldsInformationSize);
    newFile.write(newField.toStdString().c_str(), newField.size());
    newFile.write(recordsBuffer, recordsSize);

    newFile.close();

    return true;
}
