#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "classes/recordsfile.h"
#include "dialogs/createfielddialog.h"
#include "dialogs/modifyfielddialog.h"

#include <cstdio>
using std::remove;

#include <QFileDialog>
#include <QTableWidgetItem>
#include <QtPrintSupport/QPrinter>
#include <QPainter>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->popupMenu = new QMenu;

    this->actionInsertRow = new QAction(tr("Insert Row"), this);
    this->actionDeleteRecord = new QAction(tr("Delete Record"), this);

    this->popupMenu->addAction(this->actionInsertRow);
    this->popupMenu->addAction(this->actionDeleteRecord);

    connect(actionInsertRow, SIGNAL(triggered()),
            this, SLOT(insertRow()));

    connect(actionDeleteRecord, SIGNAL(triggered()),
            this, SLOT(deleteRecord()));

    this->labelFileName = new QLabel;
    this->statusBar()->addPermanentWidget(this->labelFileName);

    this->statusBar()->showMessage(tr("Welcome"), 2500);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNewFile_triggered()
{
    //Open a dialog and save the file path
    this->fileName = QFileDialog::getSaveFileName(this, tr("New File"), QDir::homePath(), tr("Text File (*.txt)"));

    //If the user select a directory and name file
    if ( !this->fileName.isEmpty() )
    {
        RecordsFile create;

        //Check if there is a problem while creating the file
        if ( !create.open(this->fileName.toStdString(), ios::out) )
        {
            QMessageBox::critical(this, tr("Error"), tr("An error occurred while trying to create the file"));
        }

        //Close
        create.close();

        this->labelFileName->setText(tr("File: ") + this->fileName);
        this->recordOperations.setFileName(this->fileName);
        this->clearTable();

        //Enable actions
        ui->actionSaveFile->setEnabled(true);
        ui->actionPrintFile->setEnabled(true);
        ui->actionCloseFile->setEnabled(true);
        ui->actionCreateField->setEnabled(true);
        ui->actionModifyField->setEnabled(true);
        ui->actionSearchRecord->setEnabled(true);
    }
}

void MainWindow::on_actionOpenFile_triggered()
{
    //Open a dialog and save the file path
    this->fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath(), tr("Text File (*.txt)"));

    //If the user select a valid file
    if ( !this->fileName.isEmpty() && this->validFile())
    {
        this->labelFileName->setText(tr("File: ") + this->fileName);
        this->recordOperations.setFileName(this->fileName);
        this->updateTable();

        //Enable actions
        ui->actionSaveFile->setEnabled(true);
        ui->actionPrintFile->setEnabled(true);
        ui->actionCloseFile->setEnabled(true);
        ui->actionCreateField->setEnabled(true);
        ui->actionModifyField->setEnabled(true);
        ui->actionSearchRecord->setEnabled(true);

        //Enable the table
        ui->tableWidgetRecords->setEnabled(true);
    }
}

void MainWindow::on_actionSaveFile_triggered()
{
}

void MainWindow::on_actionPrintFile_triggered()
{
    QString outputFileName = QFileDialog::getSaveFileName(this, tr("Save"), QDir::homePath(), tr("PDF (*.pdf)"));

    if ( !outputFileName.isEmpty() )
    {
        QPrinter printer;
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(outputFileName);

        QPainter painter;

        if ( !painter.begin(&printer) )
        {
            // failed to open file
            QMessageBox::critical(this, tr("Error"), tr("Failed to open file"));
        }

        else
        {
            /*int x = 0, y = 0;

            for ( int row = 0; row < ui->tableWidgetRecords->rowCount(); row++ )
            {
                for (int column = 1; column < ui->tableWidgetRecords->colorCount(); column++)
                {
                    painter.drawText(x, y, ui->tableWidgetRecords->item(row, column)->text());

                    x += 100;
                }

                y += 30;
            }

            painter.drawText(0, 0, "Test1");
            painter.drawText(100, 0, "Test2");
            painter.drawText(200, 0, "Test3");

            painter.drawText(0, 30, "Test4");
            painter.drawText(100, 30, "Test5");
            painter.drawText(200, 30, "Test6");*/

            painter.end();
        }
    }
}

void MainWindow::on_actionCloseFile_triggered()
{
}

void MainWindow::on_actionExit_triggered()
{
    exit(0);
}

void MainWindow::on_actionCreateField_triggered()
{
    CreateFieldDialog *dialog = new CreateFieldDialog(this->fileName);
    dialog->exec();
    delete dialog;

    //If there is a new field, update the table
    if ( recordOperations.getNumberOfFields() >  ( ui->tableWidgetRecords->columnCount() ) )
    {
        this->updateTable();
    }
}

void MainWindow::on_actionModifyField_triggered()
{
    ModifyFieldDialog *dialog = new ModifyFieldDialog(this->fileName);
    dialog->exec();
    delete dialog;

    this->updateTable();
}

void MainWindow::on_actionSearchRecord_triggered()
{
}

void MainWindow::on_actionCrossRecords_triggered()
{
}

void MainWindow::on_actionCreateSimpleIndex_triggered()
{
}

void MainWindow::on_actionCreateBTreeIndex_triggered()
{
}

void MainWindow::on_actionReindexing_triggered()
{
}

void MainWindow::on_actionImportXML_triggered()
{
}

void MainWindow::on_actionExportXML_triggered()
{
}

void MainWindow::on_actionImportJSON_triggered()
{
}

void MainWindow::on_actionExportJSON_triggered()
{
}

void MainWindow::on_actionAbout_triggered()
{
}

bool MainWindow::validFile()
{
    return true;
}

void MainWindow::clearTable()
{
    //Remove all rows
    ui->tableWidgetRecords->setRowCount(0);

    //Remove all columns
    ui->tableWidgetRecords->setColumnCount(0);
}

void MainWindow::updateTable()
{
    //If there is at least one field in the file (we show the information)
    if ( this->recordOperations.getNumberOfFields() > 0 )
    {
        //Clear the table in case that contains previos information
        this->clearTable();

        ui->tableWidgetRecords->setColumnCount(this->recordOperations.getNumberOfFields()); //Number of columns
        ui->tableWidgetRecords->setRowCount(this->recordOperations.getNumberOfRecords()); //Number of rows

        //---------------------------------------- Load fields ------------------------------------------------

        QStringList fieldsInformation = this->recordOperations.getFieldsInformation(); //All the fields
        QString fields = "";

        for ( int a = 0; a < fieldsInformation.size(); a++ )
        {
            QStringList fieldInformation = fieldsInformation.at(a).split(","); //A field information

            fields += fieldInformation.at(1) + ",";
        }

        //Remove the last comma
        fields.remove(fields.size() - 1, 1);

        //Columns name
        ui->tableWidgetRecords->setHorizontalHeaderLabels(fields.split(","));


        //---------------------------------------- Load records ------------------------------------------------

        //List of records
        QStringList recordsInformation = this->recordOperations.getRecordsInformation(); //All the records

        //Rows
        for ( int a = 0; a < recordsInformation.size(); a++ )
        {
            QStringList recordInformation = recordsInformation.at(a).split(","); //A record information

            //Columns
            for ( int b = 1; b < recordInformation.size(); b++ )
            {
                //Create a item
                QTableWidgetItem *item = new QTableWidgetItem(recordInformation.at(b));
                item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled); //item not editable

                //Insert a row
                ui->tableWidgetRecords->setItem(a, b - 1, item);
            }
        }
    }
}

void MainWindow::on_tableWidgetRecords_customContextMenuRequested(const QPoint &pos)
{
    //If there is at least one column(field) in the file (then the user can add a row)
    if ( ui->tableWidgetRecords->columnCount() > 0 )
    {
        //If the user select a row
        if ( ui->tableWidgetRecords->currentRow() >= -1 )
        {
            //Select the current row
            ui->tableWidgetRecords->selectRow(ui->tableWidgetRecords->currentRow());

            //Can delete a record
            this->actionDeleteRecord->setEnabled(true);
        }

        //There are no rows or the user does not select a row
        else
        {
            //Can not delete a record
            this->actionDeleteRecord->setEnabled(false);
        }

        QPoint globalPos = ui->tableWidgetRecords->mapToGlobal(pos);
        popupMenu->exec(globalPos); //Execute the popupMenu
    }
}

void MainWindow::insertRow()
{
    //Fix(rigth click menu) -> validate if the last row is not empty by comparing the indexFile(tree) with the number of rows

    //Insert a new row
    ui->tableWidgetRecords->insertRow(ui->tableWidgetRecords->rowCount());
}

void MainWindow::on_tableWidgetRecords_cellChanged(int row, int column)
{
    //Checks if there is a row selected (because while adding items, this function is call and there is no row selected)
    if ( ui->tableWidgetRecords->currentIndex().row() > -1 )
    {
        //item not editable
        //ui->tableWidgetRecords->itemAt(row, column)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

        //Checks if the column is the last column
        if ( column == ui->tableWidgetRecords->columnCount() - 1 )
        {
            if ( !this->insertRecord() )
            {
                //Remove the current row
                ui->tableWidgetRecords->removeRow(row);

                QMessageBox::critical(this, "Error", "An error occurred while trying to add the record");
            }

            else
            {
                QMessageBox::information(this, "Success", "The record has been added successfully");
            }
        }
    }
}

bool MainWindow::insertRecord()
{
    RecordsFile file;
    RecordsFile newFile;

    if ( !file.open(this->fileName.toStdString()) )
    {
        return false;
    }

    //------------------------------------------------------------------------------------------

    //Number of fields length + total length of field information + ' : '
    int firstPartSize = this->recordOperations.getInitialPositionOfRecordsInformation();
    char firstPartBuffer[firstPartSize];

    file.read(firstPartBuffer, firstPartSize);

    //------------------------------------New number of record------------------------------------

    QString newNumberOfRecords = "";
    newNumberOfRecords += QString::number(this->recordOperations.getNumberOfRecords() + 1);

    //-----------------------------------Current record--------------------------------------------

    int recordsInformationSize = this->recordOperations.getLengthOfRecordsInformation();
    char recordsInformationBuffer[recordsInformationSize];

    file.seek(firstPartSize + this->recordOperations.getLengthOfTheNumberOfRecords());
    file.read(recordsInformationBuffer, recordsInformationSize);

    file.close();

    //------------------------------------------New Record------------------------------------------

    QString newRecord = "";

    for ( int a = 0; a < ui->tableWidgetRecords->columnCount(); a++ )
    {
        newRecord += ui->tableWidgetRecords->item(ui->tableWidgetRecords->rowCount() - 1, a)->text() + ",";
    }

    newRecord.remove(newRecord.size() - 1, 1); //Remove last ' , '
    newRecord += "|"; //Add the last ' | '

    newRecord.prepend(QString::number(newRecord.size() + 1) + ",");

    //If this is the first record
    if ( this->recordOperations.getNumberOfRecords() == 0 )
    {
        newRecord.prepend("|");
    }

    //----------------------------------------Add Record-------------------------------------------


    if ( remove(this->fileName.toStdString().c_str()) != 0 )
    {
        return false;
    }

    if ( !newFile.open(this->fileName.toStdString(), ios::out) )
    {
        return false;
    }

    newFile.write(firstPartBuffer, firstPartSize);
    newFile.write(newNumberOfRecords.toStdString().c_str(), newNumberOfRecords.size());
    newFile.write(recordsInformationBuffer, recordsInformationSize);newFile.write(newRecord.toStdString().c_str(), newRecord.size());

    newFile.close();

    this->updateTable();

    return true;

}

void MainWindow::deleteRecord()
{
    //index of the record im trying to delete
    int index = ui->tableWidgetRecords->currentRow();

    // opening the record's file
    RecordsFile file ( this->fileName.toStdString() );

    //we open the record's clase center control
    RecordOperations a;
        a.setFileName ( this->fileName );

    // counting the length of the records we dont need to delete
    int positonIndex = 0;
    for (int i = 0; i < index; i++)
    {
        positonIndex += a.getRecordsInformation()[i].length() + 1;
    }

     //we get the length of the position we need to be
     int length1 = a.getInitialPositionOfRecordsInformation() + a.getLengthOfTheNumberOfRecords() + 1 + positonIndex;
     int length2 = 0;
     if( a.getNumberOfRecords() == index + 1)
     {
         length2 = 1;
     }else
     {
        length2 = a.getLengthOfRecordsInformation() - ( a.getRecordsInformation()[index].length() +  a.getLengthOfTheNumberOfRecords() + 1 );
     }
     //buffers
     char * buffer1 = new char[length1];
     char * buffer2 = new char[length2];

     //we read the file
     file.read ( buffer1 ,length1 );
     file.seek ( length1 + a.getRecordsInformation()[index].length() );
     file.read ( buffer2 , length2 );
     file.close();

     //delete the old file
     const char *path = this->fileName.toStdString().c_str();

     //we get the first position of the actul record
     QStringList recordList =   a.getRecordsInformation().at(index).split(',');

     //the length of the empty record
     int length3 = recordList.at(0).toInt()-2;

     //buffer is empty
     char * buffer3 = new char[length3];


     for ( int i = 0; i < length3; i++)
     {
         buffer3[i] = '-';
     }
     remove( path );

     //creating the new file
     RecordsFile create;
     //Check if there is a problem while creating the file
     if ( !create.open(this->fileName.toStdString(),  ios::out) )
     {
         QMessageBox::critical(this, tr("Error"), tr("An error occurred while trying to create the file"));
    }

     const char* lenghtOfrecord = recordList.at(0).toStdString().c_str();

     //writing new information on the new file
     create.write ( buffer1, length1);

     create.write ( lenghtOfrecord, 2);
     create.write ( ",", 1);
     create.write ( buffer3, length3);
     create.write ( buffer2, length2) ;
     create.close();

     //delete the buffer
     delete [] buffer1;
     delete [] buffer2;
     delete [] buffer3;

     //updating the table
     updateTable();
}
