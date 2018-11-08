
#include <SoftwareSerial.h>
#include <avr/pgmspace.h>

/*
 * Note:
 Not all pins on the Mega and Mega 2560 support change interrupts,
 so only the following can be used for RX:
 10, 11, 12, 13, 50, 51, 52, 53, 62, 63, 64, 65, 66, 67, 68, 69
 */
#include "logData.h"
SoftwareSerial softSerial1(50,8); // RX, TX
SoftwareSerial softSerial2(51,11); // RX, TX
SoftwareSerial softSerial3(52,10); // RX, TX
SoftwareSerial softSerial4(53,9); // RX, TX
extern const char logString[] PROGMEM;
char strBuffer[1001];
int buffIndex=0;
int nBuffIndex = 0;
char* refAddress ;
inline void loadBuffer()
{
    buffIndex=0;
    memcpy_P(&strBuffer[0], refAddress + nBuffIndex*1000,1000);
    nBuffIndex++;
    Serial.print("\nnew strBuffer:");
    Serial.println(nBuffIndex);
}
char gyroFrame[35];
long int gyroHeading;
void setup() {
    // initialize both serial ports:
    strBuffer[1000]=0;
    Serial.begin(115200);
    Serial1.begin(115200);
    softSerial1.begin(19200);
    softSerial2.begin(19200);
    softSerial3.begin(19200);
    softSerial4.begin(19200);
    Serial.print("Start:");
    refAddress = (char*)(pgm_read_word(&(logString[0])));
    refAddress-= 8952;
    nBuffIndex = 0;
    buffIndex = 0;
    strBuffer[1000]=0;
    loadBuffer();
    gyroHeading = 5000;
    gyroFrame[0]=0x5a;
    gyroFrame[1]=0xa5;
    gyroFrame[2]=0x1a;
    gyroFrame[3]=0x01;
    gyroFrame[4]=0x00;
    gyroFrame[5]=0x00;
    gyroFrame[6]=gyroHeading>>8;
    gyroFrame[7]=gyroHeading;
    gyroFrame[8]=0;
    gyroFrame[9]=0;
    gyroFrame[10]=0;
    gyroFrame[11]=0;
    gyroFrame[12]=0;
    gyroFrame[13]=0;
    gyroFrame[14]=0;
    gyroFrame[15]=0;
    gyroFrame[16]=0;
    gyroFrame[17]=0;
    gyroFrame[18]=0;
    gyroFrame[19]=200;
    gyroFrame[20]=0;
    gyroFrame[21]=100;
    gyroFrame[22]=0;
    gyroFrame[23]=0;
    gyroFrame[24]=0;
    gyroFrame[25]=0;
    gyroFrame[26]=0;
    gyroFrame[27]=0;
    gyroFrame[28]=0;
    gyroFrame[29]=0;
    gyroFrame[30]=0xab;
    gyroFrame[31]=0xaa;
    gyroFrame[32]=0x00;
    gyroFrame[33]=0x00;
    gyroFrame[34]=0x00;
    gyroFrame[35]=0x00;
}
void forwardSerial0Serial1()
{
    // read from port 1, send to port 0:
    if (Serial1.available()) {
        int inByte = Serial1.read();
        Serial.write(inByte);
    }
    // read from port 0, send to port 1:
    if (Serial.available()) {
        int inByte = Serial.read();
        Serial1.write(inByte);
    }
}
bool isStop = false;
void sendData()
{
    //Serial.print(strPointer);
    if(strBuffer[buffIndex]=='$')
    {
        if(strBuffer[buffIndex+1]=='G'&&strBuffer[buffIndex+2]=='P')//GPS message
        {
            while(strBuffer[buffIndex]!='\n')
            {
                if(strBuffer[buffIndex]==0)isStop = true;
                softSerial1.write(strBuffer[buffIndex]);
                buffIndex++;
                if(buffIndex>=1000)loadBuffer();
            };
            softSerial1.write('\r');
            softSerial1.write('\n');
            buffIndex++;
            if(buffIndex>=1000)loadBuffer();
        }
        else if(strBuffer[buffIndex+1]=='V')//Speed message
        {
            while(strBuffer[buffIndex]!='\n')
            {
                if(strBuffer[buffIndex]==0)isStop = true;
                softSerial2.write(strBuffer[buffIndex]);
                buffIndex++;
                if(buffIndex>=1000)loadBuffer();
            };
            softSerial2.write('\r');
            softSerial2.write('\n');
            buffIndex++;
            if(buffIndex>=1000)loadBuffer();
        }
    }
    else if(strBuffer[buffIndex]=='!'&&strBuffer[buffIndex+1]=='A'&&strBuffer[buffIndex+2]=='I')
    {
        while(strBuffer[buffIndex]!='\n')
        {
            if(strBuffer[buffIndex]==0)isStop = true;
            softSerial3.write(strBuffer[buffIndex]);
            buffIndex++;
            if(buffIndex>=1000)loadBuffer();
        };
        softSerial3.write('\r');
        softSerial3.write('\n');
        buffIndex++;
        if(buffIndex>=1000)loadBuffer();
    }
    else
    {
        buffIndex++;if(buffIndex>=1000)loadBuffer();
        sendData();
    }

}

void sendGyro()
{
    gyroHeading+=10;
    if(gyroHeading>10000)
        gyroHeading=5000;
    gyroFrame[06]=gyroHeading>>8;
    gyroFrame[07]=gyroHeading;
    Serial1.write(&gyroFrame[0],32);

}
unsigned long previousMillis = 0;
void loop() {
    forwardSerial0Serial1();
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= 200)
    {
        previousMillis = currentMillis;
        if(isStop)
        {
            Serial.print("\nStop at time:");
            Serial.print(currentMillis);
            Serial.print("ms");
            setup();
            isStop = false;
            return;
        }
        sendData();
        sendGyro();
    }
}

