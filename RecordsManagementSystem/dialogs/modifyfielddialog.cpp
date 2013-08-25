#include "modifyfielddialog.h"
#include "ui_modifyfielddialog.h"

#include "classes/recordsfile.h"

#include <QComboBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QTableWidgetItem>
#include <QMessageBox>

#include <qDebug>

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
                ui->tableWidgetFields->setItem(a, b - 1, new QTableWidgetItem(fieldInformation.at(b)));

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

                //Set the combobox
                ui->tableWidgetFields->setCellWidget(a, b - 1, comboBox);

                //Jump to the next column
                continue;
            }

            //Column "Length"
            else if ( b == 3 )
            {
                //Create a Spin Box
                QSpinBox *spinBox = new QSpinBox;
                spinBox->setMinimum(1);
                spinBox->setMaximum(64);

                //Set the value
                spinBox->setValue(fieldInformation.at(b).toInt());

                //Set the spinner
                ui->tableWidgetFields->setCellWidget(a, b - 1, spinBox);

                //Jump to the next column
                continue;
            }

            else if ( b == 4 )
            {
                //Create a spin box
                QSpinBox *spinBox = new QSpinBox;
                spinBox->setMinimum(0);
                spinBox->setMaximum(6);

                //Set the value
                spinBox->setValue(fieldInformation.at(b).toInt());

                //Set the spinner
                ui->tableWidgetFields->setCellWidget(a, b - 1, spinBox);

                //Jump to the next column
                continue;
            }

            //Column "Key"
            else if ( b == 5 )
            {
                //Create a check box
                QCheckBox *checkBox = new QCheckBox;

                //If the field has a key
                if ( fieldInformation.at(b) == "1" )
                {
                    checkBox->setChecked(true);
                }

                else
                {
                    checkBox->setChecked(false);
                }

                //Set the checkBox
                ui->tableWidgetFields->setCellWidget(a, b - 1, checkBox);

                //Jump to the next column
                continue;
            }
        }
    }
}

void ModifyFieldDialog::on_tableWidgetFields_cellChanged(int row, int column)
{\
    //Checks if there is a row selected (because while adding items, this function is call and there is no row selected)
    if ( ui->tableWidgetFields->currentIndex().row() > -1 )
    {
        //Checks if the user delete a item
        if ( ui->tableWidgetFields->item(row, column)->text().isEmpty() )
        {
            QMessageBox::critical(this, tr("Error"), tr("The information can not be empty"));

            ui->tableWidgetFields->setItem(row, column, new QTableWidgetItem("Unknown"));
        }

        //we got the list of slipt it by '|'
        QStringList fieldsProperties = this->recordOperations.getFieldsInformation();

        for ( int i = 0; i < fieldsProperties.size(); i++ )
        {
            qDebug() << fieldsProperties.at(i).length();
        }


        //list slipt it by ','
        QStringList fieldProperties;
        //list of all the length of all the fields
       // QStringList index;
        //for each field that has been slipt by parts
        QStringList field;
        for ( int a = 0; a < fieldsProperties.size(); a++ )
        {
            fieldProperties = fieldsProperties.at(a).split(",");

            for ( int b = 0; b <fieldProperties.size(); b++ )
            {
               /* if ( b == 0 )
                {
                    index << fieldProperties.at(b);
                }
            */
                if ( a == row)
                {
                    field << fieldProperties.at(b);
                }
             }
         }

         RecordsFile file(this->fileName.toStdString());

         unsigned int length1 = 0;

         unsigned int length2 = 0;

         int tPositionRow = 0;

         int tPositionColumn = 0;

         //fixed
         for ( int i = 0; i < row; i++ )
         {
              tPositionRow +=  fieldsProperties.at(i).length() + 1;

            /* if ( index.at(i).toInt() > 9)
             {
                 tPositionRow +=  index.at(i).toInt() + 2;
             } else if ( index.at(i).toInt() < 9)
             {
                   tPositionRow +=  index.at(i).toInt() + 1;
             }*/
         }

            //fixed
         for ( int i = 0; i < column; i++ )
         {
             tPositionColumn +=  field.at(i).length()+1;
          }


         length1 = ( this->recordOperations.getLengthOfTheNumberOfFields() + 1 ) + (tPositionRow) + tPositionColumn;

         length2 = file.fileLength() - length1-1;

         qDebug() << " length1 " <<length1;

         qDebug() << " length2 " <<length2;

         char *buffer1 = new char[length1];

         char *buffer2 = new char[length2];

         file.read( buffer1 ,length1 );

         file.seek( length1 + field.at(column).length() );

         file.read( buffer2 , length2 );

         file.close();

         qDebug() << buffer1;

         qDebug() << buffer2;

         const char *path = this->fileName.toStdString().c_str();

         remove( path );


         RecordsFile create;

         //Check if there is a problem while creating the file

         if ( !create.open(this->fileName.toStdString(),  ios::out) )
         {
             QMessageBox::critical(this, tr("Error"), tr("An error occurred while trying to create the file"));
         }

         const char * changeField = ui->tableWidgetFields->item(row, column)->text().toStdString().c_str();

         create.write( buffer1, length1);

         create.write( changeField,  ui->tableWidgetFields->item(row, column)->text().length());

         create.write( buffer2 ,length2);

         create.close();

         delete [] buffer1;
         delete [] buffer2;

    }

}
