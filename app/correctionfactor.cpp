//============================================================================
// Name        : correctionfactor.cpp
// Author      : TGYK
// Date        : 12/14/2020
// E-Mail      : tgyk@tgyk.net
// Description : This class is responsible for generating a vector of
//               values used to correct the spherical deformation on a pixel-
//               by-pixel basis. The implementation rounds off these values
//               and is not perfect, but it does produce an image pleasing to
//               the eye.
//============================================================================

#include "correctionfactor.h"

long double CorrectionFactor::calcThetaSin(long double thetaCenterAngle) const{
    return atan(earthRadius * sin(thetaCenterAngle) / (satelliteAltitude + earthRadius * (1 - cos(thetaCenterAngle))));
}
long double CorrectionFactor::calcThetaCos(long double thetaSin) const{
    long double delta_sqrt = sqrt(pow(earthRadius, 2) + pow(tan(thetaSin), 2) * (pow(earthRadius, 2) - pow(satelliteOrbitRadius, 2)));
    return acos((pow(tan(thetaSin), 2) * satelliteOrbitRadius + delta_sqrt) / (earthRadius * (pow(tan(thetaSin), 2) + 1)));
}

long double CorrectionFactor::calcCorrectionFactor(long double thetaCenterAngle) const{
    long double norm_factor = earthRadius / satelliteAltitude;
    long double tan_derivative_recip = (1 + pow((earthRadius * sin(thetaCenterAngle) / (satelliteAltitude + earthRadius * (1 - cos(thetaCenterAngle)))), 2));
    long double arg_derivative_recip = (pow((satelliteAltitude + earthRadius * (1 - cos(thetaCenterAngle))), 2) / (earthRadius * cos(thetaCenterAngle) * (satelliteAltitude + earthRadius * (1 - cos(thetaCenterAngle))) - pow(earthRadius, 2) * pow(sin(thetaCenterAngle), 2)));
    return norm_factor * tan_derivative_recip * arg_derivative_recip;
}

long double CorrectionFactor::calcThetaCenter(int imgWidth, int imgColumn) const{
    long double theta_sin = calcThetaSin(thetaCenter / 2.0) * (abs(imgColumn - imgWidth / 2.0) / (imgWidth / 2.0));
    return calcThetaCos(theta_sin);
}

void CorrectionFactor::calcRectifiedWidth(){
    this->rectifiedWidth = ceil(accumulate(this->correctionFactors.begin(), this->correctionFactors.end(), 0.0));
}

void CorrectionFactor::calcCorrectionVector(){
    correctionFactors.clear();
    for(int imgColumn = 0; imgColumn <= this->imageWidth; imgColumn++){
        correctionFactors.push_back(calcCorrectionFactor(this->calcThetaCenter(this->imageWidth, imgColumn)));
    }
}

CorrectionFactor::CorrectionFactor(int imgWidth){
    this->imageWidth = imgWidth;
    calcCorrectionVector();
    calcRectifiedWidth();
}

void CorrectionFactor::setImageWidth(int imgWidth){
    if(this->imageWidth != imgWidth){
        this->imageWidth = imgWidth;
        calcCorrectionVector();
        calcRectifiedWidth();
    }
}

void CorrectionFactor::setEarthRadius(double earthRadius){
    this->earthRadius = earthRadius;
    this->calcCorrectionVector();
    this->calcRectifiedWidth();
}

void CorrectionFactor::setSatelliteAltitude(double satelliteAltitude){
    this->satelliteAltitude = satelliteAltitude;
    this->calcCorrectionVector();
    this->calcRectifiedWidth();
}

void CorrectionFactor::setSatelliteSwath(int satelliteSwath){
    this->satelliteSwath = satelliteSwath;
    this->thetaCenter = satelliteSwath / earthRadius;
    this->calcCorrectionVector();
    this->calcRectifiedWidth();
}

int CorrectionFactor::getRectifiedWidth(){
    return this->rectifiedWidth;
}

double CorrectionFactor::getEarthRadius() const{
    return this->earthRadius;
}

double CorrectionFactor::getDefaultEarthRadius() const{
    return this->defaultEarthRadius;
}

double CorrectionFactor::getSatelliteAltitude() const{
    return this->satelliteAltitude;
}

double CorrectionFactor::getDefaultSatelliteAltitude() const{
    return this->defaultSatelliteAltitude;
}

int CorrectionFactor::getSatelliteSwath() const{
    return this->satelliteSwath;
}

int CorrectionFactor::getDefaultSatelliteSwath() const{
    return this->defaultSatelliteSwath;
}

vector<long double> CorrectionFactor::getVector(){
    return this->correctionFactors;
}
