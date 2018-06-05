#ifndef CAPTUREFILEREPLAY_H
#define CAPTUREFILEREPLAY_H
//#include "dataprocessor.h"
#include "AIS/AIS.h"
#include <QFile>
#include <QThread>
#include <stdio.h>
#include <QQueue>
#include <QTime>
#include "../../QT_Peter/c_arpa_data.h"
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
struct DataSource
{
    ip_address addr;
    QQueue <EthernetDataFrame> frameList;
    QString aisBuffer;
    AIS aisMessageHandler;

};
class DataReplay: public QThread
{
public:
    DataSource mProcessor;
    DataReplay(QString fileName);
//    void processDataPcap(const uchar *dataptr, int len);
    void processDataFrame(const uchar *pkt_data, int len);
    void    run();
    void    playbackFile();
    int     playRate;
    bool    isPlaying;
    double timeTotalms;
    QTime mTimeReal;
    QList<DataSource> mSourceList;
    void processData();
    void StopPlayback();
    QList<AIS_object_t> m_aisList;
private:
    QFile   signRepFile;
};

#endif // CAPTUREFILEREPLAY_H
