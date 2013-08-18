#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "classes/recordoperations.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_actionNewFile_triggered();

    void on_actionOpenFile_triggered();

    void on_actionSaveFile_triggered();

    void on_actionPrintFile_triggered();

    void on_actionCloseFile_triggered();

    void on_actionExit_triggered();

    void on_actionCreateField_triggered();

    void on_actionModifyField_triggered();

    void on_actionShowFields_triggered();

    void on_actionInsertRecord_triggered();

    void on_actionSearchRecord_triggered();

    void on_actionCrossRecords_triggered();

    void on_actionCreateSimpleIndex_triggered();

    void on_actionCreateBTreeIndex_triggered();

    void on_actionReindexing_triggered();

    void on_actionImportXML_triggered();

    void on_actionExportXML_triggered();

    void on_actionImportJSON_triggered();

    void on_actionExportJSON_triggered();

    void on_actionAbout_triggered();

private:
    void updateTableProperties();

    Ui::MainWindow *ui;
    QString fileName = "";
    RecordOperations recordOperations;
};

#endif // MAINWINDOW_H
