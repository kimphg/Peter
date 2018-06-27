//setx -m OPENCV_DIR D:\OpenCV\OpenCV331\opencv\build
//setx path "%path%;D:\OpenCV\OpenCV331\opencv\build\bin\Release\"
#include "device_launch_parameters.h"
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
#define OUTPUT_FRAME_LEN 2048
#define FFT_SIZE 32
#define BANG_KHONG 0
int mFFTSkip = (FFT_SIZE/10);

#define MAX_IREC 2400
#pragma comment(lib, "user32.lib")
#pragma comment (lib, "Ws2_32.lib")
//file mapping
#define FRAME_HEADER_SIZE 34

//using namespace cv;
using namespace std;
//#include "FFTcore.cuh"
// includes for FFT
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <cufft.h>
#include <cufftXt.h>
#include <stdio.h>

__global__ void complexMulKernel(cufftComplex *res, const cufftComplex *v1, const cufftComplex *v2)
{
	int i = threadIdx.x;
	res[i].x = (v1[i].x * v2[i].x + v1[i].y * (v2[i].y)) ;
	res[i].y = (v1[i].x * (-v2[i].y) + v1[i].y * v2[i].x) ;
}
class coreFFT
{
public:


	cufftHandle planTL;
	cufftHandle planNenTH;
	//cufftHandle planImageFFT;
	cufftComplex *dSignalTL;
	cufftComplex *dSignalNenRes;
	cufftComplex *dSignalNen;
	cufftComplex *dImageNen;
	int mMemSizeTL;
	int mMemSizeNen;
	int mMemSizeImage;
	int mTichLuySize;//16
	int mFrameLen;
	coreFFT(int frameLen, int ntichluy)
	{
		cudaError_t cudaStatus;

		// Choose which GPU to run on, change this on a multi-GPU system.
		cudaStatus = cudaSetDevice(0);
		if (cudaStatus != cudaSuccess) {
			fprintf(stderr, "cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?");
			
		}
		mFrameLen = frameLen;
		mTichLuySize = ntichluy;
		mMemSizeTL = sizeof(cufftComplex)* mTichLuySize*frameLen;
		mMemSizeNen = sizeof(cufftComplex)*frameLen;
		mMemSizeImage = sizeof(cufftComplex)*frameLen;


		if (cufftPlan1d(&planTL, mTichLuySize, CUFFT_C2C, frameLen) != CUFFT_SUCCESS)printf("\nFFT planTL failed to init");
		// Allocate device memory for signal tich luy
		cudaMalloc((void **)&dSignalTL, mMemSizeTL);

		if (cufftPlan1d(&planNenTH, frameLen, CUFFT_C2C, 1) != CUFFT_SUCCESS)printf("\nFFT planTL failed to init");
		// Allocate device memory for signal nen
		cudaMalloc((void **)&dSignalNen, mMemSizeNen);
		cudaMalloc((void **)&dSignalNenRes, mMemSizeNen);

		//if (cufftPlan1d(&planImageFFT, frameLen, CUFFT_C2C, 1) != CUFFT_SUCCESS)printf("\nFFT planTL failed to init");
		// Allocate device memory for image nen
		cudaMalloc((void **)&dImageNen, mMemSizeNen);
	}
	void exeFFTTL(cufftComplex *h_signal)
	{
		cudaMemcpy(dSignalTL, h_signal, mMemSizeTL, cudaMemcpyHostToDevice);
		cufftExecC2C(planTL, dSignalTL, dSignalTL, CUFFT_FORWARD);
		
		if (cudaGetLastError() != cudaSuccess) {
			fprintf(stderr, "FFT kernel launch failed: %s\n", cudaGetErrorString(cudaGetLastError()));
			return;
		}
		cudaMemcpy(h_signal, dSignalTL, mMemSizeTL, cudaMemcpyDeviceToHost);
	}
	void exeFFTNen(cufftComplex *h_signal, cufftComplex* h_image)
	{
		//move signal to gpu and process fft forward
		cudaMemcpy(dSignalNen, h_signal, mMemSizeNen, cudaMemcpyHostToDevice);
		cufftExecC2C(planNenTH, dSignalNen, dSignalNen, CUFFT_FORWARD);
		//move image to gpu and process fft forward
		cudaMemcpy(dImageNen, h_image, mMemSizeNen, cudaMemcpyHostToDevice);
		cufftExecC2C(planNenTH, dImageNen, dImageNen, CUFFT_FORWARD);
		// Element wise complext multiplication
		for (int i = 0; i < mFrameLen; i += 1024)
		{
			complexMulKernel << <1, 1024 >> >(dSignalNenRes+i, dSignalNen+i, dImageNen+i);
		}
		
		/*cudaError_t cudaStatus = cudaGetLastError();
		if (cudaStatus != cudaSuccess) {
			fprintf(stderr, "addKernel launch failed: %s\n", cudaGetErrorString(cudaStatus));
			
		}*/

		/*
		cudaMemcpy(h_image, dImageNen, mMemSizeNen, cudaMemcpyDeviceToHost);
		cudaMemcpy(h_signal, dSignalNen, mMemSizeNen, cudaMemcpyDeviceToHost);
		for (int i = 0; i < mFrameLen; i++)
		{
			float x = h_image[i].x * h_signal[i].x + h_image[i].y * h_signal[i].y;
			float y = h_signal[i].x * (-h_image[i].y) + h_signal[i].y * h_image[i].x;
			h_signal[i].x = x/1000000.0;
			h_signal[i].y = y/1000000.0;
		}
		cudaMemcpy(dSignalNenRes, h_signal, mMemSizeNen, cudaMemcpyHostToDevice);
		*/
		cufftExecC2C(planNenTH, dSignalNenRes, dSignalNenRes, CUFFT_FORWARD);
		
		cudaMemcpy(h_signal, dSignalNenRes, mMemSizeNen, cudaMemcpyDeviceToHost);
	}
	~coreFFT()
	{
		cufftDestroy(planTL);
		cufftDestroy(planNenTH);
		// cleanup memory
		cudaFree(dSignalTL);
		cudaFree(dSignalNen);
		cudaFree(dImageNen);
	}
};
//_______________________________________________________________________

struct DataFrame// buffer for data frame
{
	char header[FRAME_HEADER_SIZE];
	char dataI[FRAME_LEN];
	char dataQ[FRAME_LEN];
	char image256[256];
	bool isToFFT;
} dataBuff[MAX_IREC];

#define OUTPUT_FRAME_SIZE OUTPUT_FRAME_LEN*2+FRAME_HEADER_SIZE

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
	si_peter.sin_port = htons(34567);//port "127.0.0.1"
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
coreFFT *mFFT;


int main()
{

	/* start the capture */
	socketInit();
	mFFT = new coreFFT(FRAME_LEN, FFT_SIZE);
	StartProcessing();
	pcapRun();
	
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

cufftComplex ramSignalTL[FRAME_LEN][FFT_SIZE];
cufftComplex ramSignalNen[MAX_IREC][FRAME_LEN];
cufftComplex ramImage[FRAME_LEN];
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
	int curAzi = 0;
	
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
			/*
			for (int ir = 0; ir < FRAME_LEN; ir++)
			{
				if (ir < 256)
				{
					ramImage[ir].x = ramSignalNen[iProcessing][ir+500].x;//dataBuff[iProcessing].image256[ir];
				}
				else
				{
					ramImage[ir].x = 0;
				}
				ramImage[ir].y = 0;
			}*/
			//bat dau loc nen anh guong
			//mFFT->exeFFTNen(ramSignalNen[iProcessing], ramImage);
			/*int sum = 0;
			for (int i = 0; i < FRAME_LEN; i++)
			{
				int outx =  int(sqrt((ramSignalNen[iProcessing][i].x)*(ramSignalNen[iProcessing][i].x) + (ramSignalNen[iProcessing][i].y)*(ramSignalNen[iProcessing][i].y)));
				sum += outx;
				if (outx>255)
					outx = 255;
				//bien bo
				outputFrame[i + FRAME_HEADER_SIZE] = outx;// u_char((ramSignalNen[iProcessing][i].x)*(ramSignalNen[iProcessing][i].x) + (ramSignalNen[iProcessing][i].y)*(ramSignalNen[iProcessing][i].y));
				//kenh dopler
				outputFrame[i + FRAME_LEN + FRAME_HEADER_SIZE] = 0;
			}
			if (sum / 2048.0 > 200)
			{
				sum = sum;
			}*/
			//memcpy((char*)outputFrame + FRAME_HEADER_SIZE, ramSignalNen[iProcessing],1024);
			//memcpy((char*)outputFrame + FRAME_HEADER_SIZE + 1024, ramSignalNen[iProcessing]+1024, 1024);
			//memcpy(outputFrame, dataBuff[iProcessing].header, FRAME_HEADER_SIZE);
			//sendto(mSocket, (char*)outputFrame, OUTPUT_FRAME_SIZE, 0, (struct sockaddr *) &si_peter, sizeof(si_peter));
			//
			//continue;
			//tich luy fft
			//if (!dataBuff[iProcessing].isToFFT)continue;

			//if (iProcessing >= MAX_IREC)iProcessing -= MAX_IREC;
			//nFrames++;

			if (!dataBuff[iProcessing].isToFFT)
			{
				//jump to next period
				iProcessing++;
				if (iProcessing >= MAX_IREC)iProcessing = 0;
				continue;
			}
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
			
			/*if ((dataBuff[iProcessing].azi != oldAzi + 1) && (oldAzi!=2047))
			{
				printf("\nAzi:%d Count:%d", dataBuff[iProcessing].azi, nFrames);
				printf("Azi old:%d", oldAzi);
			}*/
			
			/*oldAzi = dataBuff[iProcessing].azi;
			*/
			//printf("\nAzi:%d Count:%d", dataBuff[iProcessing].azi, nFrames);
			
			// perform fft

			mFFT->exeFFTTL((cufftComplex*)ramSignalTL);
			/*dataBuff[iProcessing].header[2] = curAzi >> 8;
			dataBuff[iProcessing].header[3] = curAzi ;
			curAzi++;
			if (curAzi >= 4096)curAzi = 0;*/
			memcpy(outputFrame, dataBuff[iProcessing].header, FRAME_HEADER_SIZE);
			
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
				int res = sqrt(float(maxAmp) / float(FFT_SIZE));
				if (res > 255)res = 255;
				outputFrame[i + FRAME_HEADER_SIZE] = res;// u_char(sqrt(float(maxAmp)) / float(FFT_SIZE));
				outputFrame[i + OUTPUT_FRAME_LEN + FRAME_HEADER_SIZE] = u_char(indexMaxFFT*16.0 / (FFT_SIZE));
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

	if (pkt_header->len<1000)return;
	//pkt_header->
	if (((*(pkt_data + 36) << 8) | (*(pkt_data + 37))) != 5000)
	{
		//printf("\nport:%d",((*(pkt_data+36)<<8)|(*(pkt_data+37))));
		return;
	}
	int iNext = iReady + 1;
	if (iNext >= MAX_IREC)iNext = 0;
	u_char* data = (u_char*)pkt_data + UDP_HEADER_LEN;
	if (data[0] == 1)		//I chanel first part
	{
		dataBuff[iNext].isToFFT = ((iNext%mFFTSkip) == 0);
		memcpy(dataBuff[iNext].header, data, FRAME_HEADER_SIZE);
		memcpy(dataBuff[iNext].dataI, data + FRAME_HEADER_SIZE, 1024);
	}
	else if (data[0] == 2) //Q chanel first part
	{
		memcpy(dataBuff[iNext].dataQ, data + FRAME_HEADER_SIZE, 1024);
		iReady++;
		if (iReady >= MAX_IREC)iReady = 0;
	}
	else if (data[0] == 1) //I chanel second part
	{
		memcpy(dataBuff[iNext].dataI + 1024, data + FRAME_HEADER_SIZE, 1024);
	}
	else if (data[0] == 3) //Q chanel second part
	{
		memcpy(dataBuff[iNext].dataQ + 1024, data + FRAME_HEADER_SIZE, 1024);
		iReady++;
		if (iReady >= MAX_IREC)iReady = 0;
	}
	return;
}
void packet_handler_compress(u_char *param, const struct pcap_pkthdr *pkt_header, const u_char *pkt_data)
{
	if (pkt_header->len<1000)return;
	if (((*(pkt_data + 36) << 8) | (*(pkt_data + 37))) != 5000)
	{
		return;
	}
	u_char* data = (u_char*)pkt_data + UDP_HEADER_LEN;
	if (data[0] == 0)		//I chanel first part
	{
		iReady++;
		if (iReady >= MAX_IREC)iReady = 0;
		memcpy(dataBuff[iReady].header, data, FRAME_HEADER_SIZE);
		memcpy(dataBuff[iReady].dataI, data + FRAME_HEADER_SIZE, 1024);
	}
	else if (data[0] == 2) //Q chanel first part
	{
		memcpy(dataBuff[iReady].dataQ, data + FRAME_HEADER_SIZE, 1024);
	}
	else if (data[0] == 1) //I chanel second part
	{
		memcpy(dataBuff[iReady].dataI + 1024, data + FRAME_HEADER_SIZE, 1024);
	}
	else if (data[0] == 3) //Q chanel second part
	{
		memcpy(dataBuff[iReady].dataQ + 1024, data + FRAME_HEADER_SIZE, 1024);
	}
	return;
}