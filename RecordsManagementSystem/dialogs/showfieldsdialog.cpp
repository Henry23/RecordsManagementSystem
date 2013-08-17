#include "showfieldsdialog.h"
#include "ui_showfieldsdialog.h"

#include "classes/recordsfile.h"

#include <QTableWidgetItem>
#include <QMessageBox>
#include <QDebug>

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

    ui->tableWidgetFields->setColumnCount(5); //Number of columns
    ui->tableWidgetFields->setRowCount(this->getNumberOfFields()); //Number of rows

    //Columns name
    ui->tableWidgetFields->setHorizontalHeaderLabels(QString("Name;Type;Length;Decimal;Key").split(";"));
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

int ShowFieldsDialog::getLengthOfTheNumberOfFields()
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

int ShowFieldsDialog::getTotalLengthOfFieldsProperties()
{
    //Open file
    RecordsFile file(this->fileName.toStdString());

    //Amount of existing fields in the file
    int numberOfFields = this->getNumberOfFields();

    int fieldsPropertiesApproximateSize = numberOfFields * 28;
    int fieldsPropertiesSize = 0;

    char buffer[fieldsPropertiesApproximateSize];

    //put the cursor of the file after the first ' | '
    file.seek(this->getLengthOfTheNumberOfFields() + 1);

    //Read
    file.read(buffer, fieldsPropertiesApproximateSize);

    //Close
    file.close();

    //We move through the array of characters (buffer)
    while ( fieldsPropertiesSize < fieldsPropertiesApproximateSize )
    {
        //if this caracter is found, all the fields properties are already readed
        if ( buffer[fieldsPropertiesSize] == ':' )
        {
            --fieldsPropertiesSize; //Dont need the last ' | '
            break;
        }

        fieldsPropertiesSize++;
    }

    return fieldsPropertiesSize;
}

QStringList ShowFieldsDialog::getFieldsProperties()
{
    //Read file
    RecordsFile file(this->fileName.toStdString());

    int size = this->getTotalLengthOfFieldsProperties();
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
