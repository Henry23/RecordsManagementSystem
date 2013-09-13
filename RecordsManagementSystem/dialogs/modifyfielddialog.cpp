#include "modifyfielddialog.h"
#include "ui_modifyfielddialog.h"

#include "classes/recordsfile.h"

#include <QApplication>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <sstream>
#include <string.h>
#include <qDebug>

#include <fstream>

using namespace std;

using std::stringstream;

ModifyFieldDialog::ModifyFieldDialog(QString fileName, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ModifyFieldDialog)
{
    ui->setupUi(this);

    this->fileName = fileName;

    this->recordOperations.setFileName(this->fileName);

    this->tableProperties();
    this->showFields();
}

ModifyFieldDialog::~ModifyFieldDialog()
{
    delete ui;
}

void ModifyFieldDialog::tableProperties()
{
    ui->tableWidgetFields->setColumnCount(5); //Number of columns
    ui->tableWidgetFields->setRowCount(this->recordOperations.getNumberOfFields()); //Number of rows

    //Columns name
    ui->tableWidgetFields->setHorizontalHeaderLabels(QString("Name;Type;Length;Decimal;Key").split(";"));
}

void ModifyFieldDialog::showFields()
{
    QStringList fieldsInformation = this->recordOperations.getFieldsInformation();

    //Rows
    for ( int a = 0; a < fieldsInformation.size(); a++ )
    {
        QStringList fieldInformation = fieldsInformation.at(a).split(",");

        //Columns
        for ( int b = 1; b < fieldInformation.size(); b++ )
        {
            //Column "Name"
            if ( b == 1 )
            {
                //Create a comboBox
                QLineEdit *lineEdit = new QLineEdit;
                lineEdit->setText(fieldInformation.at(b)); //Set the text

                connect(lineEdit, SIGNAL(returnPressed()),
                        this, SLOT(modifyName()));

                //Set the lineEdit
                ui->tableWidgetFields->setCellWidget(a, b - 1, lineEdit);

                //Jump to the next column
                continue;
            }

            //Column "Type"
            else if ( b == 2 )
            {
                //Create a combo box
                QComboBox *comboBox = new QComboBox;
                comboBox->addItems(QString("Text,Integer,Double").split(",")); //Set the items

                //Checks the -type- field and set that combobox item
                if ( fieldInformation.at(b) == "Text" )
                {
                    comboBox->setCurrentIndex(0);
                }

                else if ( fieldInformation.at(b) == "Integer" )
                {
                    comboBox->setCurrentIndex(1);
                }

                else
                {
                    comboBox->setCurrentIndex(2);
                }

                connect(comboBox, SIGNAL(currentIndexChanged(QString)),
                        this, SLOT(modifyTypeLengthDecimal()));

                //Set the combobox
                ui->tableWidgetFields->setCellWidget(a, b - 1, comboBox);

                //Jump to the next column
                continue;
            }

            //Column "Length"
            else if ( b == 3 )
            {
                //Create a Combo box
                QComboBox *comboBox = new QComboBox;

                //Add 64 numbers
                for (int a = 1; a <= 64; a++)
                {
                    comboBox->addItem(QString::number(a));
                }

                //Get and set the index
                for (int a = 0; a < 64; a++)
                {
                    if ( comboBox->itemText(a).toInt() == fieldInformation.at(b).toInt() )
                    {
                        comboBox->setCurrentIndex(a);
                    }
                }

                connect(comboBox, SIGNAL(currentIndexChanged(int)),
                        this, SLOT(modifyTypeLengthDecimal()));

                //Set the comboBox
                ui->tableWidgetFields->setCellWidget(a, b - 1, comboBox);

                //Jump to the next column
                continue;
            }

            //Column "Decimal"
            else if ( b == 4 )
            {
                //Create a combo box
                QComboBox *comboBox = new QComboBox;

                //Add 6 numbers
                for (int a = 0; a <= 6; a++)
                {
                    comboBox->addItem(QString::number(a));
                }

                //Get and set the index
                for (int a = 0; a < 6; a++)
                {
                    if ( comboBox->itemText(a).toInt() == fieldInformation.at(b).toInt() )
                    {
                        comboBox->setCurrentIndex(a);
                    }
                }

                connect(comboBox, SIGNAL(currentIndexChanged(int)),
                        this, SLOT(modifyTypeLengthDecimal()));

                //Set the comboBox
                ui->tableWidgetFields->setCellWidget(a, b - 1, comboBox);

                //Jump to the next column
                continue;
            }

            //Column "Key"
            else if ( b == 5 )
            {
                //Create a check box
                QCheckBox *checkBox = new QCheckBox;
                checkBox->setEnabled(false);

                //If the field has a key
                if ( fieldInformation.at(b) == "1" )
                {
                    checkBox->setChecked(true);
                }

                else
                {
                    checkBox->setChecked(false);
                }

                connect(checkBox, SIGNAL(clicked()),
                        this, SLOT(modifyKey()));

                //Set the checkBox
                ui->tableWidgetFields->setCellWidget(a, b - 1, checkBox);

                //Jump to the next column
                continue;
            }
        }
    }
}

void ModifyFieldDialog::on_tableWidgetFields_cellChanged(int row, int column)
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

     }
}

void ModifyFieldDialog::modifyName()
{
    QWidget *widget = QApplication::focusWidget();
    QModelIndex index = ui->tableWidgetFields->indexAt(widget->pos());

    QString name = ((QLineEdit*)ui->tableWidgetFields->cellWidget(index.row(), index.column()))->text();

    modifyField(index.row(), index.column(), name);
}

void ModifyFieldDialog::modifyTypeLengthDecimal()
{
    QWidget *widget = QApplication::focusWidget();
    QModelIndex index = ui->tableWidgetFields->indexAt(widget->pos());

    QString type = ((QComboBox*)ui->tableWidgetFields->cellWidget(index.row(), index.column()))->currentText();

    modifyField(index.row(), index.column(), type);
}

void ModifyFieldDialog::modifyKey()
{
    QWidget *widget = QApplication::focusWidget();
    QModelIndex index = ui->tableWidgetFields->indexAt(widget->pos());

    QString key = QString::number(((QCheckBox*)ui->tableWidgetFields->cellWidget(index.row(), index.column()))->isChecked());

    modifyField(index.row(), index.column(), key);
}

void ModifyFieldDialog::modifyField(int row, int column, QString data)
{
    //qDebug() << "Row: " << row << "Column: " << column << "Data:" << data;
    //-----------------------------------------------Modifiy---------------------------------------------

    //we got the list of slipt it by '|'
    QStringList fieldsProperties = this->recordOperations.getFieldsInformation();
    //list slipt it by ','
    QStringList fieldProperties;
    //for each field that has been slipt by parts
    QStringList field;

    //in this we get the field selected
    for ( int a = 0; a < fieldsProperties.size(); a++ )
    {
        fieldProperties = fieldsProperties.at( a ).split( "," );
        for ( int b = 0; b < fieldProperties.size(); b++ )
        {
            if ( a == row )
            {
                field << fieldProperties[b];
            }
         }
     }
     //we open the file of field
     RecordsFile file(this->fileName.toStdString());
     int length1 = 0;   //length of the first posicion after | of the field selected
     int length2 = 0;   //length of the second posicion
     int length3 = 0;   //length of the third posicion at the end of the field selected
     int tPositionRow = 0; // the posicion at the all field no - selected
     int tPositionColumn = 0; //plus of all field length except the one we selected

     for ( int i = 0; i < row; i++ )
     {
          tPositionRow +=  fieldsProperties.at(i).length() + 1;
     }

     for ( int i = 0; i <= ( column + 1 ); i++ )
     {
         tPositionColumn +=  field[i].length() + 1;
     }

     length1 = ( this->recordOperations.getLengthOfTheNumberOfFields() + 1 ) + (tPositionRow);   
     length2 = tPositionColumn - ( ( field[0].length() + 1 ) + ( field[column + 1].length() ) );
     length3 = file.fileLength() - length1 - 3;

     char *buffer1 = new char[length1];//first buffer
     char *buffer2 = new char[length2];//second buffer
     char *buffer3 = new char[length3];//thrird buffer

     //we read the record's field to get the information we need
     file.read( buffer1 , length1);
     file.seek( length1 + field[0].length());
     file.read( buffer2 ,  length2 );
     file.seek( length1 + tPositionColumn  );
     file.read( buffer3 , length3 );
     file.close();//we closed the file

     int newLength = 0;//this will be the new length of the field

     for( int i = 1; i < field.size(); i++ )
     {
         if( ( column + 1 ) != i )
         {
            newLength += field[i].length();
         }
     }

     newLength += field.size() + data.length();
     const char *path = this->fileName.toStdString().c_str();
     remove( path );
     RecordsFile create;

     //Check if there is a problem while creating the file

     if ( !create.open(this->fileName.toStdString(),  ios::out) )
     {
         QMessageBox::critical(this, tr("Error"), tr("An error occurred while trying to create the file"));
     }

     //casting
     stringstream strs , sst;
     sst << data.toStdString();
     strs << newLength;
     string temp_field = sst.str();
     string temp_str = strs.str();
     const char * changeField = (char*)temp_field.c_str();
     char * char_type = (char*) temp_str.c_str();
    /*
     qDebug() << "data" << data;
     qDebug() << changeField;
    */

     //we create the new file
     create.write( buffer1, length1 ) ;
     create.write( char_type ,strlen(char_type));
     create.write( buffer2, length2 );
     create.write( changeField, strlen(changeField) );
     create.write( ",", 1);
     create.write( buffer3 ,strlen(buffer3) );
     create.close();

     //delete the pointer
     delete [] buffer1;
     delete [] buffer3;
     delete [] buffer2;


}


