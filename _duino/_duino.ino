#define SERIAL_TX_EN_1 3
#define SERIAL_TX_EN_2 4
#define SERIAL_TX_EN_3 5
#define SERIAL_TX_EN_4 6
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

pinMode(13,OUTPUT);
digitalWrite(13,HIGH);
 for (int i=PD_0;i<=P_EN;i++)
 {
   pinMode(i,OUTPUT);
 }
 for (int i=SERIAL_TX_EN_1;i<=SERIAL_TX_EN_4;i++)
 {
   pinMode(i,OUTPUT);
 }
  digitalWrite(SERIAL_TX_EN_1,LOW);
  digitalWrite(SERIAL_TX_EN_2,LOW);
  digitalWrite(SERIAL_TX_EN_3,HIGH);
  digitalWrite(SERIAL_TX_EN_4,HIGH);
}
void writePD(unsigned char mbyte)
{
  digitalWrite(PCLK,HIGH);
  int nbit=0;
  for (int i=PD_0;i<=PD_7;i++)
  {
    digitalWrite(i,mbyte&(1<<(++nbit)));
  }
  digitalWrite(PCLK,LOW);
}
void loop() {
  // put your main code here, to run repeatedly:
  /*digitalWrite(SERIAL_TX_EN_1,HIGH);
  Serial1.write(100);
  Serial1.flush();
  digitalWrite(SERIAL_TX_EN_1,LOW);
  delay(500);*/
  digitalWrite(P_EN,HIGH);
  writePD(10);
  writePD(20);
  writePD(30);
  writePD(40);
  digitalWrite(P_EN,LOW);
  Serial1.write(1);
  Serial2.write(2);
  Serial3.write(3);
  Serial4.write(4);
  delay(500);
 
  
}
