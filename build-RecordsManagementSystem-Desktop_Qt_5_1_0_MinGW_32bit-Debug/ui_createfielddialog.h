/********************************************************************************
** Form generated from reading UI file 'createfielddialog.ui'
**
** Created by: Qt User Interface Compiler version 5.1.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CREATEFIELDDIALOG_H
#define UI_CREATEFIELDDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CreateFieldDialog
{
public:
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QFormLayout *formLayout;
    QLabel *label;
    QLineEdit *lineEditName;
    QLabel *label_2;
    QComboBox *comboBoxType;
    QLabel *label_3;
    QSpinBox *spinBoxLength;
    QLabel *label_4;
    QSpinBox *spinBoxDecimal;
    QHBoxLayout *horizontalLayout;
    QCheckBox *checkBoxKey;
    QPushButton *pushButtonCancel;
    QPushButton *pushButtonAccept;

    void setupUi(QDialog *CreateFieldDialog)
    {
        if (CreateFieldDialog->objectName().isEmpty())
            CreateFieldDialog->setObjectName(QStringLiteral("CreateFieldDialog"));
        CreateFieldDialog->resize(250, 171);
        layoutWidget = new QWidget(CreateFieldDialog);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(20, 20, 207, 133));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        formLayout = new QFormLayout();
        formLayout->setObjectName(QStringLiteral("formLayout"));
        label = new QLabel(layoutWidget);
        label->setObjectName(QStringLiteral("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        lineEditName = new QLineEdit(layoutWidget);
        lineEditName->setObjectName(QStringLiteral("lineEditName"));

        formLayout->setWidget(0, QFormLayout::FieldRole, lineEditName);

        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        comboBoxType = new QComboBox(layoutWidget);
        comboBoxType->setObjectName(QStringLiteral("comboBoxType"));

        formLayout->setWidget(1, QFormLayout::FieldRole, comboBoxType);

        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QStringLiteral("label_3"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_3);

        spinBoxLength = new QSpinBox(layoutWidget);
        spinBoxLength->setObjectName(QStringLiteral("spinBoxLength"));
        spinBoxLength->setMinimum(1);
        spinBoxLength->setMaximum(12);

        formLayout->setWidget(2, QFormLayout::FieldRole, spinBoxLength);

        label_4 = new QLabel(layoutWidget);
        label_4->setObjectName(QStringLiteral("label_4"));

        formLayout->setWidget(3, QFormLayout::LabelRole, label_4);

        spinBoxDecimal = new QSpinBox(layoutWidget);
        spinBoxDecimal->setObjectName(QStringLiteral("spinBoxDecimal"));
        spinBoxDecimal->setEnabled(false);
        spinBoxDecimal->setMaximum(6);

        formLayout->setWidget(3, QFormLayout::FieldRole, spinBoxDecimal);


        verticalLayout->addLayout(formLayout);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        checkBoxKey = new QCheckBox(layoutWidget);
        checkBoxKey->setObjectName(QStringLiteral("checkBoxKey"));

        horizontalLayout->addWidget(checkBoxKey);

        pushButtonCancel = new QPushButton(layoutWidget);
        pushButtonCancel->setObjectName(QStringLiteral("pushButtonCancel"));

        horizontalLayout->addWidget(pushButtonCancel);

        pushButtonAccept = new QPushButton(layoutWidget);
        pushButtonAccept->setObjectName(QStringLiteral("pushButtonAccept"));

        horizontalLayout->addWidget(pushButtonAccept);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(CreateFieldDialog);

        QMetaObject::connectSlotsByName(CreateFieldDialog);
    } // setupUi

    void retranslateUi(QDialog *CreateFieldDialog)
    {
        CreateFieldDialog->setWindowTitle(QApplication::translate("CreateFieldDialog", "Create Field", 0));
        label->setText(QApplication::translate("CreateFieldDialog", "Name:", 0));
        label_2->setText(QApplication::translate("CreateFieldDialog", "Type:", 0));
        comboBoxType->clear();
        comboBoxType->insertItems(0, QStringList()
         << QApplication::translate("CreateFieldDialog", "Text", 0)
         << QApplication::translate("CreateFieldDialog", "Integer", 0)
         << QApplication::translate("CreateFieldDialog", "Double", 0)
        );
        label_3->setText(QApplication::translate("CreateFieldDialog", "Length", 0));
        label_4->setText(QApplication::translate("CreateFieldDialog", "Decimal:", 0));
        checkBoxKey->setText(QApplication::translate("CreateFieldDialog", "Key", 0));
        pushButtonCancel->setText(QApplication::translate("CreateFieldDialog", "Cancel", 0));
        pushButtonAccept->setText(QApplication::translate("CreateFieldDialog", "Accept", 0));
    } // retranslateUi

};

namespace Ui {
    class CreateFieldDialog: public Ui_CreateFieldDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CREATEFIELDDIALOG_H
