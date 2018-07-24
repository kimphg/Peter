#include "c_gps_parser.h"

#include <assert.h>
#include <sstream>
#include <stdlib.h>

C_GPS_Parser::C_GPS_Parser()
{

}

C_GPS_Parser::C_GPS_Parser()
{
    UTC                 = 0;
    latitude            = 0;
    longitude           = 0;
    altitude            = 0;
    numberSatellites    = 0;
    speed               = 0;
    heading             = 0;
    date                = 0;
}

// Constructor using GGA and RMC sentences only. Assigns all data.
C_GPS_Parser::C_GPS_Parser(string sentence){
    C_GPS_Parser();
    isDataValid = true;
    if (isValidGGA(sentence)) setValuesGGA(sentence);
    else if (isValidRMC(sentence)) setValuesRMC(sentence);
    else isDataValid  =false;
}

// Set values from GGA sentence

// Checks if GGA sentence is valid.
bool C_GPS_Parser::isValidGGA(const string GGASentence){
    bool returnBool = true;
    vector<std::string> elementVector = splitStringByComma(GGASentence);

    if (elementVector[0] != "$GPGGA")               returnBool = false;
    if (elementVector.size() != 15)                 returnBool = false;
    if (atoi(elementVector[6].c_str()) == 0)        returnBool = false;
    if (elementVector[4].length() < MINUTE_LENGTH)  returnBool = false;
    if (elementVector[2].length() < MINUTE_LENGTH)  returnBool = false;
    if (atoi(elementVector[7].c_str()) == 0)        returnBool = false;

    return returnBool;
}

// Input: GGA NMEA sentence
// Output: set values in class.
void C_GPS_Parser::setValuesGGA(string GGA){

    vector<std::string> elementVector = splitStringByComma(GGA);
    // Assert we have a GGA sentence
    assert(elementVector[0] == "$GPGGA");

    this->UTC                 = atoi(elementVector[1].c_str());
    this->latitude            = getCoordinates(elementVector[2]);
    if (elementVector[3] == "S") this->latitude  = -this->latitude;
    this->longitude           = getCoordinates(elementVector[4]);
    if (elementVector[5] == "W") this->longitude  = -this->longitude;
    this->altitude            = stringToDouble(elementVector[9]);
    this->numberSatellites    = atoi(elementVector[7].c_str());
}


// RMC sentence

// Check if RMC sentence is valid with NMEA standard.
bool C_GPS_Parser::isValidRMC(const string RMCSentence){
    bool returnBool = true;
    vector<std::string> elementVector = splitStringByComma(RMCSentence);

    if (elementVector[0] != "$GPRMC")               returnBool = false;
    if (elementVector.size() != 12)                 returnBool = false;
    if (elementVector[2] != "A")                    returnBool = false;
    if (elementVector[3].length() < MINUTE_LENGTH)  returnBool = false;
    if (elementVector[5].length() < MINUTE_LENGTH)  returnBool = false;

    return returnBool;

}

void C_GPS_Parser::setValuesRMC(const string RMCSentence){

    vector<std::string> elementVector = splitStringByComma(RMCSentence);
    // Assert we have an RMC  sentence
    assert(elementVector[0] == "$GPRMC");

    this->UTC               = atoi(elementVector[1].c_str());
    this->latitude          = getCoordinates(elementVector[3]);
    if (elementVector[3] == "S") this->latitude  = -this->latitude;
    this->longitude         = getCoordinates(elementVector[5]);
    if (elementVector[5] == "W") this->longitude  = -this->longitude;
    this->speed             = stringToDouble(elementVector[7])* KNOTS_TO_MPS;
    this->heading           = stringToDouble(elementVector[8]);
    this->date              = atoi(elementVector[9].c_str());


}

/*-----Auxiliary functions-----*/

// Input: coma separated string
// Output: Vector with all the elements in input.
vector<string> C_GPS_Parser::splitStringByComma(string input){

    vector<string>  returnVector;
    stringstream    ss(input);
    string          element;

    while(std::getline(ss, element, ',')) {
        returnVector.push_back(element);
    }


    return returnVector;
}
double degreesToDecimal(int degrees, double minutes, int seconds )
{
    double returnDouble = 0;

    returnDouble = degrees + minutes/60 + seconds/3600.0f;

    return returnDouble;
}
double C_GPS_Parser::stringToDouble(string inputString){

    //If string empty, return 0.
    double returnValue = 0;
    std::istringstream istr(inputString);

    istr >> returnValue;

    return (returnValue);

}
double C_GPS_Parser::getCoordinates(string array){

    double decimalDegrees = 0;
    string degreeArray;
    string minuteArray;

    // Convert input array into two sub arrays containing the degrees and the minutes
    // Check for correct array length
    if (array.length() > MINUTE_LENGTH){

        degreeArray.assign(array.begin(), array.end() - MINUTE_LENGTH);
        minuteArray.assign(array.end() - MINUTE_LENGTH, array.end());

        // Convert strings into numbers
        int degrees;
        double minutes;
        degrees = atoi(degreeArray.c_str());
        minutes = stringToDouble(minuteArray);

        // Convert degrees and mintues into decimal
        decimalDegrees = degreesToDecimal(degrees,minutes);

    }
    return decimalDegrees;

}
