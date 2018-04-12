//setx -m OPENCV_DIR D:\OpenCV\OpenCV331\opencv\build
//setx path "%path%;D:\OpenCV\OpenCV331\opencv\build\bin\Release\"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
//#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include <conio.h>
#include <tchar.h>
// includes for FFT
#include <cuda_runtime.h>
#include <cufft.h>
#include <cufftXt.h>
#define FFT_SIZE 16
//#define 
#define FRAME_LEN 2048
#pragma comment(lib, "user32.lib")
#pragma comment (lib, "Ws2_32.lib")
//file mapping
#define BUF_SIZE 256
#define FRAME_HEADER_SIZE 34
TCHAR szName[] = TEXT("Global\\RadarData");
#define HAVE_REMOTE// for pcap
#include "pcap.h"
//using namespace cv;
using namespace std;
struct DataFrame// buffer for data frame
{
	char header[FRAME_HEADER_SIZE];
	char dataI[FRAME_LEN];
	char dataQ[FRAME_LEN];
};
#define MAX_IREC 10000
#define OUTPUT_FRAME_SIZE FRAME_LEN*2+FRAME_HEADER_SIZE
DataFrame dataBuff[MAX_IREC];
u_char outputFrame[OUTPUT_FRAME_SIZE];
cufftHandle plan;
int iRead=0,iRec = 1;
void packet_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data);
void pcapRun();
cufftComplex *d_signal;
int mem_size = sizeof(cufftComplex)* FFT_SIZE*FRAME_LEN;
int mSocket;
struct sockaddr_in si_other;
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
	//create socket
	mSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (mSocket == SOCKET_ERROR)
	{
		printf("socket() failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	//setup address structure
	memset((char *)&si_other, 0, sizeof(si_other));
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(34567);//port "127.0.0.1"
	si_other.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

}
void socketDelete()
{
	closesocket(mSocket);
	WSACleanup();
}
void cufftInit()
{
	if (cufftPlan1d(&plan, FFT_SIZE, CUFFT_C2C, FRAME_LEN) != CUFFT_SUCCESS)printf("\nFFT plan failed to init");
	// Allocate device memory for signal
	cudaMalloc((void **)&d_signal, mem_size);
}
void cufftExit()
{
	cufftDestroy(plan);
	// cleanup memory
	cudaFree(d_signal);
}
DWORD WINAPI ProcessDataBuffer(LPVOID lpParam);
void StartProcessing()
{
	CreateThread(
		NULL,                   // default security attributes
		0,                      // use default stack size  
		ProcessDataBuffer,       // thread function name
		NULL,          // argument to thread function 
		0,                      // use default creation flags 
		NULL);   // returns the thread identifier 

}
int main()
{

	/* start the capture */
	socketInit();
	cufftInit();
	StartProcessing();
	pcapRun();
	cufftExit();
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

void CalculateFFT(cufftComplex *h_signal)
{
	cudaMemcpy(d_signal, h_signal, mem_size, cudaMemcpyHostToDevice);
	cufftExecC2C(plan, d_signal, d_signal, CUFFT_FORWARD);
	cudaMemcpy(h_signal, d_signal, mem_size, cudaMemcpyDeviceToHost);
}
long int nFrames = 0;

u_char dataOut[FRAME_LEN];
DWORD WINAPI ProcessDataBuffer(LPVOID lpParam)
{
	//int oldAzi;
	while (true)
	{
		Sleep(2);
		int iCur = (iRec>>2) - 1;
		if (iCur < 0)iCur += MAX_IREC>>2;
		
		while ((iRead>>2) != iCur )
		{
			iRead+=4;
			
			if (iRead >= MAX_IREC)iRead -= MAX_IREC;
			nFrames++;
			cufftComplex h_signal[FRAME_LEN][FFT_SIZE];// (cufftComplex *)malloc(sizeof(cufftComplex)* FFT_SIZE);
			for (int ir = 0; ir < FRAME_LEN; ir++)
			{
				int ia = iRead;
				
				for (int i = 0; i < FFT_SIZE; i++)
				{
					h_signal[ir][i].x = dataBuff[ia].dataI[ir];
					h_signal[ir][i].y = dataBuff[ia].dataQ[ir];
					ia--;
					if (ia < 0)ia += MAX_IREC;
				}
			}
			
			
			/*if ((dataBuff[iRead].azi != oldAzi + 1) && (oldAzi!=2047))
			{
				printf("\nAzi:%d Count:%d", dataBuff[iRead].azi, nFrames);
				printf("Azi old:%d", oldAzi);
			}*/
			
			/*oldAzi = dataBuff[iRead].azi;
			*/
			//printf("\nAzi:%d Count:%d", dataBuff[iRead].azi, nFrames);
			int iDisplay = 477;
			if (nFrames < 50)
			{
				
				printf("\nInput I:");
				for (int i = 0; i < FFT_SIZE; i++)
				{
					printf("%3.2f ", h_signal[iDisplay][i].x);
				}
				printf("\nInput Q:");
				for (int i = 0; i < FFT_SIZE; i++)
				{
					printf("%3.2f ", h_signal[iDisplay][i].y);
				}
			}
			// perform fft
			CalculateFFT((cufftComplex*)h_signal);
			memcpy(outputFrame, dataBuff[iRead].header, FRAME_HEADER_SIZE);
			
			for (int i = 0; i < FRAME_LEN; i++)
			{
				if (i == iDisplay)
				{
					i = i;
				}
				int maxAmp = 0, indexMaxFFT = 0;
				for (int j = 0; j<FFT_SIZE; j++)
				{
					int ampl = (h_signal[i][j].x * h_signal[i][j].x) + (h_signal[i][j].y * h_signal[i][j].y);
					if (ampl>maxAmp)
					{
						maxAmp = ampl;
						indexMaxFFT = j;
					}
				}
				outputFrame[i + FRAME_HEADER_SIZE] = u_char(sqrt(float(maxAmp)) / float(FFT_SIZE));
				outputFrame[i + FRAME_LEN + FRAME_HEADER_SIZE] = u_char(indexMaxFFT);
			}
			sendto(mSocket, (char*)outputFrame, OUTPUT_FRAME_SIZE, 0, (struct sockaddr *) &si_other, sizeof(si_other));
			//printf(" max FFT:%d",indexMaxFFT);
			
			if (nFrames < 50)
			{
				
				printf("\nFFT I:");
				for (int i = 0; i < FFT_SIZE; i++)
				{
					printf("%3.2f ", h_signal[iDisplay][i].x);
				}
				printf("\nFFT Q:");
				for (int i = 0; i < FFT_SIZE; i++)
				{
					printf("%3.2f ", h_signal[iDisplay][i].y);
				}
			}
			
			
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

	
	if (pkt_header->len<1000)return;
	if (((*(pkt_data + 36) << 8) | (*(pkt_data + 37))) != 5000)
	{
		//printf("\nport:%d",((*(pkt_data+36)<<8)|(*(pkt_data+37))));
		return;
	}
	
	u_char* data = (u_char*)pkt_data + UDP_HEADER_LEN;
	if (data[0] == 0)		//I chanel first part
	{
		iRec++;
		if (iRec >= MAX_IREC)iRec = 0;
		memcpy(dataBuff[iRec].header, data, FRAME_HEADER_SIZE);
		memcpy(dataBuff[iRec].dataI, data + FRAME_HEADER_SIZE, 1024);
	}
	else if (data[0] == 2) //Q chanel first part
	{
		memcpy(dataBuff[iRec].dataQ, data + FRAME_HEADER_SIZE, 1024);
	}
	else if (data[0] == 1) //I chanel second part
	{
		memcpy(dataBuff[iRec].dataI+1024, data + FRAME_HEADER_SIZE, 1024);
	}
	else if (data[0] == 3) //Q chanel second part
	{
		memcpy(dataBuff[iRec].dataQ+1024, data + FRAME_HEADER_SIZE, 1024);
	}
	return;
	//    printf("len:%d\n", header->len);
	//    //printf("%.6d len:%d\n", header->ts.tv_usec, header->len);
	//    for(short i=0;i<dataB[iRec].len;i++)
	//    {
	//        printf("%x-",dataB[iRec].data[i]);
	//    }
	//    printf("\n");

}
