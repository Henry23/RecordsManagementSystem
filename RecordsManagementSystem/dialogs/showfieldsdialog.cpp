#include "showfieldsdialog.h"
#include "ui_showfieldsdialog.h"

#include "classes/recordsfile.h"

#include <QTableWidgetItem>
#include <QMessageBox>

ShowFieldsDialog::ShowFieldsDialog(QString fileName, bool modifyMode, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowFieldsDialog)
{
    ui->setupUi(this);

    this->fileName = fileName;
    this->modifyMode = modifyMode;

    this->recordOperations.setFileName(this->fileName);

    this->tableProperties();
    this->showFields();
}

ShowFieldsDialog::~ShowFieldsDialog()
{
    delete ui;
}

void ShowFieldsDialog::tableProperties()
{
    //If the dialog was not open in modify mode
    if ( !this->modifyMode )
    {
        ui->tableWidgetFields->setEditTriggers(QAbstractItemView::NoEditTriggers); //The table can not be edited
        this->setWindowTitle(tr("Show Field"));
    }

    //If the dialog was open in modify mode
    else
    {
        this->setWindowTitle(tr("Modify Fields"));
    }

    ui->tableWidgetFields->setSelectionBehavior(QAbstractItemView::SelectItems); //clicking on a item selects only the item

    ui->tableWidgetFields->setColumnCount(6); //Number of columns
    ui->tableWidgetFields->setRowCount(this->recordOperations.getNumberOfFields()); //Number of rows

    ui->tableWidgetFields->hideColumn(0);

    //Columns name
    ui->tableWidgetFields->setHorizontalHeaderLabels(QString("Hide;Name;Type;Length;Decimal;Key").split(";"));
}

void ShowFieldsDialog::showFields()
{
    QStringList fieldsProperties = this->recordOperations.getFieldsProperties();

    //Rows
    for ( int a = 0; a < fieldsProperties.size(); a++ )
    {
        QStringList fieldProperties = fieldsProperties.at(a).split(",");

        //Columns
        for ( int b = 0; b <fieldProperties.size(); b++ )
        {
            //Insert a row
            ui->tableWidgetFields->setItem(a, b, new QTableWidgetItem(fieldProperties.at(b)));
        }
    }
}

void ShowFieldsDialog::on_tableWidgetFields_cellChanged(int row, int column)
{
    //Checks if there is a row selected (because while adding items, this function is call and there is no row selected)
    if ( ui->tableWidgetFields->currentIndex().row() > -1 )
    {
        //Checks if the user delete a item
        if ( ui->tableWidgetFields->item(row, column)->text().isEmpty() )
        {
            QMessageBox::critical(this, tr("Error"), tr("The information can not be empty"));

            ui->tableWidgetFields->setItem(row, column, new QTableWidgetItem("Unknown"));
        }

        //Henry, los paremetros de esta funcion almacenan la fila y columna del elemento que se modifico
        //Utilizalos para saber que campo y que propiedad vas a modificar en el archivo
        //Utiliza la siguiente instruccion para obtener el nombre del elemento modificado:
        //ui->tableWidgetFields->item(row, column)->text();
    }
}
