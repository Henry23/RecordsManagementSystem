#ifndef SHOWFIELDSDIALOG_H
#define SHOWFIELDSDIALOG_H

#include <QDialog>

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
    int getNumberOfFields(); //Return the number of fields created
    int getLengthOfTheNumberOfFields(); // Example: 10 fields created, return 2 (the number of digits)
    int getSizeOfTheFieldProperties(int); //Example: 2|20,Nombre,Text,10,0,1|22,Apellido,Text,10,0,0|: -> int=24 return 22
    int getLengthOfTheSizeOfTheFieldProperties(int);//Example: 2|20,Nombre,Text,10,0,1|22,Apellido,Text,10,0,0|: -> int=24 return 2
    int getTotalLengthOfFieldsProperties();//Example: 2|20,Nombre,Text,10,0,1|22,Apellido,Text,10,0,0|: -> return 48
    QStringList getFieldsProperties();
    void showFields();

    Ui::ShowFieldsDialog *ui;
    QString fileName;
    bool modifyMode;
};

#endif // SHOWFIELDSDIALOG_H
