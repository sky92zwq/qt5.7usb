#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createaction();
    createmenus();  //菜单栏
    createToolBars();//工具栏
    createdockwidget();//停靠部件
}

MainWindow::~MainWindow()
{
    delete ui;
}


//void MainWindow::on_lineEdit_editingFinished()
//{
//    QString strread;
//    strread = ui->lineEdit->text();
//    ui->listWidget->setUpdatesEnabled(false);
//    ui->listWidget->addItem(strread);

//}

void MainWindow::on_open_clicked()
{
    QString num;
    num=QString::number( usb.getnumDEv());
    ui->listWidget_2->addItem(num);

    QString label;
    if(!usb.isopened()){
        label+=usb.status.at(usb.Open(0));
        ui->listWidget_2->addItem(label);
    }
    else
        ui->listWidget_2->addItem("device 0 is allrdeay opened");
}

void MainWindow::on_close_clicked()
{
    QString label;
    if(usb.isopened()){

        label+=usb.status.at(usb.Close());
        ui->listWidget_2->addItem(label);
    }
    else
        ui->listWidget_2->addItem("device 0 is allrdeay closed");
}

void MainWindow::openusb()//closeusb action
{
    statusdock->setVisible(true);//使停靠栏可见

    QString num;
    num=QString::number( usb.getnumDEv());
    num+=" devices";
    ui->listWidget_2->addItem(num);

    QString label;
    if(!usb.isopened()){
        label+=usb.status.at(usb.Open(0));
        ui->listWidget_2->addItem(label);
    }
    else
        ui->listWidget_2->addItem("device 0 is allrdeay opened");
}

void MainWindow::closeusb()//closeusb action
{
    statusdock->setVisible(true);//使停靠栏可见

    QString label;
    if(usb.isopened()){

        label+=usb.status.at(usb.Close());
        ui->listWidget_2->addItem(label);
    }
    else
        ui->listWidget_2->addItem("device 0 is allrdeay closed");
}

void MainWindow::tdlas()
{
    tdlasDialog tdlasdl;
    tdlasdl.exec();
}
///
/// \brief MainWindow::dataacquisition
///

//thread1
QThread f ;
QBuffer bb;

//thread2

void MainWindow::dataacquisition()//数据采集动作
{

    if(1){//usb.getnumDev()
        if(1){//usb.isopened()
            QFileDialog::Options options = QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly;
            QString savedirectory = QFileDialog::getExistingDirectory(this,
                                                                      tr("Save Directory"),
                                                                      "../",
                                                                      options);
            if (savedirectory.isEmpty())
                qDebug()<<"err";
            else{
                ui->listWidget_2->addItem("save at "+savedirectory);
                if(savedirectory=="C:/")ui->listWidget_2->addItem("do not use C:/ as saving position");

                datetime=QDateTime::currentDateTime();//准备文件
                QString dt= datetime.toString("yyyy-MM-dd_HH.mm.ss");

                QDir::setCurrent(savedirectory);//准备文件
                QFile *datafile=new QFile(dt+"_acquicition.dat");
                datafile->open(QIODevice::ReadWrite|QIODevice::Append|QIODevice::Truncate);
                datafile->close();
                qDebug()<<datafile->exists();

                RWBuffer *bufferrwer=new RWBuffer;//接受写入文件准备读写类RWbuffer
                bufferrwer->moveToThread(&RWthread);//开辟线程运行读写类
                connect(&RWthread, &QThread::finished, bufferrwer, &QObject::deleteLater);
                connect(this,SIGNAL(startacquisition()),bufferrwer,SLOT(rwbuffer()));
                connect(bufferrwer,SIGNAL(rwcount()),this,SLOT(acquisitioncount()));

                emit startacquisition();//发送开始采集信号

                QByteArray TxBuffer;

                QDataStream in(&TxBuffer, QIODevice::ReadWrite);
                BYTE asdf[100];
                in<<(quint8)0x88<<(quint8)0x00;
                qDebug()<<TxBuffer.data();
                qDebug()<<TxBuffer.toHex();
                qDebug()<<TxBuffer.size();


                DWORD BytesReceived;
                usb.Write(&TxBuffer,2,&BytesReceived);//向下写入发送命令
                qDebug()<<BytesReceived;
            }
        }
        else
            ui->listWidget_2->addItem("err: open a ft_device first");
    }
    else
        ui->listWidget_2->addItem("err: find a ft_device first");
}


///
/// \brief MainWindow::createToolBars
///
void MainWindow::createToolBars()
{
    toolusb=addToolBar("usb");
    toolusb->addAction(openusb_action);
    toolusb->addAction(closeusb_action);


    tooldataacquisition=addToolBar("data_acquisition");
    tooldataacquisition->addAction(dataacquisition_action);
    //insert here

}

void MainWindow::createaction()
{
    openusb_action= new QAction("openusb",this);
    connect(openusb_action,SIGNAL(triggered()),this,SLOT(openusb()));

    closeusb_action=new QAction("closeusb",this);
    connect(closeusb_action,SIGNAL(triggered()),this,SLOT(closeusb()));

    tdlas_action=new QAction("tdlas",this);
    connect(tdlas_action,SIGNAL(triggered()),this,SLOT(tdlas()));

    dataacquisition_action=new QAction("data acquisition",this);
    connect(dataacquisition_action,SIGNAL(triggered()),this,SLOT(dataacquisition()));

}

void MainWindow::createmenus()//insert codes
{
    ui->menu->addAction(tdlas_action);
    ui->menu->addAction(openusb_action);
}

void MainWindow::createdockwidget()
{
    statusdock=new QDockWidget(tr("status"),this);
    statusdock->setWidget(ui->listWidget_2);
    addDockWidget(Qt::RightDockWidgetArea,statusdock);
}


