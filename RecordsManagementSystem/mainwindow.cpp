#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "classes/recordsfile.h"
#include "dialogs/createfielddialog.h"
#include "dialogs/modifyfielddialog.h"
#include "classes/btree.h"

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
        ui->actionCloseFile->setEnabled(true);
        ui->actionCreateField->setEnabled(true);
        ui->actionModifyField->setEnabled(true);

        //Disable actions
        ui->actionOpenFile->setEnabled(false);
        ui->actionNewFile->setEnabled(false);

        //Enable the table
        ui->tableWidgetRecords->setEnabled(true);

        //Enable search widgets
        ui->lineEditKey->setEnabled(true);
        ui->comboBoxIndex->setEnabled(true);
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

        //Checks if the record file does not have a index file
        if ( !this->hasIndexFile() )
        {
            QMessageBox::warning(this, tr("Index File"), tr("The index file does not exist, you have to create one first"));

            //Enable the option
            ui->actionCreateSimpleIndex->setEnabled(true);
        }

        //The index file exist
        else
        {
            this->loadIndexFile();

            //Checks if the index file is corrupted
            if ( this->isIndexFileCorrupted() )
            {
                QMessageBox::warning(this, tr("Index File"), tr("The index file is corrupted, you have to reindex the file first"));

                //Enable the option
                ui->actionReindexing->setEnabled(true);
            }

            else
            {
                this->showFields();
                this->showRecords();

                //-----------------------------------Enable / Disable----------------------------------------

                //If there are no records
                if ( this->indexList.isEmpty() )
                {
                    //Enable actions
                    ui->actionCloseFile->setEnabled(true);
                    ui->actionCreateField->setEnabled(true);
                    ui->actionModifyField->setEnabled(true);

                    //Disable actions
                    ui->actionOpenFile->setEnabled(false);
                    ui->actionNewFile->setEnabled(false);

                    //Enable the table
                    ui->tableWidgetRecords->setEnabled(true);

                    //Enable search widgets
                    ui->lineEditKey->setEnabled(true);
                    ui->comboBoxIndex->setEnabled(true);
                    ui->tableWidgetSearch->setEnabled(true);
                }

                else
                {
                    //Enable actions
                    ui->actionPrintFile->setEnabled(true);
                    ui->actionCloseFile->setEnabled(true);
                    ui->actionCrossRecords->setEnabled(true);
                    ui->actionCreateBTreeIndex->setEnabled(true);
                    ui->actionImportXML->setEnabled(true);
                    ui->actionExportXML->setEnabled(true);
                    ui->actionImportJSON->setEnabled(true);
                    ui->actionExportJSON->setEnabled(true);

                    //Disable actions
                    ui->actionOpenFile->setEnabled(false);
                    ui->actionNewFile->setEnabled(false);
                    ui->actionCreateField->setEnabled(false);
                    ui->actionModifyField->setEnabled(false);

                    //Enable the table
                    ui->tableWidgetRecords->setEnabled(true);

                    //Enable search widgets
                    ui->lineEditKey->setEnabled(true);
                    ui->comboBoxIndex->setEnabled(true);
                    ui->pushButtonSearch->setEnabled(true);
                    ui->tableWidgetSearch->setEnabled(true);
                }
            }
        }
    }
}

void MainWindow::on_actionSaveFile_triggered()
{
    //If the record file could compact
    if ( this->compact() )
    {
        //If a compaction is made
        if ( !this->availList.isEmpty() )
        {
            //Delete the index file
            QFile currentIndexFile(this->indexFileName);
            currentIndexFile.remove();

            this->indexList.clear();

            ui->tableWidgetRecords->setColumnCount(0);
            ui->tableWidgetRecords->setRowCount(0);

            //Create a index file
            this->on_actionCreateSimpleIndex_triggered();
        }

        //Just save the index list
        else
        {
            //Save the list of indexes in the index file
            if ( !this->saveIndexList() )
            {
                QMessageBox::critical(this, tr("Error"), tr("A problem occurred when trying to save the index file"));
            }
        }

        this->availList.clear();

        this->statusBar()->showMessage(tr("Saved!"), 2500);

        //Disable the "save" option
        ui->actionSaveFile->setEnabled(false);
    }

    else
    {
        QMessageBox::critical(this, tr("Error"), tr("A problem occurred when trying to compact the file"));
    }
}

void MainWindow::on_actionPrintFile_triggered()
{
    //---------------------
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

    //delete this->btree;
    //this->btree = nullptr;

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

void MainWindow::on_actionCrossRecords_triggered()
{
}

void MainWindow::on_actionCreateSimpleIndex_triggered()
{
    QString keyForIndexFile = "";
    int positionForIndexFile = 0;
    int lengthForIndexFile = 0;

    int numberOfRecords = this->recordOperations.getNumberOfRecords();
    QString LengthOfNumberOfRecords = QString::number(numberOfRecords);

    if ( LengthOfNumberOfRecords.length() == 1 )
    {
        LengthOfNumberOfRecords.prepend("00000");
    }

    else if ( LengthOfNumberOfRecords.length() == 2 )
    {
        LengthOfNumberOfRecords.prepend("0000");
    }

    else if ( LengthOfNumberOfRecords.length() == 3 )
    {
        LengthOfNumberOfRecords.prepend("000");
    }

    else if ( LengthOfNumberOfRecords.length() == 4 )
    {
        LengthOfNumberOfRecords.prepend("00");
    }

    else if ( LengthOfNumberOfRecords.length() == 5 )
    {
        LengthOfNumberOfRecords.prepend("0");
    }



    int position = recordOperations.getInitialPositionOfRecordsInformation() + LengthOfNumberOfRecords.length() + 1;

    for (int a = 0; a < numberOfRecords; a++)
    {
        int recordSize = recordOperations.getSizeOfARecordInformation(position);
        int lengthOfRecordSize = recordOperations.getLengthOfTheSizeOfARecordInformation(position);

        positionForIndexFile = position + lengthOfRecordSize + 1;
        lengthForIndexFile = recordSize - 2;


        //------------------------------------Search key------------------------------------------

        //All the fields information
        QStringList fieldsInformation = this->recordOperations.getFieldsInformation();

        int field = 0;

        //Search which field is a key
        for (; field < fieldsInformation.size(); field++)
        {
            QStringList fieldInformation = fieldsInformation.at(field).split(",");

            //If the field is key
            if ( fieldInformation.at(fieldInformation.size() - 1) == "1" )
            {
                break;
            }
        }

        //------------------------------------------------------------------------------------------

        RecordsFile file(this->recordFileName.toStdString());
        file.seek(position);

        char recordBuffer[recordSize + lengthOfRecordSize - 1];
        file.read(recordBuffer, recordSize + lengthOfRecordSize - 1);

        file.close();

        QString record = recordBuffer;

        keyForIndexFile = record.split(",").at(field + 1);

        //-------------------------------------------------------------------------------------------

        //Move to the next record
        position += recordSize + lengthOfRecordSize;


        //Save the index
        QString index = keyForIndexFile + "," + QString::number(positionForIndexFile) + "," + QString::number(lengthForIndexFile);
        this->indexList.append(index);
    }

    RecordsFile createIndexFile;

    //Check if there is a problem while creating the index file
    if ( !createIndexFile.open(indexFileName.toStdString(), ios::out) )
    {
        QMessageBox::critical(this, tr("Error"), tr("An error occurred while trying to create the index file"));
    }

    createIndexFile.close();


    //Save the list of indexes in the index file
    if ( !this->saveIndexList() )
    {
        QMessageBox::critical(this, tr("Error"), tr("A problem occurred when trying to save the index file"));
    }

    else
    {
        ui->statusBar->showMessage(tr("The index file was created successfully!"), 2500);

        this->showFields();
        this->showRecords();

        //-----------------------------------Enable / Disable----------------------------------------

        //If there are no records
        if ( this->indexList.isEmpty() )
        {
            //Enable actions
            ui->actionCloseFile->setEnabled(true);
            ui->actionCreateField->setEnabled(true);
            ui->actionModifyField->setEnabled(true);

            //Disable actions
            ui->actionOpenFile->setEnabled(false);
            ui->actionNewFile->setEnabled(false);

            //Enable the table
            ui->tableWidgetRecords->setEnabled(true);

            //Enable search widgets
            ui->lineEditKey->setEnabled(true);
            ui->comboBoxIndex->setEnabled(true);
            ui->tableWidgetSearch->setEnabled(true);
        }

        else
        {
            //Enable actions
            ui->actionPrintFile->setEnabled(true);
            ui->actionCloseFile->setEnabled(true);
            ui->actionCrossRecords->setEnabled(true);
            ui->actionCreateBTreeIndex->setEnabled(true);
            ui->actionImportXML->setEnabled(true);
            ui->actionExportXML->setEnabled(true);
            ui->actionImportJSON->setEnabled(true);
            ui->actionExportJSON->setEnabled(true);

            //Disable actions
            ui->actionOpenFile->setEnabled(false);
            ui->actionNewFile->setEnabled(false);
            ui->actionCreateField->setEnabled(false);
            ui->actionModifyField->setEnabled(false);

            //Enable the table
            ui->tableWidgetRecords->setEnabled(true);

            //Enable search widgets
            ui->lineEditKey->setEnabled(true);
            ui->comboBoxIndex->setEnabled(true);
            ui->pushButtonSearch->setEnabled(true);
            ui->tableWidgetSearch->setEnabled(true);
        }
    }
}

void MainWindow::on_actionCreateBTreeIndex_triggered()
{
    if( !indexList.isEmpty() )
    {
        for ( int i = 0 ; i < indexList.size(); i++)
        {
            //slipting and getting all the posicion
            QStringList a = indexList.at(i).split(',');

            //inserting the posicion on the btree
            btree->insert( a[0].toInt(), a[1].toInt(), a[2].toInt() );

        }

       QMessageBox::information(this, tr("Information"), tr("B-Tree Index has been created"));

       //btree->traverse();

    }else
    {
       QMessageBox::critical(this, tr("Error"), tr("There is no Record's file open"));
    }
}

void MainWindow::on_actionReindexing_triggered()
{
    int missingRecords = recordOperations.getNumberOfRecords() - this->indexList.size();

    QString keyForIndexFile = "";
    int positionForIndexFile = 0;
    int lengthForIndexFile = 0;

    int position = this->indexList.at(indexList.size() - 1).split(",").at(1).toInt() +
                   this->indexList.at(indexList.size() - 1).split(",").at(2).toInt() + 1;

    for (int a = 0; a < missingRecords; a++)
    {
        int recordSize = recordOperations.getSizeOfARecordInformation(position);
        int lengthOfRecordSize = recordOperations.getLengthOfTheSizeOfARecordInformation(position);

        positionForIndexFile = position + lengthOfRecordSize + 1;
        lengthForIndexFile = recordSize - 2;


        //------------------------------------Search key------------------------------------------

        //All the fields information
        QStringList fieldsInformation = this->recordOperations.getFieldsInformation();

        int field = 0;

        //Search which field is a key
        for (; field < fieldsInformation.size(); field++)
        {
            QStringList fieldInformation = fieldsInformation.at(field).split(",");

            //If the field is key
            if ( fieldInformation.at(fieldInformation.size() - 1) == "1" )
            {
                break;
            }
        }

        //------------------------------------------------------------------------------------------

        RecordsFile file(this->recordFileName.toStdString());
        file.seek(position);

        char recordBuffer[recordSize + lengthOfRecordSize - 1];
        file.read(recordBuffer, recordSize + lengthOfRecordSize - 1);

        file.close();

        QString record = recordBuffer;

        keyForIndexFile = record.split(",").at(field + 1);

        //-------------------------------------------------------------------------------------------

        //Move to the next record
        position += recordSize + lengthOfRecordSize;


        //Save the index
        QString index = keyForIndexFile + "," + QString::number(positionForIndexFile) + "," + QString::number(lengthForIndexFile);
        this->indexList.append(index);
    }

    //Save the list of indexes in the index file
    if ( !this->saveIndexList() )
    {
        QMessageBox::critical(this, tr("Error"), tr("A problem occurred when trying to save the index file"));
    }

    else
    {
        ui->statusBar->showMessage(tr("The reindexing was performed successfully!"), 2500);

        this->showFields();
        this->showRecords();

        //-----------------------------------Enable / Disable----------------------------------------

        //If there are no records
        if ( this->indexList.isEmpty() )
        {
            //Enable actions
            ui->actionCloseFile->setEnabled(true);
            ui->actionCreateField->setEnabled(true);
            ui->actionModifyField->setEnabled(true);

            //Disable actions
            ui->actionOpenFile->setEnabled(false);
            ui->actionNewFile->setEnabled(false);

            //Enable the table
            ui->tableWidgetRecords->setEnabled(true);

            //Enable search widgets
            ui->lineEditKey->setEnabled(true);
            ui->comboBoxIndex->setEnabled(true);
            ui->tableWidgetSearch->setEnabled(true);
        }

        else
        {
            //Enable actions
            ui->actionPrintFile->setEnabled(true);
            ui->actionCloseFile->setEnabled(true);
            ui->actionCrossRecords->setEnabled(true);
            ui->actionCreateBTreeIndex->setEnabled(true);
            ui->actionImportXML->setEnabled(true);
            ui->actionExportXML->setEnabled(true);
            ui->actionImportJSON->setEnabled(true);
            ui->actionExportJSON->setEnabled(true);

            //Disable actions
            ui->actionOpenFile->setEnabled(false);
            ui->actionNewFile->setEnabled(false);
            ui->actionCreateField->setEnabled(false);
            ui->actionModifyField->setEnabled(false);

            //Enable the table
            ui->tableWidgetRecords->setEnabled(true);

            //Enable search widgets
            ui->lineEditKey->setEnabled(true);
            ui->comboBoxIndex->setEnabled(true);
            ui->pushButtonSearch->setEnabled(true);
            ui->tableWidgetSearch->setEnabled(true);
        }
    }
}

void MainWindow::on_actionImportXML_triggered()
{
}

void MainWindow::on_actionExportXML_triggered()
{
    //index file name
    QString newFileName = this->recordFileName; //same filename
    newFileName.remove(this->recordFileName.length() - 4, 4); //remove last 4 characters(.txt)
    newFileName += ".xml"; //Append

    QFile outFile( newFileName );

    outFile.open( outFile.Text | outFile.WriteOnly );

    char* xml = "<?xml version= 1.0 encoding= UTF-8 ?>\n";

    RecordOperations in;
    in.setFileName( this->recordFileName );

    outFile.write( xml, strlen ( xml ) );

    QStringList fields;
    QStringList key;
    QStringList lenght;
    QStringList type;
    QStringList Decimal;
    int length1 = in.getFieldsInformation().length();
    for( int i = 0; i < length1; i++)
    {
        QStringList fieldInfo = in.getFieldsInformation().at(i).split(",");
        fields.append( fieldInfo[1] );
        lenght.append( fieldInfo[3] );
        type.append( fieldInfo[2] );
        Decimal.append( fieldInfo[4] );
        key.append( fieldInfo[5] );
    }

    char *record = "<records>\n";
    outFile.write( record, strlen ( record ) );
    int length2 =in.getRecordsInformation().length();
    QStringList Records;
    for( int j = 0; j < length2; j++)
    {
        Records = in.getRecordInformationAt(j);

        QString recordInfo;
        int i =0;
        for( ; i < key.length(); i++ )
        {
            if( key[i].toInt() == 1 )
            {
                recordInfo = "   <record "+fields[i]+"="+Records[i+1]+">\n";
                stringstream sst;
                sst << recordInfo.toStdString();
                string temp_recordInfo = sst.str();
                const char * changeField = (char*)temp_recordInfo.c_str();
                outFile.write( changeField, recordInfo.length() );
                break;
             }
         }
        for (int x = 0; x < fields.length(); x++)
        {
            if( x != i )
            {
                recordInfo = "       <"+fields[x]+">"+Records[x+1]+"</"+fields[x]+">\n";
                stringstream sst;
                sst << recordInfo.toStdString();
                string temp_recordInfo = sst.str();
                const char * changeField = (char*)temp_recordInfo.c_str();
                outFile.write( changeField, recordInfo.length() );
             }
        }
        char *recordEnd = "   </record>";
        char *jump = "\n";
        outFile.write( recordEnd, 12 );
        outFile.write( jump, 1 );
    }
    char *recordEnd = "</records>";
    char *jump = "\n";
    outFile.write( recordEnd, 10 );
    outFile.write( jump, 1 );
    outFile.close();
    QMessageBox::information(this, tr("Information"), tr("XML File has been created"));
}

void MainWindow::on_actionImportJSON_triggered()
{
}

void MainWindow::on_actionExportJSON_triggered()
{
    //index file name
    QString newFileName = this->recordFileName; //same filename
    newFileName.remove(this->recordFileName.length() - 4, 4); //remove last 4 characters(.txt)
    newFileName += ".json"; //Append

    char *json ="{\n";

    QFile outFile( newFileName );

    outFile.open( outFile.Text | outFile.WriteOnly );

    RecordOperations in;
    in.setFileName( this->recordFileName );

    outFile.write( json, 2 );

    QStringList fields;
    QStringList key;
    QStringList lenght;
    QStringList type;
    QStringList Decimal;
    int length1 = in.getFieldsInformation().length();
    int length = in.getRecordsInformation().length();
    for( int i = 0; i < length1; i++)
    {
        QStringList fieldInfo = in.getFieldsInformation().at(i).split(",");
        fields.append( fieldInfo[1] );
        lenght.append( fieldInfo[3] );
        type.append( fieldInfo[2] );
        Decimal.append( fieldInfo[4] );
        key.append( fieldInfo[5] );
    }

    char *record = "records:     [\n";
    outFile.write( record, strlen ( record ) );

    QStringList Records;
    for( int j = 0; j < length; j++)
    {
        Records = in.getRecordInformationAt(j);

        QString recordInfo;
        int i =0;
        for( ; i < key.length(); i++ )
        {
            if( key[i].toInt() == 1 )
            {
                recordInfo = "{ record "+fields[i]+": "+Records[i+1]+", ";
                stringstream sst;
                sst << recordInfo.toStdString();
                string temp_recordInfo = sst.str();
                const char * changeField = (char*)temp_recordInfo.c_str();
                outFile.write( changeField, recordInfo.length() );
                break;
              }
        }

        for (int x = 0; x < fields.length(); x++)
        {
            if( x != i && x != fields.length() - 1 )
            {
                recordInfo = fields[x]+":"+Records[x+1]+", ";
                stringstream sst;
                sst << recordInfo.toStdString();
                string temp_recordInfo = sst.str();
                const char * changeField = (char*)temp_recordInfo.c_str();
                outFile.write( changeField, recordInfo.length() );
            }
            else if ( x != i && j != length - 1)
            {
                recordInfo = fields[x]+":"+Records[x+1]+" },";
                stringstream sst;
                sst << recordInfo.toStdString();
                string temp_recordInfo = sst.str();
                const char * changeField = (char*)temp_recordInfo.c_str();
                outFile.write( changeField, recordInfo.length() );
                char *jump= "\n";
                outFile.write( jump, 1 );
            }
            else
            {
                recordInfo = fields[x]+":"+Records[x+1]+" }";
                stringstream sst;
                sst << recordInfo.toStdString();
                string temp_recordInfo = sst.str();
                const char * changeField = (char*)temp_recordInfo.c_str();
                outFile.write( changeField, recordInfo.length() );
                char *jump= "\n";
                outFile.write( jump, 1 );
            }
        }
    }

    char *jump0 = "]\n";
    char *jump1 = "}\n";
    outFile.write( jump0, 2 );
    outFile.write( jump1, 2 );
    QMessageBox::information(this, tr("Information"), tr("JSON File has been created"));
}

void MainWindow::on_actionAbout_triggered()
{
}

bool MainWindow::validFile()
{
    return true;
}

bool MainWindow::hasIndexFile()
{
    QFile indexFile(this->indexFileName);

    if ( indexFile.exists() )
    {
        return true;
    }

    return false;
}

bool MainWindow::isIndexFileCorrupted()
{
    if ( indexList.size() < this->recordOperations.getNumberOfRecords() )
    {
        return true;
    }

    return false;
}

void MainWindow::loadIndexFile()
{
    QFile file(this->indexFileName);

    if ( !file.open(QIODevice::ReadOnly | QIODevice::Text) )
        return;

    QTextStream in(&file);

    //Read all lines
    while ( !in.atEnd() )
    {
        QString line = in.readLine(); //Read a line

        this->indexList.append(line);
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

        //If the user did not save tha last record
        if ( ( ui->tableWidgetRecords->rowCount() > 0 ) && ui->tableWidgetRecords->item(ui->tableWidgetRecords->rowCount()-1, ui->tableWidgetRecords->columnCount()-1)->text().isEmpty() )
        {
            //Can not add a row or delete a record
            this->actionInsertRow->setEnabled(false);
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

    //------------------------------------------------------------------------

    ui->tableWidgetRecords->blockSignals(true);

    //All columns of the last row (new row)
    for ( int a = 0; a < ui->tableWidgetRecords->columnCount(); a++ )
    {
        ui->tableWidgetRecords->setItem(ui->tableWidgetRecords->rowCount() - 1, a, new QTableWidgetItem(""));
    }

    ui->tableWidgetRecords->blockSignals(false);
}

void MainWindow::on_tableWidgetRecords_cellChanged(int row, int column)
{
    //Checks if there is a row selected (because while adding items, this function is call and there is no row selected)
    if ( ui->tableWidgetRecords->currentIndex().row() > -1 )
    {
        //If the user has left some column in blank
        if ( ( column > 0 ) && ( ui->tableWidgetRecords->item(row, column - 1)->text().isEmpty() ) )
        {
            QMessageBox::critical(this, tr("Error"), tr("Can not leave a blank value"));

            //Block the signal (to avoid a bucle with this function)
            ui->tableWidgetRecords->blockSignals(true);

            //set item
            ui->tableWidgetRecords->item(row, column)->setText("");

            //Enable the signal
            ui->tableWidgetRecords->blockSignals(false);
        }

        //If the item is not valid
        else if ( !this->isValidItem(row, column) )
        {
            QMessageBox::critical(this, tr("Error"), tr("Value is not valid"));

            //Block the signal (to avoid a bucle with this function)
            ui->tableWidgetRecords->blockSignals(true);

            //set item
            ui->tableWidgetRecords->item(row, column)->setText("");

            //Enable the signal
            ui->tableWidgetRecords->blockSignals(false);
        }

        //Checks if the length does not match
        else if ( !this->isValidItemLength(row, column) )
        {
            QMessageBox::critical(this, tr("Error"), tr("The length of the value is greater than set"));

            //Block the signal (to avoid a bucle with this function)
            ui->tableWidgetRecords->blockSignals(true);

            //set item
            ui->tableWidgetRecords->item(row, column)->setText("");

            //Enable the signal
            ui->tableWidgetRecords->blockSignals(false);
        }

        //Checks if the column is a key field and if the value(item) is not repeated
        else if ( this->isKey(column) && !this->isValidKey(row, column) )
        {
            QMessageBox::critical(this, tr("Error"), tr("The value already exist"));

            //Block the signal (to avoid a bucle with this function)
            ui->tableWidgetRecords->blockSignals(true);

            //set item
            ui->tableWidgetRecords->item(row, column)->setText("");

            //Enable the signal
            ui->tableWidgetRecords->blockSignals(false);
        }

        else
        {
            //Create a item
            QTableWidgetItem *item = new QTableWidgetItem(ui->tableWidgetRecords->item(row, column)->text());
            item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled); //item not editable

            //Block the signal (to avoid a bucle with this function)
            ui->tableWidgetRecords->blockSignals(true);

            //set item
            ui->tableWidgetRecords->setItem(row, column, item);

            //enable the signal
            ui->tableWidgetRecords->blockSignals(false);

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
                    this->statusBar()->showMessage(tr("The record has been added successfully!"), 2500);

                    //Enable the "save" option
                    ui->actionSaveFile->setEnabled(true);

                    ui->actionPrintFile->setEnabled(true);
                    ui->actionCrossRecords->setEnabled(true);
                    ui->actionCreateBTreeIndex->setEnabled(true);
                    ui->actionImportXML->setEnabled(true);
                    ui->actionExportXML->setEnabled(true);
                    ui->actionImportJSON->setEnabled(true);
                    ui->actionExportJSON->setEnabled(true);
                    ui->pushButtonSearch->setEnabled(true);

                    ui->actionCreateField->setEnabled(false);
                    ui->actionModifyField->setEnabled(false);

                    //delete this->btree;
                    //this->btree = nullptr;
                }
            }
        }
    }
}

bool MainWindow::insertRecord()
{
    //If the user has not deleted a record, insert at the end
    if ( this->availList.isEmpty() )
    {
        if ( !this->insertRecordEOF() )
        {
            return false;
        }
    }

    //search in the availList where to save the record (fisrt fit)
    else
    {
        if ( !this->insertRecordAvailList() )
        {
            return false;
        }
    }

    return true;
}

bool MainWindow::insertRecordEOF()
{
    RecordsFile recordFile;
    RecordsFile newRecordFile;

    if ( !recordFile.open(this->recordFileName.toStdString()) )
    {
        return false;
    }

    //------------------------------------Fields information------------------------------------

    //Number of fields length + fields information + ' : '
    int fieldsInformationSize = this->recordOperations.getInitialPositionOfRecordsInformation();
    char fieldsInformationBuffer[fieldsInformationSize];

    recordFile.seek(0);
    recordFile.read(fieldsInformationBuffer, fieldsInformationSize);


    //------------------------------------New number of records------------------------------------

    QString newNumberOfRecords = QString::number(this->recordOperations.getNumberOfRecords() + 1);

    if ( newNumberOfRecords.length() == 1 )
    {
        newNumberOfRecords.prepend("00000");
    }

    else if ( newNumberOfRecords.length() == 2 )
    {
        newNumberOfRecords.prepend("0000");
    }

    else if ( newNumberOfRecords.length() == 3 )
    {
        newNumberOfRecords.prepend("000");
    }

    else if ( newNumberOfRecords.length() == 4 )
    {
        newNumberOfRecords.prepend("00");
    }

    else if ( newNumberOfRecords.length() == 5 )
    {
        newNumberOfRecords.prepend("0");
    }


    //-----------------------------------Current records--------------------------------------------

    int currentRecordsSize = this->recordOperations.getLengthOfRecordsInformation(); //all the records from ' | ' to ' | '
    char currentRecordsBuffer[currentRecordsSize];

    //Before the ' | ' of the first record
    recordFile.seek(fieldsInformationSize + this->recordOperations.getLengthOfTheNumberOfRecords());
    recordFile.read(currentRecordsBuffer, currentRecordsSize);

    recordFile.close();


    //------------------------------------------New Record------------------------------------------

    QString newRecord = "";

    int recordLengthForIndexFile = 0;
    int recordLength = 0;

    //All the columns of the last row
    for ( int a = 0; a < ui->tableWidgetRecords->columnCount(); a++ )
    {
        newRecord += ui->tableWidgetRecords->item(ui->tableWidgetRecords->rowCount() - 1, a)->text() + ",";
    }

    //Remove the last ' , '
    newRecord.remove(newRecord.size() - 1, 1);

    //Record length for the index file
    recordLengthForIndexFile = newRecord.length();

    //Add the last ' | '
    newRecord += "|";

    //Length of the record + ' , '
    recordLength = newRecord.size() + 1;

    //Add the length and the comma
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

    if ( !newRecordFile.open(this->recordFileName.toStdString(), ios::out) )
    {
        return false;
    }

    //Write
    newRecordFile.write(fieldsInformationBuffer, fieldsInformationSize);
    newRecordFile.write(newNumberOfRecords.toStdString().c_str(), newNumberOfRecords.size());
    newRecordFile.write(currentRecordsBuffer, currentRecordsSize);\

    //Position in the record file after the new record
    int newRecordPosition = newRecordFile.tell();

    newRecordFile.write(newRecord.toStdString().c_str(), newRecord.size());

    //Close the file
    newRecordFile.close();

    //--------------------------------------Add to the Index list-------------------------------------------

    //All the fields information
    QStringList fieldsInformation = this->recordOperations.getFieldsInformation();

    int column = 0;

    //Search which column(field) is a key
    for (;column < fieldsInformation.size(); column++)
    {
        QStringList fieldInformation = fieldsInformation.at(column).split(",");

        //If a field is key
        if ( fieldInformation.at(fieldInformation.size() - 1) == "1" )
        {
            break;
        }
    }

    int lastRow = ui->tableWidgetRecords->rowCount() - 1;

    QString key = ui->tableWidgetRecords->item(lastRow, column)->text() + ",";//

    //Search for the first comma ' , ' to get the position for the index file
    for (int a = 0; a < newRecord.size(); a++)
    {
        if ( newRecord.at(a) == ',' )
        {
            newRecordPosition++;
            break;
        }

        newRecordPosition++;
    }

    QString position =  QString::number(newRecordPosition) + ",";
    QString length = QString::number(recordLengthForIndexFile);

    QString newIndex = key + position + length;

    this->indexList.append(newIndex);

    return true;
}

bool MainWindow::insertRecordAvailList()
{
    //Temporary record file
    QString TemporaryRecordFileName = this->recordFileName; //same filename
    TemporaryRecordFileName.remove(this->recordFileName.length() - 4, 4); //remove last 4 characters(.txt)
    TemporaryRecordFileName += "2.txt"; //Append

    QFile recordFile(this->recordFileName);
    QFile TemporaryRecordFile(TemporaryRecordFileName);

    //Open the record file
    if ( !recordFile.open(QIODevice::ReadOnly | QIODevice::Text) )
        return false;

    //Open the temp file
    if ( !TemporaryRecordFile.open(QIODevice::WriteOnly | QIODevice::Text) )
        return false;


    //------------------------------------------New Record------------------------------------------

    QString newRecord = "";

    //All the columns of the last row
    for ( int a = 0; a < ui->tableWidgetRecords->columnCount(); a++ )
    {
        newRecord += ui->tableWidgetRecords->item(ui->tableWidgetRecords->rowCount() - 1, a)->text() + ",";
    }

    //Remove the last ' , '
    newRecord.remove(newRecord.size() - 1, 1);


    //-----------------------------search where to save the record--------------------------------

    //The position in the record file of the new record
    int newRecordPosition = 0;

    QMapIterator<int, int> iterator(this->availList);

    //All the availList
    while (iterator.hasNext())
    {
        iterator.next();

        //If the length of the new record <= of the length of a deleted record
        if ( newRecord.length() <= iterator.value() )
        {
            newRecordPosition = iterator.key();

            break;
        }
    }

    //If the new record does not fit in any space of the avaiList, save at the end
    if ( newRecordPosition == 0 )
    {
        TemporaryRecordFile.close();
        TemporaryRecordFile.remove();

        if ( !this->insertRecordEOF() )
        {
            return false;
        }

        return true;
    }


    //---------------------------------Read/Write----------------------------------------

    char informationBeforeNewRecord[newRecordPosition];
    recordFile.seek(0);
    recordFile.read(informationBeforeNewRecord, newRecordPosition);

    TemporaryRecordFile.write(informationBeforeNewRecord, newRecordPosition);
    TemporaryRecordFile.write(newRecord.toStdString().c_str(), newRecord.size());

    int endOfFilePosition = this->recordOperations.getInitialPositionOfRecordsInformation() +
                            this->recordOperations.getLengthOfTheNumberOfRecords() +
                            this->recordOperations.getLengthOfRecordsInformation();

    int informationAfterNewRecordSize = endOfFilePosition - newRecordPosition - newRecord.size();
    char informationAfterNewRecord[informationAfterNewRecordSize];
    recordFile.seek(newRecordPosition + newRecord.size());
    recordFile.read(informationAfterNewRecord, informationAfterNewRecordSize);

    TemporaryRecordFile.write(informationAfterNewRecord, informationAfterNewRecordSize);


    //---------------------------------Files--------------------------------------

    //Close the file
    recordFile.close();
    TemporaryRecordFile.close();

    //Remove the record file
    if ( !recordFile.remove() )
    {
        return false;
    }

    //Change the name of the temporary record file
    if ( !TemporaryRecordFile.rename(TemporaryRecordFileName, this->recordFileName) )
    {
        return false;
    }


    //-------------------------Identify the row to set the new record in and add a index in the list----------------------------

    int row = 0;

    for (int a = 0; a < indexList.size(); a++)
    {
        if ( newRecordPosition < indexList.at(a).split(",").at(1).toInt() )
        {
            row = a;

            //All the fields information
            QStringList fieldsInformation = this->recordOperations.getFieldsInformation();

            int column = 0;

            //Search which column(field) is a key
            for (;column < fieldsInformation.size(); column++)
            {
                QStringList fieldInformation = fieldsInformation.at(column).split(",");

                //If a field is key
                if ( fieldInformation.at(fieldInformation.size() - 1) == "1" )
                {
                    break;
                }
            }


            QString keyForIndexFile = newRecord.split(",").at(column) + ",";
            QString positionForIndexFile = QString::number(newRecordPosition) + ",";
            QString lengthForIndexFile = QString::number(this->availList.value(newRecordPosition));

            QString index = keyForIndexFile + positionForIndexFile + lengthForIndexFile;

            this->indexList.insert(row, index);

            break;
        }
    }


    //----------------------------Change the row position-----------------------------------

    ui->tableWidgetRecords->blockSignals(true);

    ui->tableWidgetRecords->insertRow(row);

    QStringList items = newRecord.split(",");

    for (int column = 0; column < items.size(); column++)
    {
        //Create a item
        QTableWidgetItem *item = new QTableWidgetItem(items.at(column));
        item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled); //item not editable

        //set item
        ui->tableWidgetRecords->setItem(row, column, item);
    }

    ui->tableWidgetRecords->removeRow(ui->tableWidgetRecords->rowCount() - 1);

    ui->tableWidgetRecords->selectRow(row);

    ui->tableWidgetRecords->blockSignals(false);

    //------------------------------------------------------------------------------------

    this->availList.remove(newRecordPosition);

    return true;
}

bool MainWindow::isValidItem(int row, int column)
{
    //All the fields
    QStringList fieldsInformation = this->recordOperations.getFieldsInformation();
    QString type = fieldsInformation.at(column).split(",").at(2);
    QString item = ui->tableWidgetRecords->item(row, column)->text();

    int textCounter = 0;
    int integerCounter = 0;
    bool isDouble = false;

    //All the characters
    for (int a = 0; a < item.length(); a++)
    {
        //If the character is NOT a number
        if ( !item.at(a).isNumber() )
        {
            textCounter++;
        }

        //The character is a number
        else
        {
            integerCounter++;
        }
    }

    //If the item has a dot and numbers, is a double
    if ( ( textCounter == 1 ) && ( integerCounter == ( item.length() - 1 ) ) )
    {
        isDouble = true;
    }

    //----------------------------------------------------------------------------

    if ( ( type == "Double" ) && ( isDouble ) )
    {
        return true;
    }

    else if ( ( type == "Text" ) && ( textCounter > 0 ) )
    {
        return true;
    }

    else if ( ( type == "Integer" ) && ( integerCounter == item.length() ) )
    {
        return true;
    }

    return false;
}

bool MainWindow::isValidItemLength(int row, int column)
{
    //All the fields information
    QStringList fieldsInformation = this->recordOperations.getFieldsInformation();
    int length = fieldsInformation.at(column).split(",").at(3).toInt() + fieldsInformation.at(column).split(",").at(4).toInt();

    QString item = ui->tableWidgetRecords->item(row, column)->text();

    if ( item.length() <= length )
    {
        return true;
    }

    return false;
}

bool MainWindow::isKey(int column)
{
    //All the fields information
    QStringList fieldsInformation = this->recordOperations.getFieldsInformation();

    if ( fieldsInformation.at(column).split(",").at(5).toInt() == 1 )
    {
        return true;
    }

    return false;
}

bool MainWindow::isValidKey(int row, int column)
{
    QString key = ui->tableWidgetRecords->item(row, column)->text();

    for (int index = 0; index < this->indexList.size(); index++)
    {
        if ( this->indexList.at(index).split(",").at(0) == key )
        {
            return false;
        }
    }

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
    BTreeNode *m = btree->search( key.toInt() );

    if ( m != NULL )
    {
        //we get the posicion and the length of the record searched
        QString keyInfo = m->getPosicionLength();

        qDebug() << keyInfo;
        QFile file( this->recordFileName );

        if ( !file.open( QIODevice::ReadOnly | QIODevice::Text ) )
        {
            return false;
        }

        //splitting the information( posicion,length )
        QStringList keyInfoList = keyInfo.split( "," );

        qDebug() << keyInfoList[0] << " , " << keyInfoList[1];
        //set the initial position of the record
        file.seek( keyInfoList.at(0).toInt() );

        //buffer
        char buffer[ keyInfoList.at(1).toInt() ];

        //Read
        file.read( buffer, keyInfoList.at(1).toInt() );

        QString record = buffer;

        qDebug() << record;
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
     }else
     {
        QMessageBox::information(this, tr("Information"), tr("First you have to create the B-Tree Index "));

        return false;
    }

    delete [] m;
}

bool MainWindow::compact()
{
    //If ths user deleted a record o more
    if ( !this->availList.isEmpty() )
    {
        //Temporary record file
        QString TemporaryRecordFileName = this->recordFileName; //same filename
        TemporaryRecordFileName.remove(this->recordFileName.length() - 4, 4); //remove last 4 characters(.txt)
        TemporaryRecordFileName += "2.txt"; //Append

        QFile recordFile(this->recordFileName);
        QFile TemporaryRecordFile(TemporaryRecordFileName);

        //Open the record file
        if ( !recordFile.open(QIODevice::ReadOnly | QIODevice::Text) )
            return false;

        //Open the temp file
        if ( !TemporaryRecordFile.open(QIODevice::WriteOnly | QIODevice::Text) )
            return false;


        //--------------Copy fields information and the new number of records--------------------

        int fieldsInformationSize = this->recordOperations.getInitialPositionOfRecordsInformation();
        char fieldsInformation[fieldsInformationSize];
        recordFile.seek(0);
        recordFile.read(fieldsInformation, fieldsInformationSize);
        TemporaryRecordFile.write(fieldsInformation, fieldsInformationSize);


        int numberOfRecords = this->recordOperations.getNumberOfRecords();
        QString previousNumberOfRecords = QString::number(numberOfRecords);
        QString newNumberOfRecords = QString::number(numberOfRecords - this->availList.size());

        if ( previousNumberOfRecords.length() == 1 )
        {
            previousNumberOfRecords.prepend("00000");
        }

        else if ( previousNumberOfRecords.length() == 2 )
        {
            previousNumberOfRecords.prepend("0000");
        }

        else if ( previousNumberOfRecords.length() == 3 )
        {
            previousNumberOfRecords.prepend("000");
        }

        else if ( previousNumberOfRecords.length() == 4 )
        {
            previousNumberOfRecords.prepend("00");
        }

        else if ( previousNumberOfRecords.length() == 5 )
        {
            previousNumberOfRecords.prepend("0");
        }

        //        --------------------------

        if ( newNumberOfRecords.length() == 1 )
        {
            newNumberOfRecords.prepend("00000");
        }

        else if ( newNumberOfRecords.length() == 2 )
        {
            newNumberOfRecords.prepend("0000");
        }

        else if ( newNumberOfRecords.length() == 3 )
        {
            newNumberOfRecords.prepend("000");
        }

        else if ( newNumberOfRecords.length() == 4 )
        {
            newNumberOfRecords.prepend("00");
        }

        else if ( newNumberOfRecords.length() == 5 )
        {
            newNumberOfRecords.prepend("0");
        }

        newNumberOfRecords += "|";

        TemporaryRecordFile.write(newNumberOfRecords.toStdString().c_str(), newNumberOfRecords.size());

        //----------------------------------------------------------------------------------------

        int position = fieldsInformationSize + previousNumberOfRecords.length() + 1; //Position of a record (first record by default)
        int endOfFilePosition = fieldsInformationSize + previousNumberOfRecords.length() + this->recordOperations.getLengthOfRecordsInformation();

        //All the records (until the last record)
        while ( position < endOfFilePosition )
        {
            int recordPositionForAvailList = position + recordOperations.getLengthOfTheSizeOfARecordInformation(position) + 1;

            //If the user has deleted the record in the current position (the availList contain the position(key))
            if ( this->availList.contains( recordPositionForAvailList ) )
            {
                int informationSize = recordOperations.getLengthOfTheSizeOfARecordInformation(position) +
                                      recordOperations.getSizeOfARecordInformation(position);

                //Jump to the next record
                position += informationSize;
            }

            else
            {
                int informationSize = recordOperations.getLengthOfTheSizeOfARecordInformation(position) +
                                      recordOperations.getSizeOfARecordInformation(position);

                char information[informationSize];

                recordFile.seek(position);
                recordFile.read(information, informationSize);//Read the record

                TemporaryRecordFile.write(information, informationSize);//write the record(in other file)

                //Set the position in the next record
                position += informationSize;
            }
        }

        //Close files
        recordFile.close();
        TemporaryRecordFile.close();

        //Remove the record file
        if ( !recordFile.remove() )
        {
            return false;
        }

        //Change the name of the temporary record file
        if ( !TemporaryRecordFile.rename(TemporaryRecordFileName, this->recordFileName) )
        {
            return false;
        }
    }

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

    //getting the information of the record we are trying to delete
    QStringList recordInfo = indexList.at(index).split(",");
    qDebug() << recordInfo[1] << " " << recordInfo[2];

    // opening the record's file
    RecordsFile file ( this->recordFileName.toStdString() );

    int length1 = recordInfo[1].toInt() + recordInfo[2].toInt();
    int length2 = file.fileLength() - length1;
    qDebug() << " length2 " << length2;

    //buffers
    char * buffer1 = new char[recordInfo[1].toInt()];
    char * buffer2 = new char[length2];

    //we read the file
    file.read ( buffer1 ,recordInfo[1].toInt() );

    //we are setting the posicion
    file.seek( length1 );

    //reading
    file.read ( buffer2 , length2 );

    file.close();
    qDebug() << "buffer 1 " << buffer1;
    qDebug() <<"buffer 2 " << buffer2;

    //delete the old file
    const char *path = this->recordFileName.toStdString().c_str();
    remove( path );

    //creating the new file
    RecordsFile create;
    //Check if there is a problem while creating the file
    if ( !create.open(this->recordFileName.toStdString(),  ios::out) )
    {
        QMessageBox::critical(this, tr("Error"), tr("An error occurred while trying to create the file"));
    }

    //buffer is empty
    char * buffer3 = new char[recordInfo[2].toInt()];
    for ( int i = 0; i < recordInfo[2].toInt(); i++)
    {
        buffer3[i] = ' ';
    }

    //writing new information on the new file
    create.write ( buffer1, recordInfo[1].toInt());
    create.write( buffer3, recordInfo[2].toInt());
    create.write ( buffer2, length2 );
    create.close();

    //delete the buffer
    delete [] buffer1;
    delete [] buffer2;
    delete [] buffer3;

    //-------------------------------------------------------------------------------------------------------------

    //using availList
    availList.insert(recordInfo[1].toInt(), recordInfo[2].toInt());

    //Delete from indexList
    this->indexList.removeAt(index);

    //updating the table
    ui->tableWidgetRecords->removeRow(index);

    ui->statusBar->showMessage(tr("Deleted!"), 2500);

    //Enable the "save" option
    ui->actionSaveFile->setEnabled(true);
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
