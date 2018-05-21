#ifndef CAPTUREFILEREPLAY_H
#define CAPTUREFILEREPLAY_H
#include <QFile>
#include <QThread>
#include <stdio.h>
#include <QQueue>
#include <QTime>
/* 4 bytes IP address */
typedef struct ip_address{
    uchar byte1;
    uchar byte2;
    uchar byte3;
    uchar byte4;
}ip_address;
/* IPv4 header */
typedef struct ip_header{
    uchar  ver_ihl;        // Version (4 bits) + Internet header length (4 bits)
    uchar  tos;            // Type of service
    ushort tlen;           // Total length
    ushort identification; // Identification
    ushort flags_fo;       // Flags (3 bits) + Fragment offset (13 bits)
    uchar  ttl;            // Time to live
    uchar  proto;          // Protocol
    ushort crc;            // Header checksum
    ip_address  saddr;      // Source address
    ip_address  daddr;      // Destination address
    uint   op_pad;         // Option + Padding
}ip_header;

/* UDP header*/
typedef struct udp_header{
    ushort sport;          // Source port
    ushort dport;          // Destination port
    ushort len;            // Datagram length
    ushort crc;            // Checksum
}udp_header;
struct EthernetDataFrame
{
    ip_address s_ip;
    uint len;
    bool isUdp;
    ushort sport,dport;
    QByteArray dataArray;
};
struct dataStream
{
    ip_address addr;
    QQueue <EthernetDataFrame> mCapData;

};
class DataReplay: public QThread
{

public:

    DataReplay(QString fileName);
    void    run();
    void    playbackFile();
    int     playRate;
    bool    isPlaying;
    QTime mTimeReal;
    QList<dataStream> mCapDataList;
    void StopPlayback();
private:
    QFile   signRepFile;
};

#endif // CAPTUREFILEREPLAY_H
