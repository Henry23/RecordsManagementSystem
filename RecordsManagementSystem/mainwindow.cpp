#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "classes/recordsfile.h"
#include "dialogs/createfielddialog.h"

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

        create.close();
    }
}

void MainWindow::on_actionOpenFile_triggered()
{
    //Open a dialog and save the file path
    this->fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath(), tr("Text File (*.txt)"));
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

}
/*
void modificarFiles(String fileName, int pos, int lengthIndex)
{

    ADTFile file;
    file.open(fileName);

    //we got from position 0 to the first letter, that we want to read
    int length1 = ( ( pos * 28 ) - 28 ) ;

    //and here we will get the rest bits of the record, that we need for the buffer 2
    int length2 = ( lengthIndex * 28 ) - length1;

    char *buffer1 = new char[length1];
    char *buffer2 = new char[length2];

    //read all between 0 the positon of the field we want to change
    file.read( buffer1 ,length1 );

    //Sets the position of the next character to be extracted or the last positon of the field
    file.seek(( pos * 28 ));

    //read the rest of the Record
    file.read( buffer2 , length2 );

    //we closed and remove the origial file
    file.close();
    remove( fileName );

    //we creade the file with the same name and the same path
    ADTFile file2;
    file2.open(fileName);

    //we write on the new file
    file2.write( buffer1, length1 );
    file2.write("Campo Modificado", lengthOfField);
    file2.write( buffer2 ,length2 );
    file2.close();
}


void deleteFiles(String fileName, int pos, int lengthIndex)
{

    ADTFile file;
    file.open(fileName);

    //we got from position 0 to the first letter, that we want to read
    int length1 = ( ( pos * 28 ) - 28 ) ;

    //and here we will get the rest bits of the record, that we need for the buffer 2
    int length2 = ( lengthIndex * 28 ) - length1;

    char *buffer1 = new char[length1];
    char *buffer2 = new char[length2];

    //read all between 0 the positon of the field we want to change
    file.read( buffer1 ,length1 );

    //Sets the position of the next character to be extracted or the last positon of the field
    file.seek(( pos * 28 ));

    //read the rest of the Record
    file.read( buffer2 , length2 );

    //we closed and remove the origial file
    file.close();
    remove( fileName );

    //we creade the file with the same name and the same path
    ADTFile file2;
    file2.open(fileName);

    //we write on the new file
    file2.write( buffer1, length1 );
    file2.write("  espacio en blanco  ", lengthOfField);
    file2.write( buffer2 ,length2 );
    file2.close();
}
*/

void MainWindow::on_actionShowFields_triggered()
{

}

void MainWindow::on_actionInsertRecord_triggered()
{
}

void MainWindow::on_actionSearchRecord_triggered()
{
}

void MainWindow::on_actionEraseRecord_triggered()
{
}

void MainWindow::on_actionShowRecords_triggered()
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
