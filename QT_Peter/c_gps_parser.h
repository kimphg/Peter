#ifndef C_GPS_PARSER_H
#define C_GPS_PARSER_H


#include <iostream>
#include <vector>
#include <string>
#define MINUTE_LENGTH 9
#define private public

using namespace std;

class C_GPS_Parser
{
public:
    C_GPS_Parser();
    C_GPS_Parser(const string GGASentence,const string RMCSentence);
    bool    isDataValid();
    int     UTC;
    double  latitude;
    double  longitude;
    double  altitude;
    double  speed;
    double  heading;
    int     numberSatellites;
    int     date;

private:
    // Set values of each sentence type

    // GGA sentences
    bool isValidGGA(const string GGASentence);
    void setValuesGGA(const string GGASentence);

    // RMC sentence
    bool isValidRMC(const string RMCSentence);
    void setValuesRMC(const string RMCSentence);

    // Auxualiary functions
    vector<string> splitStringByComma(const string);
    double stringToDouble(const string);
    double getCoordinates(string);

};

double degreesToDecimal(const int Degrees,const double Minutes,const int seconds = 0);

#endif // C_GPS_PARSER_H
