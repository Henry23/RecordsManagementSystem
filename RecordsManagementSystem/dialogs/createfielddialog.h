#ifndef CREATEFIELDDIALOG_H
#define CREATEFIELDDIALOG_H

#include <QDialog>

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

    void on_pushButtonAccept_clicked();

    void on_pushButtonCancel_clicked();

private:
    int getNumberOfFields();
    bool addFirstField();
    bool addField();

    Ui::CreateFieldDialog *ui;
    QString fileName;
};

#endif // CREATEFIELDDIALOG_H
