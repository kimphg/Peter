#include "c_radar_simulation.h"
static std::default_random_engine generator;
static std::normal_distribution<double> distribNoise(30, 8);
static std::normal_distribution<double> distribAzi(0, 1);//scale of 2048
static std::normal_distribution<double> distribRot(180, 0.5 );//deg per sec
static unsigned char n_clk_adc = 0;
double rResolution = 0.015070644;
unsigned char outputFrame[MAX_AZI][OUTPUT_FRAME_SIZE];
double ConvXYToR(double x, double y)
{
    double range;
    if (!y)
    {
        range = abs(x);
    }
    else
    {
        //*azi = atanf(x / y);
        //if (y<0)*azi += PI;
        //if (*azi<0)*azi += PI_NHAN2;
        range = sqrt(x*x + y*y);
        //*azi = *azi*DEG_RAD;
    }
    return range;

}
double ConvXYToAziRad(double x, double y)
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
void regenerate(int azi)
{
    unsigned char* dataPointer = &outputFrame[azi][0] + FRAME_HEADER_SIZE;
    for (int i = 0; i < FRAME_LEN; i++)
    {
        int num = int(distribNoise(generator));
        if (num < 0)num = 0;
        dataPointer[i] = num;
    }
    for (int i = FRAME_LEN; i < FRAME_LEN * 2; i++)
    {
        dataPointer[i] = rand()%16;
    }
}
target_t::target_t(double tx, double ty, double tspeed, double tbearing, int dople)
{
    speed = tspeed;
    x = tx;
    y = ty;
    bearing = tbearing / 180.0*3.14159265359;
    azi = ConvXYToAziRad(x, y) / 3.141592653589*1024.0;
    range = ConvXYToR(x, y);
    dopler = dople;
    targetSize = 5;
    nUpdates = 0;
    timeLast = time(0);
    rot = 0;
}

void target_t::generateSignal()
{
    if (range >= FRAME_LEN - 1)return;
    //if (rand() % 5)return;
    int azimin = azi - targetSize; if (azimin < 0)azimin += 2048;
    int azimax = azi + targetSize; if (azimax >= 2048)azimax -= 2048;
    int k = 0;
    for (int a = azimin; ; a++)
    {
        if (a < 0)a += 2048;
        if (a >= 2048) a -= 2048;
        if (a == azimax)break;
        int value = 150 * (1.0 - abs(k - targetSize) / (targetSize + 1.0));
        outputFrame[a][(int)range + FRAME_LEN + FRAME_HEADER_SIZE] = dopler;
        outputFrame[a][(int)range + FRAME_LEN + 1 + FRAME_HEADER_SIZE] = dopler;
        outputFrame[a][(int)range + FRAME_HEADER_SIZE] = value + int(distribNoise(generator));
        outputFrame[a][(int)range + 1 + FRAME_HEADER_SIZE] = value + int(distribNoise(generator));

        k++;
    }
}

void target_t::eraseSIgnal()
{
    if (range >= FRAME_LEN - 1)return;
    int azimax = azi + targetSize; if (azimax >= 2048)azimax -= 2048;
    int azimin = azi - targetSize; if (azimin < 0)azimin += 2048;
    for (int a = azimin; ; a++)
    {
        if (a < 0)a += 2048;
        if (a>=2048) a -= 2048;
        if (a == azimax)break;
        int num = int(distribNoise(generator));
        if (num < 0)num = 0;
        outputFrame[a][(int)range + FRAME_HEADER_SIZE] = num;
        outputFrame[a][(int)range + FRAME_LEN + FRAME_HEADER_SIZE] =  rand() % 16;
        num = int(distribNoise(generator));
        if (num < 0)num = 0;
        outputFrame[a][(int)range + 1 + FRAME_HEADER_SIZE] = num;
        outputFrame[a][(int)range + FRAME_LEN + 1 + FRAME_HEADER_SIZE] = rand() % 16;
    }
}

void target_t::update()
{
    eraseSIgnal();
    //recalculate coodinates
    time_t timenow = time(nullptr);
    double elapsed_secs = difftime(timenow, timeLast);
    timeLast = timenow;
    nUpdates++;
    if (nUpdates>5)
    {
        nUpdates = 0;
        rot = distribRot(generator) / DEG_RAD - PI;

    }
    bearing += rot*elapsed_secs;
    printf("\nbearing:%f", bearing);
    double distance = elapsed_secs / 3600.0*speed;
    x += distance*sin(bearing);
    y += distance*cos(bearing);
    //
    azi = ConvXYToAziRad(x, y) / 3.141592653589*1024.0;// +(distribAzi(generator));
    range	= ConvXYToR(x, y) / rResolution;


    generateSignal();
}

c_radar_simulation::c_radar_simulation()
{

}
void c_radar_simulation::updateTargets()
{
    for (uint i = 0; i < target.size(); i++)
    {
        target[i].update();
    }
}
void c_radar_simulation::socketInit()
{
    radarSocket = new QUdpSocket(this);
}
void c_radar_simulation::addtarget(double tx, double ty, double tspeed, double tbearing, int dople)
{
    target_t newTarget(tx,ty,tspeed,tbearing,dople);
}
void c_radar_simulation::run()
{
    socketInit();
    int azi = 200;
    for (int i = 0; i < MAX_AZI; i++)
    {
        regenerate(i);
        outputFrame[i][2] = i >> 8;
        outputFrame[i][3] = i;
    }
    int dazi = 1;
    int nPeriod = 0;
    n_clk_adc = 1;
    rResolution = 0.015070644 * pow(2, n_clk_adc);
    while (true)
    {
        //            msleep(8);
        azi += dazi;
        if (azi >= 2048)
            {
                nPeriod++;
                if (nPeriod > 50)nPeriod = 0;
                azi = 0;
                updateTargets();
            }

        /*if (azi>= 1000)
        {
            nPeriod++;
            dazi = -1;
            updateTargets();
            msleep(500);
        }
        else
            if (azi <= 2)
            {
                dazi = 1;
                updateTargets();
                msleep(500);
            }*/
        //if (rand() % 10 == 0)regenerate(azi);
        /*if (rand() % 10 == 0)
            {
                memset((char*)(&outputFrame[azi][0]), 0, OUTPUT_FRAME_SIZE);
            }*/
        outputFrame[azi][0] = 0x55;
        outputFrame[azi][2] = azi >> 8;
        outputFrame[azi][3] = azi;
        outputFrame[azi][4] = n_clk_adc;
        radarSocket->writeDatagram((char*)(&outputFrame[azi][0]),QHostAddress("127.0.0.1"),31000);

    }
}

