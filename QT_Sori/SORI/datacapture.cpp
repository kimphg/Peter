#include "datacapture.h"
DataReplay *globProcessingClass;
void packet_handler(u_char *param, const struct pcap_pkthdr *pkt_header, const u_char *pkt_data)
{
    //struct tm ltime;
    //char timestr[16];
    //time_t local_tv_sec;
    globProcessingClass->processDataFrame(pkt_data,pkt_header->len);
    /*
    * unused variables
    */
    //    (VOID)(param);
    //    (VOID)(pkt_data);

    /* convert the timestamp to readable format */
    //local_tv_sec = pkt_header->ts.tv_sec;
    //localtime_s(&ltime, &local_tv_sec);
    //strftime( timestr, sizeof timestr, "%H:%M:%S", &ltime);
//    if (!pFile)return;
//    fwrite(&pkt_header->len, sizeof pkt_header->len, 1, pFile);
//    fwrite(&pkt_header->ts.tv_usec, sizeof pkt_header->ts.tv_usec, 1, pFile);
//    fwrite(pkt_data, pkt_header->len, 1, pFile);
//    if (ftell(pFile) > 1000000000)
//    {
//        fclose(pFile);
//        pFile = 0;
//        printf("\nghi luu thanh cong");
//    }
    //fwrite();
    //if (pkt_header->len < 1000)return;
    //pkt_header->
    //if (((*(pkt_data + 36) << 8) | (*(pkt_data + 37))) != 5000)
    //{
    //printf("\nport:%d",((*(pkt_data+36)<<8)|(*(pkt_data+37))));
    //	return;
    //}
}


DataCapture::DataCapture(DataReplay *processingClass)
{
    globProcessingClass = processingClass;
}

QStringList DataCapture::getInterfacesList()
{

    QStringList res;
    pcap_if_t *alldevs;
    pcap_if_t *d;
    char errbuf[PCAP_ERRBUF_SIZE];
    //
    /* Retrieve the device list on the local machine */
    if (pcap_findalldevs_ex((char*)PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf) == -1)
    {
        //isRunning = false;
        printf(errbuf); return res;
    }
    //isRunning = true;
    int i = 0;
    /* Print the list */
    for (d = alldevs; d; d = d->next)
    {
        printf("\nCard mang so: %d. %s", ++i, d->name);
        if (d->description)
            printf(" (%s)\n", d->description);
        else
            printf(" (No description available)\n");
        res.append(QString(d->name)+";"+QString(d->description));
    }
    return res;

}

QString DataCapture::getInterface()
{
    return mInterface;
}

void DataCapture::setInterface( QString interf)
{
    mInterface = interf.split(";").at(0);
}

void DataCapture::run()
{
    pcap_t *adhandle;
    pcap_if_t *alldevs;
    pcap_if_t *d;
    char errbuf[PCAP_ERRBUF_SIZE];
    if (pcap_findalldevs_ex((char*)PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf) == -1)
    {
        //isRunning = false;
        printf(errbuf); return ;
    }
    if(!mInterface.size())return;
    if ((adhandle = pcap_open((char*)mInterface.data(),          // name of the device
                              65536,            // portion of the packet to capture
                              // 65536 guarantees that the whole packet will be captured on all the link layers
                              PCAP_OPENFLAG_PROMISCUOUS,    // promiscuous mode
                              1000,             // read timeout
                              NULL,             // authentication on the remote machine
                              errbuf            // error buffer
                              )) == NULL)
    {
        /* Free the device list */
        //pcap_freealldevs(alldevs);
        printf("\npcap_open() failed:");
        printf(errbuf);
        return ;
    }
    pcap_loop(adhandle, 0, packet_handler, NULL);

}

