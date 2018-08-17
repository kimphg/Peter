#include "c_gps_parser.h"

#include <assert.h>
#include <sstream>
#include <stdlib.h>



void CGPSParser::reset()
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
CGPSParser::CGPSParser(std::string sentence){
    reset();
    isDataValid = true;
    if (isValidGGA(sentence))
        setValuesGGA(sentence);
    else if (isValidRMC(sentence))
        setValuesRMC(sentence);
    else isDataValid  =false;
}

// Set values from GGA sentence

// Checks if GGA sentence is valid.
bool CGPSParser::isValidGGA(const string GGASentence){

    vector<std::string> elementVector = splitStringByComma(GGASentence);
    if(!elementVector.size())return false;
    if (elementVector[0] != "$GPGGA")               return false;
    if (elementVector.size() != 15)                 return false;
    if (atoi(elementVector[6].c_str()) == 0)        return false;
    if (elementVector[4].length() < MIN_COORD_LEN)  return false;
    if (elementVector[2].length() < MIN_COORD_LEN)  return false;
    if (atoi(elementVector[7].c_str()) == 0)        return false;

    return true;
}

// Input: GGA NMEA sentence
// Output: set values in class.
void CGPSParser::setValuesGGA(string GGA){
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
bool CGPSParser::isValidRMC(const string RMCSentence){
    vector<std::string> elementVector = splitStringByComma(RMCSentence);
    if(!elementVector.size())return false;
    if (elementVector[0] != "$GPRMC")               return false;
    if (elementVector.size() != 12)                 return false;
    if (elementVector[2] != "A")                    return false;
    if (elementVector[3].length() < MIN_COORD_LEN)  return false;
    if (elementVector[5].length() < MIN_COORD_LEN)  return false;

    return true;

}

void CGPSParser::setValuesRMC(const string RMCSentence){

    vector<std::string> elementVector = splitStringByComma(RMCSentence);
    // Assert we have an RMC  sentence
    assert(elementVector[0] == "$GPRMC");

    this->UTC               = atoi(elementVector[1].c_str());
    this->latitude          = getCoordinates(elementVector[3]);
    if (elementVector[3] == "S") this->latitude  = -this->latitude;
    this->longitude         = getCoordinates(elementVector[5]);
    if (elementVector[5] == "W") this->longitude  = -this->longitude;
    this->speed             = stringToDouble(elementVector[7]);
    this->heading           = stringToDouble(elementVector[8]);
    this->date              = atoi(elementVector[9].c_str());


}

/*-----Auxiliary functions-----*/

// Input: coma separated string
// Output: Vector with all the elements in input.
vector<string> CGPSParser::splitStringByComma(string input){

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
double CGPSParser::stringToDouble(string inputString){

    //If string empty, return 0.
    double returnValue = 0;
    std::istringstream istr(inputString);

    istr >> returnValue;

    return (returnValue);

}
double CGPSParser::getCoordinates(string array){

    double decimalDegrees = 0;
    string degreeArray;
    string minuteArray;
    std::size_t dotPos = array.find('.');
    // Convert input array into two sub arrays containing the degrees and the minutes
    // Check for correct array length
    if (array.length() > MIN_COORD_LEN&&dotPos){

        degreeArray.assign(array.begin(), array.begin()+dotPos-2);
        minuteArray.assign(array.begin()+dotPos-2, array.end());

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
