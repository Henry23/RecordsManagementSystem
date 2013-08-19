#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "classes/recordsfile.h"
#include "dialogs/createfielddialog.h"
#include "dialogs/modifyfielddialog.h"

#include <QFileDialog>
#include <QTableWidgetItem>
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

    this->statusBar()->showMessage(tr("Welcome"), 2000);
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
    if ( !this->fileName.isEmpty() )
    {
        this->recordOperations.setFileName(this->fileName);

        this->updateTable();

        //Enable actions
        ui->actionSaveFile->setEnabled(true);
        ui->actionPrintFile->setEnabled(true);
        ui->actionCloseFile->setEnabled(true);
        ui->actionCreateField->setEnabled(true);
        ui->actionModifyField->setEnabled(true);
        ui->actionSearchRecord->setEnabled(true);
    }
}

void MainWindow::on_actionSaveFile_triggered()
{
}

void MainWindow::on_actionPrintFile_triggered()
{
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
    if ( recordOperations.getNumberOfFields() >  ( ui->tableWidgetRecords->columnCount() - 1 ) )
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

void MainWindow::clearTable()
{
    //If there are rows
    if ( ui->tableWidgetRecords->rowCount() > 0 )
    {
        //Remove all rows
        ui->tableWidgetRecords->setRowCount(0);
    }

    //If there are columns
    if ( ui->tableWidgetRecords->columnCount() > 0 )
    {
        //Remove all columns
        ui->tableWidgetRecords->setColumnCount(0);
    }
}

void MainWindow::updateTable()
{
    //If there is at least one field in the file (then the user can add a record)
    if ( this->recordOperations.getNumberOfFields() > 0 )
    {
        this->clearTable();

        ui->tableWidgetRecords->setColumnCount(this->recordOperations.getNumberOfFields() + 1); //Number of columns
        ui->tableWidgetRecords->setRowCount(this->recordOperations.getNumberOfRecords()); //Number of rows

        ui->tableWidgetRecords->hideColumn(0);

        //List of records
        QStringList recordsInformation = this->recordOperations.getRecordsInformation();

        //Rows
        for ( int a = 0; a < recordsInformation.size(); a++ )
        {
            QStringList recordInformation = recordsInformation.at(a).split(",");

            //Columns
            for ( int b = 0; b < recordInformation.size(); b++ )
            {
                //Insert a row
                ui->tableWidgetRecords->setItem(a, b, new QTableWidgetItem(recordInformation.at(b)));
            }
        }
    }
}

void MainWindow::on_tableWidgetRecords_customContextMenuRequested(const QPoint &pos)
{
    //check if there are rows
    if ( ui->tableWidgetRecords->rowCount() > 0  )
    {
        //If the user select a row
        if ( ui->tableWidgetRecords->currentRow() > -1 )
        {
            //Select the current row
            ui->tableWidgetRecords->selectRow(ui->tableWidgetRecords->currentRow());

            this->actionDeleteRecord->setEnabled(true);

            QPoint globalPos = ui->tableWidgetRecords->mapToGlobal(pos);
            popupMenu->exec(globalPos); //Execute the popupMenu
        }
    }

    //If there is at least one column in the file (then the user can add a row)
    else if ( ui->tableWidgetRecords->columnCount() > 0 )
    {
        //Cannot delete row/records
        this->actionDeleteRecord->setEnabled(false);

        QPoint globalPos = ui->tableWidgetRecords->mapToGlobal(pos);
        popupMenu->exec(globalPos); //Execute the popupMenu
    }
}

void MainWindow::insertRow()
{
    //If the last row is not empty, the user can insert a new row.
    if ( ( ui->tableWidgetRecords->item(ui->tableWidgetRecords->rowCount() - 1, 0) != 0 ) )
    {
        //Insert a new row
        ui->tableWidgetRecords->insertRow(ui->tableWidgetRecords->rowCount());
    }

    //If there are columns and no rows, the user can insert a new row.
    else if ( ( ui->tableWidgetRecords->columnCount() > 0 ) && ( ui->tableWidgetRecords->rowCount() == 0 ) )
    {
        //Insert a new row
        ui->tableWidgetRecords->insertRow(ui->tableWidgetRecords->rowCount());
    }
}

void MainWindow::deleteRecord()
{
    //Henry: Los indices de la tabla empiezan en cero, asi que le podes sumar uno para que se te sea mas facil
    int index = ui->tableWidgetRecords->currentRow() + 1;
}
