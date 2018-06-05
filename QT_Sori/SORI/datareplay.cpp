#include "datareplay.h"
#include "pcap.h"
#include <QString>

DataReplay::DataReplay(QString fileName)
{
    if(signRepFile.isOpen()) signRepFile.close();
    signRepFile.setFileName(fileName);
    signRepFile.open(QIODevice::ReadOnly);
    playRate = 50;

}

//void DataReplay::processDataPcap(const uchar *dataptr,int len)
//{

//}

void DataReplay::processDataFrame(const uchar* pkt_data,int len)
{
    ip_header *ih;
    udp_header *uh;
    EthernetDataFrame frame;
    frame.dataArray.resize(len);
    memcpy(frame.dataArray.data(),pkt_data,len);
    ih = (ip_header *) (pkt_data + 14); //length of ethernet header
    /* retireve the position of the udp header */
    uint ip_len = (ih->ver_ihl & 0xf) * 4;
    uh = (udp_header *) ((u_char*)ih + ip_len);
    /* convert from network byte order to host byte order */
    frame.sport = ((uh->sport)<<8)+((uh->sport)>>8);//ntohs(  );
    frame.dport = ((uh->dport)<<8)+((uh->dport)>>8);//ntohs( uh->dport );
    if(frame.sport!=4002)return;//4001 for radar, 4002 for ais
    /* print ip addresses and udp ports */
//                printf("\n%d.%d.%d.%d:%d -> %d",
//                    ih->saddr.byte1,
//                    ih->saddr.byte2,
//                    ih->saddr.byte3,
//                    ih->saddr.byte4,
//                    frame.sport,
//                    //ih->daddr.byte1,
//                    //ih->daddr.byte2,
//                    //ih->daddr.byte3,
//                    //ih->daddr.byte4,
//                    frame.dport);
        //printf("data:");
        //printf(pkt_data+42);

    frame.s_ip = ih->saddr;
    frame.dataArray.remove(0,42);
    bool ip_exist = false;
    foreach (DataSource source, mSourceList) {
        if(frame.s_ip.byte4==source.addr.byte4)
            if(frame.s_ip.byte3==source.addr.byte3)
                if(frame.s_ip.byte2==source.addr.byte2)
                    if(frame.s_ip.byte1==source.addr.byte1)
                    {
                        source.aisBuffer.append(QString::fromLatin1(frame.dataArray));
                        ip_exist = true;
                        break;
                    }

    }
    if(!ip_exist)
    {
        DataSource newsource;
        newsource.addr = frame.s_ip;
        //newsource.frameList.enqueue(frame);
        newsource.aisBuffer.append(QString::fromLocal8Bit(frame.dataArray));
        mSourceList.append(newsource);
    }
    processData();
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
    timeTotalms=0;

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

            QByteArray dataArray;
            dataArray.resize(len);
            char *pkt_data = dataArray.data();
            signRepFile.read((char*)pkt_data,len);
            processDataFrame((uchar*)pkt_data,len);

        }
        intervalus = recTimeus-recTimeusOld;
        if(intervalus<0)intervalus+=1000000;
        recTimeusOld = recTimeus;
        timeTotalms += (intervalus/1000.0);

        while (timeTotalms>mTimeReal.elapsed()) {
            sleep(1);
        }
        //printf("\ntime:%f",timeTotalms/1000.0);
    }
}

void DataReplay::processData()
{
    foreach (DataSource source, mSourceList) {

        QStringList strlist = source.aisBuffer.split("\r\n");
        if(strlist.size() <= 1)continue;
        source.aisBuffer=strlist.at(strlist.size()-1);

        AIS *aisMsgHandler = &source.aisMessageHandler;
        for(int i = 0;i<strlist.size()-1;i++)
            if(aisMsgHandler->ProcessNMEA(strlist.at(i)))
            {
                AIS_object_t newAisObj ;
                newAisObj.mMMSI = aisMsgHandler->get_mmsi();

                if(aisMsgHandler->get_type()==AIS::AIS_MSG_24_STATIC_DATA_REPORT)
                {
                    if(aisMsgHandler->get_partno()==0)
                    {
                        newAisObj.mName = QString::fromLatin1(aisMsgHandler->get_shipname());
                        newAisObj.mType = 0;
                    }
                    else
                    {
                        newAisObj.mType = aisMsgHandler->get_shiptype();

                    }
                }
                else
                {
                    newAisObj.mName = QString::fromLatin1(aisMsgHandler->get_shipname());
                    newAisObj.mType = aisMsgHandler->get_shiptype();
                }
                newAisObj.mDst = QString(aisMsgHandler->get_destination());
                newAisObj.mImo = aisMsgHandler->get_imo();
                newAisObj.mNavStat = aisMsgHandler->get_navStatus();
                newAisObj.mBow = aisMsgHandler->get_to_bow();
                newAisObj.mStern = aisMsgHandler->get_to_stern();
                newAisObj.mStarboard = aisMsgHandler->get_to_starboard();
                newAisObj.mPort = aisMsgHandler->get_to_port();
                newAisObj.mSog = aisMsgHandler->get_SOG()/10.0;
                newAisObj.mCog = aisMsgHandler->get_COG()/10.0;
                newAisObj.mLat = aisMsgHandler->get_latitude()/600000.0;
                newAisObj.mLong = aisMsgHandler->get_longitude()/600000.0;
                newAisObj.mLut = QDateTime::currentMSecsSinceEpoch();
                newAisObj.isNewest = true;
                newAisObj.isSelected = false;
                newAisObj.mName.replace('@',"");
                QMutableListIterator<AIS_object_t> i(m_aisList);
                bool is_new_target = true;
                while (i.hasNext())
                {
                    AIS_object_t obj = i.next();
                    //elecount++;
                    //if(elecount>100){i.remove();continue;}
                    if(obj.mMMSI==newAisObj.mMMSI)
                    {
                        /*obj.isNewest = false;
                        newAisObj.isSelected = obj.isSelected;
                        if(newAisObj.mName.isEmpty()&&(!obj.mName.isEmpty()))
                            newAisObj.mName = obj.mName;
                        if(newAisObj.mLat==0)newAisObj.mLat = obj.mLat;
                        if(newAisObj.mLong==0)newAisObj.mLong = obj.mLong;
                        if(newAisObj.mDst.isEmpty())
                            newAisObj.mDst      = obj.mDst;
                        if(!newAisObj.mImo)
                            newAisObj.mImo      = obj.mImo;
                        if(!newAisObj.mType)
                            newAisObj.mType      = obj.mType;
                        if(!newAisObj.mBow)
                            newAisObj.mBow      = obj.mBow;
                        if(!newAisObj.mStern)
                            newAisObj.mStern      = obj.mStern;
                        if(!newAisObj.mStarboard)
                            newAisObj.mStarboard      = obj.mStarboard;
                        if(!newAisObj.mPort)
                            newAisObj.mPort      = obj.mPort;
                        if(!newAisObj.mSog)
                            newAisObj.mSog      = obj.mSog;
                        if(!newAisObj.mCog)
                            newAisObj.mCog      = obj.mCog;*/
                        i.setValue(newAisObj);
                        //i.remove();
                        //m_aisList.push_back(newAisObj);
                        is_new_target = false;
                        break;
                    }
                }
                if(is_new_target)m_aisList.push_front(newAisObj);
            }
    }
}
