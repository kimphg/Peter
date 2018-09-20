#include <stdio.h>
#include <math.h>
#include <time.h>
#include <QtGlobal>
#include <QDateTime>
#define PI 3.14159265358979323846
#define PI_NHAN2  6.2831853072
inline double fastPow(double a, double b) {
  union {
    double d;
    int x[2];
  } u = { a };
  u.x[1] = (int)(b * (u.x[1] - 1072632447) + 1072632447);
  u.x[0] = 0;
  return u.d;
}
inline double sinFast(double a)
{
    while (a>PI) {
        a-=PI_NHAN2;
    }
    double a2 = a*a;
    return a-a2*a/6.0+a2*a2*a/120.0-a2*a2*a2*a/5040.0;
}
int main()
{
    qint64 time_start_ms = QDateTime::currentMSecsSinceEpoch();
    //printf("\nstart:%ld", QDateTime::currentMSecsSinceEpoch()-time_start_ms);
    double res;
    for (int i=0;i<10000000;i++)
    {
        res =fastPow(3.0,-20.0);
        //res = sinFast(PI/2.0);
    }
    printf("\n checkpoint 1:%ld res:%f", QDateTime::currentMSecsSinceEpoch()-time_start_ms,res);
    time_start_ms = QDateTime::currentMSecsSinceEpoch();
    for (int i=0;i<10000000;i++)
    {
        res = pow(3.0,-20.0);
        //res = sin(PI/2.0);
    }
    printf("\n checkpoint 2:%ld res:%f", QDateTime::currentMSecsSinceEpoch()-time_start_ms,res);

}
