//setx -m OPENCV_DIR D:\OpenCV\OpenCV331\opencv\build
//setx path "%path%;D:\OpenCV\OpenCV331\opencv\build\bin\Release\"
//#include "device_launch_parameters.h"
//#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include <conio.h>
#include <tchar.h>
#define HAVE_REMOTE// for pcap
#include "pcap.h"
#define HR2D_PK//
#define FRAME_LEN 1024
#define FFT_SIZE 1
#define BANG_KHONG 0
//int mFFTSkip = FFT_SIZE/8;
#include <time.h>
#define MAX_IREC 2400
#pragma comment(lib, "user32.lib")
#pragma comment (lib, "Ws2_32.lib")
//file mapping
#define FRAME_HEADER_SIZE 34
bool isUnsigned = false;
//using namespace cv;
using namespace std;

#include <stdio.h>

//_______________________________________________________________________

struct DataFrame// buffer for data frame
{
	char header[FRAME_HEADER_SIZE];
	char dataI[FRAME_LEN];
	char dataQ[FRAME_LEN];
	char dataUnsign[FRAME_LEN];
	char image256[256];
	bool isToFFT;
} dataBuff[MAX_IREC];

#define OUTPUT_FRAME_SIZE FRAME_LEN*2+FRAME_HEADER_SIZE

u_char outputFrame[OUTPUT_FRAME_SIZE];

int iProcessing=0,iReady = 50;
void packet_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data);
void pcapRun();


int mSocket;
struct sockaddr_in si_peter;
struct sockaddr_in si_capin;
void socketInit()
{
	WSADATA wsa;
	//Initialise winsock
	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	printf("Initialised.\n");
	//init socket for UDP connect to Peter
	mSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	
	if (mSocket == SOCKET_ERROR)
	{
		printf("socket() failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
			
	memset((char *)&si_capin, 0, sizeof(si_capin));
	si_capin.sin_family = AF_INET;
	si_capin.sin_port = htons(34566);//port "127.0.0.1"
	si_capin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	int ret = bind(mSocket, (struct sockaddr *)&si_capin, sizeof(struct sockaddr));
	if (ret==-1)
	{
		printf("Port busy");
		exit(EXIT_FAILURE);
	}
	//setup address structure
	memset((char *)&si_peter, 0, sizeof(si_peter));
	si_peter.sin_family = AF_INET;
	si_peter.sin_port = htons(31000);//port "127.0.0.1"
	si_peter.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

}
void socketDelete()
{
	closesocket(mSocket);
	WSACleanup();
}

DWORD WINAPI ProcessDataBuffer(LPVOID lpParam);
DWORD WINAPI ProcessCommandBuffer(LPVOID lpParam);
void StartProcessing()
{
	CreateThread(
		NULL,                   // default security attributes
		0,                      // use default stack size  
		ProcessDataBuffer,       // thread function name
		NULL,          // argument to thread function 
		0,                      // use default creation flags 
		NULL);   // returns the thread identifier 
	CreateThread(
		NULL,                   // default security attributes
		0,                      // use default stack size  
		ProcessCommandBuffer,       // thread function name
		NULL,          // argument to thread function 
		0,                      // use default creation flags 
		NULL);   // returns the thread identifier 

}


FILE* pFile;
int main()
{

	/* start the capture */
	socketInit();

	StartProcessing();

	char fileName[50];
	time_t times = time(NULL);
	int strlen = sprintf(fileName, "raw_data_record_%d.dat", times);
	pFile = fopen(fileName, "wb");
	if (!pFile)
	{
		printf("\n Error creating .dat file");
	}
	else
	{
		printf("\nRecord start, file name:");
		printf(fileName);
	}
	
	pcapRun();
	fclose(pFile);
    return 0;
}
//precompiling code for FFT

// Complex data type


void pcapRun()
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
		printf(errbuf); return;
	}
	//isRunning = true;
	int i = 0;
	/* Print the list */
	for (d = alldevs; d; d = d->next)
	{
		printf("%d. %s", ++i, d->name);
		if (d->description)
			printf(" (%s)\n", d->description);
		else
			printf(" (No description available)\n");
	}
	d = alldevs;
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
		return;
	}
	printf("\nlistening on %s...\n", d->description);
	pcap_loop(adhandle, 0, packet_handler, NULL);
}
u_char dataOut[FRAME_LEN];
long int nFrames = 0;

float2 ramSignalTL[FRAME_LEN][FFT_SIZE];
float2 ramSignalNen[MAX_IREC][FRAME_LEN];
float2 ramImage[FRAME_LEN];
char recvDatagram[1000];
DWORD WINAPI ProcessCommandBuffer(LPVOID lpParam)
{
	while (false)
	{
		int PeterAddrSize = sizeof (si_peter);
		int iResult = recvfrom(mSocket, recvDatagram, 1000, 0, (struct sockaddr *) &si_peter, &PeterAddrSize);
		if (iResult == SOCKET_ERROR) {
			wprintf(L"recvfrom failed with error %d\n", WSAGetLastError());
		}
	}
	return 0;
}
/*
int datatestI[MAX_IREC];
int datatestQ[MAX_IREC];
int datatestA[MAX_IREC];*/

DWORD WINAPI ProcessDataBuffer(LPVOID lpParam)
{
	
	
	while (true)
	{
		Sleep(1);
		
		
		while (iProcessing!= iReady )
		{
			
			for (int ir = 0; ir < FRAME_LEN; ir++)
			{
				//ramSignalNen[iProcessing][ir].x = sqrt(double(dataBuff[iProcessing].dataI[ir])*(dataBuff[iProcessing].dataI[ir]) + double(dataBuff[iProcessing].dataQ[ir])*(dataBuff[iProcessing].dataQ[ir]));
				/*if (ir == 260)
				{
					datatestI[iProcessing] = float(dataBuff[iProcessing].dataI[264]);
					datatestQ[iProcessing] = float(dataBuff[iProcessing].dataQ[264]);
					datatestA[iProcessing] = sqrt(double(dataBuff[iProcessing].dataI[264] * dataBuff[iProcessing].dataI[264] + dataBuff[iProcessing].dataQ[264] * dataBuff[iProcessing].dataQ[264]));
					//ramSignalNen[iProcessing][ir].x = int(dataBuff[iProcessing].dataI[264]);
					//ramSignalNen[iProcessing][ir].y = int(dataBuff[iProcessing].dataQ[264]);
				}*/
				//ramSignalNen[iProcessing][ir].x = sqrt(double(dataBuff[iProcessing].dataI[ir] * dataBuff[iProcessing].dataI[ir] + dataBuff[iProcessing].dataQ[ir] * dataBuff[iProcessing].dataQ[ir]));//int(dataBuff[iProcessing].dataI[ir]);
				ramSignalNen[iProcessing][ir].x = float(dataBuff[iProcessing].dataI[ir]);
				ramSignalNen[iProcessing][ir].y = float(dataBuff[iProcessing].dataQ[ir]);//0;// 
				//ramSignalNen[iProcessing][ir].y = 0;
			}
			

			/*if (!dataBuff[iProcessing].isToFFT)
			{
				//jump to next period
				iProcessing++;
				if (iProcessing >= MAX_IREC)iProcessing = 0;
				continue;
			}*/
			int ia;
			for (int ir = 0; ir < FRAME_LEN; ir++)
			{
				ia = iProcessing;
				for (int i = 0; i < FFT_SIZE; i++)
				{
					ramSignalTL[ir][i] = ramSignalNen[ia][ir];
					ia--;
					if (ia < 0)ia += MAX_IREC;
				}
			}
			
			
			memcpy(outputFrame, dataBuff[iProcessing].header, FRAME_HEADER_SIZE);
			if(isUnsigned)
			{
				memcpy(outputFrame + FRAME_HEADER_SIZE, dataBuff[iProcessing].dataUnsign, FRAME_LEN);
			}
			else 
			for (int i = 0; i < FRAME_LEN; i++)
			{
				int maxAmp = 0, indexMaxFFT = 0;
				//for (int j = 0; j<FFT_SIZE; j++)
				int fftSkip = BANG_KHONG*FFT_SIZE / 16.0;
				for (int j = fftSkip; j<FFT_SIZE - fftSkip; j++)
				{
					int ampl = (ramSignalTL[i][j].x * ramSignalTL[i][j].x) + (ramSignalTL[i][j].y * ramSignalTL[i][j].y);
					if (ampl>maxAmp)
					{
						maxAmp = ampl;
						indexMaxFFT = j;
					}
				}
				/*if (i == 264)
				{
					datatestA[iProcessing] = u_char(sqrt(float(maxAmp)) / float(FFT_SIZE));//int(sqrt(float(maxAmp/16.0)));
				}*/
				//outputFrame[i + FRAME_HEADER_SIZE] = u_char(sqrt(float(maxAmp)));
				/*if (i%2)
				outputFrame[i + FRAME_HEADER_SIZE] = u_char(datatestI[i] + 60);
				else
				outputFrame[i + FRAME_HEADER_SIZE] = u_char(datatestQ[i]+60);//u_char(sqrt(float(maxAmp)) / float(1 ));*/
				int res = sqrt(float(maxAmp)) / float(FFT_SIZE);
				if (res > 255)res = 255;
				outputFrame[i + FRAME_HEADER_SIZE] = res;// u_char(sqrt(float(maxAmp)) / float(FFT_SIZE));
				outputFrame[i + FRAME_LEN + FRAME_HEADER_SIZE] = u_char(indexMaxFFT*16.0 / (FFT_SIZE));
			}
			sendto(mSocket, (char*)outputFrame, OUTPUT_FRAME_SIZE, 0, (struct sockaddr *) &si_peter, sizeof(si_peter));
			
			//jump to next period
			iProcessing++;
			if (iProcessing >= MAX_IREC)iProcessing = 0;
			
		}
	}
	


}

#define UDP_HEADER_LEN 42
void packet_handler(u_char *param, const struct pcap_pkthdr *pkt_header, const u_char *pkt_data)
{
	//    struct tm ltime;
	//    char timestr[16];
	//    time_t local_tv_sec;

	/*
	* unused variables
	*/
	//    (VOID)(param);
	//    (VOID)(pkt_data);

	/* convert the timestamp to readable format */
	//    local_tv_sec = header->ts.tv_sec;
	//    localtime_s(&ltime, &local_tv_sec);
	//    strftime( timestr, sizeof timestr, "%H:%M:%S", &ltime);

	//if (pkt_header->len<1000)return;
	
	//pkt_header->
	/*if (((*(pkt_data + 36) << 8) | (*(pkt_data + 37))) != 5000)
	{
		//printf("\nport:%d",((*(pkt_data+36)<<8)|(*(pkt_data+37))));
		return;
	}*/
	//int iNext = iReady + 1;
	//if (iNext >= MAX_IREC)iNext = 0;
	
	u_char* data = (u_char*)pkt_data + UDP_HEADER_LEN;
	unsigned char len1 = (pkt_header->len)>>8;
	unsigned char len2 = (pkt_header->len);
	fwrite(&len1, 1,1 , pFile);
	fwrite(&len2, 1, 1, pFile);
	fwrite(data, 1,pkt_header->len , pFile);
	if (ftell(pFile) > 100000000)
	{
		fclose(pFile);
		char fileName[50];
		time_t times = time(NULL);
		int strlen = sprintf(fileName, "raw_data_record_%d.dat", times);
		pFile = fopen(fileName, "wb");
		if (!pFile)
		{
			printf("\n Error creating .dat file");
		}
		else
		{
			printf("\nRecord start, file name:");
			printf(fileName);
		}
	}
	/*if (data[0] == 1)		//I chanel first part
	{
		isUnsigned = false;
		//dataBuff[iNext].isToFFT = ((iNext%mFFTSkip) == 0);
		memcpy(dataBuff[iNext].header, data, FRAME_HEADER_SIZE);
		memcpy(dataBuff[iNext].dataI, data + FRAME_HEADER_SIZE, 1024);
	}
	else if (data[0] == 2) //Q chanel first part
	{
		memcpy(dataBuff[iNext].dataQ, data + FRAME_HEADER_SIZE, 1024);
		iReady++;
		if (iReady >= MAX_IREC)iReady = 0;
	}
	else if (data[0] == 3) //tin hieu gia lien tuc
	{
		isUnsigned = true;
		memcpy(dataBuff[iNext].header, data, FRAME_HEADER_SIZE);
		//memcpy(dataBuff[iNext].dataI, data + FRAME_HEADER_SIZE, 1024);
		//memset(dataBuff[iNext].dataQ, 0, 1024);
		memcpy(dataBuff[iNext].dataUnsign, data + FRAME_HEADER_SIZE, FRAME_LEN);
		iReady++;
		if (iReady >= MAX_IREC)iReady = 0;
	}
	return;*/
}
