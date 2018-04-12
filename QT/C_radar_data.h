#ifndef C_RAW_DATA_H
#define C_RAW_DATA_H
//----------------------------------------------------------//
//HR2D signal processing class and Kalman tracking algorithm   //
//First release: November 2015                              //
//Project:https://github.com/kimphg/Jupiter                 //
//Last update: March 2017                                   //
//Author: Phung Kim Phuong                                  //
//----------------------------------------------------------//
#define ARMA_USE_LAPACK
#define ARMA_USE_BLAS
#define ARMA_BLAS_UNDERSCORE

#define TRACK_STABLE_STATE          5
#define MIN_TERRAIN                 10
#define TRACK_CONFIRMED_SIZE        3
#define TRACK_INIT_STATE            3
#define DEG_RAD 57.295779513
#ifndef CONST_NM
    #define CONST_NM 1.852f// he so chuyen doi tu km sang hai ly
#endif
#define PI_NHAN2                    6.2831853072
#define PI_CHIA2                    1.5707963268
#ifndef PI
   #define PI                       3.14159265358979323846
#endif
#define MAX_TRACK_LEN               400
#define MAX_TRACKS                  199
#define ENCODER_RES                 5000
#define MAX_AZIR                    2048
#define MAX_AZIR_DRAW               6144
#define RAD_M_PULSE_RES             1536
#define RAD_S_PULSE_RES             256
#define DISPLAY_RES                 768
#define RAD_FULL_RES                1792
#define SIGNAL_SCALE_7              0.24113 //215.38461538461538461538461538461
#define SIGNAL_SCALE_6              0.24113//184.61538461538461538461538461538
#define SIGNAL_SCALE_5              0.24113 //153.84615384615384615384615384615
#define SIGNAL_SCALE_4              0.24113 // 123.07692307692307692307692307692
#define SIGNAL_SCALE_3              0.12056514914 //92.307692307692307692307692307692
#define SIGNAL_SCALE_2              0.06028257456 //61.538461538461538461538461538462
#define SIGNAL_SCALE_1              0.03014128728 //30.769230769230769230769230769231
#define SIGNAL_SCALE_0              0.015070644 //15.384615384615384615384615384615
#define TERRAIN_GAIN                0.9f
#define TERRAIN_GAIN_1              0.1f
#define TERRAIN_THRESH              0.5f
#define TARGET_MIN_SPEED            3
#define TARGET_MAX_SPEED            50
#define ZOOM_SIZE                   550
#define DISPLAY_RES_ZOOM            5120
#define DISPLAY_SCALE_ZOOM          4

#include "jtarget.h"

#include <vector>
#include <QImage>
#include <QDateTime>
#include <QFile>
#include <Eigen/Dense>
#include <queue>


//#include <QDebug> //REMLATER
//#ifdef _WIN32
//#include <armadillo>
//#else
//#include <armadilloLinux/armadillo>
//#endif
//using namespace arma;
//#include <list>
//using namespace std;
using namespace Eigen;
/*typedef struct {
    short x,y;
    unsigned char level;
    unsigned char displaylevel;
    unsigned char vet;
    unsigned char dopler;
    float terrain;
    short markIndex;
}raw_point_t;

typedef struct {
    raw_point_t raw_map[RAD_FULL_RES];

}frame_t;
*/


typedef struct  {
    unsigned int sumTer,sumA,sumR;
    short lastA,firstA,ctA;
    unsigned short maxR,minR,ctR;
    unsigned short size;
    unsigned char maxLevel,dopler;
    //bool isProcessed;
} plot_t;
typedef struct  {
    float          az ,rg,x,y;
    short          azMin,azMax,rMin,rMax;
    short          size;
    char           dopler;
    bool           isManual;
    float          p;
    float          terrain;
}object_t;
typedef std::vector<plot_t> plotList;
typedef std::vector<object_t> objectList;
using Eigen::MatrixXf;
//class matrix_t
//{
//public:
//    float *data;
//    short row,col;
//    matrix_t()
//    {
//        data = 0;
//        row = 0;
//        col = 0;
//    }
//    void init(short irow,short icol) {

//        data = new float[irow*icol];
//        memset(data,0,irow*icol);
//        row = irow;
//        col = icol;

//    }
//    ~matrix_t() {
//        if(data)delete[] data;
//    }
//    float* at(short r,short c)
//    {
//        if(r>row||c>col) return 0;
//        return &data[col*r+c];
//    }
//    matrix_t mul(matrix_t *mat)
//    {
//        short r1 = this->row;
//        short c1 = this->col;
//        short c2 = mat->col;
//        matrix_t m;
//        m.init(r1,c2);
//        if(c1!=mat->row)return m;

//        for(short i=0; i<r1; ++i)
//        for(short j=0; j<c2; ++j)
//        for(short k=0; k<c1; ++k)
//        *(m.at(i,j)) += (*this->at(i,k))*(*mat->at(k,j));
//        return m;
//    }
//};
//______________________________________//
class track_t : public JTarget{
public:
    track_t()
    {

    }
    //
    MatrixXd q1;
    MatrixXd q2;
    MatrixXd h;
    MatrixXd p;
    MatrixXd x;
    //
    //qint64 currentTimeMs;
    bool isConfirmed;

    objectList suspect_list,object_list;
    char terrain;
    double rotA_r;
    double estX ,estY;
    double estA, estR;
    double mesA;
    double mesR;
    char state;
    short idCount;
    //float dTime;
    bool isTracking,isManual,isLost;
    char dopler;
    //QDateTime time;
    bool isProcessed;
    bool isUpdated;
    short trackLen;
    void init(object_t *object);
    void update();
    void predict();
    bool checkProb(object_t* object);
    void setManual(bool isMan);

private:
    void stateUpdate(bool isNewPlot);
};
typedef std::vector<track_t> trackList;
//______________________________________//
enum imgDrawMode
{
    VALUE_ORANGE_BLUE = 0,
    VALUE_YELLOW_SHADES = 1,
    DOPLER_3_COLOR = 2,
};
enum DataOverLay { m_only, s_m_200, s_m_150 , max_s_m_200, max_s_m_150};
enum RotationDir { Right , Left};
class C_radar_data {
public:

    C_radar_data();
    ~C_radar_data();
    float k_vet;// !!!!
    RotationDir             rotDir;
    float                   rotation_per_min ;
    trackList               mTrackList;
    plotList                plot_list;
    bool                    isEncoderAzi;
    int                     mEncoderAzi;
    unsigned char           spectre[16];
    unsigned char           overload, init_time, clk_adc;
    float                   scale_ppi,scale_zoom_ppi;
    short                   curAzir,arcMinAzi,arcMaxAzi,arcWidth;
    void                    setZoomRectAR(float ctx, float cty, double sizeKM, double sizeDeg);
    void                    setZoomRectXY(float ctx, float cty);
    unsigned int            sn_stat,chu_ky;
    unsigned short          *tb_tap;
    double                  tb_tap_k;
    int                     get_tb_tap();
    bool                    is_do_bup_song;
    bool                    isClkAdcChanged,xl_dopler,cut_thresh,isSled,filter2of3;
    bool                    isManualTune,rgs_auto,bo_bang_0,data_export;
    bool                    isSelfRotation;
    double                   krain,kgain,ksea,brightness;
    double                   krain_auto,kgain_auto,ksea_auto;
    void setAutorgs( bool aut);
    void                    clearPPI();
    unsigned char           moduleVal;
    float                   trueN;
    DataOverLay             dataOver;
    unsigned char           noise_level[8];
    unsigned char           tempType,rotation_speed;
    unsigned short          range_max;
    QImage                  *img_ppi,*img_RAmp,*img_zoom_ppi,*img_histogram,*img_spectre,*img_zoom_ar;
    int                     zoom_ar_w,zoom_ar_h,zoom_ar_a0,zoom_ar_r0,zoom_ar_a1,zoom_ar_r1;
    int                     zoom_ar_size_a,zoom_ar_size_r;
    imgDrawMode             imgMode;
    //double                  sn_scale;
    void deleteTrack(ushort trackNum);
    void drawRamp();

    //______________________________________//
    void        assembleDataFrame(unsigned char *data, unsigned short dataLen);
    void        UpdateData();
    void        ProcessDataFrame();
    void        ProcessData(unsigned short azi);
    void        raw_map_init();
    void        raw_map_init_zoom();
    void        drawAzi(short azi);
    void        drawBlackAzi(short azi_draw);
    void        DrawZoom(short azi_draw, short r_pos);
//    void        blackLine(short x0, short y0, short x1, short y1);
    void        addTrackManual(double x, double y);
    void        addTrack(object_t *mObject);
    static    void        kmxyToPolarDeg(double x, double y, double *azi, double *range);
    void        setTrueN(double trueN_deg){

        while(trueN_deg<0)trueN_deg+=360;
        while(trueN_deg>=360)trueN_deg-=360;
        trueN =(trueN_deg/360.0*PI_NHAN2);
        raw_map_init();
        resetTrack();
    }
    void        setScalePPI(float scale);
    void        setScaleZoom(float scale);
    void        resetData();
    void        resetSled();
    void        setProcessing(bool onOff);
    //bool        getDataOverload(){if(isDataTooLarge) {isDataTooLarge =false;return true;} else return false;}
    bool        checkFeedback(unsigned char* command);
    unsigned char* getFeedback()
    {

        return (unsigned char*)&command_feedback[0];
    }
    void        resetTrack();
    void SetHeaderLen(short len);
    bool getDoubleFilter() const;
    void setDoubleFilter(bool value);

    void SelfRotationOn(double dazi);
    void SelfRotationOff();
    void SelfRotationReset();
    void drawRamp(double azi);
    double getCurAziRad() const;

    bool getIsVtorih() const;
    void setIsVtorih(bool value);

    bool getIsSharpEye() const;
    void setIsSharpEye(bool value);

    double getArcMaxAziRad() const;
    double getArcMinAziRad() const;
private:
    float hsTap ;
    QVector<QRgb> colorTable;
    double      selfRotationDazi;
    double      selfRotationAzi;
    float       k_rain_auto,k_gain_auto;
    bool        isVtorih;
    bool        avtodetect;
    bool        doubleFilter;
    uint        getColor(unsigned char pvalue, unsigned char dopler, unsigned char sled);
    void        drawSgn(short azi_draw, short r_pos);
    void        drawSgnZoom(short azi_draw, short r_pos);
    unsigned char command_feedback[8];
    void        polarToXY(float *x, float *y, float azi, float range);
    bool        isProcessing;
    bool        isSharpEye;
    float       noiseAverage,rainLevel,noiseVar;
    void        getNoiseLevel();
    void        procPix(short proc_azi,short range);
    void        procTracks(unsigned short curA);
    void        procPLot(plot_t* mPlot);
    bool procObjectAvto(object_t* pObject);
    bool procObjectManual(object_t* pObject);
    //void status_start();
    //FILE *pFile;

    void decodeData(int azi);
    //void initZoomAR(int a0, int r0);
    bool DrawZoomAR(int a,int r,short val,short dopler,short sled);
    int getNewAzi();
public:
    //void drawZoomAR();
    float getNoiseAverage() const;
    void setNoiseAverage(float value);
    void setTb_tap_k(double value);
    double getSelfRotationAzi() const;
    void setSelfRotationAzi(int value);
    void processSocketData(unsigned char *data, short len);
};

//extern C_radar_data radarData;

#endif
