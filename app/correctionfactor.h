//============================================================================
// Name        : correctionfactor.h
// Author      : TGYK
// Date        : 12/14/2020
// E-Mail      : tgyk@tgyk.net
// Description : This is the class definition of CorrectionFactor
//============================================================================

#ifndef CORRECTIONFACTOR_H
#define CORRECTIONFACTOR_H
#include <math.h>
#include <numeric>
#include <vector>

using namespace std;

class CorrectionFactor{
private:
    vector<long double> correctionFactors;
    double earthRadius = 6371.0;
    const double defaultEarthRadius = 6371.0;
    double satelliteAltitude = 822.5;
    const double defaultSatelliteAltitude = 822.5;
    const double satelliteOrbitRadius = earthRadius + satelliteAltitude;
    int satelliteSwath = 2800;
    const int defaultSatelliteSwath = 2800;
    double thetaCenter = satelliteSwath / earthRadius;
    int imageWidth;
    int rectifiedWidth;
    long double calcThetaSin(long double thetaCenterAngle) const; //Satellite angle for given center angle
    long double calcThetaCos(long double thetaSin) const; //Inverse of theta Sin
    long double calcCorrectionFactor(long double thetaCenterAngle) const; //Calculate the needed correction factor for given center angle
    long double calcThetaCenter(int imgWidth, int imgColumn) const; //Calculate the center angle given the image column and the overall width
    void calcRectifiedWidth();
    void calcCorrectionVector();
public:
    CorrectionFactor(int imgWidth = 1568);
    void setImageWidth(int imgWidth);
    void setEarthRadius(double earthRadius);
    void setSatelliteAltitude(double satelliteAltitude);
    void setSatelliteSwath(int satelliteSwath);
    int getRectifiedWidth();
    double getEarthRadius() const;
    double getDefaultEarthRadius() const;
    double getSatelliteAltitude() const;
    double getDefaultSatelliteAltitude() const;
    int getSatelliteSwath() const;
    int getDefaultSatelliteSwath() const;
    vector<long double> getVector();
};

#endif // CORRECTIONFACTOR_H
