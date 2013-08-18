#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "classes/recordsfile.h"
#include "dialogs/createfielddialog.h"
#include "dialogs/modifyfielddialog.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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

        this->updateTableProperties();

        //Enable actions
        ui->actionSaveFile->setEnabled(true);
        ui->actionPrintFile->setEnabled(true);
        ui->actionCloseFile->setEnabled(true);
        ui->actionCreateField->setEnabled(true);
        ui->actionModifyField->setEnabled(true);
        ui->actionInsertRecord->setEnabled(true);
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

        this->updateTableProperties();

        //Enable actions
        ui->actionSaveFile->setEnabled(true);
        ui->actionPrintFile->setEnabled(true);
        ui->actionCloseFile->setEnabled(true);
        ui->actionCreateField->setEnabled(true);
        ui->actionModifyField->setEnabled(true);
        ui->actionInsertRecord->setEnabled(true);
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
}

void MainWindow::on_actionModifyField_triggered()
{
    ModifyFieldDialog *dialog = new ModifyFieldDialog(this->fileName);
    dialog->exec();
    delete dialog;
}

void MainWindow::on_actionInsertRecord_triggered()
{
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

void MainWindow::updateTableProperties()
{
    ui->tableWidgetRecords->setSelectionBehavior(QAbstractItemView::SelectRows); //clicking on a item selects all the row

    if ( this->recordOperations.getNumberOfFields() > 0 )
    {
        ui->tableWidgetRecords->setColumnCount(this->recordOperations.getNumberOfFields()); //Number of columns
    }
}
