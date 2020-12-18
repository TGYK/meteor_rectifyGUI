//============================================================================
// Name        : threadmanager.h
// Author      : TGYK
// Date        : 12/14/2020
// E-Mail      : tgyk@tgyk.net
// Description : This is the class definition of ThreadManager. Special note
//               is the slot used to handle work signals from threads as well
//               as calculate overall progress. This slot will emit a signal
//               for each progress update, as well as when the overall work is
//               finished. This class definition uses a specific preprocessor
//               directive to modify the access for private class members to
//               simplify testing- No accessors to private class members were
//               created for use in the main application, and as such, there
//               is no conventional way to verify their contents without them
//               being declared as public members.
//============================================================================

#ifndef THREADMANAGER_H
#define THREADMANAGER_H
#ifdef TESTING
#define ACCESS public
#else
#define ACCESS private
#endif

#include <QThread>
#include <QDebug>
#include <QImage>
#include <QObject>
#include <vector>
#include <math.h>
#include "rectifythread.h"

using namespace std;


class ThreadManager: public QObject{
Q_OBJECT

ACCESS:
    int progress = 0;
    int oldProgress = progress;
    int rowsCompleted = 0;
    int numberThreads = 1;
    int startRow = 0;
    int workerRows;
    int endRow;
    int rectifiedWidth;
    const QImage *originalImage;
    QImage *rectifiedImage;
    vector<long double> correctionFactorVector;
    vector<unique_ptr<RectifyThread>> workers;
public:
    ThreadManager();
    virtual ~ThreadManager() {};
    void setOriginalImage(const QImage *originalImage){this->originalImage = originalImage;}
    void setRectImage(QImage *rectifiedImage){this->rectifiedImage = rectifiedImage;}
    void setCorrectionFactorVector(vector<long double> correctionFactorVector){this->correctionFactorVector = correctionFactorVector;}
    void setRectifiedWidth(int rectifiedWidth){this->rectifiedWidth = rectifiedWidth;}
    void prepare();
    void run();
public slots:
    //This slot is responsible for catching the "I did some work" signal from threads, and updating progress accordingly
    void setProgress(){
        this->progress = (static_cast<double>(this->rowsCompleted) / this->originalImage->height()) * 100;
        if(this->progress != this->oldProgress){ //Only emit a new progress signal when there is some new progress to provide
            this->oldProgress = this->progress;
            if(this->progress == 100){
                emit processingDone();
            }
            emit progressMade(this->progress);
        }
    }
signals:
    void progressMade(int);
    void processingDone();
};

#endif // THREADMANAGER_H
