//============================================================================
// Name        : filemanager.cpp
// Author      : TGYK
// Date        : 12/14/2020
// E-Mail      : tgyk@tgyk.net
// Description : This class is responsible for the handling of file paths,
//               names, and files themselves used within the project. It
//               includes a moderate level of error detection and exception
//               handling. It is tailored to fit the use of the project based
//               on the implementation of the accessors.
//============================================================================

#include "filemanager.h"

void FileManager::setInputFileName(const string *filePath){
    //Set input bare file name with error handling
    if(filePath->empty()){
        throw string("Null file path");
    } else {
        size_t index = filePath->find_last_of("/\\");
        string fileName = filePath->substr(index+1);
        if(fileName.empty()){
            throw string("File path is directory");
        } else {
            this->inputFileName = fileName;
        }
    }
}

void FileManager::setOutputFileName(const string *filePath){
    //Set output bare file name with error handling
    if(filePath->empty()){
        throw string("Null file path");
    } else {
        size_t index = filePath->find_last_of("/\\");
        string fileName = filePath->substr(index+1);
        if(fileName.empty()){
            throw string("File path is directory");
        } else {
            this->outputFileName = fileName;
        }
    }
}

FileManager::FileManager(){

}

void FileManager::setInputFilePath(const string *filePath){
    //Set input file path with error handling
    this->inputFilePath = *filePath;
    try {
        this->setInputFileName(filePath);
    }  catch (exception &e) {
        throw e;
    }
}

void FileManager::setOutputFilePath(const string *filePath){
    //Set output file path with error handling
    this->outputFilePath = *filePath;
    try {
        this->setOutputFileName(filePath);
    }  catch (exception &e) {
        throw e;
    }
}

void FileManager::open(){
    //Set Qimage to open file based on inputFilePath with error handling
    this->image = QImage(QString::fromStdString(this->inputFilePath));
    if(this->image.isNull()){
        throw string("The file was unable to be opened");
    }
}

void FileManager::save(){
    //Save the image
    bool returnStatus = rectifiedImage.save(QString::fromStdString(this->outputFilePath));
    if(!returnStatus){
        throw string("The file was unable to be saved");
    }
}

const string &FileManager::getInputFileName() const{
    //Return the bare input file name
    return this->inputFileName;
}

const string &FileManager::getInputFilePath() const{
    //Return the input file path
    return this->inputFilePath;
}

const string &FileManager::getOutputFileName() const{
    //Return the bare output file name
    return this->outputFileName;
}

const string &FileManager::getOutputFilePath() const{
    //Return the output file path
    return this->outputFilePath;
}

const QImage *FileManager::getImagePtr() const{
    //Return the image by reference
    return &image;
}

QImage *FileManager::getRectImagePtr(){
    //Return the rectified image by reference
    return &rectifiedImage;
}
