#include "datareplay.h"
#include "pcap.h"
#include <QString>

DataReplay::DataReplay(QString fileName)
{
    if(signRepFile.isOpen()) signRepFile.close();
    signRepFile.setFileName(fileName);
    signRepFile.open(QIODevice::ReadOnly);
    playRate = 500;

}


void DataReplay::run()
{
    isPlaying = true;
    playbackFile();
}
void DataReplay::StopPlayback()
{
    isPlaying  = false;

}
void DataReplay::playbackFile()
{

    mTimeReal.start();
    unsigned long recTimeus;
    unsigned long recTimeusOld=0;
    int intervalus;
    long long timeTotalms=0;

    while(isPlaying) {
        unsigned int len;

        if(!signRepFile.isOpen())return;
        for(unsigned short i=0;i<playRate;i++)
        {
            //QMutexLocker locker(&mutex);
            if(!signRepFile.read(((char*)&len),sizeof(len)))
            {
                signRepFile.seek(0);
                //mRadarData->SelfRotationReset();
                //togglePlayPause(false);
                StopPlayback();
            }

            if(!signRepFile.read((char*)&recTimeus,4))
            {
                signRepFile.seek(0);
                //mRadarData->SelfRotationReset();
                //togglePlayPause(false);
                StopPlayback();
            }



            if(len>2000)
            {
                StopPlayback();
                return;
            }
            ip_header *ih;
            udp_header *uh;




            EthernetDataFrame frame;
            frame.dataArray.resize(len);
            char *pkt_data = frame.dataArray.data();
            signRepFile.read((char*)pkt_data,len);
            ih = (ip_header *) (pkt_data +
                    14); //length of ethernet header
            /* retireve the position of the udp header */
            uint ip_len = (ih->ver_ihl & 0xf) * 4;
            uh = (udp_header *) ((u_char*)ih + ip_len);
            /* convert from network byte order to host byte order */
            frame.sport = ((uh->sport)<<8)+((uh->sport)>>8);//ntohs(  );
            frame.dport = ((uh->dport)<<8)+((uh->dport)>>8);//ntohs( uh->dport );
            if(frame.sport!=4001&&frame.sport!=4002)continue;
            /* print ip addresses and udp ports */
                printf("%d.%d.%d.%d.%d -> %d.%d.%d.%d.%d\n",
                    ih->saddr.byte1,
                    ih->saddr.byte2,
                    ih->saddr.byte3,
                    ih->saddr.byte4,
                    frame.sport,
                    ih->daddr.byte1,
                    ih->daddr.byte2,
                    ih->daddr.byte3,
                    ih->daddr.byte4,
                    frame.dport);
            frame.s_ip = saddr;
            foreach (dataStream stream, mCapDataList) {
                if(frame.s_ip==stream.addr)stream.mCapData.enqueue(frame);
            }


        }
        intervalus = recTimeus-recTimeusOld;
        if(intervalus<0)intervalus+=1000000;
        recTimeusOld = recTimeus;
        timeTotalms += intervalus/1000;
        while (timeTotalms>mTimeReal.elapsed()) {
            sleep(1);
        }
        printf("\ntime:%d",timeTotalms);
    }
}
