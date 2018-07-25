#include <QCoreApplication>
#include <Windows.h>
#include <QtNetwork/QUdpSocket>
int main(int argc, char *argv[])
{

    QUdpSocket socket;
    socket.connectToHost(QHostAddress("127.0.0.1"),30001,QIODevice::ReadWrite);
    char gyroFrame[35];
    long int gyroHeading;
    gyroHeading = 5000;
    gyroFrame[0]=0x5a;
    gyroFrame[1]=0xa5;
    gyroFrame[2]=0x1a;
    gyroFrame[3]=0x01;
    gyroFrame[4]=0x00;
    gyroFrame[5]=0x00;
    gyroFrame[6]=gyroHeading>>8;
    gyroFrame[7]=gyroHeading;
    gyroFrame[8]=0;
    gyroFrame[9]=0;
    gyroFrame[10]=0;
    gyroFrame[11]=0;
    gyroFrame[12]=0;
    gyroFrame[13]=0;
    gyroFrame[14]=0;
    gyroFrame[15]=0;
    gyroFrame[16]=0;
    gyroFrame[17]=0;
    gyroFrame[18]=0;
    gyroFrame[19]=200;
    gyroFrame[20]=0;
    gyroFrame[21]=100;
    gyroFrame[22]=0;
    gyroFrame[23]=0;
    gyroFrame[24]=0;
    gyroFrame[25]=0;
    gyroFrame[26]=0;
    gyroFrame[27]=0;
    gyroFrame[28]=0;
    gyroFrame[29]=0;
    gyroFrame[30]=0xab;
    gyroFrame[31]=0xaa;
    gyroFrame[32]=0x00;
    gyroFrame[33]=0x00;
    gyroFrame[34]=0x00;
    while(true)
    {
        Sleep(10);
        gyroHeading+=10;
        if(gyroHeading>10000)
            gyroHeading=5000;
        gyroFrame[06]=gyroHeading>>8;
        gyroFrame[07]=gyroHeading;
        socket.write(&gyroFrame[0],32);
    }

    return 0;
}

