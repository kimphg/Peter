#define SERIAL_TX_EN_1 3
#define SERIAL_TX_EN_2 4
#define SERIAL_TX_EN_3 5
#define SERIAL_TX_EN_4 6
#define CLOCK_SSI 29
#define DATA_SSI 30
#define BIT_COUNT 28
#define PD_0 14
#define PD_1 15
#define PD_2 16
#define PD_3 17
#define PD_4 18
#define PD_5 19
#define PD_6 20
#define PD_7 21
#define PCLK 22
#define P_EN 23
void setup() {
  // put your setup code here, to run once:
  Serial1.begin(9600);
  Serial2.begin(9600);
  Serial3.begin(9600);
  Serial4.begin(9600);
  Serial.begin(115200);
  pinMode(DATA_SSI, INPUT);
  pinMode(CLOCK_SSI, OUTPUT);
  pinMode(36, OUTPUT);
  pinMode(37, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(36, LOW);
  digitalWrite(37, LOW);
  digitalWrite(13, HIGH);
  for (int i = PD_0; i <= P_EN; i++)
  {
    pinMode(i, OUTPUT);
  }
  for (int i = SERIAL_TX_EN_1; i <= SERIAL_TX_EN_4; i++)
  {
    pinMode(i, OUTPUT);
  }
  digitalWrite(SERIAL_TX_EN_1, HIGH);
  digitalWrite(SERIAL_TX_EN_2, HIGH);
  digitalWrite(SERIAL_TX_EN_3, LOW);
  digitalWrite(SERIAL_TX_EN_4, LOW);
}
void writePD(unsigned char mbyte)
{
  digitalWrite(PCLK, HIGH);
  int nbit = 0;
  for (int i = PD_0; i <= PD_7; i++)
  {
    digitalWrite(i, 0x01 & (mbyte >> nbit));
    nbit++;
  }
  digitalWrite(PCLK, LOW);
}
unsigned long shiftIn(const int data_pin, const int clock_pin, const int bit_count) {
  unsigned long data = 0;

  for (int i=0; i<bit_count; i++) {
    data <<= 1;
    digitalWrite(clock_pin, LOW);
    digitalWrite(PCLK, LOW);
    delayMicroseconds(4);
    digitalWrite(clock_pin, HIGH);
    digitalWrite(PCLK, HIGH);
    delayMicroseconds(4);
    //data |= digitalRead(data_pin);
    digitalWrite(P_EN,digitalRead(data_pin));
  }
  digitalWrite(clock_pin, LOW);
  digitalWrite(PCLK, LOW);
  delayMicroseconds(4);
  digitalWrite(clock_pin, HIGH);
  digitalWrite(PCLK, HIGH);
  return data;
}
char encoderFrame[7]={0xAA,0x55,0x6e,0x07,0x00,0x00,0x00};
unsigned char encoderFrameSum = 0xAA^0x55^0x6e^0x07;
void readEncoder()
{
  // Read the same position data twice to check for errors
  //unsigned long sample1 = shiftIn(DATA_SSI, CLOCK_SSI, BIT_COUNT);
  unsigned long sample2 = shiftIn(DATA_SSI, CLOCK_SSI, BIT_COUNT);
  //delayMicroseconds(25);  // Clock mus be high for 20 microseconds before a new sample can be taken
/*
  if (sample1 != sample2) {
    Serial.print("Samples dont match: sample1=");
    Serial.print(sample1);
    Serial.print(", sample2=");
    Serial.println(sample2);
  }*/
   

    encoderFrame[4]=sample2>>8;
    encoderFrame[5]=sample2;
    encoderFrame[6]=encoderFrameSum^encoderFrame[4]^encoderFrame[5];
  //Serial1.write(encoderFrame,7);
  Serial.println(sample2);
  /*digitalWrite(P_EN, HIGH);
  writePD( 0x01);
  writePD( sample2>>8);
  writePD( sample2&0xff);
  digitalWrite(P_EN, LOW);*/
  }

void loop() {
  // put your main code here, to run repeatedly:
  /*digitalWrite(SERIAL_TX_EN_1,HIGH);
    Serial1.write(100);
    Serial1.flush();
    digitalWrite(SERIAL_TX_EN_1,LOW);
    delay(500);*/
  /*digitalWrite(P_EN, HIGH);
  writePD(7);
  //writePD(20);
  //writePD(30);
  //writePD(40);
  delay(1000);
  digitalWrite(13, HIGH);
  digitalWrite(P_EN, LOW);
  Serial1.write(1);
  Serial2.write(2);
  Serial3.write(3);
  Serial4.write(4);
  Serial.write(1);
  delay(1000);
  digitalWrite(13, LOW);*/
  readEncoder();
  delay(1);

}
