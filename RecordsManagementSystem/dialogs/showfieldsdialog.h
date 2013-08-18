#ifndef SHOWFIELDSDIALOG_H
#define SHOWFIELDSDIALOG_H

#include <QDialog>

#include "classes/recordoperations.h"

namespace Ui {
class ShowFieldsDialog;
}

class ShowFieldsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ShowFieldsDialog(QString, bool = false, QWidget *parent = 0);
    ~ShowFieldsDialog();

private slots:
    void on_tableWidgetFields_cellChanged(int row, int column);

private:
    void tableProperties();
    void showFields();

    Ui::ShowFieldsDialog *ui;
    QString fileName;
    RecordOperations recordOperations;
    bool modifyMode;
};

#endif // SHOWFIELDSDIALOG_H
