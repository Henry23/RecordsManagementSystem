#include "createfielddialog.h"
#include "ui_createfielddialog.h"

#include "classes/recordsfile.h"

#include <cstdio>
using std::remove;

#include "QMessageBox"

CreateFieldDialog::CreateFieldDialog(QString fileName, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateFieldDialog)
{
    ui->setupUi(this);
    this->fileName = fileName;

    this->recordOperations.setFileName(this->fileName);

    //No spaces are allowed
    QRegExp noSpaces("^\\S+$");
    ui->lineEditName->setValidator(new QRegExpValidator(noSpaces,this));
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
        if ( this->recordOperations.getNumberOfFields() == 0 )
        {
            //Checks if the field was not added
            if ( !this->addFirstField() )
            {
                QMessageBox::critical(this, "Error", "An error occurred while trying to add the field");
            }

            else
            {
                QMessageBox::information(this, "Success", "The field has been added successfully");
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

            else
            {
                QMessageBox::information(this, "Success", "The field has been added successfully");
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

bool CreateFieldDialog::addFirstField()
{
    //Opening file
    RecordsFile out;

    //Checks if there is a problem while opening the file
    if ( !out.open(this->fileName.toStdString()) )
    {
        return false;
    }

    //Field information size
    int fieldInformationSize = 8; // 5(,)  1(|)  1(Decimal)  1(Key)
    fieldInformationSize += ui->lineEditName->text().size(); //Text size field name
    fieldInformationSize += ui->comboBoxType->currentText().size(); //Text size fiel type
    fieldInformationSize += QString::number(ui->spinBoxLength->value()).size(); //Digits of the field name length

    //Save field information
    QString fieldInfo = QString::number(1) + "|" + QString::number(fieldInformationSize) + "," +
            ui->lineEditName->text() + "," + ui->comboBoxType->currentText() + "," +
            QString::number(ui->spinBoxLength->value()) + "," + QString::number(ui->spinBoxDecimal->value()) +
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

    //------------------------- Read fields Information -----------------------------------
    int fieldsInformationSize = this->recordOperations.getTotalLengthOfFieldsInformation();
    char fieldsBuffer[fieldsInformationSize];

    currentFile.seek(this->recordOperations.getLengthOfTheNumberOfFields());
    currentFile.read(fieldsBuffer, fieldsInformationSize);


    //------------------------- Read records ----------------------------------
    int recordsSize = currentFile.fileLength() - this->recordOperations.getLengthOfTheNumberOfFields() - fieldsInformationSize;
    char recordsBuffer[recordsSize];

    currentFile.seek(this->recordOperations.getLengthOfTheNumberOfFields() + fieldsInformationSize);
    currentFile.read(recordsBuffer, recordsSize);

    //Close current file
    currentFile.close();


    //------------------------- New field -------------------------------------
    //Field information size
    int fieldInformationSize = 8; // 5(,)  1(|)  1(Decimal)  1(Key)
    fieldInformationSize += ui->lineEditName->text().size(); //Text size field name
    fieldInformationSize += ui->comboBoxType->currentText().size(); //Text size fiel type
    fieldInformationSize += QString::number(ui->spinBoxLength->value()).size(); //Digits of the field name length

    QString fieldInfo = QString::number(fieldInformationSize) + "," + ui->lineEditName->text() + "," + ui->comboBoxType->currentText() +
            "," + QString::number(ui->spinBoxLength->value()) + "," + QString::number(ui->spinBoxDecimal->value()) +
            "," + QString::number(ui->checkBoxKey->isChecked()) + "|";

    //convert field information into a const char*
    const char *newField = fieldInfo.toStdString().c_str();


    //------------------------- Insert new field ------------------------------
    QString newNumberOfFieldsSize = "";
    newNumberOfFieldsSize += QString::number(this->recordOperations.getNumberOfFields() + 1);
    const char *newNumberOfFields = newNumberOfFieldsSize.toStdString().c_str();

    if ( remove(this->fileName.toStdString().c_str()) != 0 )
    {
        return false;
    }

    if ( !newFile.open(this->fileName.toStdString(), ios::out) )
    {
        return false;
    }

    newFile.write(newNumberOfFields, newNumberOfFieldsSize.size());
    newFile.write(fieldsBuffer, fieldsInformationSize);
    newFile.write(newField, fieldInfo.size());
    newFile.write(recordsBuffer, recordsSize);

    newFile.close();

    return true;
}
