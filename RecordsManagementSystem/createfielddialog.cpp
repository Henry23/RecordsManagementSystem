#include "createfielddialog.h"
#include "ui_createfielddialog.h"

CreateFieldDialog::CreateFieldDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateFieldDialog)
{
    ui->setupUi(this);
}

CreateFieldDialog::~CreateFieldDialog()
{
    delete ui;
}

void CreateFieldDialog::on_comboBoxType_currentIndexChanged(int index)
{
    //
    if ( index == 0 )
    {
        ui->spinBoxDecimal->setEnabled(false);
    }

    else if ( index == 1 )
    {
        ui->spinBoxDecimal->setEnabled(false);
    }

    else
    {
        ui->spinBoxDecimal->setEnabled(true);
    }
}
