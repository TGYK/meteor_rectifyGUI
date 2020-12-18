//============================================================================
// Name        : threadmanager.cpp
// Author      : TGYK
// Date        : 12/14/2020
// E-Mail      : tgyk@tgyk.net
// Description : This class is responsible for the handling of threads which
//               perform image processing. It will determine the number of
//               threads to spawn upon construction. The prepare() method is
//               used to create threads to be used within image processing-
//               parameter values are calculated or otherwise passed to each
//               thread, which in turn is created within a vector of unique
//               pointers. The run() method is responsible for starting this
//               list of threads on work. This class also handles the signals
//               from each thread, calculating progress to be emitted as a
//               signal, as well as emitting a signal when all work has been
//               completed.
//============================================================================

#include "threadmanager.h"

ThreadManager::ThreadManager(){
    this->numberThreads = std::thread::hardware_concurrency();
    if(this->numberThreads < 1){
        this->numberThreads = 1;
    }
}

void ThreadManager::prepare(){
    //(re)set initial values in preparation for creating the threads
    this->startRow = 0;
    this->progress = 0;
    this->rowsCompleted = 0;
    this->workers.clear(); //Potential memory leak? (unique pointers not destroyed, QObject connections not destroyed)??
    int height = originalImage->height();
    *rectifiedImage = QImage(this->rectifiedWidth, originalImage->height(), originalImage->format()); //Almost definite memory leak with subsequent rectifications..

    //Calculate some starting parameters
    this->workerRows = ceil(height / this->numberThreads);
    this->endRow = this->workerRows;

    //Create threads
    do{
        this->workers.push_back(make_unique<RectifyThread>(&*originalImage, &*rectifiedImage, rectifiedWidth, correctionFactorVector, endRow, startRow, &rowsCompleted));
        this->startRow = this->endRow;

        if((this->endRow + this->workerRows) < height){
            this->endRow = this->startRow + this->workerRows;
        } else {
            this->endRow = height;
        }
        QObject::connect(&*workers[workers.size() - 1], SIGNAL(rowCompleted()), this, SLOT(setProgress()), Qt::DirectConnection);
    }while(static_cast<int>(workers.size()) < this->numberThreads);
}

void ThreadManager::run(){
    //Start threads
    QThreadPool::globalInstance()->setExpiryTimeout(-1);
    for(int workerNumber = 0; workerNumber < this->numberThreads; workerNumber++){
        workers[workerNumber]->setAutoDelete(false);
        QThreadPool::globalInstance()->start(&*workers[workerNumber]);
    }
}
