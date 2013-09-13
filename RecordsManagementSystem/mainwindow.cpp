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

    this->init();
}

MainWindow::~MainWindow()
{
    //Save and exit
    this->on_actionExit_triggered();

    delete ui;
}

void MainWindow::init()
{
    //Create a menu
    this->popupMenu = new QMenu;

    //Create actions
    this->actionInsertRow = new QAction(tr("Insert Row"), this);
    this->actionDeleteRecord = new QAction(tr("Delete Record"), this);

    //Add action to the menu
    this->popupMenu->addAction(this->actionInsertRow);
    this->popupMenu->addAction(this->actionDeleteRecord);

    //Connections
    connect(actionInsertRow, SIGNAL(triggered()),
            this, SLOT(insertRow()));

    connect(actionDeleteRecord, SIGNAL(triggered()),
            this, SLOT(deleteRecord()));

    //Create a label
    this->labelRecordFileName = new QLabel;
    this->statusBar()->addPermanentWidget(this->labelRecordFileName); //Add the label to the status bar

    ui->tableWidgetSearch->setRowCount(1);

    //Show a message in the status bar
    this->statusBar()->showMessage(tr("Welcome"), 2500);
}

void MainWindow::on_actionNewFile_triggered()
{
    //Open a dialog and save the file path
    this->recordFileName = QFileDialog::getSaveFileName(this, tr("New File"), QDir::homePath(), tr("Text File (*.txt)"));

    //If the user select a directory and name file
    if ( !this->recordFileName.isEmpty() )
    {
        RecordsFile createRecordFile;
        RecordsFile createIndexFile;

        //index file name
        indexFileName = this->recordFileName; //same filename
        indexFileName.remove(this->recordFileName.length() - 4, 4); //remove last 4 characters(.txt)
        indexFileName += "Index.txt"; //Append

        //Check if there is a problem while creating the files
        if ( !createRecordFile.open(this->recordFileName.toStdString(), ios::out) ||
             !createIndexFile.open(indexFileName.toStdString(), ios::out) )
        {
            QMessageBox::critical(this, tr("Error"), tr("An error occurred while trying to create the file"));
        }

        //Close
        createRecordFile.close();
        createIndexFile.close();

        this->labelRecordFileName->setText(tr("File: ") + this->recordFileName);
        this->recordOperations.setFileName(this->recordFileName);

        //Enable actions
        ui->actionSaveFile->setEnabled(true);
        ui->actionPrintFile->setEnabled(true);
        ui->actionCloseFile->setEnabled(true);
        ui->actionCreateField->setEnabled(true);
        ui->actionModifyField->setEnabled(true);
        ui->actionSearchRecord->setEnabled(true);
        ui->actionCrossRecords->setEnabled(true);
        ui->actionCreateSimpleIndex->setEnabled(true);
        ui->actionCreateBTreeIndex->setEnabled(true);
        ui->actionReindexing->setEnabled(true);
        ui->actionImportXML->setEnabled(true);
        ui->actionExportXML->setEnabled(true);
        ui->actionImportJSON->setEnabled(true);
        ui->actionExportJSON->setEnabled(true);

        //Disable actions
        ui->actionOpenFile->setEnabled(false);
        ui->actionNewFile->setEnabled(false);

        //Enable the table
        ui->tableWidgetRecords->setEnabled(true);

        //Enable search widgets
        ui->lineEditKey->setEnabled(true);
        ui->comboBoxIndex->setEnabled(true);
        ui->pushButtonSearch->setEnabled(true);
        ui->tableWidgetSearch->setEnabled(true);
    }
}

void MainWindow::on_actionOpenFile_triggered()
{
    //Open a dialog and save the file path
    this->recordFileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath(), tr("Text File (*.txt)"));

    //If the user select a valid file
    if ( !this->recordFileName.isEmpty() && this->validFile())
    {
        //index file name
        indexFileName = this->recordFileName; //same filename
        indexFileName.remove(this->recordFileName.length() - 4, 4); //remove last 4 characters(.txt)
        indexFileName += "Index.txt"; //Append

        this->labelRecordFileName->setText(tr("File: ") + this->recordFileName);
        this->recordOperations.setFileName(this->recordFileName);

        this->loadIndexFile();
        this->showFields();
        this->showRecords();

        //Enable actions
        ui->actionSaveFile->setEnabled(true);
        ui->actionPrintFile->setEnabled(true);
        ui->actionCloseFile->setEnabled(true);
        ui->actionCreateField->setEnabled(true);
        ui->actionModifyField->setEnabled(true);
        ui->actionSearchRecord->setEnabled(true);
        ui->actionCrossRecords->setEnabled(true);
        ui->actionCreateSimpleIndex->setEnabled(true);
        ui->actionCreateBTreeIndex->setEnabled(true);
        ui->actionReindexing->setEnabled(true);
        ui->actionImportXML->setEnabled(true);
        ui->actionExportXML->setEnabled(true);
        ui->actionImportJSON->setEnabled(true);
        ui->actionExportJSON->setEnabled(true);

        //Disable actions
        ui->actionOpenFile->setEnabled(false);
        ui->actionNewFile->setEnabled(false);

        //Enable the table
        ui->tableWidgetRecords->setEnabled(true);

        //Enable search widgets
        ui->lineEditKey->setEnabled(true);
        ui->comboBoxIndex->setEnabled(true);
        ui->pushButtonSearch->setEnabled(true);
        ui->tableWidgetSearch->setEnabled(true);
    }
}

void MainWindow::on_actionSaveFile_triggered()
{
    //If the record file could compact
    if ( this->compact() )
    {
        //Save the list of indexes in the index file
        if ( !this->saveIndexList() )
        {
            QMessageBox::critical(this, tr("Error"), tr("A problem occurred when trying to save the index file"));
        }

        this->statusBar()->showMessage(tr("Saved!"), 2500);
    }

    else
    {
        QMessageBox::critical(this, tr("Error"), tr("A problem occurred when trying to compact the file"));
    }
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
    //save
    this->on_actionSaveFile_triggered();

    //Disable actions
    ui->actionSaveFile->setEnabled(false);
    ui->actionPrintFile->setEnabled(false);
    ui->actionCloseFile->setEnabled(false);
    ui->actionCreateField->setEnabled(false);
    ui->actionModifyField->setEnabled(false);
    ui->actionSearchRecord->setEnabled(false);
    ui->actionCrossRecords->setEnabled(false);
    ui->actionCreateSimpleIndex->setEnabled(false);
    ui->actionCreateBTreeIndex->setEnabled(false);
    ui->actionReindexing->setEnabled(false);
    ui->actionImportXML->setEnabled(false);
    ui->actionExportXML->setEnabled(false);
    ui->actionImportJSON->setEnabled(false);
    ui->actionExportJSON->setEnabled(false);

    //Disable the table
    ui->tableWidgetRecords->setEnabled(false);

    //Clear table
    ui->tableWidgetRecords->clear();
    ui->tableWidgetSearch->clear();
    ui->tableWidgetRecords->setColumnCount(0);
    ui->tableWidgetSearch->setColumnCount(0);
    ui->tableWidgetRecords->setRowCount(0);

    //Disable search widgets
    ui->lineEditKey->setEnabled(false);
    ui->comboBoxIndex->setEnabled(false);
    ui->pushButtonSearch->setEnabled(false);
    ui->tableWidgetSearch->setEnabled(false);

    ui->lineEditKey->clear();

    //Clear paths
    this->recordFileName.clear();
    this->indexFileName.clear();
    this->labelRecordFileName->clear();

    //Clear structures
    this->indexList.clear();
    this->availList.clear();

    //Enable actions
    ui->actionOpenFile->setEnabled(true);
    ui->actionNewFile->setEnabled(true);
}

void MainWindow::on_actionExit_triggered()
{
    //Checks if the user create/open a record file
    if ( !this->recordFileName.isEmpty() )
    {
        //Save
        this->on_actionSaveFile_triggered();
    }

    exit(0);
}

void MainWindow::on_actionCreateField_triggered()
{
    CreateFieldDialog *dialog = new CreateFieldDialog(this->recordFileName);
    dialog->exec();
    delete dialog;

    //If there is a new field, update the columns(fields)
    if ( recordOperations.getNumberOfFields() > ui->tableWidgetRecords->columnCount() )
    {
        //Load and show the fields
        this->showFields();
    }
}

void MainWindow::on_actionModifyField_triggered()
{
    ModifyFieldDialog *dialog = new ModifyFieldDialog(this->recordFileName);
    dialog->exec();
    delete dialog;

    //Load and show the fields
    this->showFields();
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

void MainWindow::loadIndexFile(int structure)
{
    QFile file(this->indexFileName);

    if ( !file.open(QIODevice::ReadOnly | QIODevice::Text) )
        return;

    QTextStream in(&file);

    //Read all lines
    while ( !in.atEnd() )
    {
        QString line = in.readLine(); //Read a line

        //If the structure is a QList
        if ( structure == 0 )
        {
            this->indexList.append(line);
        }

        else
        {
            //B-Tree
        }
    }
}

void MainWindow::showFields()
{
    //If there is at least one field in the file (we show the information)
    if ( this->recordOperations.getNumberOfFields() > 0 )
    {
        //Number of columns
        ui->tableWidgetRecords->setColumnCount(this->recordOperations.getNumberOfFields());
        ui->tableWidgetSearch->setColumnCount(ui->tableWidgetRecords->columnCount());

        QStringList fieldsInformation = this->recordOperations.getFieldsInformation(); //All the fields
        QString fields = "";

        for ( int a = 0; a < fieldsInformation.size(); a++ )
        {
            //A field information
            QStringList fieldInformation = fieldsInformation.at(a).split(",");

            fields += fieldInformation.at(1) + ",";
        }

        //Remove the last comma
        fields.remove(fields.size() - 1, 1);

        //Columns name
        ui->tableWidgetRecords->setHorizontalHeaderLabels(fields.split(","));
        ui->tableWidgetSearch->setHorizontalHeaderLabels(fields.split(","));
    }
}

void MainWindow::showRecords()
{
    //If there are records
    if ( !this->indexList.isEmpty() )
    {
        //Number of rows
        ui->tableWidgetRecords->setRowCount(this->indexList.size());

        //Open the record file
        QFile file(this->recordFileName);

        if ( !file.open(QIODevice::ReadOnly | QIODevice::Text) )
        {
            return;
        }

        for ( int row = 0; row < indexList.size(); row++ )
        {
            //set the initial position of the record
            file.seek(indexList.at(row).split(",").at(1).toInt());

            //buffer
            char buffer[indexList.at(row).split(",").at(2).toInt()];

            //Read
            file.read(buffer, indexList.at(row).split(",").at(2).toInt());

            QString record = buffer;

            //Separate the record
            QStringList columnsList = record.split(",");

            //All the columns
            for (int column = 0; column < columnsList.size(); column++)
            {
                //Create a item
                QTableWidgetItem *item = new QTableWidgetItem(columnsList.at(column));
                item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled); //item not editable

                //Insert a row
                ui->tableWidgetRecords->setItem(row, column, item);
            }
        }

        file.close();
    }
}

void MainWindow::on_tableWidgetRecords_customContextMenuRequested(const QPoint &pos)
{
    //If there is at least one column(field) in the file (then the user can add a row)
    if ( ui->tableWidgetRecords->columnCount() > 0 )
    {
        //If the user select a row
        if ( ui->tableWidgetRecords->currentRow() > -1 )
        {
            //Select the current row
            ui->tableWidgetRecords->selectRow(ui->tableWidgetRecords->currentRow());

            //Can delete a record
            this->actionDeleteRecord->setEnabled(true);

            //Can add a row
            this->actionInsertRow->setEnabled(true);
        }

        //There are no rows or the user does not select a row
        else
        {
            //Can not delete a record
            this->actionDeleteRecord->setEnabled(false);
        }

        //If the number of records in the file is not equals to the number of rows, the user has not save the last row
        if ( this->recordOperations.getNumberOfRecords() != ui->tableWidgetRecords->rowCount() )
        {
            //Can not insert a row
            this->actionInsertRow->setEnabled(false);

            //Can not delete a record
            this->actionDeleteRecord->setEnabled(false);
        }

        QPoint globalPos = ui->tableWidgetRecords->mapToGlobal(pos);
        popupMenu->exec(globalPos); //Execute the popupMenu
    }
}

void MainWindow::insertRow()
{
    //Insert a new row
    ui->tableWidgetRecords->insertRow(ui->tableWidgetRecords->rowCount());

    //Select the new row
    ui->tableWidgetRecords->selectRow(ui->tableWidgetRecords->rowCount() - 1);
}

void MainWindow::on_tableWidgetRecords_cellChanged(int row, int column)
{
    //Checks if there is a row selected (because while adding items, this function is call and there is no row selected)
    if ( ui->tableWidgetRecords->currentIndex().row() > -1 )
    {
        //item not editable
        //ui->tableWidgetRecords->itemAt(row, column)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

        //Checks if the column is the last one
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
                this->statusBar()->showMessage(tr("The record has been added successfully"), 2500);
            }
        }
    }
}

bool MainWindow::insertRecord()
{
    RecordsFile file;
    RecordsFile newFile;

    if ( !file.open(this->recordFileName.toStdString()) )
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
    int indexLength = 0;
    int recordLength = 0;

    for ( int a = 0; a < ui->tableWidgetRecords->columnCount(); a++ )
    {
        newRecord += ui->tableWidgetRecords->item(ui->tableWidgetRecords->rowCount() - 1, a)->text() + ",";
    }

    newRecord.remove(newRecord.size() - 1, 1); //Remove last ' , '

    //Record length for the index
    indexLength = newRecord.length();

    newRecord += "|"; //Add the last ' | '

    recordLength = newRecord.size() + 1;

    newRecord.prepend(QString::number(recordLength) + ",");

    //If this is the first record
    if ( this->recordOperations.getNumberOfRecords() == 0 )
    {
        newRecord.prepend("|");
    }

    //----------------------------------------Add Record-------------------------------------------


    if ( remove(this->recordFileName.toStdString().c_str()) != 0 )
    {
        return false;
    }

    if ( !newFile.open(this->recordFileName.toStdString(), ios::out) )
    {
        return false;
    }

    newFile.write(firstPartBuffer, firstPartSize);
    newFile.write(newNumberOfRecords.toStdString().c_str(), newNumberOfRecords.size());
    newFile.write(recordsInformationBuffer, recordsInformationSize);newFile.write(newRecord.toStdString().c_str(), newRecord.size());

    newFile.close();

    int currentRow = ui->tableWidgetRecords->rowCount() - 1;
    QString temp1 = ui->tableWidgetRecords->item(currentRow, 0)->text() + ",";
    QString temp2 =  QString::number(this->recordOperations.getRecordPositionAt(currentRow) + QString::number(recordLength).length() + 1) + ",";
    QString temp3 = QString::number(indexLength);

    QString index = temp1 + temp2 + temp3;

    this->indexList.append(index);

    return true;

}

bool MainWindow::indexListSearch(QString key)
{
    for (int row = 0; row < this->indexList.size(); row++)
    {
        if ( this->indexList.at(row).split(",").at(0) == key )
        {
            QFile file(this->recordFileName);

            if ( !file.open(QIODevice::ReadOnly | QIODevice::Text) )
            {
                return false;
            }

            //set the initial position of the record
            file.seek(indexList.at(row).split(",").at(1).toInt());

            //buffer
            char buffer[indexList.at(row).split(",").at(2).toInt()];

            //Read
            file.read(buffer, indexList.at(row).split(",").at(2).toInt());

            QString record = buffer;

            //Separate the record
            QStringList columnsList = record.split(",");

            //All the columns
            for (int column = 0; column < columnsList.size(); column++)
            {
                //Create a item
                QTableWidgetItem *item = new QTableWidgetItem(columnsList.at(column));
                item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled); //item not editable

                //Insert a row
                ui->tableWidgetSearch->setItem(0, column, item);
            }

            //Close
            file.close();

            return true;
        }
    }

    return false;
}

bool MainWindow::bTreeSearch(QString key)
{
    return false;
}

bool MainWindow::compact()
{
    return true;
}

bool MainWindow::saveIndexList()
{
    QFile currentIndexFile(this->indexFileName);

    //Delete the index file
    if ( !currentIndexFile.remove() )
    {
        return false;
    }

    QFile newIndexFile(this->indexFileName);

    //Open the new index file
    if ( !newIndexFile.open(QIODevice::WriteOnly | QIODevice::Text) )
        return false;

    QTextStream out(&newIndexFile);

    //All the indexes
    for ( int a = 0; a < this->indexList.size(); a++ )
    {
        //the last index of the list
        if ( a == this->indexList.size() - 1 )
        {
            //Write without adding a "new line"
            out << indexList.at(a);
            break;
        }

        //write and add a new "line"
        out << indexList.at(a) << '\n';
    }

    //close the file
    newIndexFile.close();

    return true;
}

void MainWindow::deleteRecord()
{
    //index of the record im trying to delete
    int index = ui->tableWidgetRecords->currentRow();

    // opening the record's file
    RecordsFile file ( this->recordFileName.toStdString() );

    //we open the record's clase center control
    RecordOperations a;
        a.setFileName ( this->recordFileName );

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
     const char *path = this->recordFileName.toStdString().c_str();

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
     if ( !create.open(this->recordFileName.toStdString(),  ios::out) )
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

     //using availList
     availList.insert(length1,recordList.at(0).toInt());

     //updating the table
     ui->tableWidgetRecords->removeRow(index);
}

void MainWindow::on_pushButtonSearch_clicked()
{
    //Checks if there are records
    if ( !this->indexList.isEmpty() )
    {
        if ( ui->comboBoxIndex->currentIndex() == 0 )
        {
            if ( !this->indexListSearch(ui->lineEditKey->text()) )
            {
                QMessageBox::critical(this, tr("Error"), tr("Record not found"));
            }
        }

        else
        {
            if ( !this->bTreeSearch(ui->lineEditKey->text()) )
            {
                QMessageBox::critical(this, tr("Error"), tr("Record not found"));
            }
        }
    }
}
