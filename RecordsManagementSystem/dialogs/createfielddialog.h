#ifndef CREATEFIELDDIALOG_H
#define CREATEFIELDDIALOG_H

#include <QDialog>

#include "classes/recordoperations.h"

namespace Ui {
class CreateFieldDialog;
}

class CreateFieldDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit CreateFieldDialog(QString, QWidget *parent = 0);
    ~CreateFieldDialog();
    
private slots:
    void on_comboBoxType_currentIndexChanged(int index);

    void on_checkBoxKey_clicked();

    void on_pushButtonAccept_clicked();

    void on_pushButtonCancel_clicked();

private:
    bool fieldAlreadyExist();
    bool keyAlreadyExist();
    bool addField();

    Ui::CreateFieldDialog *ui;
    QString fileName;
    RecordOperations recordOperations;
};

#endif // CREATEFIELDDIALOG_H
