#ifndef FORM_H
#define FORM_H

#include "iec104_class.h"
#include "qiec104.h"
#include"mainwindow.h"
#include <QWidget>
#include <QToolButton>
#include <QTableWidgetItem>
namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = nullptr);
    ~Form();


private slots:
    void on_pushButton_clicked();   
    void on_tabWidget_tabCloseRequested(int index);   
    void on_tableWidget_cellDoubleClicked(int row, int column);


private:
    Ui::Form *ui;
    int Col;
    void add_to_welcomePage(int countrtu, QString tab_name, QString TCP_Port, QString IP_Address);


};

#endif // FORM_H
