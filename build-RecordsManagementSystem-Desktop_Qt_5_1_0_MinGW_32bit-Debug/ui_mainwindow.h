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
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/newFile.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionNewFile->setIcon(icon);
        actionSaveFile = new QAction(MainWindow);
        actionSaveFile->setObjectName(QStringLiteral("actionSaveFile"));
        actionSaveFile->setEnabled(false);
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/images/save.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSaveFile->setIcon(icon1);
        actionPrintFile = new QAction(MainWindow);
        actionPrintFile->setObjectName(QStringLiteral("actionPrintFile"));
        actionPrintFile->setEnabled(false);
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/images/print.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPrintFile->setIcon(icon2);
        actionCloseFile = new QAction(MainWindow);
        actionCloseFile->setObjectName(QStringLiteral("actionCloseFile"));
        actionCloseFile->setEnabled(false);
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/images/close.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCloseFile->setIcon(icon3);
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/images/exit.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionExit->setIcon(icon4);
        actionCreateField = new QAction(MainWindow);
        actionCreateField->setObjectName(QStringLiteral("actionCreateField"));
        actionCreateField->setEnabled(false);
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/images/createField.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCreateField->setIcon(icon5);
        actionModifyField = new QAction(MainWindow);
        actionModifyField->setObjectName(QStringLiteral("actionModifyField"));
        actionModifyField->setEnabled(false);
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/images/modifyField.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionModifyField->setIcon(icon6);
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
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/images/search.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSearchRecord->setIcon(icon7);
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
        QIcon icon8;
        icon8.addFile(QStringLiteral(":/images/openFile.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpenFile->setIcon(icon8);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        tableWidgetRecords = new QTableWidget(centralWidget);
        tableWidgetRecords->setObjectName(QStringLiteral("tableWidgetRecords"));
        tableWidgetRecords->setEnabled(false);
        tableWidgetRecords->setContextMenuPolicy(Qt::CustomContextMenu);
        tableWidgetRecords->setSelectionBehavior(QAbstractItemView::SelectRows);

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
        mainToolBar->setMovable(false);
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
        mainToolBar->addAction(actionNewFile);
        mainToolBar->addAction(actionOpenFile);
        mainToolBar->addAction(actionSaveFile);
        mainToolBar->addAction(actionPrintFile);
        mainToolBar->addAction(actionCreateField);
        mainToolBar->addAction(actionModifyField);
        mainToolBar->addAction(actionSearchRecord);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionCloseFile);
        mainToolBar->addAction(actionExit);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Records Management System - Alpha 1", 0));
        actionNewFile->setText(QApplication::translate("MainWindow", "New", 0));
        actionNewFile->setIconText(QApplication::translate("MainWindow", "New", 0));
#ifndef QT_NO_TOOLTIP
        actionNewFile->setToolTip(QApplication::translate("MainWindow", "New File", 0));
#endif // QT_NO_TOOLTIP
        actionSaveFile->setText(QApplication::translate("MainWindow", "Save", 0));
        actionPrintFile->setText(QApplication::translate("MainWindow", "Print", 0));
        actionCloseFile->setText(QApplication::translate("MainWindow", "Close", 0));
        actionExit->setText(QApplication::translate("MainWindow", "Exit", 0));
        actionCreateField->setText(QApplication::translate("MainWindow", "Create", 0));
#ifndef QT_NO_TOOLTIP
        actionCreateField->setToolTip(QApplication::translate("MainWindow", "Create Field", 0));
#endif // QT_NO_TOOLTIP
        actionModifyField->setText(QApplication::translate("MainWindow", "Modify", 0));
#ifndef QT_NO_TOOLTIP
        actionModifyField->setToolTip(QApplication::translate("MainWindow", "Modify Field", 0));
#endif // QT_NO_TOOLTIP
        actionCreateSimpleIndex->setText(QApplication::translate("MainWindow", "Create a Simple Index", 0));
        actionShowFields->setText(QApplication::translate("MainWindow", "Show Fields", 0));
        actionInsertRecord->setText(QApplication::translate("MainWindow", "Insert", 0));
        actionSearchRecord->setText(QApplication::translate("MainWindow", "Search", 0));
#ifndef QT_NO_TOOLTIP
        actionSearchRecord->setToolTip(QApplication::translate("MainWindow", "Search Record", 0));
#endif // QT_NO_TOOLTIP
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
#ifndef QT_NO_TOOLTIP
        actionOpenFile->setToolTip(QApplication::translate("MainWindow", "Open File", 0));
#endif // QT_NO_TOOLTIP
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
