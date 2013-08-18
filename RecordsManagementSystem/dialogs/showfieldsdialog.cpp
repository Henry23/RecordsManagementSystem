#include "showfieldsdialog.h"
#include "ui_showfieldsdialog.h"

#include "classes/recordsfile.h"

#include <QTableWidgetItem>
#include <QMessageBox>

#include <qDebug>

ShowFieldsDialog::ShowFieldsDialog(QString fileName, bool modifyMode, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowFieldsDialog)
{
    ui->setupUi(this);

    this->fileName = fileName;
    this->modifyMode = modifyMode;

    this->recordOperations.setFileName(this->fileName);

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
    ui->tableWidgetFields->setRowCount(this->recordOperations.getNumberOfFields()); //Number of rows

    ui->tableWidgetFields->hideColumn(0);

    //Columns name
    ui->tableWidgetFields->setHorizontalHeaderLabels(QString("Hide;Name;Type;Length;Decimal;Key").split(";"));
}

void ShowFieldsDialog::showFields()
{
    QStringList fieldsProperties = this->recordOperations.getFieldsProperties();

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

        //we got the list of slipt it by '|'
        QStringList fieldsProperties = this->getFieldsProperties();

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


         length1 = ( this->getLengthOfTheNumberOfFields() + 1 ) + (tPositionRow) + tPositionColumn;

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
