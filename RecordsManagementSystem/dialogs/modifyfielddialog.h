#ifndef MODIFYFIELDDIALOG_H
#define MODIFYFIELDDIALOG_H

#include <QDialog>

#include "classes/recordoperations.h"

namespace Ui {
class ModifyFieldDialog;
}

class ModifyFieldDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ModifyFieldDialog(QString, QWidget *parent = 0);
    ~ModifyFieldDialog();
    
private slots:
    void on_tableWidgetFields_cellChanged(int row, int column);
    void modifyName();
    void modifyTypeLengthDecimal();
    void modifyKey();
    void modifyField(int, int, QString);

private:
    void tableProperties();
    void showFields();

    Ui::ModifyFieldDialog *ui;
    QString fileName;
    RecordOperations recordOperations;
};

#endif // MODIFYFIELDDIALOG_H
