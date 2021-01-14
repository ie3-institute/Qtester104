#include "form.h"
#include "ui_form.h"
#include"ui_mainwindow.h"
#include"mainwindow.h"
#include <QToolButton>
#include <QFile>
#include<QBrush>
#include <QString>
#include<QLabel>
#include <QColor>
#include <QInputDialog>
#include <QDir>
#include<Qt>




Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
      ui->setupUi(this);
      this->setWindowTitle(tr("QTester104 IEC60870-5-104"));

    QString ininame = QCoreApplication::applicationDirPath() + CURDIRINIFILENAME;
    if (!QFile(ininame).exists()) {
      ininame = CONFDIRINIFILENAME;
    }

    if (QCoreApplication::arguments().count() > 1) {
      ininame = QCoreApplication::arguments().at(1);
    }

    QSettings settings(ininame, QSettings::IniFormat);
    QString rtuIP;
    int countrtu = 0;


    do {
      rtuIP = settings.value(QString("RTU%0/IP_ADDRESS").arg(countrtu + 1) , "").toString().trimmed();
      QString tab_name = settings.value(QString("RTU%0/NAME").arg(countrtu + 1), "").toString().trimmed();
      if ((rtuIP != "" || countrtu == 0) && ui->tabWidget->count()> 0) {    // (not so necessary)
              if (tab_name!= "")
              {
                  MainWindow *pageWithName = new MainWindow(this, ui->tabWidget->count()-1);
                  ui->tabWidget->addTab(pageWithName, QString(tab_name));
              }
              if (tab_name == "") {
                  MainWindow *pageWithDefName = new MainWindow(this, ui->tabWidget->count()-1);
                  ui->tabWidget->addTab(pageWithDefName, QString("Client_%0").arg(countrtu +1));
              }
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            // Configuration of the Table  !!!!!
            QString IP_Adress = settings.value(QString("RTU%0/IP_ADDRESS").arg(countrtu + 1), "").toString().trimmed();
            QString TCP_Port = settings.value(QString("RTU%0/TCP_PORT").arg(countrtu + 1), "").toString().trimmed();

            add_to_welcomePage(countrtu, tab_name, TCP_Port, IP_Adress);
            countrtu++;
      }
    } while (rtuIP != "");

    ui->tabWidget->setTabsClosable(true);
    auto tabBar = ui->tabWidget->tabBar();      
    tabBar->tabButton(0, QTabBar::RightSide)->deleteLater();
    tabBar->setTabButton(0, QTabBar::RightSide, 0);
    ui->tabWidget->setMovable(true);

}
Form::~Form()
{
    delete ui;
}


void Form::on_pushButton_clicked()              // Add Client !!
{
    bool ok_n , ok_p, ok_ip;
    QString Client_name = QInputDialog::getText(this, tr("Clientname"), tr("Name of the Client/Serve: "),QLineEdit::Normal, QDir::home().dirName(), &ok_n);


    if (ok_n && !Client_name.isEmpty())
   {
    int Port = QInputDialog::getInt(this, tr("Port Number"), tr("Portnumber of the Client/Server: "),0,0,65535,1, &ok_p);
    if (ok_p){
    QString IPAdr = QInputDialog::getText(this, tr("Remote IP Address:"), tr("Remote IP Address of the Client/Server:"),QLineEdit::Normal, QDir::home().dirName(), &ok_ip);
        if (ok_ip)
            {
            MainWindow *page = new MainWindow(this, ui->tabWidget->count());
            ui->tabWidget->addTab(page , QString(Client_name));
            page->get_input_Port_and_input_IP(Port, IPAdr);
            int cntrow = ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            add_to_welcomePage(cntrow, Client_name, QString::number(Port), IPAdr);
            }
        }
    }
}

void Form::add_to_welcomePage(int countrtu, QString tab_name, QString TCP_Port, QString IP_Adress)
{
    QTableWidgetItem* client_name = new QTableWidgetItem;
    ui->tableWidget->setItem(countrtu, 0, client_name);
    client_name->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable| Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled| Qt::ItemIsUserCheckable);
    if (tab_name =="")
        tab_name = QString("Client_%0").arg(countrtu + 1);
    client_name->setText(tab_name);

    QTableWidgetItem* port_numb = new QTableWidgetItem;
    ui->tableWidget->setItem(countrtu, 1, port_numb);
    port_numb->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable| Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled| Qt::ItemIsUserCheckable);
    port_numb->setText(TCP_Port);

    QTableWidgetItem* Remote_IP_Address = new QTableWidgetItem;
    ui->tableWidget->setItem(countrtu, 2, Remote_IP_Address);
    Remote_IP_Address->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable| Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled| Qt::ItemIsUserCheckable);
    Remote_IP_Address->setText(IP_Adress);
}

void Form::on_tabWidget_tabCloseRequested(int index)        
{
    ui->tabWidget->removeTab(index);
    ui->tableWidget->removeRow(index-1);
}

void Form::on_tableWidget_cellDoubleClicked(int row, int column)
{
    Col = column;       // not used
    ui->tabWidget->setCurrentWidget(ui->tabWidget->widget(row + 1));
}

