/********************************************************************************
** Form generated from reading UI file 'showfieldsdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.1.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SHOWFIELDSDIALOG_H
#define UI_SHOWFIELDSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableWidget>

QT_BEGIN_NAMESPACE

class Ui_ShowFieldsDialog
{
public:
    QTableWidget *tableWidgetFields;

    void setupUi(QDialog *ShowFieldsDialog)
    {
        if (ShowFieldsDialog->objectName().isEmpty())
            ShowFieldsDialog->setObjectName(QStringLiteral("ShowFieldsDialog"));
        ShowFieldsDialog->resize(532, 300);
        ShowFieldsDialog->setMaximumSize(QSize(600, 500));
        tableWidgetFields = new QTableWidget(ShowFieldsDialog);
        tableWidgetFields->setObjectName(QStringLiteral("tableWidgetFields"));
        tableWidgetFields->setGeometry(QRect(10, 11, 511, 281));

        retranslateUi(ShowFieldsDialog);

        QMetaObject::connectSlotsByName(ShowFieldsDialog);
    } // setupUi

    void retranslateUi(QDialog *ShowFieldsDialog)
    {
        ShowFieldsDialog->setWindowTitle(QApplication::translate("ShowFieldsDialog", "Show Fields", 0));
    } // retranslateUi

};

namespace Ui {
    class ShowFieldsDialog: public Ui_ShowFieldsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SHOWFIELDSDIALOG_H
