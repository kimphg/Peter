// EthernetCapture.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include <conio.h>
#include <tchar.h>
#define HAVE_REMOTE// for pcap
#define WIN32
#define WPCAP
#include "pcap.h"
#pragma comment(lib, "user32.lib")
#pragma comment (lib, "Ws2_32.lib")
#define UDP_HEADER_LEN 42
FILE* pFile;
unsigned char dataBuff[1500];
void packet_handler(u_char *param, const struct pcap_pkthdr *pkt_header, const u_char *pkt_data)
{
	//struct tm ltime;
	//char timestr[16];
	//time_t local_tv_sec;

	/*
	* unused variables
	*/
	//    (VOID)(param);
	//    (VOID)(pkt_data);

	/* convert the timestamp to readable format */
	//local_tv_sec = pkt_header->ts.tv_sec;
	//localtime_s(&ltime, &local_tv_sec);
	//strftime( timestr, sizeof timestr, "%H:%M:%S", &ltime);
	if (!pFile)return;
	fwrite(&pkt_header->len, sizeof pkt_header->len, 1, pFile);
	fwrite(&pkt_header->ts.tv_usec, sizeof pkt_header->ts.tv_usec, 1, pFile);
	fwrite(pkt_data, pkt_header->len, 1, pFile);
	if (ftell(pFile) > 1000000000)
	{
		fclose(pFile);
		pFile = 0;
		printf("\nghi luu thanh cong");
	}
		//fwrite();
	//if (pkt_header->len < 1000)return;
	//pkt_header->
	//if (((*(pkt_data + 36) << 8) | (*(pkt_data + 37))) != 5000)
	//{
		//printf("\nport:%d",((*(pkt_data+36)<<8)|(*(pkt_data+37))));
	//	return;
	//}
}
int _tmain(int argc, _TCHAR* argv[])
{
	pcap_if_t *alldevs;
	pcap_if_t *d;
	pcap_t *adhandle;
	char errbuf[PCAP_ERRBUF_SIZE];
	//
	/* Retrieve the device list on the local machine */
	if (pcap_findalldevs_ex((char*)PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf) == -1)
	{
		//isRunning = false;
		printf(errbuf); return -1;
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
	}
	d = alldevs;
	
	printf("Chon card mang can ghi luu:\n", d->description);
	int interfaceIndex = 0;
	
	scanf_s("%d", &interfaceIndex);
	if (interfaceIndex<1 || interfaceIndex>i+1)
	{
		printf("Gia tri khong hop le");
		getchar();
		return 0;
	}
	i = 0;
	for (d = alldevs; d; d = d->next)
	{
		i++;
		if (i == interfaceIndex)
		{
			printf("\nDang ghi luu tai card mang: %s\n...", d->description);
			fopen_s(&pFile,"capture.cap", "wb");
			if ((adhandle = pcap_open(d->name,          // name of the device
				65536,            // portion of the packet to capture
				// 65536 guarantees that the whole packet will be captured on all the link layers
				PCAP_OPENFLAG_PROMISCUOUS,    // promiscuous mode
				1000,             // read timeout
				NULL,             // authentication on the remote machine
				errbuf            // error buffer
				)) == NULL)
			{
				/* Free the device list */
				pcap_freealldevs(alldevs);
				return -1;
			}
			pcap_loop(adhandle, 0, packet_handler, NULL);
			
		}
	}
	
	return 0;
}

