#include "createfielddialog.h"
#include "ui_createfielddialog.h"

#include "classes/recordsfile.h"

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
    int numberOfFields = this->getNumberOfFields();

    //const int size
    //char

    RecordsFile file;
}
