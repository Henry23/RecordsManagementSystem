/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.1.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionNewFile;
    QAction *actionSaveFile;
    QAction *actionPrintFile;
    QAction *actionCloseFile;
    QAction *actionExit;
    QAction *actionCreateField;
    QAction *actionModifyField;
    QAction *actionCreateSimpleIndex;
    QAction *actionShowFields;
    QAction *actionInsertRecord;
    QAction *actionSearchRecord;
    QAction *actionEraseRecord;
    QAction *actionShowRecords;
    QAction *actionCrossRecords;
    QAction *actionCreateBTreeIndex;
    QAction *actionAbout;
    QAction *actionImportXML;
    QAction *actionExportXML;
    QAction *actionImportJSON;
    QAction *actionExportJSON;
    QAction *actionReindexing;
    QAction *actionOpenFile;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QTableWidget *tableWidgetRecords;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuFields;
    QMenu *menuRecords;
    QMenu *menuIndices;
    QMenu *menuUtilities;
    QMenu *menuHelp;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(666, 359);
        actionNewFile = new QAction(MainWindow);
        actionNewFile->setObjectName(QStringLiteral("actionNewFile"));
        actionSaveFile = new QAction(MainWindow);
        actionSaveFile->setObjectName(QStringLiteral("actionSaveFile"));
        actionSaveFile->setEnabled(false);
        actionPrintFile = new QAction(MainWindow);
        actionPrintFile->setObjectName(QStringLiteral("actionPrintFile"));
        actionPrintFile->setEnabled(false);
        actionCloseFile = new QAction(MainWindow);
        actionCloseFile->setObjectName(QStringLiteral("actionCloseFile"));
        actionCloseFile->setEnabled(false);
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionCreateField = new QAction(MainWindow);
        actionCreateField->setObjectName(QStringLiteral("actionCreateField"));
        actionCreateField->setEnabled(false);
        actionModifyField = new QAction(MainWindow);
        actionModifyField->setObjectName(QStringLiteral("actionModifyField"));
        actionModifyField->setEnabled(false);
        actionCreateSimpleIndex = new QAction(MainWindow);
        actionCreateSimpleIndex->setObjectName(QStringLiteral("actionCreateSimpleIndex"));
        actionShowFields = new QAction(MainWindow);
        actionShowFields->setObjectName(QStringLiteral("actionShowFields"));
        actionShowFields->setEnabled(false);
        actionInsertRecord = new QAction(MainWindow);
        actionInsertRecord->setObjectName(QStringLiteral("actionInsertRecord"));
        actionInsertRecord->setEnabled(false);
        actionSearchRecord = new QAction(MainWindow);
        actionSearchRecord->setObjectName(QStringLiteral("actionSearchRecord"));
        actionSearchRecord->setEnabled(false);
        actionEraseRecord = new QAction(MainWindow);
        actionEraseRecord->setObjectName(QStringLiteral("actionEraseRecord"));
        actionEraseRecord->setEnabled(false);
        actionShowRecords = new QAction(MainWindow);
        actionShowRecords->setObjectName(QStringLiteral("actionShowRecords"));
        actionShowRecords->setEnabled(false);
        actionCrossRecords = new QAction(MainWindow);
        actionCrossRecords->setObjectName(QStringLiteral("actionCrossRecords"));
        actionCreateBTreeIndex = new QAction(MainWindow);
        actionCreateBTreeIndex->setObjectName(QStringLiteral("actionCreateBTreeIndex"));
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        actionImportXML = new QAction(MainWindow);
        actionImportXML->setObjectName(QStringLiteral("actionImportXML"));
        actionExportXML = new QAction(MainWindow);
        actionExportXML->setObjectName(QStringLiteral("actionExportXML"));
        actionImportJSON = new QAction(MainWindow);
        actionImportJSON->setObjectName(QStringLiteral("actionImportJSON"));
        actionExportJSON = new QAction(MainWindow);
        actionExportJSON->setObjectName(QStringLiteral("actionExportJSON"));
        actionReindexing = new QAction(MainWindow);
        actionReindexing->setObjectName(QStringLiteral("actionReindexing"));
        actionOpenFile = new QAction(MainWindow);
        actionOpenFile->setObjectName(QStringLiteral("actionOpenFile"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        tableWidgetRecords = new QTableWidget(centralWidget);
        tableWidgetRecords->setObjectName(QStringLiteral("tableWidgetRecords"));

        horizontalLayout->addWidget(tableWidgetRecords);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 666, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuFields = new QMenu(menuBar);
        menuFields->setObjectName(QStringLiteral("menuFields"));
        menuRecords = new QMenu(menuBar);
        menuRecords->setObjectName(QStringLiteral("menuRecords"));
        menuIndices = new QMenu(menuBar);
        menuIndices->setObjectName(QStringLiteral("menuIndices"));
        menuUtilities = new QMenu(menuBar);
        menuUtilities->setObjectName(QStringLiteral("menuUtilities"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuFields->menuAction());
        menuBar->addAction(menuRecords->menuAction());
        menuBar->addAction(menuIndices->menuAction());
        menuBar->addAction(menuUtilities->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionNewFile);
        menuFile->addAction(actionOpenFile);
        menuFile->addAction(actionSaveFile);
        menuFile->addAction(actionPrintFile);
        menuFile->addSeparator();
        menuFile->addAction(actionCloseFile);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuFields->addAction(actionCreateField);
        menuFields->addAction(actionModifyField);
        menuRecords->addAction(actionInsertRecord);
        menuRecords->addAction(actionSearchRecord);
        menuRecords->addSeparator();
        menuRecords->addAction(actionCrossRecords);
        menuIndices->addAction(actionCreateSimpleIndex);
        menuIndices->addAction(actionCreateBTreeIndex);
        menuIndices->addSeparator();
        menuIndices->addAction(actionReindexing);
        menuUtilities->addAction(actionImportXML);
        menuUtilities->addAction(actionExportXML);
        menuUtilities->addSeparator();
        menuUtilities->addAction(actionImportJSON);
        menuUtilities->addAction(actionExportJSON);
        menuHelp->addAction(actionAbout);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        actionNewFile->setText(QApplication::translate("MainWindow", "New", 0));
        actionSaveFile->setText(QApplication::translate("MainWindow", "Save", 0));
        actionPrintFile->setText(QApplication::translate("MainWindow", "Print", 0));
        actionCloseFile->setText(QApplication::translate("MainWindow", "Close", 0));
        actionExit->setText(QApplication::translate("MainWindow", "Exit", 0));
        actionCreateField->setText(QApplication::translate("MainWindow", "Create", 0));
        actionModifyField->setText(QApplication::translate("MainWindow", "Modify", 0));
        actionCreateSimpleIndex->setText(QApplication::translate("MainWindow", "Create a Simple Index", 0));
        actionShowFields->setText(QApplication::translate("MainWindow", "Show Fields", 0));
        actionInsertRecord->setText(QApplication::translate("MainWindow", "Insert", 0));
        actionSearchRecord->setText(QApplication::translate("MainWindow", "Search", 0));
        actionEraseRecord->setText(QApplication::translate("MainWindow", "Erase", 0));
        actionShowRecords->setText(QApplication::translate("MainWindow", "Show Records", 0));
        actionCrossRecords->setText(QApplication::translate("MainWindow", "Cross", 0));
        actionCreateBTreeIndex->setText(QApplication::translate("MainWindow", "Create a B-Tree index", 0));
        actionAbout->setText(QApplication::translate("MainWindow", "About", 0));
        actionImportXML->setText(QApplication::translate("MainWindow", "Import XML", 0));
        actionExportXML->setText(QApplication::translate("MainWindow", "Export XML", 0));
        actionImportJSON->setText(QApplication::translate("MainWindow", "Import JSON", 0));
        actionExportJSON->setText(QApplication::translate("MainWindow", "Export JSON", 0));
        actionReindexing->setText(QApplication::translate("MainWindow", "Reindexing", 0));
        actionOpenFile->setText(QApplication::translate("MainWindow", "Open", 0));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0));
        menuFields->setTitle(QApplication::translate("MainWindow", "Fields", 0));
        menuRecords->setTitle(QApplication::translate("MainWindow", "Records", 0));
        menuIndices->setTitle(QApplication::translate("MainWindow", "Indices", 0));
        menuUtilities->setTitle(QApplication::translate("MainWindow", "Utilities", 0));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
