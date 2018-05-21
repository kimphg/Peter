#include "mainwindow.h"

#include "ui_mainwindow.h"
#include "datareplay.h"
DataReplay *pCaptureFile;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QStringList interfaceList = DataCapture::getInterfacesList();
    for (const auto& str : interfaceList) {
        ui->listWidget->addItem(str);
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Output file"), NULL, tr("Data files (*.cap)"));
    pCaptureFile = new DataReplay(fileName);
    pCaptureFile->start();
    this->startTimer(30);
}
inline bool checkPrintable(QByteArray data)
{
    unsigned char* dataPtr = (unsigned char*)data.data();
    uint size = data.size();
    for (int i=0;i<size;i++)
    {

        if((*(dataPtr+i))<32)
        {
            if((*(dataPtr+i))!='\r'&&(*(dataPtr+i))!='\n')
            return false;
        }
    }
    return true;
}
void MainWindow::timerEvent(QTimerEvent *event)
{
    if(pCaptureFile->mCapData.size())
    {

        EthernetDataFrame frame = pCaptureFile->mCapData.dequeue();
        /*int port = ((*((unsigned char*)(frame.data()+34)))<<8)+(*((unsigned char*)(frame.data()+35)));
        if(port!=4001&&port!=4002)return;
        QByteArray dataframe = frame.right(frame.size()-42);
        if(!dataframe.size())return;
        if(!checkPrintable(dataframe))
            return;
        //end checking
        ip_header *ih;
        udp_header *uh;
        QString dataString = QString::fromLatin1(dataframe);
        ui->listWidget_2->addItem("port:"+ QString::number(port)+"len:"+QString::number(dataframe.length())+"data:"+dataString);*/
    }

}
