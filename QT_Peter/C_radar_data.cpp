#define PI 3.141592654f
#include "c_radar_data.h"
#include <QElapsedTimer>
#include <cmath>
//#include <QtDebug>

#define PLOT_MAX_SIZE 80
#define PLOT_MIN_SIZE 5
#define PLOT_MAX_DR 10
#define RANGE_MIN 2
#define TERRAIN_MAX 40
#define TERRAIN_INIT 20
#define RADAR_COMMAND_FEEDBACK  6
#define RADAR_DATA_HEADER_MAX   62
#define RADAR_DATA_SPECTRE      22
#define RADAR_DATA_MAX_SIZE     2688
#define RADAR_
#define TARGET_OBSERV_PERIOD 6500//ENVAR max periods to save object in the memory
FILE *logfile;

short waitForData = 0;
short headerLen = RADAR_DATA_HEADER_MAX;
unsigned char curFrameId;
unsigned char dataBuff[RADAR_DATA_HEADER_MAX + RADAR_DATA_MAX_SIZE];
QFile *exp_file = NULL;
int sumvar = 0;
int nNoiseCalculator = 0;
short lastProcessAzi = 0;
std::queue<int>  aziToProcess;//hàng chờ các frame cần xử lý

typedef struct  {
    //processing dataaziQueue
    unsigned char level [MAX_AZIR][RAD_M_PULSE_RES];
    unsigned char level_disp [MAX_AZIR][RAD_M_PULSE_RES];
    bool          detect[MAX_AZIR][RAD_M_PULSE_RES];
    //unsigned char rainLevel[MAX_AZIR][RAD_M_PULSE_RES];
    unsigned char dopler[MAX_AZIR][RAD_M_PULSE_RES];
    unsigned char terrain[MAX_AZIR][RAD_M_PULSE_RES];
    //    unsigned char dopler_old[MAX_AZIR][RAD_M_PULSE_RES];
    //    unsigned char dopler_old2[MAX_AZIR][RAD_M_PULSE_RES];
    unsigned char sled[MAX_AZIR][RAD_M_PULSE_RES];
    unsigned char hot[MAX_AZIR][RAD_M_PULSE_RES];
    unsigned char hot_disp[MAX_AZIR][RAD_M_PULSE_RES];
    short         plotIndex[MAX_AZIR][RAD_M_PULSE_RES];
    //display data
    unsigned char display_ray [RAD_DISPLAY_RES][3];//0 - signal, 1- dopler, 2 - sled;
    unsigned char display_ray_zoom [DISPLAY_RES_ZOOM][3];
    unsigned char display_mask [RAD_DISPLAY_RES*2+1][RAD_DISPLAY_RES*2+1];
    unsigned char display_mask_zoom [DISPLAY_RES_ZOOM*2+1][DISPLAY_RES_ZOOM*2+1];
    short xkm[MAX_AZIR_DRAW][RAD_DISPLAY_RES+1];
    short ykm[MAX_AZIR_DRAW][RAD_DISPLAY_RES+1];
    short xzoom[MAX_AZIR_DRAW][DISPLAY_RES_ZOOM];
    short yzoom[MAX_AZIR_DRAW][DISPLAY_RES_ZOOM];
} signal_map_t;
double sn_scale = SIGNAL_SCALE_0;
short curIdCount = 1;
qint64 cur_timeMSecs = 0;//QDateTime::currentMSecsSinceEpoch();
signal_map_t data_mem;
float                   rot_period_sec =0;
short histogram[256];


C_radar_data::C_radar_data()
{
    mFalsePositiveCount = 0;
    mSledValue = 180;
    rotDir = 0;
    mPeriodCount = 0;
    logfile = fopen("logfile.dat", "wt");
    isMarineMode = true;
    range_max = RADAR_RESOLUTION;
    imgMode = VALUE_ORANGE_BLUE;
    brightness = 1.5;
    //    for(int i=0;i<255;i++)
    //    {

    //        colorTable.push_back((getColor(i,0,0)));
    //    }
    hsTap = 0;
    tb_tap=new unsigned short[MAX_AZIR];
    img_histogram=new QImage(257,101,QImage::Format_Mono);
    img_histogram->fill(0);
    img_ppi = new QImage(RAD_DISPLAY_RES*2+1,RAD_DISPLAY_RES*2+1,QImage::Format_ARGB32);
    img_RAmp = new QImage(RAD_M_PULSE_RES,256,QImage::Format_ARGB32);
    img_spectre = new QImage(16,256,QImage::Format_Mono);
    img_spectre->fill(0);
    img_zoom_ppi = new QImage(ZOOM_SIZE+1,ZOOM_SIZE+1,QImage::Format_ARGB32);
    img_zoom_ar = NULL;

    tb_tap_k = 1;
    setZoomRectAR(10,10,1.852,10);
    //    mEncoderAzi = 0;
    //img_zoom_ar->setColorTable(colorTable);
    img_ppi->fill(Qt::transparent);
    isSelfRotation = false;
    isProcessing = true;
    //    isEncoderAzi  =false;
    isManualTune = false;
    isVtorih = true;
    rgs_auto = false;
    //    doubleFilter = false;
    rotation_per_min = 0;
    bo_bang_0 = false;
    data_export = false;
    xl_dopler = true;
    cut_thresh = false;
    filter2of3 = false;
    is_do_bup_song = false;
    clk_adc = 1;
    noiseAverage = 30;
    noiseVar = 8;
    krain_auto = 0.4;
    kgain_auto  = 4.2;
    ksea_auto = 0;
    kgain = 1;
    krain  = ksea = 0;
    brightness = 1;
    avtodetect = true;
    isClkAdcChanged = true;
    isSled = false;
    init_time = 10;
    dataOver = max_s_m_200;
    curAzir = 0;
    arcMaxAzi = 0;
    arcMinAzi = 0;
    isSharpEye = false;
    raw_map_init();
    raw_map_init_zoom();
    setAziOffset(0);
    setScalePPI(1);
    resetData();
    setScaleZoom(4);
    //setZoomRectAR(0,0);
}
C_radar_data::~C_radar_data()
{
    delete img_ppi;
    fclose(logfile);
    //if(pFile)fclose(pFile);
    //    if(pScr_map)
    //    {
    //        delete[] pScr_map;
    //    }
}

double C_radar_data::getArcMaxAziRad() const
{
    double result = (aziOffset+(double)arcMaxAzi/(double)MAX_AZIR*PI_NHAN2);
    if(result>PI_NHAN2)result-=PI_NHAN2;
    return ( result);
}
double C_radar_data::getArcMinAziRad() const
{
    double result = (aziOffset+(double)arcMinAzi/(double)MAX_AZIR*PI_NHAN2);
    if(result>PI_NHAN2)result-=PI_NHAN2;
    return (result );
}

double C_radar_data::getSelfRotationAzi() const
{
    return selfRotationAzi;
}

void C_radar_data::setSelfRotationAzi(int value)
{
    selfRotationAzi = value;

}
double C_radar_data::getCurAziRad() const
{

    double result = (aziOffset+(double)curAzir/(double)MAX_AZIR*PI_NHAN2);
    if(result>PI_NHAN2)result-=PI_NHAN2;
    return ( result);
}
bool C_radar_data::getIsVtorih() const
{
    return isVtorih;
}

void C_radar_data::setIsVtorih(bool value)
{
    isVtorih = value;
}
void C_radar_data::setProcessing(bool onOff)
{
    if(onOff)
    {

        //initData(true);
        isProcessing = true;
        printf("\nSecondary processing mode - on.");
    }
    else
    {
        isProcessing = false;
        printf("\nSecondary processing mode - off.");
    }
}

bool C_radar_data::checkFeedback(unsigned char *command)
{
    for (short i=0;i<8;i++)
    {if(command[i]!=command_feedback[i])return false;}
    memset(&command_feedback[0],0,8);
    return true;
}


void C_radar_data::drawSgn(short azi_draw, short r_pos)
{

    unsigned char value = data_mem.display_ray[r_pos][0];
    unsigned char dopler    = data_mem.display_ray[r_pos][1];
    unsigned char sled     = data_mem.display_ray[r_pos][2];
    short px = data_mem.xkm[azi_draw][r_pos];
    short py = data_mem.ykm[azi_draw][r_pos];
    if(px<=0||py<=0)return;
    short pSize = 1;
    if(r_pos<100)pSize=0;
    else if(r_pos>800)pSize=2;
    if((px<pSize)||(py<pSize)||(px>=img_ppi->width()-pSize)||(py>=img_ppi->height()-pSize))return;
    for(short x = -pSize;x <= pSize;x++)
    {
        for(short y = -pSize;y <= pSize;y++)
        {
            double k ;
            switch((x*x+y*y))
            {

            case 0:
                k=1;
                break;
            case 1:
                if(data_mem.display_mask[px+x][py+y])k=0.6;
                else k=1;
                break;
            case 2:
                if(data_mem.display_mask[px+x][py+y])k=0.4;
                else k=1;
                break;
            default:
                if(data_mem.display_mask[px+x][py+y])continue;

                k=0.2;
                break;
            }
            unsigned char pvalue = value*k;
            if( data_mem.display_mask[px+x][py+y] <= pvalue)
            {
                data_mem.display_mask[px+x][py+y] = pvalue;
                img_ppi->setPixel(px+x,py+y,getColor(pvalue,dopler,sled));//todo: set color table

            }
        }
    }



}

void C_radar_data::drawBlackAzi(short azi_draw)
{
    for (short r_pos = 1;r_pos < RAD_DISPLAY_RES;r_pos++)
    {

        short px = data_mem.xkm[azi_draw][r_pos];
        short py = data_mem.ykm[azi_draw][r_pos];
        if(px<0||py<0)continue;
        short pSize = 1;
        if(r_pos<100)pSize=0;
        else if(r_pos>800)pSize=2;
        else if((px<pSize)||(py<pSize)||(px>=img_ppi->width()-pSize)||(py>=img_ppi->height()-pSize))continue;

        for(short x = -pSize;x <= pSize;x++)
        {
            for(short y = -pSize;y <= pSize;y++)
            {
                //                if(r_pos<100)data_mem.display_mask[px+x][py+y] *=0.9;
                //                else
                data_mem.display_mask[px+x][py+y] =0;
            }
        }
    }
    for (short r_pos = 1;r_pos<DISPLAY_RES_ZOOM;r_pos++)
    {

        short px = data_mem.xzoom[azi_draw][r_pos];
        short py = data_mem.yzoom[azi_draw][r_pos];
        if(px<0||py<0)continue;
        short pSize = 1;
        if((px<pSize)||(py<pSize)||(px>=img_zoom_ppi->width()-pSize)||(py>=img_zoom_ppi->height()-pSize))continue;

        for(short x = -pSize;x <= pSize;x++)
        {
            for(short y = -pSize;y <= pSize;y++)
            {

                data_mem.display_mask_zoom[px+x][py+y] = 0;
            }
        }
    }
}
void C_radar_data::drawAzi(short azi)
{

    int leftAzi = azi-1;if(leftAzi<0)leftAzi+=MAX_AZIR;
    int rightAzi = azi +1; if(rightAzi>=MAX_AZIR)rightAzi-=MAX_AZIR;
    if(lastProcessAzi==leftAzi)
    {
        if(rotDir>-3)
        {
            rotDir--;
            if(rotDir==-1)init_time+=2;
        }
    }
    else if(lastProcessAzi==rightAzi)
    {
        if(rotDir<2)
        {
            rotDir++;
            if(rotDir==0)init_time+=2;
        }
    }


    if(rotDir<0)
    {
        //reset the display masks
        short prev_azi = azi - 20;
        if(prev_azi<0)prev_azi += MAX_AZIR;
        drawBlackAzi(prev_azi*3+2);
        drawBlackAzi(prev_azi*3+1);
        drawBlackAzi(prev_azi*3);
        //reset the drawing ray
        memset(&data_mem.display_ray[0][0],0,RAD_DISPLAY_RES*3);
        //memset(&signal_map.display_zoom[0][0],0,DISPLAY_RES_ZOOM*3);
        //set data to the drawing ray

    }
    else
    {
        //reset the display masks
        short prev_azi = azi + 20;
        if(prev_azi>=MAX_AZIR)prev_azi -= MAX_AZIR;
        drawBlackAzi(prev_azi*3);
        drawBlackAzi(prev_azi*3+1);
        drawBlackAzi(prev_azi*3+2);
        //reset the drawing ray
        memset(&data_mem.display_ray[0][0],0,RAD_DISPLAY_RES*3);

    }


    unsigned short  lastDisplayPos =0;
    for (short r_pos = 0;r_pos<range_max-1;r_pos++)
    {
        unsigned short value = data_mem.level_disp[azi][r_pos];
        unsigned short dopler = data_mem.dopler[azi][r_pos];
        if(DrawZoomAR(azi,r_pos,
                      value,
                      dopler,
                      data_mem.sled[azi][r_pos]))
        {
            value+=30;
            if(value>255)value=255;
        }
        //zoom to view scale
        short display_pos = r_pos*scale_ppi;
        short display_pos_next = (r_pos+1)*scale_ppi;
        for(;;)
        {
            if(display_pos>=RAD_DISPLAY_RES)break;
            if(data_mem.display_ray[display_pos][0]<value)
            {
                data_mem.display_ray[display_pos][0] = value;
                data_mem.display_ray[display_pos][1] = dopler;

            }
            if(data_mem.display_ray[display_pos][2] < data_mem.sled[azi][r_pos])
            {
                data_mem.display_ray[display_pos][2] = data_mem.sled[azi][r_pos];
            }
            display_pos++;
            if(display_pos>=display_pos_next)break;
        }
        if(lastDisplayPos<display_pos_next)lastDisplayPos = display_pos_next;
        //zoom to zoom scale !
        short display_pos_zoom = r_pos*scale_zoom_ppi;
        short display_pos_next_zoom  = (r_pos+1)*scale_zoom_ppi;
        for(;;)
        {
            if(display_pos_zoom>=DISPLAY_RES_ZOOM)break;
            if(true)
            {
                data_mem.display_ray_zoom[display_pos_zoom][0] += (value-data_mem.display_ray_zoom[display_pos_zoom][0])/1.4;
                data_mem.display_ray_zoom[display_pos_zoom][1] = dopler;

            }
            if(true)//signal_map.display_zoom[display_pos_zoom][2] < signal_map.sled[azi][r_pos])
            {
                data_mem.display_ray_zoom[display_pos_zoom][2] = data_mem.sled[azi][r_pos];
            }
            display_pos_zoom++;
            if(display_pos_zoom>=display_pos_next_zoom)break;
        }

    }
    if (lastDisplayPos<RAD_DISPLAY_RES)
    {
        for(;lastDisplayPos<RAD_DISPLAY_RES;lastDisplayPos++)
        {

            data_mem.display_ray[lastDisplayPos][0] = 0;
            data_mem.display_ray[lastDisplayPos][1] = 0;
            data_mem.display_ray[lastDisplayPos][2] = 0;
        }
    }
    //smooothing the image
    float k  = scale_ppi/2;
    //printf("\nviewScale:%f",viewScale);
    for(short display_pos = 1;display_pos<DISPLAY_RES_ZOOM; display_pos++)
    {
        data_mem.display_ray_zoom[display_pos][0] = data_mem.display_ray_zoom[display_pos-1][0]
                + ((float)data_mem.display_ray_zoom[display_pos][0]
                -(float)data_mem.display_ray_zoom[display_pos-1][0])/2;
        //signal_map.display_zoom[display_pos][1] = signal_map.display_zoom[display_pos-1][1] + ((float)signal_map.display_zoom[display_pos][1]-(float)signal_map.display_zoom[display_pos-1][1])/3;
        drawSgnZoom(azi*3,display_pos);
        drawSgnZoom(azi*3+1,display_pos);
        drawSgnZoom(azi*3+2,display_pos);
    }


    if(k<=2)
    {
        for(short display_pos = 1;display_pos<RAD_DISPLAY_RES;display_pos++)
        {
            drawSgn(azi*3,display_pos);
            drawSgn(azi*3+1,display_pos);
            drawSgn(azi*3+2,display_pos);
        }

    }
    else
    {
        for(short display_pos = 1;display_pos<RAD_DISPLAY_RES;display_pos++)
        {
            data_mem.display_ray[display_pos][0] = data_mem.display_ray[display_pos-1][0] + ((float)data_mem.display_ray[display_pos][0]-(float)data_mem.display_ray[display_pos-1][0])/k;
            //signal_map.display[display_pos][1] = signal_map.display[display_pos-1][1] + ((float)signal_map.display[display_pos][1]-(float)signal_map.display[display_pos-1][1])/k;
            drawSgn(azi*3,display_pos);
            drawSgn(azi*3+1,display_pos);
            drawSgn(azi*3+2,display_pos);

        }

    }
    //drawingDone = true;

}

void  C_radar_data::getNoiseLevel()
{


    if(nNoiseCalculator<50)return;

    short histogram_max_val=1;
    short histogram_max_pos=0;
    noiseVar+=(sumvar/float(nNoiseCalculator)-noiseVar)/2.0f;
    if(noiseVar<7)noiseVar = 7;
    for(short i = 0;i<256;i++)
    {
        if(histogram[i]>histogram_max_val)
        {
            histogram_max_val = histogram[i];
            histogram_max_pos = i;
        }

    }

    if(noiseAverage)
    {
        noiseAverage += (histogram_max_pos-noiseAverage)/3.0f;
    }
    else
    {
        noiseAverage = 31;//histogram_max_pos;
    }
    img_histogram->fill(0);
    for(short i = 0;i<256;i++)
    {
        histogram[i] = histogram[i]*100/histogram_max_val;
        img_histogram->setPixel(i,100-histogram[i],1);
    }
    short thresh = noiseAverage+(short)noiseVar*kgain;
    if(thresh<0)thresh=0;
    for(short j = 99;j>100-histogram[histogram_max_pos];j--)
    {
        img_histogram->setPixel(histogram_max_pos,j,1);
        if(j>50)img_histogram->setPixel(thresh,j,1);
    }
    //printf("\ntrung binh tap:%f, var:%f",noiseAverage,noiseVar);
    nNoiseCalculator=0;
    memset(histogram,0,256);
    sumvar = 0;

}
void C_radar_data::SetHeaderLen( short len)
{
    headerLen = len;
}
//bool C_radar_data::getDoubleFilter() const
//{
//    return doubleFilter;
//}

//void C_radar_data::setDoubleFilter(bool value)
//{
//    doubleFilter = value;
//}
void C_radar_data::decodeData(int azi)
{/*
    //read spectre
    memcpy((char*)&spectre,(char*)&dataBuff[RADAR_DATA_HEADER_MAX],16);
    img_spectre->fill(0);
    //draw spectre

    for(short i=0;i<16;i++)
    {
        for(short j=255;j>255-spectre[i];j--)
        {
            img_spectre->setPixel(i,j,1);
        }
    }
    // sharpyeye dopler procession

    short i_m  = headerLen;
    short i_s  = i_m + range_max;
    short i_md = i_s + RAD_S_PULSE_RES;
    short i_sd = i_md+ range_max/2;
    for(short r_pos = 0;r_pos<range_max;r_pos++)
    {
        //data_mem.dopler_old[azi][r_pos] = data_mem.dopler[azi][r_pos];
        if(r_pos<RAD_S_PULSE_RES)
        {
            switch (dataOver) {
            case m_only:
                data_mem.level[azi][r_pos] = dataBuff[i_m+r_pos];
                data_mem.dopler[azi][r_pos] = dataBuff[i_md+r_pos/2];
                break;
            case s_m_200:
                if(r_pos<200)
                {
                    data_mem.level[azi][r_pos] = dataBuff[i_s+r_pos];
                    data_mem.dopler[azi][r_pos] = dataBuff[i_sd+r_pos/2];
                }
                else
                {
                    data_mem.level[azi][r_pos] = dataBuff[i_m+r_pos];
                    data_mem.dopler[azi][r_pos] = dataBuff[i_md+r_pos/2];
                }
                break;
            case max_s_m_200:
                if(r_pos<200&&(dataBuff[i_s+r_pos]>dataBuff[i_m+r_pos]))
                {
                    data_mem.level[azi][r_pos]  = dataBuff[i_s  + r_pos];
                    data_mem.dopler[azi][r_pos] = dataBuff[i_sd + r_pos/2];
                }
                else
                {
                    data_mem.level[azi][r_pos] = dataBuff[i_m+r_pos];
                    data_mem.dopler[azi][r_pos] = dataBuff[i_md+r_pos/2];
                }
                break;
            default:
                break;

            }
        }
        else
        {
            data_mem.level[azi][r_pos] = dataBuff[i_m+r_pos];
            data_mem.dopler[azi][r_pos] = dataBuff[i_md+r_pos/2];
        }
        //unzip the dopler data
        if(!(r_pos&0x01))
        {

            data_mem.dopler[azi][r_pos] = 0x0f&data_mem.dopler[azi][r_pos];

        }
        else
        {
            data_mem.dopler[azi][r_pos] = data_mem.dopler[azi][r_pos]>>4;
        }
    }*/
}
short threshRay[RADAR_RESOLUTION];

void C_radar_data::ProcessData(unsigned short azi,unsigned short lastAzi)
{
    float maxRain = 180;//noiseAverage+noiseVar*15;
    rainLevel = noiseAverage ;
    //    int leftAzi = curAzir-1;if(leftAzi<0)leftAzi+=MAX_AZIR;
    //int rightAzi = curAzir +1; if(rightAzi>=MAX_AZIR)rightAzi-=MAX_AZIR;
    //manual threshold
    //auto threshold
    //memset(&threshRay[0],0,RADAR_RESOLUTION*2);

    //    for(short r_pos=0;r_pos<range_max;r_pos++)
    //    {
    //        //update dopler histogram
    //        unsigned char dopl = data_mem.dopler[azi][r_pos];
    //        doplerHistogram[dopl]++;
    //        if(doplerHistMaxVal<doplerHistogram[dopl])
    //        {
    //            doplerHistMaxVal=doplerHistogram[dopl];
    //            doplerHistMaxPoint = dopl;
    //        }
    //    }
    for(short r_pos=0;r_pos<range_max;r_pos++)
    {
        // RGS threshold
        rainLevel += krain_auto*(data_mem.level[azi][r_pos]-rainLevel);
        if(rainLevel>maxRain)rainLevel = maxRain;
        short nthresh = rainLevel + noiseVar*kgain_auto;
        threshRay[r_pos] += (nthresh-threshRay[r_pos])*0.5;
        bool cutoff = data_mem.level[azi][r_pos]<threshRay[r_pos];
        if(data_mem.dopler[azi][r_pos]!=data_mem.dopler[lastAzi][r_pos])cutoff = true;

        if(cut_thresh)
        {
            short dif = (data_mem.level[azi][r_pos]+32+ noiseVar*kgain_auto -threshRay[r_pos]);
            if(dif<0)dif=0;
            else if(dif>255)dif=255;
            data_mem.level_disp[azi][r_pos]=dif;
        }
        else
            data_mem.level_disp[azi][r_pos]=data_mem.level[azi][r_pos];
        data_mem.detect[azi][r_pos] = (!cutoff);
        //data_mem.detect[azi][r_pos] = (!cutoff);
        //detect = vuot nguong & dopler lap lai

        if(data_mem.detect[azi][r_pos])
        {
            if(!init_time)
            {data_mem.sled[azi][r_pos] = mSledValue;
                if(r_pos>RANGE_MIN)procPix(azi,r_pos);}
        }
        else
        {
            data_mem.sled[azi][r_pos] -= (data_mem.sled[azi][r_pos])/20.0f;
            if(rgs_auto)data_mem.level_disp[azi][r_pos]= 0;
        }


    }
    return ;
}
void C_radar_data::ProcessEach90Deg()
{
    DetectTracks();
    getNoiseLevel();
    //remove old points
    int nObj = 0;
    for (uint i=0;i<mObjList.size();i++)
    {

        if(!mObjList.at(i).isDead)
        {
            if((now_ms-mObjList.at(i).timeMs)>20000)
                mObjList.at(i).isDead = true;
            else nObj++;
        }

    }

    //kill old tracks
    for(int i=0;i<mTrackList.size();i++)
    {
        if(!mTrackList[i].isDead)
        {
            if((now_ms -mTrackList[i].objectList.back().timeMs)>30000)
                mTrackList[i].isDead = true;
        }
    }
    //
    if(nObj>1500)
    {
        if(kgain_auto<7.5)kgain_auto*=1.01;
        printf("\ntoo many obj,kgain_auto:%f",kgain_auto);
    }
    else if(nObj<500)if(kgain_auto>4.2)kgain_auto/=1.01;
    if(mFalsePositiveCount>15)//ENVAR
    {
        if(kgain_auto<10)kgain_auto*=1.01;
        printf("\ntoo many false positive kgain_auto:%f",kgain_auto);
    }
    mFalsePositiveCount = 0;
    //remove old lines
    for (int k=0;k<mLineList.size();k++)
    {
        if(now_ms - mLineList[k].obj1.timeMs>30000)
            mLineList[k].isDead = true;
    }
    //calculate rotation speed
    if(cur_timeMSecs)
    {
        qint64 newtime = now_ms;
        qint64 dtime = newtime - cur_timeMSecs;
        if(dtime<100000&&dtime>0)
        {
            rot_period_sec = (dtime/1000.0);
            rotation_per_min += (15.0/rot_period_sec-rotation_per_min)/2.0;
            cur_timeMSecs = newtime;
            if(isSelfRotation)
            {
                double rateError = rotation_per_min/selfRotationRate;
                selfRotationDazi/=rateError;
            }
        }
    }
    else
    {
        cur_timeMSecs = now_ms;
    }


}
int gray_decode(ushort n) {
    int p = n;
    while (n >>= 1) p ^= n;
    return p;
}
unsigned char inverseBit =0;
bool isInverseSSI = false;
//short dirSSI ;
int C_radar_data::ssiDecode(ushort nAzi)
{
    inverseBit<<=1;
    inverseBit&=0x03;
    nAzi = gray_decode(nAzi);//&0x1fff
    inverseBit |= (nAzi&0x01);
    if(inverseBit==3)isInverseSSI = false;
    if(inverseBit==0)isInverseSSI = true;
    nAzi&=0x1fff;

    nAzi>>=1;
    if(isInverseSSI)nAzi = 4096-nAzi;
    mEncoderVal = nAzi;
    return nAzi>>1;
}

int rot;
void C_radar_data::processSocketData(unsigned char* data,short len)
{
    unsigned char n_clk_adc = data[4];
    sn_stat = (data[5]<<8)+data[6];
    if(clk_adc != n_clk_adc)
    {
        // clock adc
        clk_adc = n_clk_adc;
        isClkAdcChanged = true;
        UpdateData();
        resetData();
    }
    //isClkAdcChanged = true;
    //moduleVal = dataBuff[3];//
    //tempType = dataBuff[2]&0x0f;
    //if(tempType>4)printf("Wrong temperature\n");
    //sn_stat = dataBuff[14]<<8|dataBuff[15];
    //chu_ky = dataBuff[16]<<8|dataBuff[17];
    //tb_tap[newAzi] = dataBuff[18]<<8|dataBuff[19];
    //memcpy(command_feedback,&dataBuff[RADAR_COMMAND_FEEDBACK],8);
    //memcpy(noise_level,&dataBuff[RADAR_COMMAND_FEEDBACK+8],8);
    int newAzi =0;
    if(isSelfRotation)
    {
        selfRotationAzi+=selfRotationDazi;
        if(selfRotationAzi>=MAX_AZIR)
        {
            selfRotationAzi -= MAX_AZIR;
            //ProcessRound();
        }
        if(selfRotationAzi<0)
        {
            selfRotationAzi += MAX_AZIR;
            //ProcessRound();
        }
        newAzi = selfRotationAzi;
    }
    else
    {

        newAzi = (data[2]<<8)|data[3];
        if(data[0]!=5)newAzi = ssiDecode(newAzi);
        /*if(!newAzi)
        {
            if(rot==7)isLeft = true;
            if(rot==0)isLeft = false;
        }*/

        //if((rot&0x01))newAzi = MAX_AZIR-newAzi;
        //uchar datapad = newAzi&31;
        //newAzi = (newAzi&0x1fff)>>1;
        //        if(newAzi<0)newAzi=-newAzi;
        //newAzi=((data[2]<<8)|data[3]);
        //0 dung 1 dung 2 dung 3 dung
        if(newAzi>=MAX_AZIR)
        {
            //printf("\nWrong Azi:%d",newAzi);
            return;
        }
        //if(!dir)newAzi=MAX_AZIR-newAzi;
        //if(rotDir==Left)newAzi = MAX_AZIR-newAzi;
    }
    if(curAzir==newAzi)return;
    //if(newAzi==0)dir= !dir;
    int dazi = newAzi-curAzir;
    if(abs(dazi)>10&&((MAX_AZIR -abs(dazi))>10))
    {
        init_time+=2;
        curAzir=newAzi;
        memcpy(&data_mem.level[curAzir][0],data+34,range_max);
        memcpy(&data_mem.dopler[curAzir][0],data+34+RADAR_RESOLUTION,range_max);
        aziToProcess.push(curAzir);
    }
    else if(dazi<0&&(-dazi)<MAX_AZIR/2)//quay nguoc
    {
        while(curAzir != newAzi)
        {
            curAzir--;
            if(curAzir<0)curAzir+=MAX_AZIR;
            memcpy(&data_mem.level[curAzir][0],data+34,range_max);
            memcpy(&data_mem.dopler[curAzir][0],data+34+RADAR_RESOLUTION,range_max);
            aziToProcess.push(curAzir);
        }
    }
    else if(dazi>0&&(dazi)<MAX_AZIR/2)//quay thuan
    {

        while(curAzir != newAzi)
        {

            curAzir++;
            if(curAzir>=MAX_AZIR)curAzir-=MAX_AZIR;
            memcpy(&data_mem.level[curAzir][0],data+34,range_max);
            memcpy(&data_mem.dopler[curAzir][0],data+34+RADAR_RESOLUTION,range_max);
            aziToProcess.push(curAzir);
        }
    }
    else if(dazi<0&&(-dazi)>MAX_AZIR/2)//quay thuan qua diem 0
    {
        while(curAzir != newAzi)
        {

            curAzir++;
            if(curAzir>=MAX_AZIR)curAzir-=MAX_AZIR;
            memcpy(&data_mem.level[curAzir][0],data+34,range_max);
            memcpy(&data_mem.dopler[curAzir][0],data+34+RADAR_RESOLUTION,range_max);
            aziToProcess.push(curAzir);
        }
    }
    else if(dazi>0&&(dazi)>MAX_AZIR/2)//quay nguoc qua diem 0
    {
        while(curAzir != newAzi)
        {

            curAzir--;
            if(curAzir<0)curAzir+=MAX_AZIR;
            memcpy(&data_mem.level[curAzir][0],data+34,range_max);
            memcpy(&data_mem.dopler[curAzir][0],data+34+RADAR_RESOLUTION,range_max);
            aziToProcess.push(curAzir);
        }
    }


    return;
}
void C_radar_data::SelfRotationOn( double rate)
{
    isSelfRotation = true;
    printf("\nself rotation");
    SelfRotationReset();
    selfRotationDazi = 2;
    cur_timeMSecs =0;
    selfRotationRate = rate;
    if(selfRotationRate<1)selfRotationRate=1;
    //ProcessEach90Deg();
}
void C_radar_data::SelfRotationReset()
{
    //selfRotationAzi = 0;

    selfRotationAzi = 0;
}
void C_radar_data::SelfRotationOff()
{
    isSelfRotation = false;
}

int C_radar_data::getNewAzi()
{
    int newAzi;
    if(isSelfRotation)
    {
        selfRotationAzi-=selfRotationDazi;
        if(selfRotationAzi>=MAX_AZIR)selfRotationAzi = 0;
        if(selfRotationAzi<0)selfRotationAzi += MAX_AZIR;
        newAzi = selfRotationAzi;
    }
    else
    {
        newAzi = (0xfff & (dataBuff[4] << 8 | dataBuff[5]))>>1;
    }
    if(newAzi>MAX_AZIR||newAzi<0)
        return 0;
    return newAzi;
}
void C_radar_data::ProcessDataFrame()
{/*
    int newAzi = getNewAzi();

    int leftAzi = curAzir-1;if(leftAzi<0)leftAzi+=MAX_AZIR;
    int rightAzi = curAzir +1; if(rightAzi>=MAX_AZIR)rightAzi-=MAX_AZIR;
    if(newAzi == leftAzi )
    {
        if(rotDir==Right)
        {
            rotDir  = Left;
            arcMaxAzi = curAzir;
            init_time =5;

        }
    }
    else if(newAzi == rightAzi) {
        if(rotDir==Left)
        {
            rotDir = Right;
            arcMinAzi = curAzir;
            init_time =5;
        }
    }
    else if(newAzi ==curAzir)
    {
        //printf("\ncurAzir:%d",curAzir);
        return;
    }
    else
    {
        //clearPPI();
    }
    rotation_speed = dataBuff[1];
    overload = dataBuff[4]>>7;
    unsigned char n_clk_adc = (dataBuff[4]&(0xe0))>>5;
    if(clk_adc != n_clk_adc)
    {
        // clock adc

        clk_adc = n_clk_adc;
        isClkAdcChanged = true;
        resetData();

    }
    moduleVal = dataBuff[3];//
    tempType = dataBuff[2]&0x0f;
    if(tempType>4)printf("Wrong temperature\n");
    sn_stat = dataBuff[14]<<8|dataBuff[15];
    chu_ky = dataBuff[16]<<8|dataBuff[17];
    tb_tap[newAzi] = dataBuff[18]<<8|dataBuff[19];
    memcpy(command_feedback,&dataBuff[RADAR_COMMAND_FEEDBACK],8);
    memcpy(noise_level,&dataBuff[RADAR_COMMAND_FEEDBACK+8],8);
    curAzir = newAzi;
    aziToProcess.push(curAzir);
    decodeData(curAzir);
    if(!((unsigned char)(curAzir<<3))){
        procTracks(curAzir);
        getNoiseLevel();

    }
    if(curAzir==0)
    {
        ProcessEach90Deg();

    }*/
}
void C_radar_data::clearPPI()
{
    img_ppi->fill(0);

}
void C_radar_data::DetectTracks()
{
    //create tracks
    for (int i=0;i<mLineList.size();i++)
    {
        object_line* pLine1 = &mLineList[i];
        if(pLine1->isProcessed)continue;
        pLine1->isProcessed = true;
        std::vector <track_t> possibleTracks;
        for (int j=0;j<mLineList.size();j++)
        {
            if(i==j)continue;

            object_line* pLine2 = &mLineList.at(j);
            if(pLine2->isDead)continue;

            if(pLine1->obj2.uniqID!=pLine2->obj1.uniqID)continue;

            if(pLine1->obj2.period!=pLine2->obj1.period)continue;
            //                float distanceCoeff = (pLine2->distanceCoeff+pLine1->distanceCoeff)/2.0;
            //                float speedDif      = abs(pLine2->speedkmh-pLine1->speedkmh);
            //                float speed = (pLine2->speedkmh+pLine1->speedkmh)/2.0;
            float rgSpeedDif    = abs(pLine2->rgSpeedkmh-pLine1->rgSpeedkmh);
            //                float accHead = speedDif/(pLine2->dtimeMSec/3600000.0);
            //                float bearingDiff = abs(pLine2->bearingRad-pLine1->bearingRad);
            //                float accPerpendir = (bearingDiff*DEG_RAD*1000.0)/(pLine2->dtimeMSec)*speed;
            //                float rangeSpeed = abs(pLine2->rgSpeedkmh+pLine1->rgSpeedkmh)/2.0;

#ifdef LOG_DATA_TRAINING
            fprintf(logfile,"%f,",distanceCoeff);
            fprintf(logfile,"%f,",speedDif);
            fprintf(logfile,"%f,",rgSpeedDif);
            //                fprintf(logfile,"%f,",accHead);
            //                fprintf(logfile,"%f,",bearingDiff);
            fprintf(logfile,"%f,",accPerpendir);
            fprintf(logfile,"%f,",rangeSpeed);
            if(pLine1->obj1.dopler==pLine1->obj2.dopler&&pLine1->obj2.dopler==pLine2->obj2.dopler)
            {fprintf(logfile," 1");
            }
            else
            {

                fprintf(logfile," 0");
            }
            fprintf(logfile,"\n");
#else
            /*
                split by feature: rgSpeedDif at value:54.478218
                Left 1:stop by gain, prediction:0.924401
                Right 1:
                split by feature: rgSpeedDif at value:79.045364
                Left 2:stop by gain, prediction:0.550000
                Right 2:stop by gain, prediction:0.030000
                */
            float score=1-rgSpeedDif/80.0;
            if(score<0.2)continue;
            if(score>pLine1->trackScore&&score>pLine2->trackScore)
            {
                track_t newtrack;
                newtrack.lineScore = score;
                newtrack.isDead = false;
                newtrack.objectList.push_back((pLine2->obj2));
                newtrack.objectList.push_back((pLine1->obj2));
                newtrack.objectList.push_back((pLine1->obj1));
                possibleTracks.push_back(newtrack);
            }
#endif


        }
        if(possibleTracks.size())
        {
            float maxScore = 0;
            int maxIndex = 0;
            int i;
            for(i=0;i<possibleTracks.size();i++)
            {
                if(possibleTracks[i].lineScore>maxScore)
                {
                    maxScore = possibleTracks[i].lineScore;
                    maxIndex=i;
                }
            }
            //replace the dead ones
            for(i=0;i<mTrackList.size();i++)
            {
                if(mTrackList[i].isDead)
                {
                    mTrackList[i]=possibleTracks[maxIndex];
                    break;
                }
            }
            if(i<mTrackList.size())continue;
            if(mTrackList.size()>500)
            {
                printf("\nfull tracklist");
                continue;
            }
            if(i>=mTrackList.size())mTrackList.push_back(possibleTracks[maxIndex]);
        }
    }
}
ushort mulOf16Azi = 0;
void C_radar_data::UpdateData()
{
    while(aziToProcess.size()>10)
    {

        int azi = aziToProcess.front();

        if(azi==lastProcessAzi)
        {
            aziToProcess.pop();
            continue;
        }
        ProcessData(azi,lastProcessAzi);
        mPeriodCount++;
        drawAzi(azi);
        if(!((unsigned char)(((unsigned char)mPeriodCount)<<3)))//xu ly moi 32 chu ky
        {
            now_ms = QDateTime::currentMSecsSinceEpoch();
            ProcessObjects();
            mulOf16Azi++;
            if(mulOf16Azi>16)// 1/4 vong quet
            {
                mulOf16Azi=0;
                ProcessEach90Deg();
            }
            if(init_time)init_time--;
            for(unsigned short i = 0;i<plot_list.size();++i)
            {
                if(plot_list.at(i).isUsed)
                {
                    if((plot_list.at(i).lastA!=lastProcessAzi)&&(plot_list.at(i).lastA!=azi))
                    {
                        procPLot(&plot_list.at(i));
                        plot_list.at(i).isUsed = false;
                    }

                }
            }
        }
        // update histogram
        nNoiseCalculator++;
        sumvar+= abs(data_mem.level[azi][range_max-200]-data_mem.level[azi][range_max-205]);;
        unsigned char value = data_mem.level[azi][range_max-200];
        if(value>5&&value<200)
        {
            histogram[value-3]+=1;
            histogram[value-2]+=2;
            histogram[value-1]+=3;
            histogram[value  ]+=4;
            histogram[value+1]+=3;
            histogram[value+2]+=2;
            histogram[value+3]+=1;
        }//

        lastProcessAzi = azi;
        aziToProcess.pop();
    }

}
void C_radar_data::assembleDataFrame(unsigned char* data,unsigned short dataLen)
{

    if((dataLen<headerLen)){printf("Too short data.1\n");return;}
    char dataId = data[0]&0x0f;
    if(dataId==1)
    {
        //printf("%x-",data[0]);
        curFrameId = (data[0]&0xf0)>>4;
        range_max = (dataLen - headerLen)*4/3 - RAD_S_PULSE_RES;
        //printf("range_max:%d\n",range_max);
        if(range_max < RAD_S_PULSE_RES){printf("Too short data.2\n");return;}
        if(range_max > RAD_M_PULSE_RES){printf("Too long data.3\n");return;}
        memcpy(dataBuff,data,dataLen);
        waitForData = dataLen;
    }
    else if(dataId==2)
    {
        //check if we are waiting for second half data frame
        if(!waitForData){printf("First frame is mising\n");return;}
        //check if frame ID is the one that we are expecting
        short secondFrameId = (data[0]&0xf0)>>4;
        if(curFrameId!=secondFrameId){
            printf("\nWrong data.-%d-%d-%d",secondFrameId,curFrameId,dataLen);
            printf("\nWrong:%x\n",data[0]);
            //return;
        }
        // check if the data size is correct
        if(dataLen!=waitForData){printf("Wrong data.6\n");return;}
        //load data to buffer
        memcpy(dataBuff + waitForData,data + headerLen,dataLen-headerLen);
        //process data
        ProcessDataFrame();
        waitForData = 0;
    }
    else{
        printf("\nWrong data id. ID = %d",dataId);
    }
    //if(!dopler){frameId = data[0]>>4; }else {if(frameId =! (data[0]>>4))return;}//check id of dopler data

    /*
    short azi = 0xfff & (buff[ADDR_AZI_H] << 8 | buff[ADDR_AZI_L]);
    if(curAzir==azi) return GetData();
    curAzir = azi;
    if(curAzir==4095){
        curPeriodIndex++;
        procTracks();
    }
    for(short r = 1; r < 1023; r++)
    {
        short i = (r>>3);
        short j = (r&0x0007);
        if((buff[VIDEO_RAW_LENGTH+i]>>j & 0x1))
        {


            //signal_map.frame[azi].raw_map[r].level = signal_map.frame[azi].raw_map[r].level<<
            //if(signal_map.frame[azi].raw_map[r].level<80)
            signal_map.frame[azi].raw_map[r].displaylevel  = 1;
            signal_map.frame[azi].raw_map[r].level = buff[r];
            signal_map.frame[azi].raw_map[r].vet = float(signal_map.frame[azi].raw_map[r].vet*0.95 + 0.05);//255*0.125;

            procPix(azi,r);
        }
        else
        {
            signal_map.frame[azi].raw_map[r].displaylevel  = 0;
            signal_map.frame[azi].raw_map[r].level = buff[r];
            signal_map.frame[azi].raw_map[r].vet = float(signal_map.frame[azi].raw_map[r].vet*0.95);
            //signal_map.frame[azi].raw_map[r].level = 0;
        }

    }
    delete[] buff;
    return azi;*/

}
unsigned int doplerHistogram[256];
void C_radar_data::procPLot(plot_t* mPlot)
{
    if(init_time)
        return;
    // remove too big or too small
    if(mPlot->sumEnergy<100)
    {
        mFalsePositiveCount++;
        return;
    }
    else if(mPlot->sumEnergy>20000)
    {
        return;
    }
    //if(mPlot->minR<500)return;
    if(xl_dopler)
    {
        memset(doplerHistogram,0,256);
        int leftA = mPlot->riseA-15;
        if(leftA<0)leftA+=MAX_AZIR;
        int rightA = mPlot->riseA+15;
        if(rightA>=MAX_AZIR)rightA-=MAX_AZIR;
        int maxRg = mPlot->maxR+15;
        int minRg = mPlot->maxR-15;
        if(minRg<0)minRg=0;if(maxRg>=range_max)maxRg=range_max-1;
        for(int lAzi=leftA;;lAzi++)
        {
            if(lAzi>=MAX_AZIR)lAzi-=MAX_AZIR;
            if(lAzi==rightA)break;
            for(int lRg = minRg;lRg<maxRg;lRg++)
            {
                if(data_mem.detect[lAzi][lRg])
                    doplerHistogram[data_mem.dopler[lAzi][lRg]]++;
            }
        }
        //printf("\n");
        int sumHis = 0;
        int maxHis = 0;
        for(int i=0;i<16;i++)
        {
            //printf(" %d:%d ",i,doplerHistogram[i]);
            //sumHis+=doplerHistogram[i];
            //if(doplerHistogram[i]>20)
            sumHis+=doplerHistogram[i];
            if(doplerHistogram[i]>maxHis)maxHis = doplerHistogram[i];
            //            if((doplerHistogram[i]-mPlot->size)>50&&mPlot->dopler==i)
            //            {
            //                printf("mPlot->size: %d",mPlot->size);
            //                return;
            //            }
        }
        //printf("mPlot->size: %d",mPlot->size);
        //printf("\n maxhis/sumHis: %f",maxHis/(float)sumHis);
        if(maxHis/(float)sumHis<0.75)return;


    }

    object_t newobject;
    newobject.timeMs = now_ms;
    newobject.scorep1 = 0;
    newobject.scorep2 = 0;
    newobject.uniqID = rand();
    newobject.isDead = false;
    newobject.isProcessed = false;
    newobject.dazi = mPlot->lastA-mPlot->riseA;
    newobject.period = mPeriodCount;
    float ctA;
    if(abs(newobject.dazi)>(MAX_AZIR/2))//quay qua diem 0
    {
        newobject.dazi = MAX_AZIR-newobject.dazi;
        ctA = (mPlot->fallA+mPlot->riseA+MAX_AZIR)/2.0;

    }
    else
    {
        ctA = (mPlot->riseA + mPlot->fallA)/2.0;

    }
    if(ctA >= MAX_AZIR)ctA -= MAX_AZIR;
    float ctR = (float)mPlot->sumR/(float)mPlot->size+1;//min value starts at 1
    if(ctR<mPlot->minR||ctR>mPlot->maxR+1)printf("\nWrong ctR");
    //todo: tinh dopler histogram


    newobject.size = mPlot->size;
    newobject.drg = mPlot->maxR-mPlot->minR;
    newobject.aziRes = 0.017;
    newobject.rangeRes = sn_scale*pow(2,clk_adc);
    if(ctA<0|| ctR>=RADAR_RESOLUTION)
    {
        return;
    }
    newobject.dopler = mPlot->dopler;
    //newobject.terrain = data_mem.terrain[short(ctA)][short(ctR)];
    newobject.az   = ctA/float(MAX_AZIR/PI_NHAN2)+aziOffset;
    if(newobject.az>PI_NHAN2)newobject.az-=PI_NHAN2;
    newobject.rg   = ctR;
    newobject.rgKm =  ctR*sn_scale;
    newobject.p   = -1;
    newobject.xkm = newobject.rgKm*sin( newobject.az);
    newobject.ykm = newobject.rgKm*cos( newobject.az);
    // add to mObjList
    bool full = true;
    for(int i=0;i<mObjList.size(); i++) {
        if(mObjList.at(i).isDead)
        {
            mObjList.at(i) = newobject;
            full = false;
            break;
        }
    }
    if(full&&mObjList.size()<2000)
    {
        mObjList.push_back(newobject);
    }

    /*
    if(!procObjectManual(&newobject))//check existing confirmed tracks
    {
        if(!procObjectAvto(&newobject))
        {
            if(avtodetect)addTrack(&newobject);
        }
    }*/
    mPlot->isUsed = false;





}
void C_radar_data::procTracks(unsigned short curA)
{
    //process all marks
    //short pr_curA = curA-1;
    //if(pr_curA<0)pr_curA+=MAX_AZIR;

    /*
    //proc track
    float azi = (float)curA/MAX_AZIR*PI_NHAN2+aziOffset;
    for(unsigned short i=0;i<mTrackList.size();i++)
    {
        if(!mTrackList.at(i).state)continue;
        float dA = azi - mTrackList.at(i).estA;
        if(dA>PI) dA-=PI_NHAN2;
        else if(dA<-PI)dA+=PI_NHAN2;
        if(mTrackList.at(i).isProcessed)
        {
            if((abs(dA)<0.35f)&&(mTrackList.at(i).isTracking))//20 deg
            {
                mTrackList.at(i).isProcessed = false;
            }
            if(!mTrackList.at(i).isTracking)
            {
                if(abs(dA)>0.35f)//20 deg
                {
                    mTrackList.at(i).isProcessed = true;
                    mTrackList.at(i).update();
                }
            }
        }
        else
        {
            if(abs(dA)>0.35f)//20 deg
            {
                mTrackList.at(i).isProcessed = true;
                mTrackList.at(i).update();
            }
        }
    }
*/

}
void C_radar_data::kmxyToPolarDeg(double x, double y, double *azi, double *range)
{
    if(!y)
    {
        *azi = x>0? PI_CHIA2:(PI_NHAN2-PI_CHIA2);
        *azi = *azi*DEG_RAD;
        *range = abs(x);
    }
    else
    {
        *azi = atanf(x/y);
        if(y<0)*azi+=PI;
        if(*azi<0)*azi += PI_NHAN2;
        *range = sqrt(x*x+y*y);
        *azi = *azi*DEG_RAD;
    }

}
void C_radar_data::addTrackManual(double x,double y)
{/*
    float azi,range;
    if(!y)
    {
        azi = x>0? PI_CHIA2:(PI_NHAN2-PI_CHIA2);
        range = abs(x);
    }
    else
    {
        azi = atanf(x/y);//tinh azi,range
        if(y<0)azi+=PI;
        if(azi<0)azi += PI_NHAN2;
        range = sqrt(x*x+y*y);
    }
    object_t newobj;
    newobj.az = azi;
    newobj.rg = range;
    newobj.dopler = 17;
    newobj.isManual= true;
    newobj.xkm = x*sn_scale;
    newobj.ykm = y*sn_scale;
    //
    bool newtrack=true;
    short trackId = -1;
    ushort max_length = 0;
    for(unsigned short i=0;i<mTrackList.size();i++)
    {
        if(mTrackList.at(i).state>5)
        {
            if(mTrackList.at(i).checkProb(&newobj)){
                if(max_length<mTrackList.at(i).object_list.size())
                {
                    max_length = mTrackList.at(i).object_list.size();
                    trackId = i;
                    newtrack = false;
                }
            }
        }
    }
    if(newtrack)
    {
        addTrack( &newobj);
        //printf("newtrack ");
    }
    else
    {
        mTrackList.at(trackId).suspect_list.push_back(newobj);
    }
*/

}
void C_radar_data::addTrack(object_t* mObject)
{/*
    //add new track
    //printf("new track \n");
    for(unsigned short i=0;i<mTrackList.size();i++)
    {
        if(!mTrackList.at(i).state)
        {
            mTrackList.at(i).init(mObject);
            return;
        }
    }
    if(mTrackList.size()<500)
    {
        track_t newTrack;
        newTrack.init(mObject);
        mTrackList.push_back(newTrack);
    }*/
}
void C_radar_data::deleteTrack(ushort trackNum)
{
    /*
    if(mTrackList.size()>trackNum)
    {
        mTrackList[trackNum].state = 0;
        if(mTrackList[trackNum].object_list.size())mTrackList[trackNum].object_list.clear();
        if(mTrackList[trackNum].suspect_list.size())mTrackList[trackNum].suspect_list.clear();
    }*/
}

void C_radar_data::drawRamp()
{
    img_RAmp->fill(Qt::black);
    for (short r_pos = 0;r_pos<RAD_M_PULSE_RES;r_pos++)
    {
        unsigned char value = data_mem.level[curAzir][r_pos];
        char dopler = data_mem.dopler[curAzir][r_pos];

        uint color ;
        if(dopler==0)
        {
            color = 0xffff00;
        }else
        {
            char dDopler = dopler-1;
            if(dDopler>7)dDopler = 15-dDopler;
            color = 0x00ff00 | ((dDopler<<5));
        }
        color = color|(0xff000000);

        if(((r_pos%100)==0)||(((r_pos+1)%100)==0)||(((r_pos-1)%100)==0))
        {
            for(short i=0;i<128;i++)
            {
                img_RAmp->setPixel(r_pos,i,0xffffffff);
            }
        }
        for(short i=255;i>255 - value;i--)
        {
            img_RAmp->setPixel(r_pos,i,color);
        }
    }

}

void C_radar_data::drawRamp(double azi)
{
    img_RAmp->fill(Qt::black);
    //newobject.az   = ctA/MAX_AZIR*PI_NHAN2+trueN;
    azi/=DEG_RAD;
    azi-=aziOffset;
    if(azi<0)azi+=PI_NHAN2;
    int az = azi/PI_NHAN2*MAX_AZIR;
    for (short r_pos = 0;r_pos<RAD_M_PULSE_RES;r_pos++)
    {
        unsigned char value = data_mem.level[az][r_pos];
        char dopler = data_mem.dopler[az][r_pos];

        uint color ;
        if(dopler==0)
        {
            color = 0xffff00;
        }else
        {
            char dDopler = dopler-1;
            if(dDopler>7)dDopler = 15-dDopler;
            color = 0x00ff00 | ((dDopler<<5));
        }
        color = color|(0xff000000);

        if(((r_pos%100)==0)||(((r_pos+1)%100)==0)||(((r_pos-1)%100)==0))
        {
            for(short i=0;i<128;i++)
            {
                img_RAmp->setPixel(r_pos,i,0xffffffff);
            }
        }
        for(short i=255;i>255 - value;i--)
        {
            img_RAmp->setPixel(r_pos,i,color);
        }

    }

}
/*bool C_radar_data::procObjectAvto(object_t* pObject)
{
    bool newtrack = true;
    short trackId = -1;
    short max_length = 0;
    for(unsigned short i=0;i<mTrackList.size();i++)
    {
        if(mTrackList.at(i).isManual)continue;
        if(mTrackList.at(i).state&&(! mTrackList.at(i).isProcessed))
        {
            if(mTrackList.at(i).checkProb(pObject)){
                if(max_length<mTrackList.at(i).object_list.size())
                {
                    max_length=mTrackList.at(i).object_list.size();
                    trackId = i;
                    newtrack = false;
                }
            }
        }
    }
    if(!newtrack)
    {
        //add object to a processing track
        mTrackList.at(trackId).suspect_list.push_back(*pObject);
        return true;
    }
    else
    {

        return false;
    }


    return false;
}*/
/*bool C_radar_data::procObjectManual(object_t* pObject)// !!!
{

    short trackId = -1;
    ushort max_length = 0;
    for(unsigned short i=0;i<mTrackList.size();i++)
    {
        if(!mTrackList.at(i).isManual)continue;
        if(mTrackList.at(i).state&&(! mTrackList.at(i).isProcessed))
        {
            if(mTrackList.at(i).checkProb(pObject)){
                if(max_length<mTrackList.at(i).object_list.size())
                {
                    max_length = mTrackList.at(i).object_list.size();
                    trackId = i;
                }
            }
        }
    }
    if(trackId>=0)
    {
        //add object to a processing track
        mTrackList.at(trackId).suspect_list.push_back(*pObject);
        return true;
    }
    else return false;

    return false;
}*/
void C_radar_data::procPix(short proc_azi,short range)//_______signal detected, check 4 last neighbour points for nearby mark_______________//
{
    short pr_proc_azi = lastProcessAzi;
    short plotIndex =-1;
    char dopler_0 = data_mem.dopler[proc_azi][range];
    char dopler_1 = dopler_0 +1;
    if(dopler_1>15)dopler_1-=16;
    char dopler_2 = dopler_0 - 1;
    if(dopler_2<0)dopler_2+=16;
    if(data_mem.detect[pr_proc_azi][range]
            //            &&(data_mem.dopler[pr_proc_azi][range]==dopler_0
            //               ||data_mem.dopler[pr_proc_azi][range]==dopler_1
            //               ||data_mem.dopler[pr_proc_azi][range]==dopler_2)
            )
    {
        plotIndex = data_mem.plotIndex[pr_proc_azi][range];

    }else if(data_mem.detect[proc_azi][range-1]
             //             &&(data_mem.dopler[proc_azi][range-1]==dopler_0
             //                ||data_mem.dopler[proc_azi][range-1]==dopler_1
             //                ||data_mem.dopler[proc_azi][range-1]==dopler_2)
             )
    {
        plotIndex = data_mem.plotIndex[proc_azi][range-1];
    }
    else if(data_mem.detect[pr_proc_azi][range-1]
            //            &&(data_mem.dopler[pr_proc_azi][range-1]==dopler_0
            //               ||data_mem.dopler[pr_proc_azi][range-1]==dopler_1
            //               ||data_mem.dopler[pr_proc_azi][range-1]==dopler_2)
            )
    {
        plotIndex = data_mem.plotIndex[pr_proc_azi][range-1];

    }
    else if(data_mem.detect[pr_proc_azi][range+1]
            //            &&(data_mem.dopler[pr_proc_azi][range+1]==dopler_0
            //               ||data_mem.dopler[pr_proc_azi][range+1]==dopler_1
            //               ||data_mem.dopler[pr_proc_azi][range+1]==dopler_2)
            )
    {
        plotIndex = data_mem.plotIndex[pr_proc_azi][range+1];
    }
    if((plotIndex<plot_list.size())
            &&(plotIndex>=0)
            &&(plot_list.at(plotIndex).isUsed)
            )// add to existing plot
    {
        /*if(((plot_list.at(plotIndex).maxLevel*0.8)>data_mem.level[proc_azi][range]))//cut fall of target plot
        {
            data_mem.detect[proc_azi][range] = 0;
            return;
        }*/
        data_mem.plotIndex[proc_azi][range] = plotIndex;
        plot_list.at(plotIndex).size++;
        plot_list.at(plotIndex).sumEnergy+=data_mem.level[proc_azi][range];
        if(plot_list.at(plotIndex).maxR<range)plot_list.at(plotIndex).maxR= range;
        if(plot_list.at(plotIndex).minR>range)plot_list.at(plotIndex).minR= range;
        plot_list.at(plotIndex).sumR    +=  range;
        plot_list.at(plotIndex).lastA   = proc_azi;
        //
        // get max dopler and max level of this plot
        if(plot_list.at(plotIndex).maxLevel<data_mem.level[proc_azi][range])
        {
            plot_list.at(plotIndex).riseA = proc_azi;
            plot_list.at(plotIndex).maxLevel = data_mem.level[proc_azi][range];
            plot_list.at(plotIndex).dopler = data_mem.dopler[proc_azi][range];
        }
        if((plot_list.at(plotIndex).maxLevel-noiseVar)<data_mem.level[proc_azi][range])
            plot_list.at(plotIndex).fallA = proc_azi;
    }
    else//_________new plot_____________//
    {

        plot_t                  new_plot;
        new_plot.isUsed = true;
        new_plot.lastA =        new_plot.riseA  = new_plot.fallA= proc_azi;
        new_plot.maxLevel =     data_mem.level[proc_azi][range];
        new_plot.sumEnergy =    data_mem.level[proc_azi][range];
        new_plot.dopler =       data_mem.dopler[proc_azi][range];
        new_plot.size =         1;
        new_plot.sumR =         range;
        new_plot.maxR =         range;
        new_plot.minR =         range;
        bool listFull = true;

        for(unsigned short i = 0;i<plot_list.size();++i)
        {
            //  overwrite
            if(!plot_list.at(i).isUsed)
            {
                data_mem.plotIndex[proc_azi][range] =  i;

                plot_list.at(i) = new_plot;
                listFull = false;
                break;
            }
        }
        if(listFull)
        {
            plot_list.push_back(new_plot);
            data_mem.plotIndex[proc_azi][range]  = plot_list.size()-1;
        }

    }

}
/*void C_radar_data::polarToSnXY(short *xsn, short *ysn, short azi, short range)
{
    *xsn = signal_map.frame[azi].raw_map[range].x;
    *ysn = signal_map.frame[azi].raw_map[range].y;
}
//static short ctX=0,ctY=0;
//static float dr = 0;
*/
void C_radar_data::polarToXY(float *x, float *y, float azi, float range)
{

    *x = ((sinf(azi)))*range;
    *y = ((cosf(azi)))*range;
}

float C_radar_data::getNoiseAverage() const
{
    return noiseAverage;
}

void C_radar_data::setNoiseAverage(float value)
{
    noiseAverage = value;
}

bool C_radar_data::getIsSharpEye() const
{
    return isSharpEye;
}

void C_radar_data::setIsSharpEye(bool value)
{
    isSharpEye = value;
}
short zoomXmax,zoomYmax,zoomXmin,zoomYmin;
short zoomCenterX=RAD_DISPLAY_RES,zoomCenterY=RAD_DISPLAY_RES;
void C_radar_data::setZoomRectXY(float ctx, float cty)
{
    zoomXmax = 2*ctx*2.0/scale_ppi+ZOOM_SIZE/2;
    zoomYmax = 2*cty*2.0/scale_ppi+ZOOM_SIZE/2;
    zoomXmin = 2*ctx*2.0/scale_ppi-ZOOM_SIZE/2;
    zoomYmin = 2*cty*2.0/scale_ppi-ZOOM_SIZE/2;
    raw_map_init_zoom();
}


int C_radar_data::get_tb_tap(){

    hsTap += ((tb_tap[curAzir])-hsTap)/5.0;
    return int(hsTap);
}

void C_radar_data::setTb_tap_k(double value)
{
    tb_tap_k = value;
    if(tb_tap_k<=0)tb_tap_k=1;
}
void C_radar_data::setZoomRectAR(float ctx, float cty,double sizeKM,double sizeDeg)// unit km
{
    double cta,ctr = sqrt(ctx*ctx+cty*cty);
    if(cty==0)
    {
        if(ctx>0)cta = PI_CHIA2;
        else cta = -PI_CHIA2;
    }
    else cta = atan(ctx/cty)-aziOffset;
    if(cty<0)cta+=PI;
    if(cta<0)cta += PI_NHAN2;
    if(cta>PI_NHAN2)cta-=PI_NHAN2;
    cta=cta/PI_NHAN2*MAX_AZIR;
    ctr/=sn_scale;
    zoom_ar_size_a = MAX_AZIR/360.0*sizeDeg;
    zoom_ar_size_r = sizeKM/sn_scale;
    zoom_ar_a0 = cta-zoom_ar_size_a/2.0;
    zoom_ar_a1 = zoom_ar_a0+zoom_ar_size_a;
    if(zoom_ar_a1>MAX_AZIR)zoom_ar_a1-=MAX_AZIR;
    if(zoom_ar_a0<0)zoom_ar_a0+=MAX_AZIR;
    zoom_ar_r0 = ctr-zoom_ar_size_r/2.0;
    if(zoom_ar_r0 <0)zoom_ar_r0=0;
    zoom_ar_r1 = zoom_ar_r0+zoom_ar_size_r;
    if(zoom_ar_r1>range_max)zoom_ar_r1=range_max;
    img_zoom_ar = new QImage(zoom_ar_size_r+1,zoom_ar_size_a+1,QImage::Format_ARGB32);
    //img_zoom_ar->// toto:resize
    //drawZoomAR(a0,r0);
    
}
bool C_radar_data::DrawZoomAR(int a,int r,short val,short dopler,short sled)
{
    //return true if point is on the edges of the zone
    //if(a<zoom_ar_size_a)a+=MAX_AZIR;
    if(!img_zoom_ar)return false;
    int pa= a-zoom_ar_a0;
    if(pa>=MAX_AZIR)pa-=MAX_AZIR;
    if(pa<0)pa+=MAX_AZIR;
    if(pa>zoom_ar_size_a)return false;
    int pr = r-zoom_ar_r0;
    if(pr>zoom_ar_size_r)return false;
    if(pr<0)return false;
    img_zoom_ar->setPixel(pr,pa,getColor(val,dopler,sled));
    if(pa==zoom_ar_size_a)return true;
    if(pr==zoom_ar_size_r)return true;
    if(pa==0)return true;
    if(pr==0)return true;
    return false;

}
void C_radar_data::resetGain()
{
    //krain_auto = 0.3;
    kgain_auto  = 4;
    //ksea_auto = 0;
}
void C_radar_data::setAutorgs(bool aut)
{
    if(aut)
    {
        rgs_auto = true;
        //krain_auto = 0.3;
        //kgain_auto  = 4;
        //ksea_auto = 0;
    }else
    {
        rgs_auto = false;
    }

}
void C_radar_data::raw_map_init()
{
    float theta=aziOffset;
    float dTheta = 2*PI/MAX_AZIR_DRAW;
    for(short azir = 0; azir < MAX_AZIR_DRAW; azir++)
    {
        float cost = cosf(theta);
        float sint = sinf(theta);
        for(short range = 0;range<RAD_DISPLAY_RES;range++)
        {
            data_mem.xkm[azir][range]     =  short(sint*(range+1))+RAD_DISPLAY_RES;
            data_mem.ykm[azir][range]    =  -short(cost*(range+1))+RAD_DISPLAY_RES;
            if(data_mem.xkm[azir][range]<0||data_mem.xkm[azir][range]>=img_ppi->width()||data_mem.ykm[azir][range]<0||data_mem.ykm[azir][range]>=img_ppi->height())
            {
                data_mem.xkm[azir][range] = 0;
                data_mem.ykm[azir][range] = 0;
            }
        }
        theta+=dTheta;
    }
}
void C_radar_data::raw_map_init_zoom()
{
    img_zoom_ppi->fill(Qt::black);
    float theta=aziOffset;
    float dTheta = 2*PI/MAX_AZIR_DRAW;
    for(short azir = 0; azir < MAX_AZIR_DRAW; azir++)
    {

        float cost = cosf(theta);
        float sint = sinf(theta);
        for(short range = 0;range<DISPLAY_RES_ZOOM;range++)
        {
            data_mem.xzoom[azir][range]     =  short(sint*(range+1)) - zoomXmin;
            data_mem.yzoom[azir][range]    =  -short(cost*(range+1)) - zoomYmin;
            if(data_mem.xzoom[azir][range]<0||
                    data_mem.yzoom[azir][range]<0||
                    data_mem.xzoom[azir][range]>ZOOM_SIZE||
                    data_mem.yzoom[azir][range]>ZOOM_SIZE)
            {
                data_mem.xzoom[azir][range] = 0;
                data_mem.yzoom[azir][range] = 0;
            }
        }
        theta += dTheta;
    }
}
void C_radar_data::resetData()
{
    switch(clk_adc)
    {
    case 0:
        sn_scale = SIGNAL_SCALE_0;
        break;
    case 1:
        sn_scale = SIGNAL_SCALE_1;//printf("1");
        break;
    case 2:
        sn_scale = SIGNAL_SCALE_2;//printf("2");
        break;
    case 3:
        sn_scale = SIGNAL_SCALE_3;//printf("2");
        break;
    case 4:
        sn_scale = SIGNAL_SCALE_4;//printf("2");
        break;
    case 5:
        sn_scale = SIGNAL_SCALE_5;//printf("2");
        break;
    case 6:
        sn_scale = SIGNAL_SCALE_6;//printf("2");
        break;
    case 7:
        sn_scale = SIGNAL_SCALE_7;//printf("2");
        break;
    default:
        sn_scale = SIGNAL_SCALE_0;
    }
    int dataLen = RADAR_RESOLUTION*MAX_AZIR;
    memset(data_mem.level,      0,dataLen);
    memset(data_mem.dopler,     0,dataLen);
    memset(data_mem.detect,     0,dataLen);
    memset(data_mem.plotIndex,  0,dataLen);
    memset(data_mem.hot,        0,dataLen);
    std::queue<int> empty;
    std::swap( aziToProcess, empty );
    //memset(data_mem.terrain,    TERRAIN_INIT,dataLen);
    //memset(data_mem.rainLevel,  0,dataLen);
    //noiseAverage = 30;
    resetSled();
    init_time += 5;

}
void C_radar_data::resetSled()
{
    memset(data_mem.sled,0,RAD_M_PULSE_RES*MAX_AZIR);
}
void C_radar_data::setScalePPI(float scale)
{
    scale_ppi = sn_scale*scale;
    //updateZoomRect();
}
void C_radar_data::setScaleZoom(float scale)
{

    scale_zoom_ppi = SIGNAL_SCALE_0*scale/scale_ppi;
    //updateZoomRect();
}

//void C_radar_data::drawZoomAR()
//{

//      //memcpy(imgAR->bits(),(unsigned char *)&data_mem.level[0][0],MAX_AZIR*RAD_M_PULSE_RES);
//      QImage* imgAR = new QImage((unsigned char *)&data_mem.level[0][0],RAD_M_PULSE_RES,MAX_AZIR,QImage::Format_Indexed8);
//        imgAR->setColorTable(colorTable);
//      QRect rect(zoom_ar_r0, zoom_ar_a0, 225, 225);
//      *img_zoom_ar = imgAR->copy(rect);//.convertToFormat(QImage::Format_Indexed8,colorTable,Qt::ColorOnly);

//}
void C_radar_data::drawSgnZoom(short azi_draw, short r_pos)
{
    unsigned char value    = data_mem.display_ray_zoom[r_pos][0];
    unsigned char dopler    = data_mem.display_ray_zoom[r_pos][1];
    unsigned char sled     = data_mem.display_ray_zoom[r_pos][2];

    short px = data_mem.xzoom[azi_draw][r_pos];
    short py = data_mem.yzoom[azi_draw][r_pos];
    if(px<=0||py<=0)return;
    short pSize = 1;
    if(r_pos<100)pSize=0;
    else if(r_pos>800)pSize=2;

    //if(pSize>2)pSize = 2;
    if((px<pSize)||(py<pSize)||(px>=img_zoom_ppi->width()-pSize)||(py>=img_zoom_ppi->height()-pSize))return;
    for(short x = -pSize;x <= pSize;x++)
    {
        for(short y = -pSize;y <= pSize;y++)
        {
            float k ;
            switch(short(x*x+y*y))
            {
            case 0:
                k=1;
                break;
            case 1:
                if(data_mem.display_mask_zoom[px+x][py+y])k=0.95f;
                else k=1;
                break;
            case 2:
                if(data_mem.display_mask_zoom[px+x][py+y])k=0.7f;
                else k=1;

            default:
                if(data_mem.display_mask_zoom[px+x][py+y])continue;
                k=0.7f;
                break;
            }
            unsigned char pvalue = value*k;
            if( data_mem.display_mask_zoom[px+x][py+y] <= pvalue)
            {
                data_mem.display_mask_zoom[px+x][py+y] = pvalue;
                img_zoom_ppi->setPixel(px+x,py+y,getColor(pvalue,dopler,sled));
                //DrawZoom(px,py,pvalue);
            }
        }
    }

}
uint C_radar_data::getColor(unsigned char pvalue,unsigned char dopler,unsigned char sled)
{
    dopler = dopler%16;
    unsigned short value = ((unsigned short)pvalue)*brightness;
    if(!isSled)sled = 0;
    else
        if(sled>=32)sled = 0xff; else sled*=8;
    if(value>0xff)
    {
        value = 0xff;
    }
    unsigned char alpha;
    unsigned char red   = 0;
    unsigned char green = 0;
    unsigned char blue  = 0;
    unsigned char gradation = value<<2;
    uint color;
    switch(imgMode)
    {
    case DOPLER_3_COLOR:
        if(pvalue>1)
        {
            if(dopler==0)
            {
                color = 0xffff00;
            }else
            {
                char dDopler = dopler-1;
                if(dDopler>7)dDopler = 15-dDopler;
                color = 0x00ff00 | ((dDopler<<5));
            }
            alpha = value;//0xff - ((0xff - value)*0.75);
            color = color|(alpha<<24);
        }
        else
        {
            color = (sled<<24)|(0xff);
        }
        //

        break;

    case VALUE_ORANGE_BLUE:
        if(pvalue>1)
        {
            alpha = 0xff - ((0xff - value)*0.75);
            //pvalue-=(pvalue/10);
            switch(value>>6)
            {
            case 3:
                red = 0xff;
                green = 0xff - gradation;
                break;
            case 2:
                red = gradation;
                green = 0xff;
                break;
            case 1:
                green = 0xff ;
                blue = 0xff - gradation;
                break;
            case 0:
                green = gradation ;
                blue = 0xff;
                break;
            }
            color = (alpha<<24)|(red<<16)|(green<<8)|blue;
        }
        else
        {
            color = (sled<<24)|(0xff);
        }

        break;
    case VALUE_YELLOW_SHADES:
        if(pvalue>1)
        {
            alpha = value;//0xff - ((0xff - pvalue)*0.75);
            color = (value<<24)|(0xff<<16)|(0xff<<8);
        }
        else
        {
            color = (sled<<24)|(0xff);
        }
        break;
    default:
        break;
    }
    return color;
}
void C_radar_data::resetTrack()
{
    init_time += 3;
    curIdCount = 1;
    mTrackList.clear();
    //    for(unsigned short i=0;i<mTrackList.size();i++)
    //    {
    //        if(mTrackList.at(i).state)
    //        {
    //            mTrackList.at(i).state = 0;
    //        }
    //    }
}
double C_radar_data::estimateScore(object_t *obj1,object_t *obj2)
{
    int dtime = (obj1->timeMs - obj2->timeMs);
    if(dtime<1000)return 0;//ENVAR min time between plots in a line(1s)
    if(dtime>40000)return 0;//ENVAR max time between plots in a line(40s)
    float rgSpeedkmh = (obj1->rgKm-obj2->rgKm)/(dtime/3600000.0);
    if(rgSpeedkmh>85)return 0;//ENVAR
    float dx = obj1->xkm - obj2->xkm;
    float dy = obj1->ykm - obj2->ykm;
    double maxDistance = TARGET_MAX_SPEED_MARINE/3600000.0*dtime   + obj1->rgKm*atan(obj1->aziRes*3);
    double distancekm = sqrt(dx*dx+dy*dy);
    double speedkmh = distancekm/(dtime/3600000.0);
    double distanceCoeff = distancekm/maxDistance;
    if(distanceCoeff>1.0)return 0;

    // calculate score using machine learning model

    if(distanceCoeff<0.285928)
    {
        if(abs(rgSpeedkmh)<44.409351)return 0.971;
        else
        {
            if(distanceCoeff<0.127)return 0.57;
            else return 0.116;
        }
    }
    else
    {
        if(speedkmh<54.774822)
        {
            return 0.926174;
        }
        else
            return 0.01;
    }

}
void C_radar_data::ProcessObjects()
{
    //processTracks

    for (ushort j=0;j<mTrackList.size();j++)
    {
        track_t* track = &(mTrackList[j]);
        if(now_ms-track->updateTimeMs>500)
        {
            track->updateTimeMs=now_ms;
            if(track->possibleList.size())
            {
                int maxIndex =0;
                float maxScore = 0;
                for(int i=0;i<track->possibleList.size();i++)
                {
                    if(track->possibleList[i].scorep1>maxScore)
                    {
                        maxScore = track->possibleList[i].scorep1;
                        maxIndex = i;
                    }
                }
                track->objectList.push_back(track->possibleList[maxIndex]);
                track->possibleList.clear();
//                track->xkm = track->objectList.back().xkm;
//                track->ykm = track->objectList.back().ykm;
//                track->xkmo = track->objectList[0].xkm;
//                track->ykmo = track->objectList[0].ykm;
            }
        }
    }
    for (ushort i=0;i<mObjList.size();i++)
    {
        object_t *obj1 = &(mObjList[i]);
        if(obj1->isDead)continue;
        if(obj1->isProcessed)continue;
        obj1->isProcessed = true;
        obj1->scorep1=0;
        //check  if object_t belonging to tracks
        bool isBelongingToTrack = false;
        track_t* chosenTrack ;
        for (ushort j=0;j<mTrackList.size();j++)
        {
            track_t* track = &(mTrackList[j]);
            object_t *obj2 = &(track->objectList.back());

            double score = estimateScore(obj1,obj2);
            if(score>0.2&&score>obj1->scorep1)
            {
                obj1->scorep1=score;
                chosenTrack = track;

                isBelongingToTrack = true;
            }

        }
        if(isBelongingToTrack)
        {
            chosenTrack->possibleList.push_back(*obj1);
            chosenTrack->updateTimeMs = now_ms;
            obj1->isDead = true;
            continue;
        }
        // check for new lines
        std::vector<object_line> possibleLines;
        for (ushort j=0;j<mObjList.size();j++)
        {
            if(i==j)continue;
            object_t *obj2 = &(mObjList.at(j));
            //find new line
            if(obj2->isDead)continue;
            int dtime = (obj1->timeMs - obj2->timeMs);
            if(dtime>30000)
            {
                obj2->isDead = true;
                continue;
            }
            float rgSpeedkmh = (obj1->rgKm-obj2->rgKm)/(dtime/3600000.0);
            if(rgSpeedkmh>85)continue;//ENVAR
            double score = estimateScore(obj1,obj2);
#ifndef LOG_DATA_TRAINING
            //filtering using learned score
            if(score<0.5||score<obj2->scorep2||score<obj1->scorep1) continue;
            object_line newline;
            newline.pointScore      = score;
            newline.trackScore      = 0;
//            newline.distanceCoeff   = distanceCoeff;
//            newline.speedkmh        = speedkmh;
            newline.rgSpeedkmh      = rgSpeedkmh;
//            newline.bearingRad      = ConvXYToAziRad(dx,dy);
            newline.dtimeMSec       = dtime;
            newline.obj1            = *obj1;
            newline.obj2            = *obj2;
            newline.isProcessed     = false;
            newline.isDead          = false;
            possibleLines.push_back(newline);
#else
            if(obj1->dopler==obj2->dopler)
            {
                fprintf(logfile,"%f",rgSpeedkmh);//
                //fprintf(logfile,",%d",dtime);
                fprintf(logfile,",%f",speedkmh);
                //fprintf(logfile,",%f",newline.rgSpeedkmh);
                fprintf(logfile,",%f",distancekm/maxDistance);
                fprintf(logfile,",1");
                fprintf(logfile,"\n");
            }
            else
            {
                fprintf(logfile,"%f",rgSpeedkmh);//
                //fprintf(logfile,"%d",dtime);
                fprintf(logfile,",%f",speedkmh);
                //fprintf(logfile,",%f",newline.rgSpeedkmh);//
                fprintf(logfile,",%f",distancekm/maxDistance);
                fprintf(logfile,",0");
                fprintf(logfile,"\n");
            }
#endif
        }
        if(!possibleLines.size())continue;
        double maxScore = 0;
        short maxIndex=0;
        for(ushort i =0;i<possibleLines.size();i++)
        {
            if(possibleLines[i].pointScore>maxScore)
            {
                maxScore = possibleLines[i].pointScore;
                maxIndex=i;
            }
        }

        object_line *newline = &(possibleLines[maxIndex]);
        newline->obj1.scorep1 = maxScore;
        newline->obj2.scorep2 = maxScore;

        //replace the dead
        ushort k=0;
        for (;k<mLineList.size();k++)
        {
            if(mLineList[k].isDead)
            {
                mLineList[k] = possibleLines[maxIndex];
                break;
            }
        }
        if(k>=mLineList.size())
        {
            if(k<3000)mLineList.push_back(possibleLines[maxIndex]);
            else {
                printf("\nfull lines memory");
                continue;
            }
        }
    }
    return;

}
double C_radar_data::ConvXYToRange(double x, double y)
{
    return sqrt(x*x + y*y);

}
double C_radar_data::ConvXYToAziRad(double x, double y)
{
    double azi;
    if (!y)
    {
        azi = x>0 ? PI_CHIA2 : (PI_NHAN2 - PI_CHIA2);

    }
    else
    {
        azi = atanf(x / y);
        if (y<0)azi += PI;
        if (azi<0)azi += PI_NHAN2;
    }
    return azi;
}
