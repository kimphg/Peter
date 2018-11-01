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
}
void c_target_manager::OutputTargetData()
{
    udpSocketSend->connectToHost(QHostAddress(CConfig::getString("KASU_IP","192.168.0.1")),30000,QIODevice::ReadWrite);
    udpSocketSend->write((char*)&kasudatagram[0],KASU_DATA_SIZE);
}
