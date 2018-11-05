#include "c_target_manager.h"


c_target_manager::c_target_manager()
{
    trackTable = new TrackPointer[TRACK_TABLE_SIZE];
    memset(trackTable,0,sizeof(TrackPointer)*TRACK_TABLE_SIZE);
    targetTable = new TrackPointer[TARGET_TABLE_SIZE];
    memset(targetTable,0,sizeof(TrackPointer)*TARGET_TABLE_SIZE);
    udpSocketSend = new QUdpSocket();
    initDataGram();

}

c_target_manager::~c_target_manager()
{
    delete [] trackTable;
    delete [] targetTable;
}
void c_target_manager::initDataGram()
{
    kasudatagram[0] = 0x16;
    kasudatagram[1] = 0x16;
    kasudatagram[2] = 0x01;
    kasudatagram[3] = 0x16;
    kasudatagram[4] = 0x00;
    kasudatagram[5] = 0x00;//Циклический номер последнего правильного принятого пакета
    kasudatagram[6] = 0x00;//Циклический номер пакета
    kasudatagram[7] = 0x00;
    kasudatagram[8] = 0x16;//Контрольная сумма байт 3-8
    kasudatagram[9] = 0x16;//Контрольная сумма байт 3-8
    //end of header
    memset(&kasudatagram[10],0,22*6);
    kasudatagram[142] = 0x00;
    kasudatagram[143] = 0x00;//Контрольная сумма поля данных
}
void c_target_manager::OutputTargetData()
{
    unsigned char dataPacket[22];
    //targets:
    for(int i=0;i<6;i++)
    {
        track_t* target = targetTable[i].track;
        if(target==nullptr)continue;
        dataPacket[0] = (unsigned char)(i+1);
        //distance
        int distance = int(target->rgKm*1000.0);
        if(distance<100||distance>260000)
        { printf("\nTarget %d wrong distance value: %d",i+1,distance);}
        dataPacket[1] = (unsigned char)(distance);
        dataPacket[2] = (unsigned char)(distance>>8);
        dataPacket[3] = (unsigned char)(distance>>16);
        dataPacket[4] = (unsigned char)(distance>>24);
        //peleng
        int peleng = int(target->aziDeg/180.0*16384);
        dataPacket[5] = (unsigned char)(peleng);
        dataPacket[6] = (unsigned char)(peleng>>8);
        int course = int(target->courseRadFit/180.0*16384);
        dataPacket[7] = (unsigned char)(course);
        dataPacket[8] = (unsigned char)(course>>8);
        int speed = int(target->mSpeedkmhFit);

    }
    udpSocketSend->connectToHost(QHostAddress(CConfig::getString("KASU_IP","192.168.0.1")),30000,QIODevice::ReadWrite);
    udpSocketSend->write(reinterpret_cast<char*>(&kasudatagram[0]),KASU_DATA_SIZE);
}
