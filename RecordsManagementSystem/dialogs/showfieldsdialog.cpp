#include "showfieldsdialog.h"
#include "ui_showfieldsdialog.h"

#include "classes/recordsfile.h"

#include <QTableWidgetItem>
#include <QMessageBox>

ShowFieldsDialog::ShowFieldsDialog(QString fileName, bool modifyMode, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowFieldsDialog)
{
    ui->setupUi(this);

    this->fileName = fileName;
    this->modifyMode = modifyMode;

    this->tableProperties();
    this->showFields();
}

ShowFieldsDialog::~ShowFieldsDialog()
{
    delete ui;
}

void ShowFieldsDialog::tableProperties()
{
    //If the dialog was not open in modify mode
    if ( !this->modifyMode )
    {
        ui->tableWidgetFields->setEditTriggers(QAbstractItemView::NoEditTriggers); //The table can not be edited
        this->setWindowTitle(tr("Show Field"));
    }

    //If the dialog was open in modify mode
    else
    {
        this->setWindowTitle(tr("Modify Fields"));
    }

    ui->tableWidgetFields->setSelectionBehavior(QAbstractItemView::SelectItems); //clicking on a item selects only the item

    ui->tableWidgetFields->setColumnCount(6); //Number of columns
    ui->tableWidgetFields->setRowCount(this->getNumberOfFields()); //Number of rows

    ui->tableWidgetFields->hideColumn(0);

    //Columns name
    ui->tableWidgetFields->setHorizontalHeaderLabels(QString("Hide;Name;Type;Length;Decimal;Key").split(";"));
}

int ShowFieldsDialog::getNumberOfFields()
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

int ShowFieldsDialog::getLengthOfTheNumberOfFields()
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

int ShowFieldsDialog::getSizeOfTheFieldProperties(int position)
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

int ShowFieldsDialog::getLengthOfTheSizeOfTheFieldProperties(int position)
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

int ShowFieldsDialog::getTotalLengthOfFieldsProperties()
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

    return (characterCounter - this->getLengthOfTheNumberOfFields());
}

QStringList ShowFieldsDialog::getFieldsProperties()
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

    return fieldsProperties;
}

void ShowFieldsDialog::showFields()
{
    QStringList fieldsProperties = this->getFieldsProperties();

    //Rows
    for ( int a = 0; a < fieldsProperties.size(); a++ )
    {
        QStringList fieldProperties = fieldsProperties.at(a).split(",");

        //Columns
        for ( int b = 0; b <fieldProperties.size(); b++ )
        {
            //Insert a row
            ui->tableWidgetFields->setItem(a, b, new QTableWidgetItem(fieldProperties.at(b)));
        }
    }
}

void ShowFieldsDialog::on_tableWidgetFields_cellChanged(int row, int column)
{
    //Checks if there is a row selected (because while adding items, this function is call and there is no row selected)
    if ( ui->tableWidgetFields->currentIndex().row() > -1 )
    {
        //Checks if the user delete a item
        if ( ui->tableWidgetFields->item(row, column)->text().isEmpty() )
        {
            QMessageBox::critical(this, tr("Error"), tr("The information can not be empty"));

            ui->tableWidgetFields->setItem(row, column, new QTableWidgetItem("Unknown"));
        }

        //Henry, los paremetros de esta funcion almacenan la fila y columna del elemento que se modifico
        //Utilizalos para saber que campo y que propiedad vas a modificar en el archivo
        //Utiliza la siguiente instruccion para obtener el nombre del elemento modificado:
        //ui->tableWidgetFields->item(row, column)->text();
    }
}
