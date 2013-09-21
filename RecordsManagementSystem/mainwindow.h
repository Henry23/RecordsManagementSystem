#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "classes/recordoperations.h"
#include "classes/btree.h"

#include <QMainWindow>
#include <QLabel>
#include <QList>
#include <QMap>

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

    void on_actionCrossRecords_triggered();

    void on_actionCreateSimpleIndex_triggered();

    void on_actionCreateBTreeIndex_triggered();

    void on_actionReindexing_triggered();

    void on_actionImportXML_triggered();

    void on_actionExportXML_triggered();

    void on_actionImportJSON_triggered();

    void on_actionExportJSON_triggered();

    void on_actionAbout_triggered();

    void on_tableWidgetRecords_customContextMenuRequested(const QPoint &pos);

    void insertRow();

    void deleteRecord();

    void on_tableWidgetRecords_cellChanged(int row, int column);

    void on_pushButtonSearch_clicked();

private:
    void init();
    bool validFile(); //Checks if the selected file is compatible with the application
    bool hasIndexFile();
    bool isIndexFileCorrupted();
    void loadIndexFile();
    void showFields();
    void showRecords();
    bool insertRecord();
    bool isValidItem(int, int);
    bool isValidItemLength(int, int);
    bool isKey(int);
    bool isValidKey(int, int);
    bool indexListSearch(QString);
    bool bTreeSearch(QString);
    bool compact();
    bool saveIndexList();

    Ui::MainWindow *ui;
    QMenu *popupMenu;
    QAction *actionInsertRow;
    QAction *actionDeleteRecord;
    QLabel *labelRecordFileName;

    QString recordFileName = "";
    QString indexFileName = "";
    RecordOperations recordOperations;
    QList<QString> indexList; //( key, position, length )
    QMap<int, int> availList; //position, length
    BTree *btree; // btree

};

#endif // MAINWINDOW_H
