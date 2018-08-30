#include "c_mainwindow.h"
#include "statuswindow.h"
#include "ui_mainwindow.h"

//#include "gdal/ogr/ogrsf_frmts/ogrsf_frmts.h"
//#include "gdal/gcore/gdal.h"
//#define mapWidth 2000
//#define mapWidth mapWidth
//#define mapHeight mapWidth
//#define CONST_NM 1.852f// he so chuyen doi tu km sang hai ly
#define MAX_VIEW_RANGE_KM 50
QStringList                 commandLogList;
QPixmap                     *pMap=NULL;// painter cho ban do
//QPixmap                     *pViewFrame=NULL;// painter cho ban do
CMap *osmap ;
bool toolButton_grid_checked = true;
StatusWindow                *mstatWin;
double                      mHeadingT2,mHeadingT,mAziCorrecting;
int                         mRangeLevel = 0;
int                         mDistanceUnit=0;//0:NM;1:KM
double                      mZoomSizeRg = 2;
double                      mZoomSizeAz = 10;
double                      mLat=DEFAULT_LAT,mLon = DEFAULT_LONG;
double                      mHeadingGPSNew = 0,mHeadingGPSOld=0;
bool                        isMapOutdated = true;
bool isHeadUp = false;
short   mMousex =0,mMousey=0;
dataProcessingThread        *processing;// thread xu ly du lieu radar
C_radar_data                *pRadar;
QThread                     *t2,*t1;
QPen penBackground(QBrush(QColor(24 ,48 ,64,255)),250);
QPen penOuterGrid4(QBrush(QColor(255,255,50 ,255)),4);//xoay mui tau
QPen penOuterGrid2(QBrush(QColor(255,255,50 ,255)),2);
QPen mGridViewPen1(QBrush(QColor(150,150,150,255)),1);
QPoint points[6];
double                      mMapOpacity;
int                         mMaxTapMayThu=18;
//Q_vnmap                     vnmap;
QTimer                      timerVideoUpdate,timerMapUpdate;
QTimer                      syncTimer1s,syncTimer5p ;
QTimer                      dataPlaybackTimer ;
bool                        displayAlpha = false;
//QList<CTarget*>             targetDisplayList;
short                       dxMax,dyMax;
C_ARPA_data                 arpa_data;
short                       scrCtX= SCR_H/2 + SCR_LEFT_MARGIN, scrCtY= SCR_H/2;
short                       dx =0,dy=0,dxMap=0,dyMap=0;
short                       radCtX= SCR_H/2 + SCR_LEFT_MARGIN;
short                       radCtY= SCR_H/2;
short                       mZoomCenterx,mZoomCentery,mMouseLastX,mMouseLastY;
//bool                        isDraging = false;
bool                        isScaleChanged =true;
double                      mScale;
double                      centerAzi=0;
double                      temperature[255];
int         curTempIndex = 0;
double      rangeRatio = 1;
//extern CConfig         mGlobbalConfig;
QStringList     warningList;
QString         strDistanceUnit;
short selectedTargetIndex;
mouseMode mouse_mode = MouseNormal;
DialogCommandLog *cmLog;
enum drawModes{
    SGN_DIRECT_DRAW,SGN_IMG_DRAW,NOTERR_DRAW
}drawMode = SGN_IMG_DRAW;
enum TargetType{
    RADAR,AIS,NOTARGET
}selectedTargetType  = NOTARGET;
int targetID = -1;
//short config.getRangeView() = 1;
float ringStep = 1;
double curAziRad = 3;
//typedef struct {
//    unsigned char        bytes[8];
//}
//Command_Control;
//typedef std::queue<Command_Control> CommandList;
//static CommandList command_queue;
bool isDrawSubTg = true;

class guard_zone_t
{
public:
    guard_zone_t(){}
    ~guard_zone_t(){}
    short x1,y1,x2,y2;
    float maxAzi,minAzi;
    float maxR,minR;
    char  isActive;
    void update()
    {
        //float azi,rg;
        //        C_radar_data::kmxyToPolar((x1 - radCtX)/mScale,-(y1 - radCtY)/mScale,&minAzi,&minR);
        //        C_radar_data::kmxyToPolar((x2 - radCtX)/mScale,-(y2 - radCtY)/mScale,&maxAzi,&maxR);
        //        if(minAzi<0)minAzi += PI_NHAN2;
        //        minAzi = minAzi*DEG_RAD;
        //        if(maxAzi<0)maxAzi += PI_NHAN2;
        //        maxAzi = maxAzi*DEG_RAD;
    }
};
guard_zone_t gz1,gz2,gz3;
//static unsigned short cur_object_index = 0;
short lon2x(float lon)
{
    float refLat = mLat*0.00872664625997f;
    return  (- dx + scrCtX + ((lon - mLon) * 111.31949079327357f*cosf(refLat))*mScale);
}
short lat2y(float lat)
{

    return (- dy + scrCtY - ((lat - mLat) * 111.31949079327357f)*mScale);
}
double y2lat(short y)
{
    return (y  )/mScale/111.31949079327357f + mLat;
}
double x2lon(short x)
{
    float refLat = mLat*0.00872664625997;
    return (x  )/mScale/111.31949079327357f/cosf(refLat) + mLon;
}
inline QString demicalDegToDegMin(double demicalDeg)
{
    return QString::number( (short)demicalDeg) +
            QString::fromLocal8Bit("\260")+
            QString::number((demicalDeg-(short)demicalDeg)*60.0,'f',2);
}
void Mainwindow::mouseDoubleClickEvent( QMouseEvent * e )
{
    if ( e->button() == Qt::RightButton )
    {

    }
    else
    {
        if(isInsideViewZone(mMouseLastX,mMouseLastY))
        {
        float xRadar = (mMouseLastX - radCtX)/pRadar->scale_ppi ;//coordinates in  radar xy system
        float yRadar = -(mMouseLastY - radCtY)/pRadar->scale_ppi;
        pRadar->addTrackManual(xRadar,yRadar);
        }
        //ui->toolButton_manual_track->setChecked(false);

    }
    //Test doc AIS

}
void Mainwindow::sendToRadarString(QString command)
{
    command.replace(" ", "");
    QStringList list = command.split(';');
    for(int i=0;i<list.size();i++)
    {
        QByteArray ba=list.at(i).toLatin1();
        sendToRadarHS(ba.data());
    }

}
void Mainwindow::sendToRadarHS(const char* hexdata)//todo:move to radar class
{
    short len = strlen(hexdata)/2;
    unsigned char* sendBuff = new unsigned char[len];
    hex2bin(hexdata,sendBuff);
    processing->sendCommand(sendBuff,len);
    delete[] sendBuff;
}
void Mainwindow::sendToRadar(unsigned char* hexdata)
{
    m_udpSocket->writeDatagram((char*)hexdata,8,QHostAddress("192.168.0.44"),2572);
}
//ham test ve tu AIS
void Mainwindow::drawAisTarget(QPainter *p)
{
    //draw targets
    QPen penTarget(QColor(250,100,250));
    penTarget.setWidth(1);
    QPen penSelectedtarget = penTarget;
    penSelectedtarget.setWidth(2);
    p->setFont(QFont("Times", 6));
    QList<AIS_object_t>::iterator iter = processing->m_aisList.begin();
    while(iter!=processing->m_aisList.end())
    {
        AIS_object_t aisObj = *iter;
        iter++;

        double fx,fy;
        ConvWGSToKm(&fx,&fy,aisObj.mLong,aisObj.mLat);
        short x = (fx*mScale)+radCtX;
        short y = (fy*mScale)+radCtY;
        if((aisObj.mType/10)==3)continue;
        if(aisObj.isNewest)
        {
            //printf("ais draw\n");
            //draw ais mark
            QPolygon poly;
            QPoint point;
            float head = aisObj.mCog*PI_NHAN2/360.0;
            point.setX(x+8*sinf(head));
            point.setY(y-8*cosf(head));
            poly<<point;
            point.setX(x+8*sinf(head+2.3562f));
            point.setY(y-8*cosf(head+2.3562f));
            poly<<point;
            point.setX(x+8*sinf(head-2.3562f));
            point.setY(y-8*cosf(head-2.3562f));
            poly<<point;
            point.setX(x+8*sinf(head));
            point.setY(y-8*cosf(head));
            poly<<point;
            point.setX(x+16*sinf(head));
            point.setY(y-16*cosf(head));
            poly<<point;
            if(aisObj.isSelected)
            {
                p->setPen(penSelectedtarget);
                p->drawPolygon(poly);
            }
            else
            {
                p->setPen(penTarget);
                p->drawPolygon(poly);
                if(ui->toolButton_ais_name->isChecked())
                    p->drawText(x,y,100,20,0,aisObj.mName);
            }


        }
        else
        {
            p->drawPoint(x,y);
        }
    }

}
void Mainwindow::mouseReleaseEvent(QMouseEvent *event)
{
    event = event;
    setMouseMode(MouseDrag,false);
    //    if(isAddingTarget)
    //    {
    //        float xRadar = (mouseX - radCtX)/signsize ;//coordinates in  radar xy system
    //        float yRadar = -(mouseY - radCtY)/signsize;
    //        pRadar->addTrack(xRadar,yRadar);
    //        ui->actionAddTarget->toggle();
    //        isScreenUp2Date = false;
    //        return;
    //    }

    isMapOutdated = true;
    //    isScreenUp2Date = false;
    //isDraging = false;
    /*currMaxRange = (sqrtf(dx*dx+dy*dy)+scrCtY)/signsize;
    if(currMaxRange>RADAR_MAX_RESOLUTION)currMaxRange = RADAR_MAX_RESOLUTION;
    if((dx*dx+dy*dy)*3>scrCtX*scrCtX)
    {
        if(dx<0)
        {
            currMaxAzi = (unsigned short)((atanf((float)dy/(float)dx)-pRadar->trueN)/PI_NHAN2*4096.0f);
            if(currMaxAzi<0)currMaxAzi+=MAX_AZIR;
            if(currMaxAzi>MAX_AZIR)currMaxAzi-=MAX_AZIR;
        }
        if(dx>0)
        {
            currMaxAzi = (unsigned short)(((atanf((float)dy/(float)dx)+PI-pRadar->trueN))/PI_NHAN2*4096.0f);
            if(currMaxAzi>MAX_AZIR)currMaxAzi-=MAX_AZIR;
            if(currMaxAzi<0)currMaxAzi+=MAX_AZIR;
        }
        currMinAzi = currMaxAzi - MAX_AZIR/2;
        if(currMinAzi<0)currMinAzi+=MAX_AZIR;
        //printf("\n currMinAzi:%d currMaxAzi:%d ",currMinAzi,currMaxAzi);
    }else
    {
        currMaxAzi = MAX_AZIR;
        currMinAzi = 0;
    }*/
}
void Mainwindow::wheelEvent(QWheelEvent *event)
{
    event = event;
    //if(event->delta()>0)ui->horizontalSlider->setValue(ui->horizontalSlider->value()+1);
    //if(event->delta()<0)ui->horizontalSlider->setValue(ui->horizontalSlider->value()-1);
}
void Mainwindow::mouseMoveEvent(QMouseEvent *event) {

    if((mouse_mode&MouseDrag)&&(event->buttons() & Qt::LeftButton)) {
        /*if(ui->tabWidget_iad->isDragging)
        {
            ui->tabWidget_iad->move(ui->tabWidget_iad->pos().x()+event->x()-mMouseLastX,
                    ui->tabWidget_iad->pos().y()+event->y()-mMouseLastY
                    );
            mMouseLastX=event->x();
            mMouseLastY=event->y();
        }
        else if(ui->tabWidget_menu->isDragging)
        {
            ui->tabWidget_menu->move(ui->tabWidget_iad->pos().x()+event->x()-mMouseLastX,
                    ui->tabWidget_iad->pos().y()+event->y()-mMouseLastY
                    );
            mMouseLastX=event->x();
            mMouseLastY=event->y();

        }else*/
        {
        short olddx = dx;
        short olddy = dy;

        dx+= mMouseLastX-event->x();
        dy+= mMouseLastY-event->y();

        dxMap += mMouseLastX-event->x();
        dyMap += mMouseLastY-event->y();
        while(dx*dx+dy*dy>dxMax*dxMax)
        {
            if(abs(dx)>abs(dy))
            {
                if(dx>0){dx--;dxMap--;}else {dx++;dxMap++;}}
            else
            {
                if(dy>0){dy--;dyMap--;}else {dy++;dyMap++;}
            }
        }
        mZoomCenterx+= olddx - dx;
        mZoomCentery+= olddy - dy;
        mMouseLastX=event->x();
        mMouseLastY=event->y();
        isMapOutdated = true;
        radCtX = scrCtX-dx;
        radCtY = scrCtY-dy;
        }
    }
}
bool controlPressed = false;
void Mainwindow::keyPressEvent(QKeyEvent *event)
{
    this->setFocus();
    int key = event->key();
    if(key == Qt::Key_Control)
    {
        controlPressed  = true;
    }
    else if(controlPressed)
    {
        if(key==Qt::Key_1)
        {
            mMousex=this->mapFromGlobal(QCursor::pos()).x();
            mMousey=this->mapFromGlobal(QCursor::pos()).y();

            SetGPS(y2lat(-(mMousey - radCtY)),
                   x2lon(mMousex - radCtX),
                   0);

        }
        else if(key==Qt::Key_2)
        {
            pRadar->clearPPI();
        }

    }
    else if(key == Qt::Key_Space)
    {
        mMousex=this->mapFromGlobal(QCursor::pos()).x();
        mMousey=this->mapFromGlobal(QCursor::pos()).y();
        if(!isInsideViewZone(mMousex,mMousey))return;
        double azid,rg;
        C_radar_data::kmxyToPolarDeg((mMousex - radCtX)/mScale,-(mMousey - radCtY)/mScale,&azid,&rg);
        int aziBinary = azid/360.0*4096;
        unsigned char command[]={0xaa,0x55,0x6a,0x08,aziBinary>>8,aziBinary,0x00,0x00,0x00,0x00,0x00,0x00};
        processing->sendCommand(command,9,false);
        mZoomCenterx = mMousex;
        mZoomCentery = mMousey;

        pRadar->setZoomRectAR((mMousex - radCtX)/mScale,
                              -(mMousey - radCtY)/mScale,
                              mZoomSizeRg,mZoomSizeAz);
        pRadar->setZoomRectXY((mMousex - radCtX),(mMousey - radCtY));
    }

}
void Mainwindow::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Control)
    {
        controlPressed  = false;
    }
}


short selZone_x1, selZone_x2, selZone_y1, selZone_y2;
bool isSelectingTarget = false;
void Mainwindow::detectZone()
{
    short sx,sy;
    float scale_ppi = pRadar->scale_ppi;
    if(selZone_x1>selZone_x2)
    {
        short tmp = selZone_x1;
        selZone_x1 = selZone_x2;
        selZone_x2 = tmp;
    }
    if(selZone_y1>selZone_y2)
    {
        short tmp = selZone_y1;
        selZone_y1 = selZone_y2;
        selZone_y2 = tmp;
    }/*
    trackList* trackListPt = &pRadar->mTrackList;
    if(true)
    {
        for(uint trackId=0;trackId<trackListPt->size();trackId++)
        {
            if(!trackListPt->at(trackId).isConfirmed)continue;
            if(trackListPt->at(trackId).isManual)continue;
            if(!trackListPt->at(trackId).state)continue;
            sx = trackListPt->at(trackId).estX*scale_ppi + radCtX;
            sy = -trackListPt->at(trackId).estY*scale_ppi + radCtY;
            if((sx>=selZone_x1)&&(sx<=selZone_x2)&&(sy>selZone_y1)&&(sy<selZone_y2))
            {
                trackListPt->at(trackId).setManual(true);
            }
        }

    }*/
}
bool Mainwindow::isInsideViewZone(short x, short y)
{
    short dx = x-scrCtX;
    short dy = y-scrCtY;
    if((dx*dx+dy*dy)>(SCR_H*SCR_H/4))
        return false;
    else
        return true;
}
void Mainwindow::mousePressEvent(QMouseEvent *event)
{
    mMouseLastX = (event->x());
    mMouseLastY = (event->y());
    if(!isInsideViewZone(mMouseLastX,mMouseLastY))return;
    if(event->buttons() & Qt::LeftButton) {
        if(mouse_mode&MouseAutoSelect)//(ui->toolButton_auto_select->isChecked())
        {
            if(isSelectingTarget)
            {
                selZone_x2 = mMouseLastX;
                selZone_y2 = mMouseLastY;
                detectZone();
                isSelectingTarget = false;
            }else
            {
                selZone_x1 = mMouseLastX;
                selZone_y1 = mMouseLastY;
                isSelectingTarget = true;
            }
        }
        else if(mouse_mode&MouseScope)
        {
            double azid,rg;
            C_radar_data::kmxyToPolarDeg((mMouseLastX - radCtX)/mScale,-(mMouseLastY - radCtY)/mScale,&azid,&rg);

            pRadar->drawRamp(azid);
        }
        /*else if(ui->toolButton_create_zone->isChecked())
        {
            gz1.isActive = 1;
            gz1.x1 = event->x();
            gz1.y1 = event->y();
        }
        else if(ui->toolButton_create_zone_2->isChecked())
        {
            gz2.isActive = 1;
            gz2.x1 = event->x();
            gz2.y1 = event->y();
        }
        else if(ui->toolButton_create_zone_3->isChecked())
        {
            gz3.isActive = 1;
            gz3.x1 = event->x();
            gz3.y1 = event->y();
        }*/
        else
        {
            if(!isHeadUp)setMouseMode(MouseDrag,true);
            //mouse_mode=MouseDrag;//isDraging = true;
        }
    }
    if(event->buttons() & Qt::RightButton)
    {
        //select radar target
        trackList* trackListPt = &pRadar->mTrackList;
        for(uint trackId=0;trackId<trackListPt->size();trackId++)
        {/*
            if(!trackListPt->at(trackId).isConfirmed)continue;
            if(!trackListPt->at(trackId).isManual)continue;
            //if(trackListPt->at(trackId).state<5)continue;
            short sx = trackListPt->at(trackId).estX*pRadar->scale_ppi + radCtX;
            short sy = -trackListPt->at(trackId).estY*pRadar->scale_ppi + radCtY;
            if( qAbs(sx-event->x()) <5 && qAbs(sy-event->y())<5)
            {
                selectedTargetType = RADAR;
                selectedTargetIndex = trackId;
            }*/
        }


        //select ais target
        if(ui->toolButton_ais_show->isChecked())
        {
            checkClickAIS(event->x(),event->y());
        }
    }

}
void Mainwindow::checkClickAIS(int xclick, int yclick)
{
    QList<AIS_object_t>::iterator iter = processing->m_aisList.begin();
    while(iter!=processing->m_aisList.end())
    {
        AIS_object_t aisObj = *iter;
        iter++;
        if(aisObj.isSelected)continue;
        if(!aisObj.isNewest)continue;
        double fx,fy;
        ConvWGSToKm(&fx,&fy,aisObj.mLong,aisObj.mLat);
        int x = (fx*mScale)+radCtX;
        int y = (fy*mScale)+radCtY;
        if(abs(x-xclick)<5&&abs(y-yclick)<5)
        {
            DialogAisInfo *dialog = new DialogAisInfo(this);
            dialog->setAttribute( Qt::WA_DeleteOnClose, true );
            dialog->setWindowFlags(dialog->windowFlags()&(~Qt::WindowContextHelpButtonHint));
            dialog->setFixedSize(dialog->width(),dialog->height());
            dialog->setAisData(&processing->m_aisList,aisObj.mMMSI);
            dialog->show();
            break;
        }
    }
}
/*void MainWindow::wheelEvent(QWheelEvent *event)
{
//    if(event->delta()>0)ui->horizontalSlider->raise();
//    if(event->delta()<0)ui->horizontalSlider->setValue(3);
//    if(scale>SCALE_MAX)scale=SCALE_MAX;
//    if(scale<SCALE_MIN)scale=SCALE_MIN;
//    //signsize = SIGNAL_SCALE/scale;
//    DrawMap();
//    update();
}*/
Mainwindow::Mainwindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //ui->frame_RadarViewOptions->hide();
    QFont font;
    font.setPointSize(12);
    cmLog = new DialogCommandLog();
    mShowobjects = false;
    mShowLines = false;
    mShowTracks = false;
    InitNetwork();
    InitTimer();
    setFocusPolicy(Qt::StrongFocus);
    InitSetting();
    setRadarState(DISCONNECTED);
    //    GDALAllRegister();
    //    GDALDataset       *poDS;

    //init drawing context

    //this->setFixedSize(900 + ui->toolBar_Main->width()*3,850);
    //scale = SCALE_MIN;



    //isSettingUp2Date = false;
    //UpdateSetting();

}

//void Mainwindow::DrawSignal(QPainter *p)
//{


//}

//void MainWindow::createMenus()
//{
//    m_fileMenu = menuBar()->addMenu(tr("&File"));
//    m_fileMenu->addAction(a_openShp);
//    m_fileMenu->addAction(a_openPlace);
//    m_fileMenu->addAction(a_openSignal);

//    //
//    m_connectionMenu = menuBar()->addMenu(tr("&Connect"));
//    m_connectionMenu->addAction(a_gpsOption);
//}
void Mainwindow::gpsOption()
{
    //GPSDialog *dlg = new GPSDialog;
    //dlg->show();
}

void Mainwindow::PlaybackRecFile()//
{


}
//void MainWindow::createActions()
//{
//    a_openShp = new QAction(tr("&Open Shp"), this);
//    a_openShp->setShortcuts(QKeySequence::Open);
//    a_openShp->setStatusTip(tr("Open shp file"));
//    connect(a_openShp, SIGNAL(triggered()), this, SLOT(openShpFile()));
//    //______________________________________//
//    a_openPlace = new QAction(tr("&Open place file"), this);
//    a_openPlace->setStatusTip(tr("Open place file"));
//    connect(a_openPlace, SIGNAL(triggered()), this, SLOT(openPlaceFile()));
//    //______________________________________//
//    a_gpsOption = new QAction(tr("&GPS option"), this);
//    a_gpsOption->setStatusTip(tr("GPS option"));
//    connect(a_gpsOption, SIGNAL(triggered()), this, SLOT(gpsOption()));
//    //______________________________________//
//    a_openSignal = new QAction(tr("&Open signal file"), this);
//    a_openSignal->setStatusTip(tr("Mở file tín hiệu đã lưu."));
//    connect(a_openSignal, SIGNAL(triggered()), this, SLOT(openSignalFile()));

//}
//void MainWindow::openSignalFile()
//{
//    //printf("shp file max ");
//    QString fileName = QFileDialog::getOpenFileName(this,
//        tr("Open signal file"), NULL, tr("Signal data Files (*.dat)"));
//    rawData.OpenFile(fileName.toStdString().c_str());

//    //SHPHandle hSHP = SHPOpen(fileName.toStdString().c_str(), "rb" );
//    //if(hSHP == NULL) return;
//}
/*
static short curMapLayer=0;

void MainWindow::openShpFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open SHP file"), NULL, tr("Shp Files (*.shp)"));
    if(!fileName.size())return;
    vnmap.OpenShpFile(fileName.toStdString().c_str(), curMapLayer );
    vnmap.LoadPlaces(fileName.toStdString().c_str());
    curMapLayer++;
    DrawMap();
    //DrawToPixmap(pPixmap);
    update();

}*/

Mainwindow::~Mainwindow()
{
    delete ui;
    CConfig::SaveToFile();
    if(pMap)delete pMap;
}

void Mainwindow::DrawMap()
{
    if(!isMapOutdated)return;
    isMapOutdated = false;
    if(!pMap) return;
    pMap->fill(Qt::black);
    dxMap = 0;
    dyMap = 0;
    //
    QPainter pMapPainter(pMap);
    double dLat, dLong;
    ConvKmToWGS((double(dx))/mScale,(double(-dy))/mScale,&dLong,&dLat);
    osmap->setCenterPos(dLat,dLong);
    QPixmap pix = osmap->getImage(mScale);
    if(isHeadUp)
    {
        QTransform transform;
        QTransform trans = transform.rotate(-mHeadingGPSOld);
        pix=pix.transformed(trans);
    }
    pMapPainter.setOpacity(mMapOpacity);
    pMapPainter.drawPixmap((-pix.width()/2+pMap->width()/2),
                 (-pix.height()/2+pMap->height()/2),pix.width(),pix.height(),pix
                 );

    //view frame
    //fill back ground
    //p.setBrush(QColor(40,60,100,255));
    //p.drawRect(scrCtX+scrCtY,0,width()-scrCtX-scrCtY,height());
    //p.drawRect(0,0,scrCtX-scrCtY,height());
    //p.setCompositionMode(QPainter::CompositionMode_SourceOver);
    pMapPainter.setOpacity(1);
    //grid
    /*if(toolButton_grid_checked)
    {

        if(ui->toolButton_measuring->isChecked())
        {
            DrawGrid(&pMapPainter,mMouseLastX-SCR_LEFT_MARGIN,mMouseLastY);
        }
        else
        {
            DrawGrid(&pMapPainter,scrCtX- SCR_LEFT_MARGIN-dx,radCtY);
        }
    }
    //frame
    pMapPainter.setBrush(Qt::NoBrush);
    pMapPainter.setPen(penBackground);
    short i=200;
    pMapPainter.drawEllipse(-i/2,-i/2,SCR_H+i,SCR_H+i);*/

}
void Mainwindow::DrawGrid(QPainter* p,short centerX,short centerY)
{
    p->setCompositionMode(QPainter::CompositionMode_Plus);
    //return;
    QPen pen(QColor(150,150,150,0xff));

    pen.setStyle(Qt::DashLine);
    QFont font;
    font.setPointSize(10);
    p->setFont(font);
    p->setBrush(QBrush(Qt::NoBrush));
    p->setPen(pen);
    p->drawLine(centerX-5,centerY,centerX+5,centerY);
    p->drawLine(centerX,centerY-5,centerX,centerY+5);
    //pen.setColor(QColor(30,90,150,120));
    pen.setWidth(1);
    p->setPen(pen);
    for(short i = 1;i<8;i++)
    {
        int rad = i*ringStep*rangeRatio*mScale;
        p->drawEllipse(QPoint(centerX,centerY),
                       (short)(rad),
                       (short)(rad));
        p->drawText(centerX+2,centerY-rad+2,100,20,0,QString::number(i*ringStep)+strDistanceUnit);
        p->drawText(centerX+2,centerY+rad+2,100,20,0,QString::number(i*ringStep)+strDistanceUnit);
        p->drawText(centerX+rad+2,centerY+2,100,20,0,QString::number(i*ringStep)+strDistanceUnit);
        p->drawText(centerX-rad+2,centerY+2,100,20,0,QString::number(i*ringStep)+strDistanceUnit);
    }


    short theta;
    short gridR = ringStep*1.852f*mScale*7;
    for(theta=0;theta<360;theta+=90){
        QPoint point1,point2;
        short dx = gridR*cosf(theta/DEG_RAD);
        short dy = gridR*sinf(theta/DEG_RAD);
        point1.setX(centerX);
        point1.setY(centerY);
        point2.setX(centerX+dx);
        point2.setY(centerY+dy);
        p->drawLine(point1,point2);

    }
    for(theta=0;theta<360;theta+=30){
        QPoint point1,point2;
        short dx = gridR*cosf(theta/DEG_RAD);
        short dy = gridR*sinf(theta/DEG_RAD);
        point1.setX(centerX);
        point1.setY(centerY);
        point2.setX( centerX+dx);
        point2.setY(centerY+dy);
        p->drawLine(point1,point2);
        point2.setX(centerX+dx*1.02-9);
        point2.setY(centerY+dy*1.02+5);
        //                if(theta<270)p->drawText(point2,QString::number(theta+90));
        //                else p->drawText(point2,QString::number(theta-270));

    }

    //end grid
    p->setCompositionMode(QPainter::CompositionMode_SourceOver);



}

void Mainwindow::initGraphicView()
{
    //scene = new QGraphicsScene(-200, -200, 400, 400);
    //view = new jViewPort(scene,this);
    //view->setGeometry(SCR_LEFT_MARGIN,0,SCR_H,SCR_H);
    //view->lower();
    //view->setRenderHint(QPainter::Antialiasing);
    //view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    //view->setBackgroundBrush(Qt::transparent);

}

void Mainwindow::DrawRadarTargetByPainter(QPainter* p)//draw radar target from pRadar->mTrackList
{
    QPen penTarget(Qt::magenta);
    penTarget.setWidth(1);
    QPen penSelTarget(Qt::magenta);
    penSelTarget.setWidth(2);
    penSelTarget.setStyle(Qt::DashLine);
    QPen penTargetBlue(Qt::cyan);
    penTargetBlue.setWidth(1);
    //penTargetBlue.setStyle(Qt::DashLine);
    //QPen penARPATrack(Qt::darkYellow);
    //draw radar targets
    //float x,y;
    short sx,sy;
    short sx1,sy1;
    //float scale_ppi = pRadar->scale_ppi;
    //short targetId = 0;
    std::vector<object_t>* pObjList = &(pRadar->mObjList);
    p->setPen(penTargetBlue);
    if(mShowobjects)//raw objects
    {
        foreach (object_t obj, *pObjList) {
            if(obj.uniqID<0)continue;
            sx = obj.xkm*mScale + radCtX;
            sy = -obj.ykm*mScale + radCtY;
            //p->drawPoint(sx,sy);
            p->drawRect(sx-5,sy-5,10,10);
        }

    }
    penTargetBlue.setWidth(2);
    p->setPen(penTargetBlue);
    if(mShowLines)//raw lines
    {
        foreach (object_line line, pRadar->mLineList) {
            if(line.isDead)continue;
            sx = line.obj1.xkm*mScale + radCtX;
            sy = -line.obj1.ykm*mScale + radCtY;
            sx1 = line.obj2.xkm*mScale + radCtX;
            sy1 = -line.obj2.ykm*mScale + radCtY;
            p->drawLine(sx,sy,sx1,sy1);

            //p->drawRect(sx-5,sy-5,10,10);
        }

    }
    p->setPen(penTarget);
    if(mShowTracks)//raw tracks
    {
        foreach (track_t track, pRadar->mTrackList) {
            sx = track.xkm*mScale + radCtX;
            sy = -track.ykm*mScale + radCtY;
            sx1 = track.xkmo*mScale + radCtX;
            sy1 = -track.ykmo*mScale + radCtY;
            p->drawLine(sx,sy,sx1,sy1);

            //p->drawRect(sx-5,sy-5,10,10);
        }

    }
    /*
    trackList* trackListPt = &pRadar->mTrackList;
    if(true)//ui->toolButton_blue_tracks->isChecked())
    {
        for(uint trackId=0;trackId<trackListPt->size();trackId++)
        {
            if(!trackListPt->at(trackId).isConfirmed)continue;
            if(trackListPt->at(trackId).isManual)continue;
            //if(trackListPt->at(trackId).state<5)continue;
            sx = trackListPt->at(trackId).estX*scale_ppi + radCtX;
            sy = -trackListPt->at(trackId).estY*scale_ppi + radCtY;
            p->setPen(penTargetBlue);
            p->drawRect(sx-5,sy-5,10,10);
        }

    }

    //draw red targets
    for(uint trackId=0;trackId<trackListPt->size();trackId++)
    {
        if(!trackListPt->at(trackId).isManual)continue;
        if(!trackListPt->at(trackId).isLost)
        {
            //x= trackListPt->at(trackId).estX*scale_ppi/mScale;
            //y= trackListPt->at(trackId).estY*scale_ppi/mScale;
            sx = trackListPt->at(trackId).estX*scale_ppi + radCtX;
            sy = -trackListPt->at(trackId).estY*scale_ppi + radCtY;
            if(trackListPt->at(trackId).dopler==17)//diem dau dat bang tay
            {
                p->setPen(penTargetBlue);
                p->drawRect(sx-6,sy-6,12,12);
                continue;
            }
            else if(trackListPt->at(trackId).isManual)
            {
                p->setPen(penTarget);
                //ve huong di chuyen
                if(trackListPt->at(trackId).object_list.size()>12)
                {
                    sx = trackListPt->at(trackId).estX*scale_ppi + radCtX;
                    sy =-trackListPt->at(trackId).estY*scale_ppi + radCtY;
                    p->drawLine(sx,sy,sx+15*sin(trackListPt->at(trackId).heading),sy-15*cos(trackListPt->at(trackId).heading));
                }
                //ve so hieu MT
                p->drawText(sx+7,sy+7,300,40,0,QString::number(trackListPt->at(trackId).idCount));
                //ve lich su qui dao
                if(selectedTargetIndex==trackId)
                {
                    for(uint j=0 ;j<trackListPt->at(trackId).object_list.size();j+=3)
                    {
                        sx = trackListPt->at(trackId).object_list.at(j).xkm*mScale + radCtX;
                        sy = -trackListPt->at(trackId).object_list.at(j).ykm*mScale + radCtY;
                        p->drawPoint(sx,sy);
                    }
                    p->drawRect(sx-6,sy-6,12,12);
                    p->setPen(penSelTarget);
                    p->drawRect(sx-9,sy-9,18,18);
                }
                else
                {
                    p->drawRect(sx-6,sy-6,12,12);

                }
                continue;
            }
        }*/
        //                draw track:

        //                j--;
        //                if(j<0)continue;
        //printf("red");
        /*if(trackListPt->at(trackId).confirmed)
                {
                    p->setPen(penTargetRed);

                }
                else
                {
                    p->setPen(penTargetBlue);
                }
                p->drawRect(x-6,y-6,12,12);
                p->drawText(x+5,y+5,300,40,0,QString::number(trackListPt->at(trackId).state)
                            + "-" + QString::number(trackListPt->at(trackId).terrain)
                            + "-" + QString::number(trackListPt->at(trackId).dopler),0);*/
        /*if(false)//trackListPt->at(i).isMoving) // moving obj
                {

                    QPolygon poly;
                    QPoint   point,point2;
                    point2.setX(x+trackListPt->at(i).velocity*500*sinf(trackListPt->at(i).course));
                    point2.setY(y-trackListPt->at(i).velocity*500*cosf(trackListPt->at(i).course));

                    point.setX(x+10*sinf(trackListPt->at(i).course));
                    point.setY(y-10*cosf(trackListPt->at(i).course));
                    p->setPen(penTargetBlue);
                    p->drawLine(point,point2);
                    poly<<point;
                    point.setX(x+10*sinf(trackListPt->at(i).course+2.3562));
                    point.setY(y-10*cosf(trackListPt->at(i).course+2.3562));
                    poly<<point;
                    point.setX(x);
                    point.setY(y);
                    poly<<point;
                    point.setX(x+10*sinf(trackListPt->at(i).course-2.3562));
                    point.setY(y-10*cosf(trackListPt->at(i).course-2.3562));
                    poly<<point;
                    point.setX(x+10*sinf(trackListPt->at(i).course));
                    point.setY(y-10*cosf(trackListPt->at(i).course));
                    poly<<point;
                    p->setPen(penTargetRed);
                    p->drawPolygon(poly);
                    p->drawText(x-30,y-20,100,40,0,QString::number(i+1),0);
                }else*/

        /*}
            else if(trackListPt->at(i).tclass==BLUE_OBJ)
            {
                p->setPen(penTargetBlue);
                //printf("b");
                p->drawRect(x-6,y-6,12,12);
                p->drawText(x-30,y-20,100,40,0,QString::number(i+1),0);
                p->drawLine(x,
                            y,
                            x+trackListPt->at(i).velocity*500*sinf(trackListPt->at(i).course),
                            y-trackListPt->at(i).velocity*500*cosf(trackListPt->at(i).course));

            }

    }*/
    /*else for(uint i=0;i<trackListPt->size();i++)
    {
        if(!trackListPt->at(i).state)continue;
        short x,y;
        p->setPen(penTrack);
        short j;
        //draw track:
        for(j=0;j<((short)trackListPt->at(i).object_list.size());j++)
        {
            x = (trackListPt->at(i).object_list[j].x + RADAR_MAX_RESOLUTION)*signsize - (RADAR_MAX_RESOLUTION*signsize-scrCtX)-dx;
            y = (RADAR_MAX_RESOLUTION - trackListPt->at(i).object_list[j].y)*signsize - (RADAR_MAX_RESOLUTION*signsize-scrCtY)-dy;
            p->drawPoint(x,y);
        }
        j--;
        if(j<0)continue;


            if(trackListPt->at(i).isMoving) // moving obj
            {

                QPolygon poly;
                QPoint   point,point2;
                point2.setX(x+trackListPt->at(i).velocity*500*sinf(trackListPt->at(i).course));
                point2.setY(y-trackListPt->at(i).velocity*500*cosf(trackListPt->at(i).course));

                point.setX(x+10*sinf(trackListPt->at(i).course));
                point.setY(y-10*cosf(trackListPt->at(i).course));
                p->setPen(penTargetSub);
                p->drawLine(point,point2);
                poly<<point;
                point.setX(x+10*sinf(trackListPt->at(i).course+2.3562));
                point.setY(y-10*cosf(trackListPt->at(i).course+2.3562));
                poly<<point;
                point.setX(x);
                point.setY(y);
                poly<<point;
                point.setX(x+10*sinf(trackListPt->at(i).course-2.3562));
                point.setY(y-10*cosf(trackListPt->at(i).course-2.3562));
                poly<<point;
                point.setX(x+10*sinf(trackListPt->at(i).course));
                point.setY(y-10*cosf(trackListPt->at(i).course));
                poly<<point;
                p->setPen(penTarget);
                p->drawPolygon(poly);
                p->drawText(x-30,y-20,100,40,0,QString::number(i+1),0);
            }else
            {
                p->setPen(penTarget);
                p->drawRect(x-6,y-6,12,12);
                p->drawText(x-30,y-20,100,40,0,QString::number(i+1),0);
            }


    }*/
    //draw arpa targets
    /*
    for(uint i=0;i<processing->arpaData->track_list.size();i++)
    {
        short x,y;
        if(!processing->arpaData->track_list[i].lives)continue;
        for(uint j=0;j<(processing->arpaData->track_list[i].object_list.size());j++)
        {
            x = processing->arpaData->track_list[i].object_list[j].centerX*scale+(radCtX);
            y = processing->arpaData->track_list[i].object_list[j].centerY*scale+(radCtY);
            //printf("\n x:%d y:%d",x,y);
            p->setPen(penARPATrack);
            p->drawPoint(x,y);
        }
        QPolygon poly;
        QPoint point;

        point.setX(x+10*sinf(processing->arpaData->track_list[i].course));
        point.setY(y-10*cosf(processing->arpaData->track_list[i].course));
        poly<<point;
        point.setX(x+10*sinf(processing->arpaData->track_list[i].course+2.3562f));
        point.setY(y-10*cosf(processing->arpaData->track_list[i].course+2.3562f));
        poly<<point;
        point.setX(x);
        point.setY(y);
        poly<<point;
        point.setX(x+10*sinf(processing->arpaData->track_list[i].course-2.3562f));
        point.setY(y-10*cosf(processing->arpaData->track_list[i].course-2.3562f));
        poly<<point;
        /*if(processing->arpaData->track_list[i].selected)
        {
            char buf[50];
            p.setPen(penyellow);
            sprintf(buf, "%3d:%3.3fNM:%3.3f\260",processing->arpaData->track_list[i].id,processing->arpaData->track_list[i].centerR/DEFAULT_NM, processing->arpaData->track_list[i].centerA*57.2957795);
            QString info = QString::fromAscii(buf);
            p.drawText(10,infoPosy,150,20,0,info);
            infoPosy+=20;
            if(processing->arpaData->track_list[i].id==curTargetId)
            {
                p.setPen(penyellow);
                p.setBrush(Qt::red);
            }
                else
            {
                p.setPen(penTarget);
                p.setBrush(Qt::red);
            }

        }else
        {

            p.setPen(pensubtaget);
            p.setBrush(QColor(100,100,50,100));
        }
        p->setPen(penARPATarget);
        //p->setBrush(Qt::red);
        p->drawPolygon(poly);
        p->drawText(x-20,y-20,20,40,0,QString::number(processing->arpaData->track_list[i].id),0);

    }*/

}
void Mainwindow::ConvWGSToKm(double* x, double *y, double m_Long,double m_Lat)
{

    double refLat = (mLat + (m_Lat))*0.00872664625997;//pi/360
    *x	= (((m_Long) - mLon) * 111.31949079327357)*cos(refLat);// 3.14159265358979324/180.0*6378.137);//deg*pi/180*rEarth
    *y	= ((mLat - (m_Lat)) * 111.132954);
    //tinh toa do xy KM so voi diem center khi biet lat-lon
}
void Mainwindow::ConvKmToWGS(double x, double y, double *m_Long, double *m_Lat)
{

    *m_Lat  = mLat +  (y)/(111.132954);
    double refLat = (mLat +(*m_Lat))*0.00872664625997;//3.14159265358979324/180.0/2;
    *m_Long = (x)/(111.31949079327357*cos(refLat))+ mLon;
    //tinh toa do lat-lon khi biet xy km (truong hop coi trai dat hinh cau)
}
void Mainwindow::drawAisTarget2(QPainter *p)
{
    /*
    //draw radar  target:
    QPen penTargetRed(QColor(255,50,150));
    penTargetRed.setWidth(0);
    for(uint i=0;i<processing->m_AISList.size();i++)
    {
            p->setPen(penTargetRed);
//            short j;
            //draw track:
            double fx,fy;
//            float mlat = m_trackList.at(i).getLat();
//            mlat =  mlat/bit23* 180.0f ;
//            float mlon = m_trackList.at(i).mLong_double;
//            mlon = mlon/bit23* 180.0f ;
                ConvWGSToKm(&fx,&fy,processing->m_AISList.at(i).getLon(),processing->m_AISList.at(i).getLat());

                short x = (fx*mScale)+radCtX;
                short y = (fy*mScale)+radCtY;
                //draw ais mark
                QPolygon poly;
                QPoint point;
                float head = processing->m_AISList.at(i).m_Head*PI_NHAN2/(1<<16);
                point.setX(x+8*sinf(head));
                point.setY(y-8*cosf(head));
                poly<<point;
                point.setX(x+8*sinf(head+2.3562f));
                point.setY(y-8*cosf(head+2.3562f));
                poly<<point;
                point.setX(x);
                point.setY(y);
                poly<<point;
                point.setX(x+8*sinf(head-2.3562f));
                point.setY(y-8*cosf(head-2.3562f));
                poly<<point;
                p->drawPolygon(poly);
                //draw ais name
                if(ui->toolButton_ais_name->isChecked())
                {
                    QFont font = p->font() ;
                    font.setPointSize(6);
                    p->setFont(font);
                    p->drawText(x+5,y+10,(processing->m_AISList.at(i).m_szName));
                }
//                p->drawText(x+5,y+5,QString::fromAscii((char*)&m_trackList.at(i).m_MMSI[0],9));
                //printf("\nj:%d,%d,%d,%f,%f",j,x,y,arpa_data.ais_track_list[i].object_list[j].mlong,arpa_data.ais_track_list[i].object_list[j].mlat);

    }*/
}
void Mainwindow::UpdateMouseStat(QPainter *p)
{

    if(!isInsideViewZone(mMousex,mMousey))return;
    if((mouse_mode&MouseVRM)||(mouse_mode&MouseELB))
    {
        QPen penmousePointer(QColor(0x50ffffff));
        penmousePointer.setWidth(2);

        short r = sqrt((mMousex - radCtX)*(mMousex - radCtX)+(mMousey - radCtY)*(mMousey - radCtY));
        p->setPen(penmousePointer);
        if(mouse_mode&MouseVRM)p->drawEllipse(QPoint(radCtX,radCtY),r,r);
        if(mouse_mode&MouseELB)p->drawLine(QPoint(radCtX,radCtY),QPoint(radCtX-(radCtX-mMousex)*100,radCtY-(radCtY-mMousey)*100));
    }
    if(isSelectingTarget)
    {

        QPen penmousePointer(QColor(0x50ffffff));
        penmousePointer.setWidth(2);
        penmousePointer.setStyle(Qt::DashDotLine);
        p->setPen(penmousePointer);
        p->drawLine(selZone_x1,selZone_y1,mMousex,selZone_y1);
        p->drawLine(selZone_x1,selZone_y1,selZone_x1,mMousey);
        p->drawLine(selZone_x1,mMousey,mMousex,mMousey);
        p->drawLine(mMousex,selZone_y1,mMousex,mMousey);
    }
}
void Mainwindow::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    if(pMap)
    {
        p.drawPixmap(scrCtX-scrCtY,0,SCR_H,SCR_H,
                     *pMap,
                     dxMap,dyMap,SCR_H,SCR_H);
    }
    //draw signal
    QRectF signRect(RAD_DISPLAY_RES-(radCtX),RAD_DISPLAY_RES-(radCtY),SCR_W,SCR_H);
    QRectF screen(0,0,SCR_W,SCR_H);
    p.drawImage(screen,*pRadar->img_ppi,signRect,Qt::AutoColor);

    DrawRadarTargetByPainter(&p);
    //if(ui->toolButton_ais_show->isChecked())drawAisTarget(&p);
    //draw cursor
    //    QPen penmousePointer(QColor(0x50ffffff));

    //    penmousePointer.setWidth(2);
    //    p.setPen(penmousePointer);
    //    p.drawLine(mousePointerX-15,mousePointerY,mousePointerX-10,mousePointerY);
    //    p.drawLine(mousePointerX+15,mousePointerY,mousePointerX+10,mousePointerY);
    //    p.drawLine(mousePointerX,mousePointerY-10,mousePointerX,mousePointerY-15);
    //    p.drawLine(mousePointerX,mousePointerY+10,mousePointerX,mousePointerY+15);
    //draw mouse coordinates
    UpdateMouseStat(&p);
    if(ui->toolButton_ais_show->isChecked())drawAisTarget(&p);
    //ve luoi cu ly phuong vi

    DrawViewFrame(&p);
    DrawIADArea(&p);

    //    updateTargets();
}
void Mainwindow::DrawIADArea(QPainter* p)
{
    if(ui->tabWidget_iad->isHidden())return;
    p->setCompositionMode(QPainter::CompositionMode_SourceOver);
    QRect rect = ui->tabWidget_iad->geometry();
    rect.adjust(4,30,-5,-5);
    p->setBrush(QBrush(Qt::black));
    p->drawRect(rect);
    if(ui->tabWidget_iad->currentIndex()==0)
    {
        p->setPen(QPen(Qt::white,2));
        QPoint p1(rect.x(),rect.y());
        //QPoint p2(rect.x(),rect.y());
        QPoint p11(rect.x()+rect.width(),rect.y());
        QPoint p22(rect.x(),rect.y()+rect.height());
        p->drawLine(p1,p11);
        p->drawLine(p1,p22);
        int step = rect.width()/5;
        for(int i = 0;i<5;i++)
        {
            p->drawLine(rect.x()+step*i,rect.y(),rect.x()+step*i,rect.y()+5);
            p->drawLine(rect.x(),rect.y()+step*i,rect.x()+5,rect.y()+step*i);
        }
        p->setFont(QFont("Times",10));
        p->drawText(rect.x()+rect.width()-50,rect.y()+15,QString::number(mZoomSizeRg/0.1852,'f',1)+QString::fromUtf8(" Liên"));
        p->drawText(rect.x()+5,rect.y()+rect.height()-5,QString::number(mZoomSizeAz,'f',1)+QString::fromUtf8(" Độ"));
        if(pRadar->img_zoom_ar)
        {
            QImage img = pRadar->img_zoom_ar->scaled(rect.width(),rect.height(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
            p->drawImage(rect,img);//todo:resize

        }

    }
    else if(ui->tabWidget_iad->currentIndex()==4)
    {
        //C_radar_data *prad = pRadar;
         p->drawImage(rect,*pRadar->img_zoom_ppi,pRadar->img_zoom_ppi->rect());
        if(mRangeLevel>2)
        {
            short zoom_size = ui->tabWidget_iad->width()/pRadar->scale_zoom_ppi*pRadar->scale_ppi;
            p->setPen(QPen(QColor(255,255,255,200),0,Qt::DashLine));
            p->setBrush(Qt::NoBrush);
            p->drawRect(mZoomCenterx-zoom_size/2.0,mZoomCentery-zoom_size/2.0,zoom_size,zoom_size);
        }

    }
    else if(ui->tabWidget_iad->currentIndex()==1)
    {

        p->drawImage(rect,*pRadar->img_histogram,
                     pRadar->img_histogram->rect());

    }
    else if(ui->tabWidget_iad->currentIndex()==2)
    {

        p->drawImage(rect,*pRadar->img_spectre,
                     pRadar->img_spectre->rect());
    }
    else if(ui->tabWidget_iad->currentIndex()==3)
    {
        if(ui->toolButton_scope_2->isChecked()==false)pRadar->drawRamp();
        QRect rect1 = rect;
        rect1.adjust(0,0,0,-rect.height()/2);
        //        pengrid.setWidth(10);
        //        p->setPen(pengrid);
        p->drawImage(rect1,*pRadar->img_RAmp);
        double rampos = ui->horizontalSlider_ramp_pos_2->value()/(double(ui->horizontalSlider_ramp_pos_2->maximum()));
        QRect rect2 = rect;
        rect2.adjust(0,rect.height()/2,0,0);
        int zoomw = rect2.width()/2;
        int ramposInt = (pRadar->img_RAmp->width()-zoomw)*rampos;
        QRect srect(ramposInt,0,zoomw,pRadar->img_RAmp->height());
        p->drawImage(rect2,*pRadar->img_RAmp,srect);
        //p->drawRect(rect1,pRadar->img_RAmp->width()+5,pRadar->img_RAmp->height()+5);
        //        pengrid.setWidth(2);
        //        pengrid.setColor(QColor(128,128,0,120));
        //        p->setPen(pengrid);
        //        for(short i=60;i<pRadar->img_RAmp->height();i+=50)
        //        {
        //            p->drawLine(0,height()-i,pRadar->img_RAmp->width()+5,height()-i);
        //        }
        //        for(short i=110;i<pRadar->img_RAmp->width();i+=100)
        //        {
        //            p->drawLine(i,height()-266,i,height());
        //        }
    }
}
//void MainWindow::keyPressEvent(QKeyEvent *event)
//{
//    if(event->key() == Qt::Key_F1)
//    {
//    selectobject = true;
//    }
//    switch(event->key())
//    {
//    case Qt::Key_Alt:
//        if(ui->menuBar->isVisible())
//            ui->menuBar->hide();
//        else
//            ui->menuBar->show();
//        break;
//    default:
//        break;
//    }

//}


void Mainwindow::SaveBinFile()
{
    //vnmap.SaveBinFile();

}
void Mainwindow::setDistanceUnit(int unit)//0:NM, 1:KM
{
    mDistanceUnit = unit;
    CConfig::setValue("mDistanceUnit",mDistanceUnit);
    if(mDistanceUnit==0)
    {
        rangeRatio = 1.852;
        strDistanceUnit = "NM";
        ui->toolButton_setRangeUnit->setText(QString::fromUtf8("Đơn vị đo:NM"));
        UpdateScale();
    }
    else if(mDistanceUnit==1)
    {
        rangeRatio = 1.0;
        strDistanceUnit = "KM";
        ui->toolButton_setRangeUnit->setText(QString::fromUtf8("Đơn vị đo:KM"));
        UpdateScale();
    }
    isMapOutdated = true;
}
void Mainwindow::InitSetting()
{
    ui->tabWidget_iad->SetTransparent(true);
    QApplication::setOverrideCursor(Qt::CrossCursor);
    QString systemCommand = CConfig::getString("systemCommand","D:\\HR2D\\cudaCv.exe");
    if(systemCommand.size()){
        systemCommand= "start "+systemCommand;
    system((char*)systemCommand.toStdString().data());}
    mMaxTapMayThu = CConfig::getInt("mMaxTapMayThu");
    mRangeLevel = CConfig::getInt("mRangeLevel");
    assert(mRangeLevel>=0&&mRangeLevel<8);
    setDistanceUnit(CConfig::getInt("mDistanceUnit"));
    assert(mDistanceUnit>=0&&mDistanceUnit<2);

    mHeadingT2 = CConfig::getDouble("mHeadingT2",0);
    mHeadingT = CConfig::getDouble("mHeadingT",0);
    mAziCorrecting = CConfig::getDouble("mAziCorrecting",0);
    pRadar->setAziOffset(mHeadingT);
    ui->textEdit_heading->setText(CConfig::getString("mHeadingT"));
    ui->textEdit_heading_2->setText(CConfig::getString("mHeadingT2"));
    mZoomSizeAz = CConfig::getDouble("mZoomSizeAz");
    ui->textEdit_size_ar_a->setText(QString::number(mZoomSizeAz));
    mZoomSizeRg = CConfig::getDouble("mZoomSizeRg");
    ui->textEdit_size_ar_r->setText(QString::number(mZoomSizeRg));
    pRadar->setAziOffset(mHeadingT);
    //load map
    osmap = new CMap();
    SetGPS(CConfig::getDouble("mLat"), CConfig::getDouble("mLon"),0);
    osmap->setCenterPos(mLat,mLon);
    osmap->setImgSize(SCR_H,SCR_H);
    osmap->SetType(0);
    mMapOpacity = CConfig::getDouble("mMapOpacity");
    //config.setMapOpacity(value/50.0);
        ui->horizontalSlider_map_brightness->setValue(mMapOpacity*50);
    //    ui->toolButton_xl_nguong_3->setChecked(true);
    ui->groupBox_control->setHidden(true);
    //    ui->groupBox_control_setting->setHidden(true);
    setMouseTracking(true);
    //initGraphicView();21.433170, 106.624043
    //init the guard zone
    gz1.isActive = 0;
    gz2.isActive = 0;
    gz3.isActive = 0;
    //    ui->groupBox_3->setCurrentIndex(0);
    ui->tabWidget_iad->setCurrentIndex(0);
    ui->tabWidget_menu->setCurrentIndex(0);
    QRect rec = QApplication::desktop()->screenGeometry(0);
    setFixedSize(SCR_W,SCR_H);
    if((rec.height()==SCR_H)&&(rec.width()==SCR_W))
    {
        this->showFullScreen();
        this->setGeometry(QApplication::desktop()->screenGeometry(0));//show on first screen
    }
    else
    {

        rec = QApplication::desktop()->screenGeometry(1);
        if((rec.height()==SCR_H)&&(rec.width()==SCR_W))
        {
            this->showFullScreen();
            //printf("error");
            this->setGeometry(QApplication::desktop()->screenGeometry(1));//show on second screen
            //setFixedSize(QApplication::desktop()->screenGeometry(1));
        }

    }

    dxMax = SCR_H/4-10;
    dyMax = SCR_H/4-10;
    mZoomCenterx = scrCtX ;
    mZoomCentery = scrCtY ;
    UpdateScale();

    //ui->horizontalSlider_2->setValue(config.m_config.cfarThresh);

    ui->horizontalSlider_brightness->setValue(ui->horizontalSlider_brightness->maximum()/3.5);
    //    ui->horizontalSlider_gain->setValue(ui->horizontalSlider_gain->maximum());
    //    ui->horizontalSlider_rain->setValue(ui->horizontalSlider_rain->minimum());
    //    ui->horizontalSlider_sea->setValue(ui->horizontalSlider_sea->minimum());
    //ui->comboBox_radar_resolution->setCurrentIndex(0);
    connect(ui->textEdit_heading, SIGNAL(returnPressed()),ui->toolButton_set_heading,SIGNAL(clicked()));
    connect(ui->lineEdit_byte_1, SIGNAL(returnPressed()),ui->toolButton_send_command,SIGNAL(clicked()));
    connect(ui->lineEdit_byte_2, SIGNAL(returnPressed()),ui->toolButton_send_command,SIGNAL(clicked()));
    connect(ui->lineEdit_byte_3, SIGNAL(returnPressed()),ui->toolButton_send_command,SIGNAL(clicked()));
    connect(ui->lineEdit_byte_4, SIGNAL(returnPressed()),ui->toolButton_send_command,SIGNAL(clicked()));
    connect(ui->lineEdit_byte_5, SIGNAL(returnPressed()),ui->toolButton_send_command,SIGNAL(clicked()));
    connect(ui->lineEdit_byte_6, SIGNAL(returnPressed()),ui->toolButton_send_command,SIGNAL(clicked()));
    connect(ui->lineEdit_byte_7, SIGNAL(returnPressed()),ui->toolButton_send_command,SIGNAL(clicked()));
    //setCursor(QCursor(Qt::ArrowCursor));
    UpdateScale();


    //vnmap.setUp(config.m_config.lat(), config.m_config.lon(), 200,config.m_config.mapFilename.data());
    if(pMap)delete pMap;
    pMap = new QPixmap(SCR_H,SCR_H);
    //pViewFrame = new QPixmap(SCR_W,SCR_H);
    setMouseMode(MouseDrag,true);
    isMapOutdated = true;
    // hide menu
    ui->tabWidget_menu->setGeometry(200,-800,ui->tabWidget_menu->width(),ui->tabWidget_menu->height());
    ui->tabWidget_menu->hide();
    //hide iad
    ui->tabWidget_iad->setGeometry(200,-800,ui->tabWidget_iad->width(),ui->tabWidget_iad->height());
    ui->tabWidget_iad->hide();
}
void Mainwindow::ReloadSetting()
{



}
bool Mainwindow::CalcAziContour(double theta, QPoint *point0,QPoint *point1,QPoint *point2,int d)
{
    if(theta>=360)theta-=360.0;
    if(theta<0)theta+=360.0;
    double tanA = tan(theta/57.295779513);
    double sinA = sin(theta/57.295779513);
    double cosA = cos(theta/57.295779513);

    if(theta==0)
    {
        point2->setX(scrCtX  - dx);
        point2->setY(scrCtY - sqrt((d*d/4.0- dx*dx)));
        point1->setX(point2->x());
        point1->setY(point2->y()-5.0);
        point0->setX(point2->x());
        point0->setY(point2->y()-18.0);
    }
    else if(theta==180)
    {

        point2->setX(scrCtX  - dx);
        point2->setY(scrCtY + sqrt((d*d/4.0- dx*dx)));
        point1->setX(point2->x());
        point1->setY(point2->y()+5.0);
        point0->setX(point2->x());
        point0->setY(point2->y()+18.0);
    }
    else if (theta<180)
    {
        double a = (1.0+1.0/tanA/tanA);//4*(dy/tanA-dx)*(dy/tanA-dx) -4*(1+1/tanA)*(dx*dx+dy*dy-width()*width()/4);
        double b= 2.0*(dy/tanA - dx);
        double c= dx*dx+dy*dy-d*d/4.0;
        double delta = b*b-4.0*a*c;
        if(delta<30.0)return false;
        delta = sqrt(delta);
        double rx = (-b + delta)/2.0/a;
        double ry = -rx/tanA;
        if(abs(rx)<100&&abs(ry)<100)return false;
        point2->setX(scrCtX + rx -dx);
        point2->setY(scrCtY + ry-dy);
        point1->setX(point2->x()+5.0*sinA);
        point1->setY(point2->y()-5.0*cosA);
        point0->setX(point2->x()+18.0*sinA);
        point0->setY(point2->y()-18.0*cosA);
    }
    else
    {
        double a = (1+1.0/tanA/tanA);//4*(dy/tanA-dx)*(dy/tanA-dx) -4*(1+1/tanA)*(dx*dx+dy*dy-width()*width()/4);
        double b= 2.0*(dy/tanA - dx);
        double c= dx*dx+dy*dy-d*d/4.0;
        double delta = b*b-4.0*a*c;
        if(delta<30.0)return false;
        delta = sqrt(delta);
        double rx;
        double ry;
        rx =  (-b - delta)/2.0/a;
        ry = -rx/tanA;
        if(abs(rx)<100&&abs(ry)<100)return false;
        point2->setX(scrCtX + rx - dx);
        point2->setY(scrCtY + ry - dy);
        point1->setX(point2->x()+5.0*sinA);
        point1->setY(point2->y()-5.0*cosA);
        point0->setX(point2->x()+18.0*sinA);
        point0->setY(point2->y()-18.0*cosA);
    }
    return true;

}
void Mainwindow::DrawViewFrame(QPainter* p)
{

    if(toolButton_grid_checked)
        {

            if(ui->toolButton_measuring->isChecked())
            {
                DrawGrid(p,mMouseLastX,mMouseLastY);
            }
            else
            {
                DrawGrid(p,radCtX,radCtY);
            }
        }
    //fill back ground

    p->setBrush(QColor(24,48,64,255));
    p->drawRect(SCR_H+SCR_LEFT_MARGIN,0,SCR_W-SCR_H-SCR_LEFT_MARGIN,SCR_H);
    p->drawRect(0,0,SCR_LEFT_MARGIN,SCR_H);
    p->setBrush(Qt::NoBrush);
    p->setPen(penBackground);
//    for (short i=60;i<650;i+=110)
//    {
//        p->drawEllipse(-i/2+(scrCtX-scrCtY)+25,-i/2+25,SCR_H -50+i,SCR_H -50+i);
//    }
    p->drawEllipse(-100+SCR_LEFT_MARGIN,-100,SCR_H+200,SCR_H+200);
    p->setPen(penOuterGrid2);
    p->drawEllipse(scrCtX-scrCtY+25,25,SCR_H -50,SCR_H -50);
    //p->setPen(penOuterGrid2);
    QFont font10 = p->font() ;
    font10.setPointSize(10);
    p->setFont(font10);
    for(short theta=0;theta<360;theta+=10)
    {
        if(CalcAziContour(theta,&points[0],&points[1],&points[2],SCR_H -50))
        {
            p->drawLine(points[1],points[2]);
            p->drawText(points[0].x()-25,points[0].y()-10,50,20,
                    Qt::AlignHCenter|Qt::AlignVCenter,
                    QString::number(theta));
        }
    }
    //double aziDeg = rad2deg(pRadar->getCurAziRad());
    //plot center azi
    /*double centerAzi = processing->getSelsynAzi()+mHeadingT2 ;
    if(centerAzi>360)centerAzi-=360;
    if(CalcAziContour(centerAzi,&points[0],&points[2],&points[1],height()-70))
    {
        p->setPen(QPen(Qt::yellow,8,Qt::SolidLine,Qt::RoundCap,Qt::MiterJoin));
        p->drawLine(points[2],points[1]);
        //p->drawText(720,40,200,20,0,"Sector:  "+QString::number(centerAzi,'f',1));
    }*/
    //plot heading azi
    if(mHeadingGPSOld)
    {
        double radHeading;
        if(isHeadUp)
        {
            radHeading=0;
        }else radHeading = mHeadingGPSOld/180.0*PI;
        p->setPen(QPen(Qt::cyan,6,Qt::SolidLine,Qt::RoundCap));

        points[0].setX(20*sin(radHeading)+radCtX);
        points[0].setY(20*cos(radHeading)+radCtX);
        double dX = 20*sin(radHeading);
        double dY = 20*cos(radHeading);
        p->drawLine(radCtX,radCtY,radCtX+dX,radCtY-dY);
        //p->drawText(720,60,200,20,0,"Heading: "+QString::number(mHeadingGPS,'f',1));

    }

    //plot cur azi
    if(CalcAziContour(processing->mAntennaAzi,&points[0],&points[1],&points[2],height()-70))
    {
        p->setPen(QPen(Qt::red,4,Qt::SolidLine,Qt::RoundCap));
        p->drawLine(points[2],points[1]);
        //draw text
        //p->drawText(720,20,200,20,0,"Antenna: "+QString::number(aziDeg,'f',1));

    }
    if(CalcAziContour(pRadar->getCurAziRad()/3.141592653589*180.0,&points[0],&points[1],&points[2],height()-70))
    {
        p->setPen(QPen(Qt::cyan,4,Qt::SolidLine,Qt::RoundCap));
        p->drawLine(points[2],points[1]);
        //draw text
        //p->drawText(720,20,200,20,0,"Antenna: "+QString::number(aziDeg,'f',1));

    }



}
//void Mainwindow::setScaleNM(unsigned short rangeNM)
//{
//    float oldScale = mScale;
//    mScale = (float)height()/((float)rangeNM*CONST_NM)*0.7f;
//    //printf("scale:%f- %d",scale,rangeNM);
//    isScaleChanged = true;// scale*SIGNAL_RANGE_KM/2048.0f;

//    dyMax = MAX_VIEW_RANGE_KM*mScale;
//    dxMax = dyMax;
//    dx =short(mScale/oldScale*dx);
//    dy =short(mScale/oldScale*dy);
//    DrawMap();
//    /*currMaxRange = (sqrtf(dx*dx+dy*dy)+scrCtY)/signsize;
//    if(currMaxRange>RADAR_MAX_RESOLUTION)currMaxRange = RADAR_MAX_RESOLUTION;*/
//    isScreenUp2Date = false;
//}
short waittimer =0;
void Mainwindow::DisplayClkAdc(int clk)
{
    switch(clk)
    {
    case 0:
        ui->label_range_resolution->setText("15m");
        break;
    case 1:
        ui->label_range_resolution->setText("30m");
        break;
    case 2:
        ui->label_range_resolution->setText("60m");
        break;
    case 3:
        ui->label_range_resolution->setText("120m");
        break;
    case 4:
        ui->label_range_resolution->setText("240m");
        break;
    case 5:
        ui->label_range_resolution->setText("0x05");
        break;
    case 6:
        ui->label_range_resolution->setText("0x06");
        break;
    default:
        ui->label_range_resolution->setText("N/A");
        break;

    }
}
void Mainwindow::UpdateVideo()
{
    if(!processing->getIsDrawn())
    {
        if(pRadar->isClkAdcChanged)
        {
            //ui->comboBox_radar_resolution->setCurrentIndex(pRadar->clk_adc);
            DisplayClkAdc(pRadar->clk_adc);
            pRadar->setScalePPI(mScale);
            this->UpdateScale();
            //            printf("\nsetScale:%d",pRadar->clk_adc);
            pRadar->isClkAdcChanged = false;
        }
        pRadar->UpdateData();

    }
    repaint();
    /*QStandardItemModel* model = new QStandardItemModel(trackListPt->size(), 5);
    for (int row = 0; row < trackListPt->size(); ++row)
    {
       for (int column = 0; column < 5; ++column)
       {
           QString text = QString('A' + row) + QString::number(column + 1);
           QStandardItem* item = new QStandardItem(text);
           model->setItem(row, column, item);
       }
    }
    ui->tableTargetList->setModel(model);*/
}
void Mainwindow::readBuffer()
{

}
void Mainwindow::InitTimer()
{
    t2 = new QThread();

    processing = new dataProcessingThread();
    pRadar = processing->mRadarData;
    connect(&syncTimer1s, SIGNAL(timeout()), this, SLOT(sync1S()));
    syncTimer1s.start(1000);
    connect(&syncTimer5p, SIGNAL(timeout()), this, SLOT(sync5p()));
    syncTimer5p.start(300000);
    //syncTimer1s.moveToThread(t);

    connect(&timerVideoUpdate, SIGNAL(timeout()), this, SLOT(UpdateVideo()));
    timerVideoUpdate.start(30);//ENVDEP
    //scrUpdateTimer.moveToThread(t2);
    //connect(t2,SIGNAL(finished()),t2,SLOT(deleteLater()));

    connect(this,SIGNAL(destroyed()),processing,SLOT(deleteLater()));
    connect(&dataPlaybackTimer,SIGNAL(timeout()),processing,SLOT(playbackRadarData()));
    processing->start(QThread::TimeCriticalPriority);
    t2->start(QThread::HighPriority);

    connect(&timerMapUpdate, SIGNAL(timeout()), this, SLOT(Update100ms()));
    timerMapUpdate.start(100);//ENVDEP

}
void Mainwindow::Update100ms()
{
    //smooth the heading
    float gpsHeadingDiff = mHeadingGPSNew-mHeadingGPSOld;
    if(abs(gpsHeadingDiff)>0.1)
    {
        if(gpsHeadingDiff<-180)gpsHeadingDiff+=360;
        if(gpsHeadingDiff>180)gpsHeadingDiff-=360;
        mHeadingGPSOld+=gpsHeadingDiff/3.0;
        isMapOutdated = true;
    }
    DrawMap();
    mMousex=this->mapFromGlobal(QCursor::pos()).x();
    mMousey=this->mapFromGlobal(QCursor::pos()).y();
    this->ui->label_azi_antenna_head_true->setText(QString::number((processing->mAntennaAzi)));
    if(isInsideViewZone(mMousex,mMousey))
    {
        QApplication::setOverrideCursor(Qt::CrossCursor);
        double azi,rg;
        if(ui->toolButton_measuring->isChecked())
        {
            C_radar_data::kmxyToPolarDeg((mMousex - mMouseLastX)/mScale,-(mMousey - mMouseLastY)/mScale,&azi,&rg);

        }
        else
        {
            C_radar_data::kmxyToPolarDeg((mMousex - radCtX)/mScale,-(mMousey - radCtY)/mScale,&azi,&rg);
        }
        rg/=rangeRatio;
        ui->label_cursor_range->setText(QString::number(rg,'f',2)+strDistanceUnit);
        ui->label_cursor_azi->setText(QString::number(azi)+QString::fromLocal8Bit("\260"));
        ui->label_cursor_lat->setText(demicalDegToDegMin( y2lat(-(mMousey - radCtY)))+"'N");
        ui->label_cursor_long->setText(demicalDegToDegMin(x2lon(mMousex - radCtX))+"'E");
    }
    else
    {
        QApplication::setOverrideCursor(Qt::ArrowCursor);
    }
}
void Mainwindow::InitNetwork()
{
    m_udpSocket = new QUdpSocket(this);
    if(!m_udpSocket->bind(8900))
    {
        if(!m_udpSocket->bind(8901))
        {
            m_udpSocket->bind(8902);
        }
    }
    m_udpSocket->setSocketOption(QAbstractSocket::MulticastTtlOption, 10);

    connect(m_udpSocket, SIGNAL(readyRead()),
            this, SLOT(processARPA()));
}
void Mainwindow::processARPA()
{

    while (m_udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(m_udpSocket->pendingDatagramSize());
        m_udpSocket->readDatagram(datagram.data(), datagram.size());
        if(datagram.size()==5)
        {
            if(*((unsigned char*)datagram.data())==0xff)
            {
                curAziRad = (*((unsigned char*)datagram.data()+1)*256
                             + (*((unsigned char*)datagram.data()+2)))/2500.0*3.14159265;
            }
        }
        //printf(datagram.data());
        QString str(datagram.data());
        QStringList list = str.split(",");
        short dataStart = 0;
        for(short i=0;i<list.size()-5;i++)
        {

            if(list.at(i).contains("RATTM"))
            {
                //            short tNum = (*(list.begin()+1)).toInt();
                //            float tDistance = (*(list.begin()+2)).toFloat();
                //            float tRange = (*(list.begin()+3)).toFloat();
                //            arpa_data.adde(tNum,tDistance,tRange);
            }

            dataStart+= list.at(i).size();
        }

    }

}





//void MainWindow::on_pauseButton_clicked()
//{
//    if(playbackTimer.isActive()){
//        playbackTimer.stop();
//        //ui->pauseButton->setText("Start");
//    }else
//    {
//        playbackTimer.start(10);
//        //ui->pauseButton->setText("Stop");
//    }
//}



//void MainWindow::on_comboBoxViewMode_currentIndexChanged(int index)
//{
//    viewMode=index;
//}
/*
void MainWindow::CameraControl(int x,int y, int zoom)
{
    char* sendBuff = new char[25];
    sprintf(sendBuff,"PTZSET %05d %05d %05d", x, y, zoom);
    udpSocket->writeDatagram(sendBuff,24,QHostAddress("127.0.0.1"),1989);
    delete[] sendBuff;
}
void MainWindow::CameraControl(int direction)
{
    char* sendBuff = new char[12];
    switch(direction)
    {
    case 1:

        sprintf(sendBuff,"PTZMOV IN  ");
        udpSocket->writeDatagram(sendBuff,11,QHostAddress("127.0.0.1"),1989);

        break;
    case 2:
        sprintf(sendBuff,"PTZMOV OUT ");
        udpSocket->writeDatagram(sendBuff,11,QHostAddress("127.0.0.1"),1989);
        break;
    case 3:
        sprintf(sendBuff,"PTZMOV LEFT");
        udpSocket->writeDatagram(sendBuff,11,QHostAddress("127.0.0.1"),1989);
        break;
    case 4:
        sprintf(sendBuff,"PTZMOV RGHT");
        udpSocket->writeDatagram(sendBuff,11,QHostAddress("127.0.0.1"),1989);
        break;
    default:
        break;
    }
    delete[] sendBuff;
}
*/


/*
void MainWindow::sendFrame(const char* hexdata,QHostAddress host,int port )
{
    short len = strlen(hexdata)/2+1;
    unsigned char* sendBuff = new unsigned char[len];
    hex2bin(hexdata,sendBuff);
    udpSendSocket->writeDatagram((char*)sendBuff,len-1,host,port);
    delete[] sendBuff;
}
*/
void Mainwindow::on_actionExit_triggered()
{
    processing->stopThread();
    processing->wait();
    ExitProgram();
}
void Mainwindow::ExitProgram()
{
    //config.SaveToFile();
    QApplication::quit();
#ifdef _WIN32
    QProcess::startDetached("shutdown -s -f -t 0");
#else
    //system("/sbin/halt -p");
#endif
}

void Mainwindow::on_actionConnect_triggered()
{

}
void Mainwindow::sync5p()//period 10 second
{

    if(radar_state!=DISCONNECTED)
    {
        QFile logFile;
        QDateTime now = QDateTime::currentDateTime();
        if(!QDir("D:\\logs\\"+now.toString("\\dd.MM\\")).exists())
        {
            QDir().mkdir("D:\\logs\\"+now.toString("\\dd.MM\\"));
        }
        logFile.setFileName("D:\\logs\\"+now.toString("\\dd.MM\\")+now.toString("dd.MM-hh.mm.ss")+"_radar_online.log");
        logFile.open(QIODevice::WriteOnly);

        logFile.close();

    }

}
void Mainwindow::updateTargetInfo()
{
    if(selectedTargetType==RADAR)
    {/*
        trackList* trackListPt = &pRadar->mTrackList;
        for(uint trackId=0;trackId<trackListPt->size();trackId++)
        {

            if(!trackListPt->at(trackId).isConfirmed)continue;
            if(selectedTargetIndex == trackId)
            {
                //printf("\ntrackId:%d",trackId);
                double mLat,mLon;
                this->ConvKmToWGS(trackListPt->at(trackId).estX*pRadar->scale_ppi/mScale,trackListPt->at(trackId).estY*pRadar->scale_ppi/mScale,&mLon,&mLat);
                //ui->label_data_id->setText(QString::number(trackListPt->at(trackId).idCount));
                float tmpazi = trackListPt->at(trackId).estA*DEG_RAD;
                if(tmpazi<0)tmpazi+=360;
                //ui->label_data_type->setText("Radar");
                ui->label_data_range->setText(QString::number(trackListPt->at(trackId).estR*pRadar->scale_ppi/mScale/1.852f,'f',2)+"Nm");
                ui->label_data_azi->setText( QString::number(tmpazi,'f',2)+QString::fromLocal8Bit("\260"));
                ui->label_data_lat->setText( QString::number((short)mLat)+QString::fromLocal8Bit("\260")+QString::number((mLat-(short)mLat)*60,'f',2)+"'N");
                ui->label_data_long->setText(QString::number((short)mLon)+QString::fromLocal8Bit("\260")+QString::number((mLon-(short)mLon)*60,'f',2)+"'E");
                ui->label_data_speed->setText(QString::number(trackListPt->at(trackId).speed,'f',2)+"Kn");
                ui->label_data_heading->setText(QString::number(trackListPt->at(trackId).heading*DEG_RAD)+QString::fromLocal8Bit("\260"));
                // ui->label_data_dopler->setText(QString::number(trackListPt->at(trackId).dopler));
            }
        }*/

    }
    else if(selectedTargetType == AIS){
        /*
    C2_Track *selectedTrack = &processing->m_AISList.at(selectedTargetIndex);
    double azi,rg;
    double fx,fy;
    ConvWGSToKm(&fx,&fy,selectedTrack->getLon(),selectedTrack->getLat());
    C_radar_data::kmxyToPolarDeg(fx,fy,&azi,&rg);
    ui->label_data_id->setText(QString::fromUtf8((char*)(&selectedTrack->m_MMSI),9));
    ui->label_data_range->setText(QString::number(rg,'f',2));
    ui->label_data_azi->setText(QString::number(azi,'f',2));
    ui->label_data_type->setText("AIS");
    ui->label_data_lat->setText( QString::number((short)selectedTrack->getLat())+QString::fromLocal8Bit("\260")+QString::number((selectedTrack->getLat()-(short)selectedTrack->getLat())*60,'f',2)+"N");
    ui->label_data_long->setText(QString::number((short)selectedTrack->getLon())+QString::fromLocal8Bit("\260")+QString::number((selectedTrack->getLon()-(short)selectedTrack->getLon())*60,'f',2)+"E");
    ui->label_data_speed->setText(QString::number(selectedTrack->m_Speed,'f',2)+"Kn");
    ui->label_data_heading->setText(QString::number(selectedTrack->getHead()*DEG_RAD)+QString::fromLocal8Bit("\260"));
    */}
    else if(selectedTargetType==NOTARGET)
    {
        //ui->label_data_id->setText("--");
        //ui->label_data_type->setText("--");
        ui->label_data_range->setText("--");
        ui->label_data_azi->setText( "--");
        ui->label_data_lat->setText( "--");
        ui->label_data_long->setText("--");
        ui->label_data_speed->setText("--");
        ui->label_data_heading->setText("--");
        //ui->label_data_dopler->setText("--");
    }
}
void Mainwindow::autoSwitchFreq()
{
    int newFreq = rand()%6;
    if(newFreq==0)
    {
        ui->toolButton_tx_2->setChecked(true);
    }
    else  if(newFreq==1)
    {
        ui->toolButton_tx_3->setChecked(true);
    }
    else if(newFreq==2)
    {
        ui->toolButton_tx_4->setChecked(true);
    }
    else if(newFreq==3)
    {
        ui->toolButton_tx_5->setChecked(true);
    }
    else if(newFreq==4)
    {
        ui->toolButton_tx_6->setChecked(true);
    }
    else if(newFreq==5)
    {
        ui->toolButton_tx_7->setChecked(true);
    }


}//label_data_range_2
int failCounter = 0;
void Mainwindow::UpdateDataStatus()
{
    if(processing->mRadarStat.isStatChanged())
    {
        failCounter=0;

        if(processing->mRadarStat.mMayPhatOK)
        {
            ui->label_data_range_2->setStyleSheet("background-color: rgb(10, 255, 10);");
            ui->label_data_range_2->setText(QString::fromUtf8("Máy phát hoạt động bình thường"));
        }
        else
        {
            ui->label_data_range_2->setStyleSheet("background-color: rgb(255, 10, 10);");
            ui->label_data_range_2->setText(QString::fromUtf8("Máy phát không hoạt động"));
        }

        if(processing->mRadarStat.mTaiAngTen==1)ui->toolButton_dk_2->setChecked(true);//tai ang ten
        else if(processing->mRadarStat.mTaiAngTen==0)ui->toolButton_dk_13->setChecked(true);//tai ang ten
        if(processing->mRadarStat.mMaHieu==0)ui->toolButton_dk_11->setChecked(true);//ma hieu
        else if(processing->mRadarStat.mMaHieu==1)ui->toolButton_dk_16->setChecked(true);//ma hieu
        else if(processing->mRadarStat.mMaHieu==2)ui->toolButton_dk_17->setChecked(true);//ma hieu
        if(processing->mRadarStat.mSuyGiam==7)ui->toolButton_dk_3->setChecked(true);//suy giam
        else if(processing->mRadarStat.mSuyGiam==6)ui->toolButton_dk_4->setChecked(true);//suy giam
        else if(processing->mRadarStat.mSuyGiam==5)ui->toolButton_dk_5->setChecked(true);//suy giam
        else if(processing->mRadarStat.mSuyGiam==4)ui->toolButton_dk_6->setChecked(true);//suy giam
        else if(processing->mRadarStat.mSuyGiam==3)ui->toolButton_dk_7->setChecked(true);//suy giam
        else if(processing->mRadarStat.mSuyGiam==2)ui->toolButton_dk_8->setChecked(true);//suy giam
        else if(processing->mRadarStat.mSuyGiam==1)ui->toolButton_dk_9->setChecked(true);//suy giam
        if(processing->mRadarStat.mCaoApKetNoi==0)ui->toolButton_dk_15->setChecked(true);//cao ap
        else if(processing->mRadarStat.mCaoApKetNoi==1)ui->toolButton_dk_10->setChecked(true);//cao ap
        else if(processing->mRadarStat.mCaoApKetNoi==2)ui->toolButton_dk_14->setChecked(true);//cao ap
    }
    else
    {
        if(failCounter>5)ui->label_data_range_2->setText(QString::fromUtf8("Chưa có kết nối"));
        else
        failCounter++;

    }
}
void Mainwindow::sync1S()//period 1 second
{

    UpdateDataStatus();
    UpdateGpsData();
    if(isScaleChanged ) {

        pRadar->setScalePPI(mScale);
        isScaleChanged = false;
    }
    ui->label_speed_2->setText(QString::number(pRadar->rotation_per_min,'f',1)+"v/p");
    return;
//    this->updateTargetInfo();
    if(processing->isConnected())
        setRadarState(CONNECTED);
    else
        setRadarState(DISCONNECTED);
    if(warningList.size())
    {
        ui->label_status_warning->setText(warningList.at(warningList.size()-1));
        ui->label_status_warning->setStyleSheet("background-color: rgb(230, 160, 10);");
        if(ui->label_status_warning->isHidden())ui->label_status_warning->setHidden(false);
        else ui->label_status_warning->setHidden(true);
    }



    showTime();
    /*
    // display radar temperature:
    temperature[pRadar->tempType] = pRadar->moduleVal;

    //    ui->label_noiseAverrage->setText(QString::number(pRadar->getNoiseAverage(),'f',1));
    ui->label_temp->setText(QString::number(pRadar->tempType)
                            +"|"+QString::number(pRadar->moduleVal,'f',0)
                            + QString::fromLocal8Bit("\260 C"));
    // request radar temperature:
    if(radar_state!=DISCONNECTED)
    {
        if(ui->toolButton_temp_update->isChecked())
        {
            processing->radRequestTemp(curTempIndex);
            curTempIndex++;
            if(curTempIndex>4)curTempIndex=0;
        }
        QByteArray array((char*)pRadar->getFeedback(), 8);
        QString commandLog = QString(array.toHex());
        ui->label_command->setText(commandLog);
        cmLog->AddString(commandLog);
    }


    switch(radar_state)
    {
    case DISCONNECTED:
        ui->label_status->setText(QString::fromUtf8("Chưa k. nối"));
        //ui->toolButton_tx->setEnabled(false);
        //        ui->toolButton_scan->setEnabled(false);
        if(!warningList.size())warningList.push_back(QString::fromUtf8("Chưa kết nối radar"));
        m_udpSocket->writeDatagram("d",1,QHostAddress("127.0.0.1"),8001);
        break;
    case CONNECTED:
        //printf("\ns_tx");
        //ui->label_status_warning->setText(QString::fromUtf8("Đã kết nối radar"));
        ui->label_status->setText(QString::fromUtf8("sẵn sàng"));
        //ui->label_status_warning->setHidden(false);

        m_udpSocket->writeDatagram("c",1,QHostAddress("127.0.0.1"),8001);
        break;
    default:
        break;
    }*/
    ui->label_debug_data->setText("Chu ky: "+QString::number(pRadar->chu_ky));
    unsigned int chuKy = 1000000/(pRadar->chu_ky*(pow(2,pRadar->clk_adc))/10.0);

    ui->label_sn_freq->setText(QString::number(chuKy));
    ui->label_he_so_tap->setText(QString::fromUtf8("Hệ số tạp: ")+QString::number(pRadar->get_tb_tap()));
    if(ui->toolButton_auto_freq->isChecked())
    {
        if(pRadar->get_tb_tap()>mMaxTapMayThu)
        {
            this->autoSwitchFreq();
        }
    }
    int value;
    switch((pRadar->sn_stat>>8)&0x07)
    {
    case 4:
        ui->label_sn_type->setText("DTTT");
        //ui->label_sn_param->setText(QString::number(32<<());
        if(((pRadar->sn_stat)&0xff)==0)
        {
            ui->label_sn_param->setText("32");
        }
        else if(((pRadar->sn_stat)&0xff)==1)
        {
            ui->label_sn_param->setText("48");
        }
        else if(((pRadar->sn_stat)&0xff)==2)
        {
            ui->label_sn_param->setText("64");
        }
        else if(((pRadar->sn_stat)&0xff)==3)
        {
            ui->label_sn_param->setText("96");
        }
        else if(((pRadar->sn_stat)&0xff)==4)
        {
            ui->label_sn_param->setText("128");
        }
        else if(((pRadar->sn_stat)&0xff)==5)
        {
            ui->label_sn_param->setText("192");

        }
        else
        {
            ui->label_sn_param->setText("256");
        }
        break;
    case 0:
        ui->label_sn_type->setText(QString::fromUtf8("Xung đơn"));
        ui->label_sn_param->setText(QString::number((((pRadar->sn_stat)&0xff))));
        break;
    case 2:
        ui->label_sn_type->setText(QString::fromUtf8("Giả liên tục"));
        value = ((pRadar->sn_stat)&0xff);
        if(value<5)
        {
            ui->label_sn_param->setText(QString::number(16*pow(2,value)));

            ui->label_sn_pulsew->setText(QString::number(((16*pow(2,value))*(pow(2,pRadar->clk_adc))/10.0),'f',1));
        }
        else
        {
            ui->label_sn_param->setText(QString::number(16*pow(2,value-5))+"x2");
            ui->label_sn_pulsew->setText(QString::number(((32*(value-5))*(pow(2,pRadar->clk_adc))/10.0),'f',1));
        }

        break;
    case 3:
        ui->label_sn_type->setText(QString::fromUtf8("Mã 2 pha"));//todo:change back to "ma ngau nhien" later
        value = ((pRadar->sn_stat)&0xff);
        ui->label_sn_param->setText(QString::number(value-15.0));
        ui->label_sn_pulsew->setText(QString::number(((value-15.0)*(pow(2,pRadar->clk_adc))/10.0),'f',1));
        break;
    case 1:
        ui->label_sn_type->setText(QString::fromUtf8("Mã xung"));
        ui->label_sn_param->setText(QString::number((((pRadar->sn_stat)&0xff))));
        break;
    default:
        ui->label_sn_param->setText(QString::number(((pRadar->sn_stat)&0xff)));
        break;
    }
    /*switch((pRadar->rotation_speed)&0x07)
            {
            case 0:
                ui->label_speed->setText(QString::fromUtf8("Dừng quay"));break;
            case 1:
                ui->label_speed->setText("5 v/p");break;
            case 2:
                ui->label_speed->setText("8 v/p");break;
            case 3:
                ui->label_speed->setText("12 v/p");break;
            case 4:
                ui->label_speed->setText("15 v/p");break;
            case 5:
                ui->label_speed->setText("18 v/p");break;
            default:

                break;
            }*/




}
void Mainwindow::setRadarState(radarSate radarState)
{
    if(radar_state!=radarState)
    {
        radar_state = radarState;
        on_label_status_warning_clicked();
    }
}

void Mainwindow::on_actionTx_On_triggered()
{
    //sendFrame("aaab030200000000", QHostAddress("192.168.0.44"),2573);
    //on_actionRotateStart_toggled(true);
    //    Command_Control new_com;
    //    new_com.bytes[0] = 0xaa;
    //    new_com.bytes[1] = 0xab;
    //    new_com.bytes[2] = 0x02;
    //    new_com.bytes[3] = 0x01;
    //    new_com.bytes[4] = 0x00;
    //    new_com.bytes[5] = 0x00;
    //    new_com.bytes[6] = 0x00;
    //    new_com.bytes[7] = 0;//new_com.bytes[0]+new_com.bytes[1]+new_com.bytes[2]+new_com.bytes[3]+new_com.bytes[4]+new_com.bytes[5]+new_com.bytes[6];
    //    command_queue.push(new_com);
    //    new_com.bytes[0] = 0xaa;
    //    new_com.bytes[1] = 0xab;
    //    new_com.bytes[2] = 0x00;
    //    new_com.bytes[3] = 0x01;
    //    new_com.bytes[4] = 0x00;
    //    new_com.bytes[5] = 0x00;
    //    new_com.bytes[6] = 0x00;
    //    new_com.bytes[7] = 0;//new_com.bytes[0]+new_com.bytes[1]+new_com.bytes[2]+new_com.bytes[3]+new_com.bytes[4]+new_com.bytes[5]+new_com.bytes[6];
    //    command_queue.push(new_com);

}

void Mainwindow::on_actionTx_Off_triggered()
{
    //    //on_actionRotateStart_toggled(false);
    //    Command_Control new_com;
    //    new_com.bytes[0] = 0xaa;
    //    new_com.bytes[1] = 0xab;
    //    new_com.bytes[2] = 0x00;
    //    new_com.bytes[3] = 0x00;
    //    new_com.bytes[4] = 0x00;
    //    new_com.bytes[5] = 0x00;
    //    new_com.bytes[6] = 0x00;
    //    new_com.bytes[7] = 0;//new_com.bytes[0]+new_com.bytes[1]+new_com.bytes[2]+new_com.bytes[3]+new_com.bytes[4]+new_com.bytes[5]+new_com.bytes[6];
    //    command_queue.push(new_com);
    //    new_com.bytes[0] = 0xaa;
    //    new_com.bytes[1] = 0xab;
    //    new_com.bytes[2] = 0x02;
    //    new_com.bytes[3] = 0x00;
    //    new_com.bytes[4] = 0x00;
    //    new_com.bytes[5] = 0x00;
    //    new_com.bytes[6] = 0x00;
    //    new_com.bytes[7] = 0;//new_com.bytes[0]+new_com.bytes[1]+new_com.bytes[2]+new_com.bytes[3]+new_com.bytes[4]+new_com.bytes[5]+new_com.bytes[6];
    //    command_queue.push(new_com);
}

void Mainwindow::on_actionRecording_toggled(bool arg1)
{
    if(arg1)
    {

        QDateTime now = QDateTime::currentDateTime();
        QString filename = now.toString("dd.MM_hh.mm.ss")+
                "_"+ui->label_range_resolution->text()+
                "_"+ui->label_sn_type->text()+
                "_"+ui->label_sn_param->text();
        ui->label_record_file_name->setText(filename);
        processing->startRecord("D:/HR2D/rec_"+filename+HR_FILE_EXTENSION);
    }
    else
    {
        processing->stopRecord();
    }
}

void Mainwindow::on_actionOpen_rec_file_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this,    tr("Open signal file"), NULL, tr("HR signal record files (*.r2d)"));
    if(!filename.size())return;
    processing->loadRecordDataFile(filename);
    ui->label_record_file_name->setText(filename);
}



void Mainwindow::on_actionOpen_map_triggered()
{
    //openShpFile();
}
void Mainwindow::showTime()
{
    QDateTime time = QDateTime::currentDateTime();
    QString text = time.toString("hh:mm:ss");
    ui->label_date->setText(text);
    text = time.toString("dd/MM/yy");
    ui->label_time->setText(text);
}

void Mainwindow::on_actionSaveMap_triggered()
{
    //vnmap.SaveBinFile();
}

void Mainwindow::on_actionSetting_triggered()
{
    //    GPSDialog *dlg = new GPSDialog(this);
    //    dlg->setModal(false);
    //    dlg->loadConfig(&config);
    //    dlg->show();
    //    dlg->setAttribute(Qt::WA_DeleteOnClose);
    //    connect(dlg, SIGNAL(destroyed(QObject*)), SLOT(UpdateSetting()));
    //    connect(dlg, SIGNAL(destroyed(QObject*)), SLOT(setCodeType()));
}
void Mainwindow::on_actionAddTarget_toggled(bool arg1)
{
    //isAddingTarget=arg1;

}




void Mainwindow::on_actionClear_data_triggered()
{
    pRadar->resetData();
    //    isScreenUp2Date = false;
}

//void Mainwindow::on_actionView_grid_triggered(bool checked)
//{
//    gridOff = checked;
//    dx=0;dy=0;
//    DrawMap();
//    //UpdateSetting();
//}


void Mainwindow::on_actionPlayPause_toggled(bool arg1)
{
    processing->togglePlayPause(arg1);
    if(arg1)dataPlaybackTimer.start(25);else dataPlaybackTimer.stop();

}


/*
        void MainWindow::on_pushButton_clicked()
        {

            Command_Control new_com;
            hex2bin(ui->lineEdit_byte_1->text().toStdString().data(),&new_com.bytes[0]);
            hex2bin(ui->lineEdit_byte_2->text().toStdString().data(),&new_com.bytes[1]);
            hex2bin(ui->lineEdit_byte_3->text().toStdString().data(),&new_com.bytes[2]);
            hex2bin(ui->lineEdit_byte_4->text().toStdString().data(),&new_com.bytes[3]);
            hex2bin(ui->lineEdit_byte_5->text().toStdString().data(),&new_com.bytes[4]);
            hex2bin(ui->lineEdit_byte_6->text().toStdString().data(),&new_com.bytes[5]);
            hex2bin(ui->lineEdit_byte_7->text().toStdString().data(),&new_com.bytes[6]);
            hex2bin(ui->lineEdit_byte_8->text().toStdString().data(),&new_com.bytes[7]);
            command_queue.push(new_com);
        }
        */

void Mainwindow::SendCommandControl()
{/*
              if(command_queue.size())
              {

                  if(pRadar->checkFeedback(&command_queue.front().bytes[0]))// check if the radar has already recieved the command
                  {


                      command_queue.pop();
                      udpFailure = 0;

                  }
                  else
                  {
                    if(udpFailure<20)//ENVDEP 20*50ms = 1s
                    {udpFailure++;}
                    else{
                        setRadarState( DISCONNECTED);
                        udpFailure = 0;
                    }
                    udpSendSocket->writeDatagram((char*)&command_queue.front().bytes[0],8,QHostAddress("192.168.0.44"),2572);
                    //
                    char xx[3];
                    xx[2]=0;
                    QString str;
                    for(short i =0;i<8;i++)
                    {
                        bin2hex(command_queue.front().bytes[i],&xx[0]);
                        str.append(xx);
                        str.append('-');
                    }

                    ui->label_command->setText(str);
                    //printf((const char*)str.data());
                    //

                  }

              }*/

}

void Mainwindow::on_actionRecording_triggered()
{

}


//void Mainwindow::on_comboBox_temp_type_currentIndexChanged(int index)
//{

//}

//void RadarGui::on_horizontalSlider_brightness_actionTriggered(int action)
//{

//}

void Mainwindow::on_horizontalSlider_brightness_valueChanged(int value)
{
    pRadar->brightness = 0.5f+(float)value/ ui->horizontalSlider_brightness->maximum()*4.0f;
}

/*void MainWindow::on_horizontalSlider_3_valueChanged(int value)
        {
            switch (value) {
            case 1:
                Command_Control new_com;
                new_com.bytes[0] = 4;
                new_com.bytes[1] = 0xab;
                new_com.bytes[2] = 0;
                new_com.bytes[3] = 0;
                new_com.bytes[4] = 1;
                new_com.bytes[5] = 0;
                new_com.bytes[6] = 0;
                new_com.bytes[7] = 0;//new_com.bytes[0]+new_com.bytes[1]+new_com.bytes[2]+new_com.bytes[3]+new_com.bytes[4]+new_com.bytes[5]+new_com.bytes[6];
                command_queue.push(new_com);
                break;
            case 2:
                printf("2");
                break;
            case 3:
                printf("3");
                break;
            default:
                break;
            }
        }*/



//void MainWindow::on_toolButton_toggled(bool checked)
//{
//    //if(checked)ui->toolBar_Main->show();
//    //else ui->toolBar_Main->hide();
//}

void Mainwindow::on_actionSector_Select_triggered()
{

}


//void MainWindow::on_toolButton_10_clicked()
//{
//    //if(ui->frame_RadarViewOptions->isHidden())ui->frame_RadarViewOptions->show();
//    //else ui->frame_RadarViewOptions->hide();
//}




/*
        void MainWindow::on_toolButton_14_clicked()
        {
            //if(event->delta()>0)ui->horizontalSlider->setValue(ui->horizontalSlider->value()+1);
        }

        void MainWindow::on_toolButton_13_clicked()
        {
            //if(event->delta()<0)ui->horizontalSlider->setValue(ui->horizontalSlider->value()-1);
        }
        */
void Mainwindow::setScaleRange(double srange)
{
    if(mDistanceUnit==0)
    {
        mScale = (SCR_H/2.0-5.0)/(rangeRatio*srange );
        ringStep = srange/6.0f;
        ui->label_range->setText(QString::number(srange)+strDistanceUnit);
    }
    else if(mDistanceUnit==1)
    {
        mScale = (SCR_H/2.0-5.0)/(rangeRatio*srange );
        ringStep = srange/5;
        ui->label_range->setText(QString::number(srange)+strDistanceUnit);
    }
}
void Mainwindow::UpdateScale()
{
    float oldScale = mScale;

    //char byte2;
    bool isAdaptSn = ui->toolButton_auto_adapt->isChecked();
    if(mDistanceUnit==0)//NM
    {
        switch(mRangeLevel)
        {
        case 0:
            setScaleRange(1.5);
            if(isAdaptSn) sendToRadarString(CConfig::getString("mR0Command"));
            break;
        case 1:
            setScaleRange(3);
            if(isAdaptSn) sendToRadarString(CConfig::getString("mR1Command"));
            break;
        case 2:
            setScaleRange(6);
            if(isAdaptSn) sendToRadarString(CConfig::getString("mR2Command"));
            break;
        case 3:
            setScaleRange(12);
            if(isAdaptSn) sendToRadarString(CConfig::getString("mR3Command"));
            break;
        case 4:
            setScaleRange(24);
            if(isAdaptSn) sendToRadarString(CConfig::getString("mR4Command"));
            break;
        case 5:
            setScaleRange(48);
            if(isAdaptSn) sendToRadarString(CConfig::getString("mR5Command"));
            break;
        case 6:
            setScaleRange(96);
            if(isAdaptSn) sendToRadarString(CConfig::getString("mR6Command"));
            break;
        case 7:
            setScaleRange(192);
            if(isAdaptSn) sendToRadarString(CConfig::getString("mR7Command"));
            break;
        default:
            setScaleRange(48);
            break;
        }
    }
    else if(mDistanceUnit==1)
    {
        switch(mRangeLevel)
        {
        case 0:
            setScaleRange(2.5);
            if(isAdaptSn) sendToRadarString(CConfig::getString("mR0Command"));
            break;
        case 1:
            setScaleRange(5);
            if(isAdaptSn) sendToRadarString(CConfig::getString("mR1Command"));
            break;
        case 2:
            setScaleRange(10);
            if(isAdaptSn) sendToRadarString(CConfig::getString("mR2Command"));
            break;
        case 3:
            setScaleRange(20);
            if(isAdaptSn) sendToRadarString(CConfig::getString("mR3Command"));
            break;
        case 4:
            setScaleRange(50);
            if(isAdaptSn) sendToRadarString(CConfig::getString("mR4Command"));
            break;
        case 5:
            setScaleRange(100);
            if(isAdaptSn) sendToRadarString(CConfig::getString("mR5Command"));
            break;
        case 6:
            setScaleRange(200);
            if(isAdaptSn) sendToRadarString(CConfig::getString("mR6Command"));
            break;
        case 7:
            setScaleRange(400);
            if(isAdaptSn) sendToRadarString(CConfig::getString("mR7Command"));
            break;
        default:
            break;
        }
    }
    isScaleChanged = true;
    short sdx = mZoomCenterx - radCtX;
    short sdy = mZoomCentery - radCtY;
    sdx =(sdx*mScale/oldScale);
    sdy =(sdy*mScale/oldScale);
    mZoomCenterx = scrCtX+sdx-dx;
    mZoomCentery = scrCtY+sdy-dy;
}




//void MainWindow::on_toolButton_10_toggled(bool checked)
//{

//}

//void MainWindow::on_actionRotateStart_toggled(bool arg1)
//{
//    if(arg1)
//    {
//        Command_Control new_com;
//        new_com.bytes[0] = 0xaa;
//        new_com.bytes[1] = 0xab;
//        new_com.bytes[2] = 0x03;
//        new_com.bytes[3] = 0x02;
//        new_com.bytes[4] = 0x00;
//        new_com.bytes[5] = 0x00;
//        new_com.bytes[6] = 0x00;
//        new_com.bytes[7] = 0;//new_com.bytes[0]+new_com.bytes[1]+new_com.bytes[2]+new_com.bytes[3]+new_com.bytes[4]+new_com.bytes[5]+new_com.bytes[6];
//        command_queue.push(new_com);
//    }
//    else
//    {

//        Command_Control new_com;
//        new_com.bytes[0] = 0xaa;
//        new_com.bytes[1] = 0xab;
//        new_com.bytes[2] = 0x03;
//        new_com.bytes[3] = 0x00;
//        new_com.bytes[4] = 0x00;
//        new_com.bytes[5] = 0x00;
//        new_com.bytes[6] = 0x00;
//        new_com.bytes[7] = 0;//new_com.bytes[0]+new_com.bytes[1]+new_com.bytes[2]+new_com.bytes[3]+new_com.bytes[4]+new_com.bytes[5]+new_com.bytes[6];
//        command_queue.push(new_com);
//    }
//}


//void MainWindow::on_comboBox_temp_type_2_currentIndexChanged(int index)
//{



//}

//void MainWindow::on_toolButton_11_toggled(bool checked)
//{


//}

//void MainWindow::on_pushButton_removeTarget_2_clicked()
//{

//}

//void MainWindow::on_pushButton_removeTarget_2_released()
//{
//    pRadar->resetTrack();
//}

//void MainWindow::on_pushButton_avtodetect_toggled(bool checked)
//{
//    isDrawSubTg = !checked;
//    pRadar->avtodetect = checked;
//    pRadar->terrain_init_time = 3;
//}


void Mainwindow::setCodeType(short index)// chuyen ma
{
    unsigned char bytes[8];
    bytes[0] = 1;
    bytes[1] = 0xab;

    //printf("\n code:%d",index);
    switch (index)
    {
    case 0://M32
        bytes[2] = 2;
        bytes[3] = 0;
        break;
    case 1://M64
        bytes[2] = 2;
        bytes[3] = 1;
        break;
    case 2://M128
        bytes[2] = 2;
        bytes[3] = 2;
        break;
    case 3://M255
        bytes[2] = 2;
        bytes[3] = 3;
        break;
    case 4://M32x2
        bytes[2] = 2;
        bytes[3] = 4;
        break;
    case 5://M64x2
        bytes[2] = 2;
        bytes[3] = 5;
        break;
    case 6://M128x2
        bytes[2] = 2;
        bytes[3] = 6;
        break;
    case 7://baker
        bytes[2] = 1;
        bytes[3] = 1;
        break;
    case 8://single pulse
        bytes[2] = 0;
        bytes[3] = 1;

        break;
    default:
        bytes[2] = 0;
        bytes[3] = 0;
        break;
    }
    bytes[4] = 0;
    bytes[5] = 0;
    bytes[6] = 0;
    bytes[7] = 0;//new_com.bytes[0]+new_com.bytes[1]+new_com.bytes[2]+new_com.bytes[3]+new_com.bytes[4]+new_com.bytes[5]+new_com.bytes[6];
    sendToRadar(&bytes[0]);

}
//void MainWindow::on_toolButton_4_toggled(bool checked)
//{
//    if(checked)
//    {
//        this->on_actionTx_On_triggered();
//    }
//    else
//    {
//        this->on_actionTx_Off_triggered();
//    }

//}



void Mainwindow::on_horizontalSlider_gain_valueChanged(int value)
{
        pRadar->kgain = 7-(float)value/(ui->horizontalSlider_gain->maximum())*10;
        ui->label_gain->setText("Gain:"+QString::number(-pRadar->kgain));
    //printf("pRadar->kgain %f \n",pRadar->kgain);
}

void Mainwindow::on_horizontalSlider_rain_valueChanged(int value)
{
        pRadar->krain = (float)value/(ui->horizontalSlider_rain->maximum()+ui->horizontalSlider_rain->maximum()/3);
        ui->label_rain->setText("Rain:" + QString::number(pRadar->krain,'f',2));
}

void Mainwindow::on_horizontalSlider_sea_valueChanged(int value)
{
        pRadar->ksea = (float)value/(ui->horizontalSlider_sea->maximum());
    //ui->label_rain->setText("Rain:" + QString::number(-pRadar->krain));
}


/*
        void MainWindow::on_pushButton_loadAis_clicked()
        {
            QString filename = QFileDialog::getOpenFileName(this,    QString::fromUtf8("M? file "), NULL, tr("ISM file (*.txt)"));
            if(!filename.size())return;
            QFile gpsfile( filename);
            if (!gpsfile.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                return ;
            }
            QTextStream in(&gpsfile);
            QString line ;int k=0;
            line = in.readLine();

            while(!in.atEnd()) {
                //printf((char*)line.data());
                QStringList  list = line.split(",");

                if (list[0] == "$GPRMC")
                {

                    float mlat = (*(list.begin()+3)).toFloat()/100.0f +0.0097;
                    float mlong = (*(list.begin()+5)).toFloat()/100.0f + 0.355;
                    arpa_data.addAIS(list[0].toStdString(),mlat,mlong,0,0);

                }line = in.readLine();
                k=list.size();
                //printf("size:%d",arpa_data.ais_track_list[0].id.data());
            }

        }

        */


void Mainwindow::on_toolButton_exit_clicked()
{
    on_actionExit_triggered();
}

//void Mainwindow::on_toolButton_setting_clicked()
//{
//    this->on_actionSetting_triggered();
//}

/*
        void Mainwindow::on_toolButton_tx_toggled(bool checked)
        {

        //    if(checked)

        //    {   //0xaa,0xab,0x00,0x01,0x00,0x00,0x00
        //        unsigned char        bytes[8] = {0xaa,0xab,0x02,0x01,0x00,0x00,0x00};
        //        udpSendSocket->writeDatagram((char*)&bytes[0],8,QHostAddress("192.168.0.44"),2572);
        //        bytes[2] = 0x00;//{0xaa,0xab,0x00,0x01,0x00,0x00,0x00};
        //        Sleep(100);
        //        udpSendSocket->writeDatagram((char*)&bytes[0],8,QHostAddress("192.168.0.44"),2572);
        //        //ui->toolButton_tx->setChecked(false);
        //    }
        //    else
        //    {

        //        unsigned char        bytes[8] = {0xaa,0xab,0x02,0x00,0x00,0x00,0x00};
        //        udpSendSocket->writeDatagram((char*)&bytes[0],8,QHostAddress("192.168.0.44"),2572);
        //        bytes[2] = 0x00;// = {0xaa,0xab,0x00,0x01,0x00,0x00,0x00};
        //        Sleep(100);
        //        udpSendSocket->writeDatagram((char*)&bytes[0],8,QHostAddress("192.168.0.44"),2572);
        //        //ui->toolButton_tx->setChecked(true);
        //    }

        }
        */


void Mainwindow::on_toolButton_xl_nguong_toggled(bool checked)
{
    pRadar->setAutorgs(checked);
    if(checked)
    {
        //        ui->horizontalSlider_gain->setVisible(false);
        //        ui->horizontalSlider_rain->setVisible(false);
        //        ui->horizontalSlider_sea->setVisible(false);
    }
    else
    {
        //        ui->horizontalSlider_gain->setVisible(true);
        //        ui->horizontalSlider_rain->setVisible(true);
        //        ui->horizontalSlider_sea->setVisible(true);
    }
}

void Mainwindow::on_toolButton_replay_toggled(bool checked)
{
    this->on_actionPlayPause_toggled(checked);
}


void Mainwindow::on_toolButton_replay_fast_toggled(bool checked)
{
    if(checked)
    {
        processing->playRate = 200;
    }else
    {
        processing->playRate = 50;
    }
}

void Mainwindow::on_toolButton_record_toggled(bool checked)
{
    this->on_actionRecording_toggled(checked);
}

void Mainwindow::on_toolButton_open_record_clicked()
{
    this->on_actionOpen_rec_file_triggered();
}



//void Mainwindow::on_toolButton_alphaView_toggled(bool checked)
//{
//    displayAlpha = checked;
//    pRadar->isDisplayAlpha = checked;
//}

/*
        void Mainwindow::updateTargets()
        {
            trackList* trackListPt = &pRadar->mTrackList;

            for(short i = 0;i<targetDisplayList.size();i++)
            {
                if(!targetDisplayList.at(i)->isUsed)
                {
                    continue;

                    targetDisplayList.at(i)->hide();

                }
                if(trackListPt->at(targetDisplayList.at(i)->trackId).isManual == 0)
                {
                    targetDisplayList.at(i)->isUsed = false;
                    ui->label_status_warning->setText(QString::fromUtf8("Mất MT số:")+QString::number(i+1));
                    warningList.append(QString::fromUtf8("Mất MT số:")+QString::number(i+1));
                    ui->label_status_warning->setStyleSheet("background-color: rgb(255, 150, 50,255);");
                    targetDisplayList.at(i)->hide();
                    //targetList.at(i)->isLost=true;
                    continue;
                }
                float x	= targetDisplayList.at(i)->x*mScale + radCtX ;
                float y	= -targetDisplayList.at(i)->y*mScale + radCtY ;
                float w = scrCtY-30;
                float dx = x-scrCtX;
                float dy = y-scrCtY;
                if(dx*dx+dy*dy>(w*w))
                {
                    targetDisplayList.at(i)->hide();
                }
                else
                {
                    targetDisplayList.at(i)->show();
                    targetDisplayList.at(i)->setScrPos(x,y);
                }

                if(targetDisplayList.at(i)->clicked)
                {

                    selected_target_index = i;
                    targetDisplayList.at(i)->setSelected(true);
                    targetDisplayList.at(i)->clicked = false;
                }
                if(targetDisplayList.at(i)->doubleClicked)
                {

                    selected_target_index = i;
                    trackListPt->at((targetDisplayList.at(i)->trackId)).isManual = true;
                    targetDisplayList.at(i)->isManual = true;
                    targetDisplayList.at(i)->doubleClicked = false;
                }
                if(selected_target_index == i)
                {
                    float tmpazi = trackListPt->at(targetDisplayList.at(i)->trackId).estA*DEG_RAD;
                    if(tmpazi<0)tmpazi+=360;
                    ui->label_data_id->setText(QString::number(i+1));
                    ui->label_data_type->setText("Radar");
                    ui->label_data_range->setText(QString::number(trackListPt->at(targetDisplayList.at(i)->trackId).estR*pRadar->scale_ppi/mScale/1.852f,'f',2)+"Nm");
                    ui->label_data_azi->setText( QString::number(tmpazi,'f',2)+QString::fromLocal8Bit("\260"));
                    ui->label_data_lat->setText( QString::number((short)targetDisplayList.at(i)->m_lat)+QString::fromLocal8Bit("\260")+QString::number((targetDisplayList.at(i)->m_lat-(short)targetDisplayList.at(i)->m_lat)*60,'f',2)+"'N");
                    ui->label_data_long->setText(QString::number((short)targetDisplayList.at(i)->m_lon)+QString::fromLocal8Bit("\260")+QString::number((targetDisplayList.at(i)->m_lon-(short)targetDisplayList.at(i)->m_lon)*60,'f',2)+"'E");
                    ui->label_data_speed->setText(QString::number(trackListPt->at(targetDisplayList.at(i)->trackId).speed,'f',2)+"Kn");
                    ui->label_data_heading->setText(QString::number(trackListPt->at(targetDisplayList.at(i)->trackId).head_r*DEG_RAD)+QString::fromLocal8Bit("\260"));
                    ui->label_data_dopler->setText(QString::number(trackListPt->at(targetDisplayList.at(i)->trackId).dopler));
                }
                else
                {
                    targetDisplayList.at(i)->setSelected(false);// = false;
                }
                //printf("\nx:%f y:%f",x,y);
            }
            //ui->
            //t1.setGeometry(400,400,20,20);
            //targetList.append(t1);
        }
        */
void Mainwindow::on_toolButton_centerView_clicked()
{
    dx = 0;
    dy = 0;
    radCtX = scrCtX-dx;
    radCtY = scrCtY-dy;
    isMapOutdated = true;
    //    isScreenUp2Date = false;
}

void Mainwindow::on_comboBox_currentIndexChanged(int index)
{
    switch (index)
    {
    case 0:
        pRadar->dataOver = m_only;
        break;
    case 1:
        pRadar->dataOver = s_m_150;
        break;
    case 2:
        pRadar->dataOver = s_m_200;
        break;
    case 3:
        pRadar->dataOver = max_s_m_150;
        break;
    case 4:
        pRadar->dataOver = max_s_m_200;
        break;
    default:
        break;
    }

}

void Mainwindow::on_comboBox_img_mode_currentIndexChanged(int index)
{
    pRadar->imgMode = imgDrawMode(index) ;
}


void Mainwindow::on_toolButton_send_command_clicked()
{

    unsigned char        bytes[8];
    hex2bin(ui->lineEdit_byte_1->text().toStdString().data(),&bytes[0]);
    hex2bin(ui->lineEdit_byte_2->text().toStdString().data(),&bytes[1]);
    hex2bin(ui->lineEdit_byte_3->text().toStdString().data(),&bytes[2]);
    hex2bin(ui->lineEdit_byte_4->text().toStdString().data(),&bytes[3]);
    hex2bin(ui->lineEdit_byte_5->text().toStdString().data(),&bytes[4]);
    hex2bin(ui->lineEdit_byte_6->text().toStdString().data(),&bytes[5]);
    hex2bin(ui->lineEdit_byte_7->text().toStdString().data(),&bytes[6]);
    bytes[7] = bytes[0]+bytes[1]+bytes[2]+bytes[3]+bytes[4]+bytes[5]+bytes[6];
    ui->lineEdit_byte_8->setText(QString::number(bytes[7]));
    //hex2bin(ui->lineEdit_byte_8->text().toStdString().data(),&bytes[7]);
    sendToRadar((unsigned char*)&bytes[0]);
    //udpSendSocket->writeDatagram((char*)&bytes[0],8,QHostAddress("192.168.0.44"),2572);

}


void Mainwindow::on_toolButton_zoom_in_clicked()
{
    if(mRangeLevel >0) mRangeLevel-=1;
    CConfig::setValue("mRangeLevel",mRangeLevel);
    UpdateScale();
    isMapOutdated = true;
}

void Mainwindow::on_toolButton_zoom_out_clicked()
{
    if(mRangeLevel <7) mRangeLevel+=1;
    CConfig::setValue("mRangeLevel",mRangeLevel);
    UpdateScale();
    isMapOutdated = true;
}

//void Mainwindow::on_toolButton_reset_clicked()
//{
//    pRadar->resetSled();
//}

//void Mainwindow::on_toolButton_send_command_2_clicked()
//{
//    unsigned char        bytes[8] = {0xaa,0xab,0x02,0x02,0x0a,0,0,0};
//    udpSendSocket->writeDatagram((char*)&bytes[0],8,QHostAddress("192.168.0.44"),2572);
////    bytes[0] = 0xaa;
////    bytes[1] = 0xab;
////    bytes[2] = 0x02;
////    bytes[3] = 0x02;
////    bytes[4] = 0x0a;
////    bytes[5] = 0x00;
////    bytes[6] = 0x00;
////    bytes[7] = 0x00;

//}

void Mainwindow::SetGPS(double lat,double lon,double heading)
{
    mLat = lat;
    mLon = lon;
    if(heading)
    {
        mHeadingGPSNew = heading;
    }
    CConfig::setValue("mLat",mLat);
    CConfig::setValue("mLon",mLon);
    CConfig::setValue("mHeadingGPS",heading);
    ui->label_gps_lat->setText(demicalDegToDegMin(lat)+"'N");
    ui->label_gps_lon->setText(demicalDegToDegMin(lon)+"'E");
    ui->label_gps_heading->setText(QString::number(mHeadingGPSNew,'f',2));
    ui->label_azi_heading_gps->setText(QString::number(mHeadingGPSNew,'f',2));
    osmap->setCenterPos(mLat, mLon);
    isMapOutdated = true;
    repaint();
}

//void Mainwindow::on_dial_valueChanged(int value)
//{
//    float heading = value/100.0f;
//    ui->textEdit_heading->setText(QString::number(heading));

//}

void Mainwindow::on_toolButton_set_heading_clicked()
{

    mHeadingT = ui->textEdit_heading->text().toFloat();
    mHeadingT2 = ui->textEdit_heading_2->text().toFloat();
    CConfig::setValue("mHeadingT",mHeadingT);
    CConfig::setValue("mHeadingT2",mHeadingT2);
    pRadar->setAziOffset(mHeadingT);

}

void Mainwindow::on_toolButton_gps_update_clicked()
{

    SetGPS(ui->text_latInput_2->text().toFloat(),ui->text_longInput_2->text().toFloat(),0);

}




//void Mainwindow::on_toolButton_centerZoom_clicked()
//{
//    pRadar->updateZoomRect(mousePointerX - radCtX,mousePointerY - radCtY);
//}

void Mainwindow::on_toolButton_xl_dopler_clicked()
{

}

void Mainwindow::on_toolButton_xl_dopler_toggled(bool checked)
{
    pRadar->xl_dopler = checked;
}


void Mainwindow::on_toolButton_xl_nguong_3_toggled(bool checked)
{
    pRadar->cut_thresh = checked;
}

void Mainwindow::on_groupBox_3_currentChanged(int index)
{
    if(index==1)
    {
        pRadar->isManualTune = true;
    }
    else
    {
        pRadar->isManualTune = false;
    }
}

void Mainwindow::on_toolButton_xl_dopler_2_toggled(bool checked)
{
    pRadar->bo_bang_0 = checked;
}



void Mainwindow::on_toolButton_reset_3_clicked()
{
    pRadar->resetTrack();
    //    for(short i = 0;i<targetDisplayList.size();i++)
    //    {
    //        targetDisplayList.at(i)->deleteLater();
    //    }
    //    targetDisplayList.clear();
}

void Mainwindow::on_toolButton_reset_2_clicked()
{
    pRadar->resetSled();
}
void Mainwindow::on_toolButton_vet_clicked(bool checked)
{
    pRadar->isSled = checked;
}

void Mainwindow::on_label_status_warning_clicked()
{
    if(warningList.size())warningList.removeAt(warningList.size()-1);
    if(warningList.size())
    {
        ui->label_status_warning->setText(warningList.at(warningList.size()-1));
    }
    else
    {
        ui->label_status_warning->setText(QString::fromUtf8("Không cảnh báo"));
        ui->label_status_warning->setStyleSheet("background-color: rgb(20, 40, 60,255);");
    }
}

void Mainwindow::on_toolButton_delete_target_clicked()
{
    /*if(targetList.at(selected_target_index)->isLost)
            {
                targetList.at(selected_target_index)->hide();
            }

            else*/
    //    pRadar->mTrackList.at(targetDisplayList.at(selected_target_index)->trackId).isManual = false;
}

void Mainwindow::on_toolButton_tx_clicked()
{
    //processing->radTxOn();
    sendToRadarString(CConfig::getString("mTxCommand"));
}


void Mainwindow::on_toolButton_tx_off_clicked()
{
    //processing->radTxOff();
    sendToRadarString(CConfig::getString("mRxCommand"));
}

void Mainwindow::on_toolButton_filter2of3_clicked(bool checked)
{
    pRadar->filter2of3 = checked;
}




//void Mainwindow::on_comboBox_radar_resolution_currentIndexChanged(int index)
//{

//}

void Mainwindow::on_toolButton_create_zone_2_clicked(bool checked)
{
    if(checked)
        gz2.isActive = false;
}

void Mainwindow::on_toolButton_measuring_clicked()
{
    mMouseLastX = radCtX;
    mMouseLastY = radCtY;
}


/*void Mainwindow::on_comboBox_2_currentIndexChanged(int index)
{
    return;

}*/

void Mainwindow::on_toolButton_measuring_clicked(bool checked)
{
    ui->toolButton_grid->setChecked(true);
}

void Mainwindow::on_toolButton_export_data_clicked(bool checked)
{
    pRadar->data_export = checked;
}





void Mainwindow::on_toolButton_auto_select_toggled(bool checked)
{
    setMouseMode(MouseAutoSelect,checked);
    //    if(!checked)
    //    {
    //        this->setCursor(Qt::ArrowCursor);
    //    }
    //    else
    //    {
    //        this->setCursor(Qt::CrossCursor);
    //    }
}

void Mainwindow::on_toolButton_ais_reset_clicked()
{
    processing->m_aisList.clear();
}



//void Mainwindow::on_toolButton_2x_zoom_clicked(bool checked)
//{
//    if(checked)
//    {
//        pRadar->setScaleZoom(8);
//    }
//    else
//    {
//        pRadar->setScaleZoom(4);
//    }
//}

void Mainwindow::on_toolButton_auto_adapt_clicked()
{/*
            if(config.getRangeView()<=2)
            {
                sendToRadarHS("1aab200100000000");// bat thich nghi
                sendToRadarHS("14abff1100000000");// do trong
                sendToRadarHS("08ab000000000000");//do phan giai
                sendToRadarHS("01ab040000000000");//tin hieu dttt32
                sendToRadarHS("aaab020000000000");//tat phat
                sendToRadarHS("aaab020000000000");//tat phat
                sendToRadarHS("aaab020000000000");//tat phat
                sendToRadarHS("1aab200000000000");//tat thich nghi
                sendToRadarHS("aaab020100000000");//bat phat
                sendToRadarHS("aaab020100000000");//bat phat
                sendToRadarHS("aaab020100000000");//bat phat
                sendToRadarHS("aaab030500000000");//toc do quay
                sendToRadarHS("aaab030500000000");//toc do quay
                sendToRadarHS("aaab030500000000");//toc do quay

            }
            else if(config.getRangeView() ==3)
            {
                sendToRadarHS("1aab200100000000");// bat thich nghi
                sendToRadarHS("08ab010000000000");//do phan giai
                sendToRadarHS("14abff0100000000");// do trong
                sendToRadarHS("01ab040100000000");//tin hieu dttt64
                sendToRadarHS("aaab020000000000");//tat phat
                sendToRadarHS("aaab020000000000");//tat phat
                sendToRadarHS("aaab020000000000");//tat phat
                sendToRadarHS("1aab200000000000");//tat thich nghi
                sendToRadarHS("aaab020100000000");//bat phat
                sendToRadarHS("aaab020100000000");//bat phat
                sendToRadarHS("aaab020100000000");//bat phat
                sendToRadarHS("aaab030400000000");//toc do quay
                sendToRadarHS("aaab030400000000");//toc do quay
                sendToRadarHS("aaab030400000000");//toc do quay
            }
            else if(config.getRangeView()==4)
            {
                sendToRadarHS("1aab200100000000");// bat thich nghi
                sendToRadarHS("08ab020000000000");//do phan giai 30
                sendToRadarHS("14abff0100000000");// do trong
                sendToRadarHS("01ab040200000000");//tin hieu dttt128
                sendToRadarHS("aaab020000000000");//tat phat
                sendToRadarHS("aaab020000000000");//tat phat
                sendToRadarHS("aaab020000000000");//tat phat
                sendToRadarHS("1aab200000000000");//tat thich nghi
                sendToRadarHS("aaab020100000000");//bat phat
                sendToRadarHS("aaab020100000000");//bat phat
                sendToRadarHS("aaab020100000000");//bat phat
                sendToRadarHS("aaab030400000000");//toc do quay
                sendToRadarHS("aaab030400000000");//toc do quay
                sendToRadarHS("aaab030400000000");//toc do quay
            }
            else if(config.getRangeView()==5)
            {
                sendToRadarHS("1aab200100000000");// bat thich nghi
                sendToRadarHS("08ab020000000000");//do phan giai 60
                sendToRadarHS("14abff0100000000");// do trong
                sendToRadarHS("01ab040300000000");//tin hieu dttt256
                sendToRadarHS("aaab020000000000");//tat phat
                sendToRadarHS("aaab020000000000");//tat phat
                sendToRadarHS("aaab020000000000");//tat phat
                sendToRadarHS("1aab200000000000");//tat thich nghi
                sendToRadarHS("aaab020100000000");//bat phat
                sendToRadarHS("aaab020100000000");//bat phat
                sendToRadarHS("aaab020100000000");//bat phat
                sendToRadarHS("aaab030300000000");//toc do quay
                sendToRadarHS("aaab030300000000");//toc do quay
                sendToRadarHS("aaab030300000000");//toc do quay
            }
            else if(config.getRangeView()==6)
            {
                sendToRadarHS("1aab200100000000");// bat thich nghi
                sendToRadarHS("08ab030000000000");//do phan giai 90
                sendToRadarHS("14abff0100000000");// do trong
                sendToRadarHS("01ab040300000000");//tin hieu dttt256
                sendToRadarHS("aaab020000000000");//tat phat
                sendToRadarHS("aaab020000000000");//tat phat
                sendToRadarHS("aaab020000000000");//tat phat
                sendToRadarHS("1aab200000000000");//tat thich nghi
                sendToRadarHS("aaab020100000000");//bat phat
                sendToRadarHS("aaab020100000000");//bat phat
                sendToRadarHS("aaab020100000000");//bat phat
                sendToRadarHS("aaab030200000000");//toc do quay
                sendToRadarHS("aaab030200000000");//toc do quay
                sendToRadarHS("aaab030200000000");//toc do quay
            }
            else if(config.getRangeView()==7)
            {
                sendToRadarHS("1aab200100000000");// bat thich nghi
                sendToRadarHS("08ab040000000000");//do phan giai 120
                sendToRadarHS("14abff0100000000");// do trong
                sendToRadarHS("01ab040300000000");//tin hieu dttt256
                sendToRadarHS("aaab020000000000");//tat phat
                sendToRadarHS("aaab020000000000");//tat phat
                sendToRadarHS("aaab020000000000");//tat phat
                sendToRadarHS("1aab200000000000");//tat thich nghi
                sendToRadarHS("aaab020100000000");//bat phat
                sendToRadarHS("aaab020100000000");//bat phat
                sendToRadarHS("aaab020100000000");//bat phat
                sendToRadarHS("aaab030100000000");//toc do quay
                sendToRadarHS("aaab030100000000");//toc do quay
                sendToRadarHS("aaab030100000000");//toc do quay
            }
            else if(config.getRangeView() ==8)
            {
                sendToRadarHS("1aab200100000000");// bat thich nghi
                sendToRadarHS("08ab050000000000");//do phan giai 150
                sendToRadarHS("14abff0100000000");// do trong
                sendToRadarHS("01ab040300000000");//tin hieu dttt256
                sendToRadarHS("aaab020000000000");//tat phat
                sendToRadarHS("aaab020000000000");//tat phat
                sendToRadarHS("aaab020000000000");//tat phat
                sendToRadarHS("1aab200000000000");//tat thich nghi
                sendToRadarHS("aaab020100000000");//bat phat
                sendToRadarHS("aaab020100000000");//bat phat
                sendToRadarHS("aaab020100000000");//bat phat
                sendToRadarHS("aaab030100000000");//toc do quay
                sendToRadarHS("aaab030100000000");//toc do quay
                sendToRadarHS("aaab030100000000");//toc do quay
            }
            else if(config.getRangeView()==9)
            {
                sendToRadarHS("1aab200100000000");// bat thich nghi
                sendToRadarHS("08ab060000000000");//do phan giai 180
                sendToRadarHS("14abff0100000000");// do trong
                sendToRadarHS("01ab040300000000");//tin hieu dttt256
                sendToRadarHS("aaab020000000000");//tat phat
                sendToRadarHS("aaab020000000000");//tat phat
                sendToRadarHS("aaab020000000000");//tat phat
                sendToRadarHS("1aab200000000000");//tat thich nghi
                sendToRadarHS("aaab020100000000");//bat phat
                sendToRadarHS("aaab020100000000");//bat phat
                sendToRadarHS("aaab020100000000");//bat phat
                sendToRadarHS("aaab030100000000");//toc do quay
                sendToRadarHS("aaab030100000000");//toc do quay
                sendToRadarHS("aaab030100000000");//toc do quay
            }
            pRadar->resetTrack();*/
    //    for(short i = 0;i<targetDisplayList.size();i++)
    //    {
    //        targetDisplayList.at(i)->deleteLater();
    //    }
    //    targetDisplayList.clear();
}

void Mainwindow::on_toolButton_set_header_size_clicked()
{
    pRadar->SetHeaderLen(ui->textEdit_header_len->text().toInt());
}

void Mainwindow::on_toolButton_xl_nguong_clicked()
{

}

void Mainwindow::on_toolButton_xl_nguong_clicked(bool checked)
{

}

void Mainwindow::on_toolButton_filter2of3_2_clicked(bool checked)
{
    //pRadar->setDoubleFilter(checked);
}

void Mainwindow::on_toolButton_command_dopler_clicked()
{
    ui->lineEdit_byte_1->setText("05");
    ui->lineEdit_byte_2->setText("ab");
    ui->lineEdit_byte_3->setText("02");
    ui->lineEdit_byte_4->setText("00");
    ui->lineEdit_byte_5->setText("00");
    ui->lineEdit_byte_6->setText("00");
}

void Mainwindow::on_toolButton_command_period_clicked()
{
    ui->lineEdit_byte_1->setText("14");
    ui->lineEdit_byte_2->setText("ab");
    ui->lineEdit_byte_3->setText("ff");
    ui->lineEdit_byte_4->setText("00");
    ui->lineEdit_byte_5->setText("00");
    ui->lineEdit_byte_6->setText("00");
}

void Mainwindow::on_toolButton_noise_balance_clicked()
{
    ui->lineEdit_byte_1->setText("1a");
    ui->lineEdit_byte_2->setText("ab");
    ui->lineEdit_byte_3->setText("20");
    ui->lineEdit_byte_4->setText("01");
    ui->lineEdit_byte_5->setText("00");
    ui->lineEdit_byte_6->setText("00");
}

void Mainwindow::on_toolButton_limit_signal_clicked()
{
    ui->lineEdit_byte_1->setText("17");
    ui->lineEdit_byte_2->setText("ab");
    ui->lineEdit_byte_3->setText("64");
    ui->lineEdit_byte_4->setText("00");
    ui->lineEdit_byte_5->setText("00");
    ui->lineEdit_byte_6->setText("00");
}

void Mainwindow::on_toolButton_command_amplifier_clicked()
{
    ui->lineEdit_byte_1->setText("aa");
    ui->lineEdit_byte_2->setText("ab");
    ui->lineEdit_byte_3->setText("01");
    ui->lineEdit_byte_4->setText("01");
    ui->lineEdit_byte_5->setText("1f");
    ui->lineEdit_byte_6->setText("00");
}

void Mainwindow::on_toolButton_command_dttt_clicked()
{
    ui->lineEdit_byte_1->setText("01");
    ui->lineEdit_byte_2->setText("ab");
    ui->lineEdit_byte_3->setText("04");
    ui->lineEdit_byte_4->setText("00");
    ui->lineEdit_byte_5->setText("00");
    ui->lineEdit_byte_6->setText("00");
}

void Mainwindow::on_toolButton_command_res_clicked()
{
    ui->lineEdit_byte_1->setText("08");
    ui->lineEdit_byte_2->setText("ab");
    ui->lineEdit_byte_3->setText("00");
    ui->lineEdit_byte_4->setText("00");
    ui->lineEdit_byte_5->setText("00");
    ui->lineEdit_byte_6->setText("00");
}

void Mainwindow::on_toolButton_command_antenna_rot_clicked()
{
    ui->lineEdit_byte_1->setText("aa");
    ui->lineEdit_byte_2->setText("ab");
    ui->lineEdit_byte_3->setText("03");
    ui->lineEdit_byte_4->setText("00");
    ui->lineEdit_byte_5->setText("00");
    ui->lineEdit_byte_6->setText("00");
}

void Mainwindow::on_comboBox_3_currentIndexChanged(int index)
{
    osmap->SetType(index);
    isMapOutdated = true;

}

void Mainwindow::on_horizontalSlider_map_brightness_valueChanged(int value)
{
    mMapOpacity = value/50.0;
    CConfig::setValue("mMapOpacity",mMapOpacity);
    isMapOutdated = true;
}



void Mainwindow::on_toolButton_selfRotation_toggled(bool checked)
{
    if(checked)
    {
        double rate = ui->lineEdit_selfRotationRate->text().toDouble();
        pRadar->SelfRotationOn(rate);
    }
    else
        pRadar->SelfRotationOff();
}

void Mainwindow::on_toolButton_scope_toggled(bool checked)
{
    setMouseMode(MouseScope,checked);
}

void Mainwindow::on_toolButton_manual_track_toggled(bool checked)
{
    setMouseMode(MouseAddingTrack,checked);
}
void Mainwindow::setMouseMode(mouseMode mode,bool isOn)
{
    if(isOn)
    {
        mouse_mode = static_cast<mouseMode>(mouse_mode|mode) ;
        //printf("\ntrue:%d",mouse_mode|mode);
    }
    else
    {
        mouse_mode = static_cast<mouseMode>(mouse_mode-(mode&mouse_mode));
        //printf("\nfalse:%d",mouse_mode-(mode&mouse_mode));
    }

}
void Mainwindow::on_toolButton_measuring_toggled(bool checked)
{
    setMouseMode(MouseMeasuring,checked);
}

void Mainwindow::on_toolButton_VRM_toggled(bool checked)
{
    setMouseMode(MouseVRM,checked);
}

void Mainwindow::on_toolButton_ELB_toggled(bool checked)
{
    setMouseMode(MouseELB,checked);
}

void Mainwindow::on_toolButton_record_clicked()
{

}

void Mainwindow::on_toolButton_sharp_eye_toggled(bool checked)
{
    pRadar->setIsSharpEye(checked);
}

void Mainwindow::on_toolButton_help_clicked()
{

    DialogDocumentation *dlg=new DialogDocumentation();
    dlg->setModal(false);
    dlg->showNormal();
    printf("\nNew windows");

}

void Mainwindow::on_toolButton_setRangeUnit_clicked()
{
    switch(mDistanceUnit)
    {
    case 0:
        this->setDistanceUnit(1);
        break;
    case 1:
        this->setDistanceUnit(0);
        break;
    default:
        break;
    }


}



void Mainwindow::on_toolButton_tx_2_clicked()
{
    sendToRadarString(CConfig::getString("mFreq1Command"));

}

void Mainwindow::on_toolButton_tx_3_clicked()
{
    sendToRadarString(CConfig::getString("mFreq2Command"));
}

void Mainwindow::on_toolButton_tx_4_clicked()
{
    sendToRadarString(CConfig::getString("mFreq3Command"));
}

void Mainwindow::on_toolButton_tx_5_clicked()
{
    sendToRadarString(CConfig::getString("mFreq4Command"));
}

void Mainwindow::on_toolButton_tx_6_clicked()
{
    sendToRadarString(CConfig::getString("mFreq5Command"));
}

void Mainwindow::on_toolButton_tx_7_clicked()
{
    sendToRadarString(CConfig::getString("mFreq6Command"));
}

void Mainwindow::on_toolButton_gps_update_auto_clicked()
{
    UpdateGpsData();
}

void Mainwindow::UpdateGpsData()
{

    if(processing->mGpsData.size()>4)
    {
        ui->groupBox_gps->setTitle(QString::fromUtf8("GPS(đã kết nối)"));
        GPSData data = processing->mGpsData.back();
        processing->mGpsData.pop();
        SetGPS(data.lat, data.lon,data.heading);
    }
    else ui->groupBox_gps->setTitle(QString::fromUtf8("GPS(chưa kết nối)"));
}
void Mainwindow::on_toolButton_set_zoom_ar_size_clicked()
{
    mZoomSizeRg = ui->textEdit_size_ar_r->text().toDouble();
    mZoomSizeAz = ui->textEdit_size_ar_a->text().toDouble();
    CConfig::setValue("mZoomSizeRg",mZoomSizeRg);
    CConfig::setValue("mZoomSizeAz",mZoomSizeAz);
}

void Mainwindow::on_toolButton_advanced_control_clicked()
{
    if(ui->lineEdit_password->text()=="ccndt3108")
    {
        ui->groupBox_control->setHidden(false);
    }
}



void Mainwindow::on_toolButton_set_command_clicked()
{/*
    CConfig::getString("mR0Command") = ui->plainTextEdit_range_0->toPlainText();
    CConfig::getString("mR1Command") = ui->plainTextEdit_range_1->toPlainText();
    CConfig::getString("mR2Command") = ui->plainTextEdit_range_2->toPlainText();
    CConfig::getString("mR3Command") = ui->plainTextEdit_range_3->toPlainText();
    CConfig::getString("mR4Command") = ui->plainTextEdit_range_4->toPlainText();
    CConfig::getString("mR5Command") = ui->plainTextEdit_range_5->toPlainText();
    CConfig::getString("mR6Command") = ui->plainTextEdit_range_6->toPlainText();
    CConfig::getString("mR7Command") = ui->plainTextEdit_range_7->toPlainText();
    CConfig::getString("mRxCommand") = ui->plainTextEdit_command_rx->toPlainText();
    CConfig::getString("mTxCommand") = ui->plainTextEdit_command_tx->toPlainText();
    CConfig::setValue("CConfig::getString("mR0Command")",CConfig::getString("mR0Command"));
    CConfig::setValue("CConfig::getString("mR1Command")",CConfig::getString("mR1Command"));
    CConfig::setValue("CConfig::getString("mR2Command")",CConfig::getString("mR2Command"));
    CConfig::setValue("CConfig::getString("mR3Command")",CConfig::getString("mR3Command"));
    CConfig::setValue("CConfig::getString("mR4Command")",CConfig::getString("mR4Command"));
    CConfig::setValue("CConfig::getString("mR5Command")",CConfig::getString("mR5Command"));
    CConfig::setValue("CConfig::getString("mR6Command")",CConfig::getString("mR6Command"));
    CConfig::setValue("CConfig::getString("mR7Command")",CConfig::getString("mR7Command"));
    CConfig::setValue("CConfig::getString("mRxCommand")",CConfig::getString("mRxCommand"));
    CConfig::setValue("CConfig::getString("mTxCommand")",CConfig::getString("mTxCommand"));
    //
    CConfig::getString("mFreq1Command") =  ui->plainTextEdit_freq_1->toPlainText();
    CConfig::getString("mFreq2Command") =  ui->plainTextEdit_freq_2->toPlainText();
    CConfig::getString("mFreq3Command") =  ui->plainTextEdit_freq_3->toPlainText();
    CConfig::getString("mFreq4Command") =  ui->plainTextEdit_freq_4->toPlainText();
    CConfig::getString("mFreq5Command") =  ui->plainTextEdit_freq_5->toPlainText();
    CConfig::getString("mFreq6Command") =  ui->plainTextEdit_freq_6->toPlainText();
    //
    CConfig::setValue("CConfig::getString("mFreq1Command")",CConfig::getString("mFreq1Command"));
    CConfig::setValue("CConfig::getString("mFreq2Command")",CConfig::getString("mFreq2Command"));
    CConfig::setValue("CConfig::getString("mFreq3Command")",CConfig::getString("mFreq3Command"));
    CConfig::setValue("CConfig::getString("mFreq4Command")",CConfig::getString("mFreq4Command"));
    CConfig::setValue("CConfig::getString("mFreq5Command")",CConfig::getString("mFreq5Command"));
    CConfig::setValue("CConfig::getString("mFreq6Command")",CConfig::getString("mFreq6Command"));
    ui->groupBox_control_setting->setHidden(true);
    ui->toolButton_set_commands->setChecked(false);*/

}

void Mainwindow::on_toolButton_grid_clicked(bool checked)
{

}

void Mainwindow::on_toolButton_auto_freq_toggled(bool checked)
{
    if(checked) autoSwitchFreq();
}

void Mainwindow::on_toolButton_set_default_clicked()
{
    CConfig::setDefault();
}



void Mainwindow::on_toolButton_heading_update_clicked()
{
    /*if(processing->isHeadingAvaible)
    {
        mHeadingT = processing->getHeading()+CConfig::getDouble("mHeadingT3");
        if(mHeadingT>=360)mHeadingT-=360;
        ui->label_compass_value->setText(QString::number(processing->getHeading(),'f',1));
        ui->textEdit_heading->setText(QString::number(mHeadingT));
    }
    else
    {
        warningList.push_back(QString::fromUtf8("Chưa kết nối la bàn"));
    }*/
}

void Mainwindow::on_toolButton_sled_clicked()
{

}

void Mainwindow::on_toolButton_sled_toggled(bool checked)
{
    pRadar->isSled=checked;
}

void Mainwindow::on_toolButton_sled_reset_clicked()
{
    pRadar->resetSled();
}



void Mainwindow::on_toolButton_dobupsong_toggled(bool checked)
{
    pRadar->is_do_bup_song = checked;
    if(checked)
    {
        pRadar->setTb_tap_k(ui->textEdit_dobupsongk->text().toDouble());

    }
}


void Mainwindow::on_toolButton_set_commands_clicked()
{
    DialogCommandSet *dlg=new DialogCommandSet();
    dlg->setModal(false);
    dlg->showNormal();

}

void Mainwindow::on_toolButton_command_log_toggled(bool checked)
{
    if(checked)
    {
        cmLog->show();
    }
    else
    {
        cmLog->hide();
    }
}

void Mainwindow::on_toolButton_exit_2_clicked()
{
    mstatWin = new StatusWindow(processing);

    mstatWin->show();
}

void Mainwindow::on_toolButton_selfRotation_2_toggled(bool checked)
{
    //pRadar->isEncoderAzi  = checked;
    if(checked)
    {
        double rate = ui->lineEdit_selfRotationRate->text().toDouble();
        //        rate = rate/MAX_AZIR;
        pRadar->SelfRotationOn(rate);
    }
    else
        pRadar->SelfRotationOff();
}

void Mainwindow::on_toolButton_selfRotation_clicked()
{

}

void Mainwindow::on_toolButton_tx_2_toggled(bool checked)
{

}

void Mainwindow::on_toolButton_tx_2_clicked(bool checked)
{

}

void Mainwindow::on_toolButton_menu_clicked()
{
    if(ui->tabWidget_menu->isHidden())
    {
        ui->tabWidget_menu->setHidden(false);
        ui->tabWidget_menu->setGeometry(550,180,ui->tabWidget_menu->width(),ui->tabWidget_menu->height());
        ui->tabWidget_menu->setCurrentIndex(0);
    }
    else
    {
        ui->tabWidget_menu->setGeometry(200,-800,ui->tabWidget_menu->width(),ui->tabWidget_menu->height());
        ui->tabWidget_menu->setHidden(true);
    }
}

void Mainwindow::on_toolButton_iad_clicked()
{
    if(ui->tabWidget_iad->isHidden())
    {
        ui->tabWidget_iad->setHidden(false);
        ui->tabWidget_iad->setGeometry(5,180,ui->tabWidget_iad->width(),ui->tabWidget_iad->height());
        ui->tabWidget_iad->setCurrentIndex(0);
    }
    else
    {
        ui->tabWidget_iad->setGeometry(200,-800,ui->tabWidget_iad->width(),ui->tabWidget_iad->height());
        ui->tabWidget_iad->setHidden(true);
    }
}

void Mainwindow::on_tabWidget_menu_tabBarClicked(int index)
{
    if(ui->tabWidget_menu->count()-1==index)
    {
        ui->tabWidget_menu->setHidden(true);
    }

}

void Mainwindow::on_tabWidget_iad_tabBarClicked(int index)
{
    if(ui->tabWidget_iad->count()-1==index)
    {
        ui->tabWidget_iad->setHidden(true);
    }
}

void Mainwindow::on_toolButton_xl_nguong_3_clicked()
{

}

void Mainwindow::on_toolButton_head_up_toggled(bool checked)
{
    isHeadUp = checked;
    dx = 0;
    dy = 0;
    isMapOutdated = true;
}

//void Mainwindow::on_toolButton_delete_target_2_clicked()
//{
//    QStringList list = ui->textEdit_systemCommand->toPlainText().split(';');
//    for(int i=0;i<list.size();i++)
//    {
//        QByteArray ba=list.at(i).toLatin1();
//        sendToRadarHS(ba.data());
//    }
//}
unsigned char commandMay22[]={0xaa,0x55,0x02,0x0c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
void Mainwindow::on_toolButton_dk_1_toggled(bool checked)
{
    return;
    if(checked)
    {
        commandMay22[4]=0x00;
        processing->sendCommand(commandMay22,12,false);
    }

}

void Mainwindow::on_toolButton_dk_2_toggled(bool checked)
{
    if(checked)
    {

    }
}

void Mainwindow::on_toolButton_dk_3_toggled(bool checked)
{
    if(checked)
    {

    }
}

void Mainwindow::on_toolButton_dk_4_toggled(bool checked)
{
    if(checked)
    {

    }
}

void Mainwindow::on_toolButton_dk_5_toggled(bool checked)
{
    if(checked)
    {

    }
}

void Mainwindow::on_toolButton_dk_6_toggled(bool checked)
{
    if(checked)
    {

    }
}

void Mainwindow::on_toolButton_dk_7_toggled(bool checked)
{
    if(checked)
    {

    }
}

void Mainwindow::on_toolButton_dk_8_toggled(bool checked)
{
    if(checked)
    {

    }
}

void Mainwindow::on_toolButton_dk_9_toggled(bool checked)
{
    if(checked)
    {

    }
}

void Mainwindow::on_toolButton_dk_10_toggled(bool checked)
{
    return;
    if(checked)
    {
        commandMay22[8]=0x01;
        processing->sendCommand(commandMay22,12,false);
    }

}

void Mainwindow::on_toolButton_dk_12_toggled(bool checked)
{
    return;
    if(checked)
    {
        commandMay22[4]=0x01;
        processing->sendCommand(commandMay22,12,false);
    }
}

void Mainwindow::on_toolButton_dk_14_toggled(bool checked)
{
    return;
    if(checked)
    {
        commandMay22[8]=0x02;
        processing->sendCommand(commandMay22,12,false);
    }
}

void Mainwindow::on_toolButton_dk_13_toggled(bool checked)
{
    if(checked)
    {

    }
}

void Mainwindow::on_toolButton_dk_15_toggled(bool checked)
{
    return;
    if(checked)
    {
        commandMay22[8]=0x00;
        processing->sendCommand(commandMay22,12,false);
    }
}

void Mainwindow::on_toolButton_dk_11_toggled(bool checked)
{
    return;
    if(checked)
    {
        commandMay22[7]=0x00;
        processing->sendCommand(commandMay22,12,false);
    }
}

void Mainwindow::on_toolButton_dk_16_toggled(bool checked)
{
    return;
    if(checked)
    {
        commandMay22[7]=0x01;
        processing->sendCommand(commandMay22,12,false);
    }
}

void Mainwindow::on_toolButton_dk_17_toggled(bool checked)
{
    return;
    if(checked)
    {
        commandMay22[7]=0x02;
        processing->sendCommand(commandMay22,12,false);
    }
}

void Mainwindow::on_toolButton_grid_toggled(bool checked)
{
    toolButton_grid_checked = checked;
}

void Mainwindow::on_toolButton_dk_4_clicked()
{

    commandMay22[6]=0x06;
    processing->sendCommand(commandMay22,12,false);
}

void Mainwindow::on_toolButton_dk_3_clicked()
{
    commandMay22[6]=0x00;
    processing->sendCommand(commandMay22,12,false);
}

void Mainwindow::on_toolButton_dk_5_clicked()
{
    commandMay22[6]=0x05;
    processing->sendCommand(commandMay22,12,false);
}

void Mainwindow::on_toolButton_dk_6_clicked()
{
    commandMay22[6]=0x04;
    processing->sendCommand(commandMay22,12,false);
}

void Mainwindow::on_toolButton_dk_7_clicked()
{
    commandMay22[6]=0x03;
    processing->sendCommand(commandMay22,12,false);
}

void Mainwindow::on_toolButton_dk_8_clicked()
{
    commandMay22[6]=0x02;
    processing->sendCommand(commandMay22,12,false);
}

void Mainwindow::on_toolButton_dk_9_clicked()
{
    commandMay22[6]=0x01;
    processing->sendCommand(commandMay22,12,false);
}

void Mainwindow::on_toolButton_dk_2_clicked()
{
    commandMay22[5]=0x00;
    processing->sendCommand(commandMay22,12,false);
}

void Mainwindow::on_toolButton_dk_13_clicked()
{
    commandMay22[5]=0x01;
    processing->sendCommand(commandMay22,12,false);
}

void Mainwindow::on_toolButton_dk_12_clicked()
{
    commandMay22[4]=0x01;
    processing->sendCommand(commandMay22,12,false);
}

void Mainwindow::on_toolButton_dk_10_clicked()
{
    commandMay22[8]=0x01;
    processing->sendCommand(commandMay22,12,false);
}

void Mainwindow::on_toolButton_dk_14_clicked()
{
    commandMay22[8]=0x02;
    processing->sendCommand(commandMay22,12,false);
}

void Mainwindow::on_toolButton_dk_15_clicked()
{
    commandMay22[8]=0x00;
    processing->sendCommand(commandMay22,12,false);
}

void Mainwindow::on_toolButton_sled_time25_clicked()
{
    pRadar->mSledValue = 180;
}

void Mainwindow::on_toolButton_sled_time8_clicked()
{
    pRadar->mSledValue = 50;
}

void Mainwindow::on_toolButton_sled_time3_clicked()
{
    pRadar->mSledValue = 10;
}

void Mainwindow::on_toolButton_sled_reset_2_clicked(bool checked)
{
    mShowobjects = checked;
}

void Mainwindow::on_toolButton_sled_reset_3_clicked(bool checked)
{
    mShowLines = checked;
}

void Mainwindow::on_toolButton_sled_reset_4_clicked(bool checked)
{
    mShowTracks = checked;
}
