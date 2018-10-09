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
	si_capin.sin_port = htons(34565);//port "127.0.0.1"
	si_capin.sin_addr.S_un.S_addr = inet_addr("192.168.0.70");
	int ret = bind(mSocket, (struct sockaddr *)&si_capin, sizeof(struct sockaddr));
	if (ret==-1)
	{
		printf("Port busy");
		exit(EXIT_FAILURE);
	}
	//setup address structure
	memset((char *)&si_peter, 0, sizeof(si_peter));
	si_peter.sin_family = AF_INET;
	si_peter.sin_port = htons(5000);//port "127.0.0.1"
	si_peter.sin_addr.S_un.S_addr = inet_addr("192.168.0.55");

}
void socketDelete()
{
	closesocket(mSocket);
	WSACleanup();
}


FILE* pFile;
char buff[3000];
void ReplayData()
{
	unsigned char len1,len2;
	unsigned long long dataSize = 0;
	while (!feof(pFile))
	{

		fread(&len1, 1, 1, pFile);
		fread(&len2, 1, 1, pFile);
		int len = (len1 << 8) | len2;
		if (len > 3000)
		{
			printf("\nwrong datalen, total data sent:%d", dataSize);
			break;
		}
		fread(buff, 1, len, pFile);
		dataSize += len;
		 sendto(mSocket, buff, len, 0, (struct sockaddr *) &si_peter, sizeof(si_peter));
		Sleep(5);
	}
	printf("\n total data sent:%d", dataSize);
}

int main(int argc, char **argv)
{

	/* start the capture */
	socketInit();

	//StartProcessing();
	if (argc < 2)
	{
		printf("\nNo file selected");
		return;
	}
	char *fileName = argv[1];
	//int strlen = sprintf(fileName, "raw_data_record_%d.dat", times);
	pFile = fopen(fileName, "rb");
	if (!pFile)
	{
		printf("\n Error opening .dat file");
	}
	else
	{
		printf("\nReplay start, file name:");
		printf(fileName);
	}
	
	ReplayData();
	fclose(pFile);
	socketDelete();
    return 0;
}
//precompiling code for FFT

// Complex data type


/*
int datatestI[MAX_IREC];
int datatestQ[MAX_IREC];
int datatestA[MAX_IREC];*/
