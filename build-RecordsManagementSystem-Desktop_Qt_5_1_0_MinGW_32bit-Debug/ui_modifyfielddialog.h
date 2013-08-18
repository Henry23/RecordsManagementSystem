/********************************************************************************
** Form generated from reading UI file 'modifyfielddialog.ui'
**
** Created by: Qt User Interface Compiler version 5.1.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MODIFYFIELDDIALOG_H
#define UI_MODIFYFIELDDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableWidget>

QT_BEGIN_NAMESPACE

class Ui_ModifyFieldDialog
{
public:
    QTableWidget *tableWidgetFields;

    void setupUi(QDialog *ModifyFieldDialog)
    {
        if (ModifyFieldDialog->objectName().isEmpty())
            ModifyFieldDialog->setObjectName(QStringLiteral("ModifyFieldDialog"));
        ModifyFieldDialog->resize(512, 363);
        tableWidgetFields = new QTableWidget(ModifyFieldDialog);
        tableWidgetFields->setObjectName(QStringLiteral("tableWidgetFields"));
        tableWidgetFields->setGeometry(QRect(10, 11, 491, 341));

        retranslateUi(ModifyFieldDialog);

        QMetaObject::connectSlotsByName(ModifyFieldDialog);
    } // setupUi

    void retranslateUi(QDialog *ModifyFieldDialog)
    {
        ModifyFieldDialog->setWindowTitle(QApplication::translate("ModifyFieldDialog", "Modify Field", 0));
    } // retranslateUi

};

namespace Ui {
    class ModifyFieldDialog: public Ui_ModifyFieldDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODIFYFIELDDIALOG_H
