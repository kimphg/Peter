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

    if(!pCaptureFile->isPlaying)
    {
        ui->pushButton_replay->setDisabled(false);
    }
    ui->label_time->setText(QString::number(pCaptureFile->timeTotalms/1000.0));
    int stt =0;
    ui->listWidget_ais_list->clear();

    foreach (AIS_object_t target, pCaptureFile->m_aisList) {
        stt++;
        if(target.isNewest)ui->listWidget_ais_list->addItem(
                    "STT:"+QString::number(stt)
                    +"Name:"+target.mName
                    +"MMSI:"+QString::number(target.mMMSI)
                    +"Lat:"+QString::number(target.mLat,'f',4)
                    +"Lon:"+QString::number(target.mLong,'f',4));
    }
    stt =0;
    ui->listWidget_source_list->clear();
    foreach (DataSource source, pCaptureFile->mSourceList) {
        stt++;
        ui->listWidget_source_list->addItem(
                    "STT:"+QString::number(stt)
                    +"IP:"+QString::number(source.addr.byte1)
                    +"."+QString::number(source.addr.byte2)
                    +"."+QString::number(source.addr.byte3)
                    +"."+QString::number(source.addr.byte4));
    }

}
DataCapture *dataCap;
void MainWindow::on_listWidget_doubleClicked(const QModelIndex &index)
{
    dataCap = new DataCapture(pCaptureFile);
    dataCap->setInterface(ui->listWidget->item(index.row())->text());
    dataCap->start();

}

void MainWindow::on_pushButton_replay_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Output file"), NULL, tr("Data files (*.cap)"));
    pCaptureFile = new DataReplay(fileName);
    pCaptureFile->start();
    this->startTimer(30);
    ui->pushButton_replay->setDisabled(true);
}
