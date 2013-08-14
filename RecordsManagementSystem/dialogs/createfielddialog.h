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
    explicit CreateFieldDialog(QWidget *parent = 0);
    ~CreateFieldDialog();
    
private slots:
    void on_comboBoxType_currentIndexChanged(int index);

private:
    Ui::CreateFieldDialog *ui;
};

#endif // CREATEFIELDDIALOG_H
