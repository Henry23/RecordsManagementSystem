#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "createfielddialog.h"

#include <QFileDialog>

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
    QString file = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath(), tr("Text Files (*.txt)"));
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
    CreateFieldDialog *dialog = new CreateFieldDialog;
    dialog->exec();
    delete dialog;
}

void MainWindow::on_actionModifyField_triggered()
{
}

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
