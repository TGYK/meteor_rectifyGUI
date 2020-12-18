//============================================================================
// Name        : filemanager.h
// Author      : TGYK
// Date        : 12/14/2020
// E-Mail      : tgyk@tgyk.net
// Description : This is the class definition of FileManager
//============================================================================

#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include <iostream>
#include <QImage>
#include <QString>

using namespace std;

class FileManager{
private:
    string inputFilePath = "";
    string inputFileName = "";
    string outputFilePath = "";
    string outputFileName = "";
    QImage image;
    QImage rectifiedImage;
    void setInputFileName(const string *filePath);
    void setOutputFileName(const string *filePath);
public:
    FileManager();
    FileManager(const string *inputFilePath);
    FileManager(const string *inputFilePath, const string *outputFilePath);
    void setInputFilePath(const string *filePath);
    void setOutputFilePath(const string *filePath);
    void open();
    void save();
    const string &getInputFileName() const;
    const string &getInputFilePath() const;
    const string &getOutputFileName() const;
    const string &getOutputFilePath() const;
    const QImage *getImagePtr() const;
    QImage *getRectImagePtr();
};

#endif // FILEMANAGER_H
