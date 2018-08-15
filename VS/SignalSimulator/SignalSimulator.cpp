// SignalSimulator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#define FRAME_HEADER_SIZE 34
#define FRAME_LEN 1024
#define OUTPUT_FRAME_SIZE FRAME_LEN*2+FRAME_HEADER_SIZE
#define PI_NHAN2                    6.2831853072
#define PI_CHIA2                    1.5707963268
#define PI                       3.14159265358979323846
#define DEG_RAD 57.295779513
#include <winsock2.h>
#include <windows.h>
#include <random>
#pragma comment (lib, "Ws2_32.lib")
int mSocket;
unsigned char n_clk_adc = 0;
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
	si_capin.sin_port = htons(12345);//port "127.0.0.1"
	si_capin.sin_addr.S_un.S_addr = inet_addr("192.168.16.109");
	int ret = bind(mSocket, (struct sockaddr *)&si_capin, sizeof(struct sockaddr));
	if (ret == -1)
	{
		printf("Port busy");
		exit(EXIT_FAILURE);
	}
	//setup address structure
	memset((char *)&si_peter, 0, sizeof(si_peter));
	si_peter.sin_family = AF_INET;
	si_peter.sin_port = htons(5000);//port "127.0.0.1"
	si_peter.sin_addr.S_un.S_addr = inet_addr("27.72.56.161");

}
void socketDelete()
{
	closesocket(mSocket);
	WSACleanup();
}

u_char outputFrame[4096][OUTPUT_FRAME_SIZE];

std::default_random_engine generator;
std::normal_distribution<double> distribution(30, 8);
void regenerate(int azi)
{
	u_char* dataPointer = &outputFrame[azi][0] + FRAME_HEADER_SIZE;
	for (int i = 0; i < 1024; i++)
	{
		int num = int(distribution(generator));
		if (num < 0)num = 0;
		dataPointer[i] = num;
		
	}
	for (int i = 1024; i < 2018; i++)
	{
		
		dataPointer[i] = rand()%16;

	}
}
double ConvXYToRange(double x, double y)
{
	double range;
	if (!y)
	{
		range = abs(x);
	}
	else
	{
		//*azi = atanf(x / y);
		//if (y<0)*azi += PI;
		//if (*azi<0)*azi += PI_NHAN2;
		range = sqrt(x*x + y*y);
		//*azi = *azi*DEG_RAD;
	}
	return range;

}
double ConvXYToAziRad(double x, double y)
{
	double azi;
	if (!y)
	{
		azi = x>0 ? PI_CHIA2 : (PI_NHAN2 - PI_CHIA2);
		
	}
	else
	{
		azi = atanf(x / y);
		if (y<0)azi += PI;
		if (azi<0)azi += PI_NHAN2;
	}
	return azi;
}
int targetSize = 10;
class target_t
{
public:
	double speed,x,y,azi,range;
	double bearing;
	int dopler;
	target_t(double tx,double ty,double tspeed,double tbearing,int dople)
	{
		speed = tspeed;
			x = tx;
			y = ty;
		bearing = tbearing/180.0*3.14159265359;
		azi = ConvXYToAziRad(x, y)/3.141592653589*1024.0;
		range = ConvXYToRange(x, y);
		dopler = dople;
	}
	void generateSignal()
	{
		
		azi = ConvXYToAziRad(x, y) / 3.141592653589*1024.0+(rand()%5);
		range = ConvXYToRange(x, y)/pow(2,n_clk_adc);
		int azimin = azi - targetSize; if (azimin < 0)azimin += 2048;
		int azimax = azi + targetSize; if (azimax >= 2048)azimax -= 2048;
		int k = 0;
		for (int a = azimin; a != azimax; a++)
		{
			
			if (a < 0)a += 2048;
			if (a >= 2048) a -= 2048;
			int value = 150 * (1.0 - abs(k - targetSize) / (targetSize+1.0));
			outputFrame[a][(int)range + FRAME_LEN + FRAME_HEADER_SIZE] = dopler;
			outputFrame[a][(int)range + FRAME_LEN + 1 + FRAME_HEADER_SIZE] = dopler;
			outputFrame[a][(int)range + FRAME_HEADER_SIZE] = value + int(distribution(generator));
			outputFrame[a][(int)range + 1 + FRAME_HEADER_SIZE] = value + int(distribution(generator));
			k++;
		}
	}
	void eraseSIgnal()
	{
		//azi = ConvXYToAziRad(x, y) / 3.141592653589*1024.0;
		//range = ConvXYToRange(x, y);
		int azimax = azi + targetSize; if (azimax >= 2048)azimax -= 2048;
		int azimin = azi - targetSize; if (azimin < 0)azimin += 2048;
		for (int a = azimin; a != azimax; a++)
		{
			if (a < 0)a += 2048;
			if (a>2048) a -= 2048;
			int num = int(distribution(generator));
			if (num < 0)num = 0;
			outputFrame[a][(int)range + FRAME_HEADER_SIZE] = num;
			outputFrame[a][(int)range + FRAME_LEN + FRAME_HEADER_SIZE] = rand() % 16;
			num = int(distribution(generator));
			if (num < 0)num = 0;
			outputFrame[a][(int)range + 1 + FRAME_HEADER_SIZE] = num;
			outputFrame[a][(int)range + FRAME_LEN + 1 + FRAME_HEADER_SIZE] = rand() % 16;
		}
	}
	void update()
	{
		eraseSIgnal();
		x += speed*sin(bearing);
		y += speed*cos(bearing);
		generateSignal();
	}
	~target_t();
};
#define NUM_OF_TARG 4
target_t* target[NUM_OF_TARG];

void initTargets()
{
	target[0] = new target_t(250, -300, 4, 100,4);
	target[1] = new target_t(50, 50, 4, 180,6);
	target[2] = new target_t(200, -300, 4, 110,8);
	target[3] = new target_t(200, -350, 4, 80,5);
}
void updateTargets()
{
	for (int i = 0; i < NUM_OF_TARG; i++)
	{
		target[i]->update();
	}
}
int _tmain(int argc, _TCHAR* argv[])
{ 
	socketInit();
	int azi = 200;
	for (int i = 0; i < 2048; i++)
	{
		regenerate(i);
		outputFrame[i][2] = i >> 8;
		outputFrame[i][3] = i;
	}
	initTargets();
	int dazi = 1;
	int nPeriod = 0;
	while (true)
	{
		
		
		Sleep(3);
		azi += dazi;
		if (azi >= 2048)
		{
			nPeriod++;
			if (nPeriod > 50)
			{
				nPeriod = 0;
				initTargets();
			}
			if (nPeriod == 5)
			{
				n_clk_adc = 1;
			}
			azi = 0;
			updateTargets();
		}
		/*
		if (azi>= 900)
		{
			nPeriod++;
			dazi = -1;
			updateTargets();
			Sleep(500);
		}
		else
		if (azi <= 50)
		{
			dazi = 1;
			updateTargets();
			Sleep(500);
		}*/
		//if (rand() % 10 == 0)regenerate(azi);
		outputFrame[azi][4] = n_clk_adc;
		outputFrame[azi][0] = 1;
		sendto(mSocket, (char*)(&outputFrame[azi][0]), OUTPUT_FRAME_SIZE, 0, (struct sockaddr *) &si_peter, sizeof(si_peter));
		outputFrame[azi][0] = 2;
		sendto(mSocket, (char*)(&outputFrame[azi][0]), OUTPUT_FRAME_SIZE, 0, (struct sockaddr *) &si_peter, sizeof(si_peter));
		regenerate(azi);
	}
	socketDelete();
	return 0;
}

