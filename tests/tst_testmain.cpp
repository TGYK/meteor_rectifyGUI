//============================================================================
// Name        : tst_testmain.cpp
// Author      : TGYK
// Date        : 12/14/2020
// E-Mail      : tgyk@tgyk.net
// Description : This is the main testing class. It is responsible for testing
//               and verifying function of all but the GUI elements of the
//               application. GUI elements were meant to be tested but due to
//               time constraints and QTest constraints, GUI tests could not
//               be implemented at this time. This class uses a preprocessor
//               directive to notify ThreadManager to build with private
//               members as public members to allow for proper testing.
//============================================================================

#define TESTING
#include <QtTest>
#include <QCoreApplication>
#include <QDebug>
#include <mainwindow.h>
#include <rectifythread.h>

// add necessary includes here
const int IMAGE_WIDTH = 1568;
const double EARTH_RADIUS = 6371.0;
const double SATELLITE_ALTITUDE = 822.5;
const int SATELLITE_SWATH = 2800;
const string INPUT_PATH = "C:\\users\\foo\\bar\\image.png";
const string OUTPUT_PATH = "C:\\users\\foo\\bar\\image-rectified.png";
const string INPUT_FILE = "image.png";
const string OUTPUT_FILE = "image-rectified.png";
const string ERR_PATH = "C\\users\\foo\\bar\\";
const string ERR_PATH_2 = "";
const string TEST_IMAGE_INPUT_PATH = ":/testimage.png";
const string TEST_IMAGE_OUTPUT_PATH = QString(QDir::currentPath() + QString("/testimage-rectified.png")).toStdString();
const QImage TEST_IMAGE = QImage(":/testimage.png");
const QImage TEST_IMAGE_RECTIFIED = QImage(":/testimage-rectified.png");

QMutex RectifyThread::mutex;

class testMain : public QObject
{
    Q_OBJECT
public:
    testMain();
    ~testMain();
private slots:
    //CorrectionFactor tests
    void testSetImageWidth();
    void testSetEarthRadius();
    void testSetSatelliteAltitude();
    void testSetSatelliteSwath();
    void testGetRectifiedWidth();
    void testGetEarthRadius();
    void testGetSatelliteAltitude();
    void testGetSatelliteSwath();
    void testGetDefaultEarthRadius();
    void testGetDefaultSatelliteAltitude();
    void testGetDefaultSatelliteSwath();
    void testGetVector();
    //FileManager tests
    void testSetInputFilePath();
    void testSetOutputFilePath();
    void testOpen();
    void testSave();
    void testGetInputFileName();
    void testGetInputFilePath();
    void testGetOutputFileName();
    void testGetOutputFilePath();
    void testGetImagePtr();
    void testGetRectImagePtr();
    //RectifyThread tests
    void testRunRT();
    //ThreadManager tests
    void testSetOriginalImage();
    void testSetRectImage();
    void testSetCorrectionFactorVector();
    void testSetRectifiedWidth();
    void testPrepare();
    void testRunTM();
    //MainWindow tests --- Not implemented due to use of fileDialog: Unable to find
    //                     resources documenting how to control the fileDialog
    //                     popup window.. All other actions are barred based on use
    //                     of initial Mainwindow::open() method that gets called on
    //                     button press to open file to process.
//    void testOpenClicked();
//    void testUpdateSlider();
//    void testResetClicked();
//    void testRectifyClicked();
//    void testSaveClicked();
};

testMain::testMain(){
}

testMain::~testMain(){
}

void testMain::testSetImageWidth(){
    CorrectionFactor correctionFactor(IMAGE_WIDTH);
    double vectorSum = 0;

    correctionFactor.setImageWidth(500);
    for(int i = 0; i < static_cast<int>(correctionFactor.getVector().size()); i++){
        vectorSum += correctionFactor.getVector()[i];
    }

    QCOMPARE(correctionFactor.getRectifiedWidth(), 894);
    qDebug() << QString::number(vectorSum, 'f');
    QCOMPARE(vectorSum, 893.46929470511677);
}

void testMain::testSetEarthRadius(){
    CorrectionFactor correctionFactor(IMAGE_WIDTH);
    double vectorSum = 0;

    correctionFactor.setEarthRadius(EARTH_RADIUS + 500 );
    for(int i = 0; i < static_cast<int>(correctionFactor.getVector().size()); i++){
        vectorSum += correctionFactor.getVector()[i];
    }

    QCOMPARE(correctionFactor.getEarthRadius(), 6871);
    QCOMPARE(vectorSum, 1734.4340804715121);
}

void testMain::testSetSatelliteAltitude(){
    CorrectionFactor correctionFactor(IMAGE_WIDTH);
    double vectorSum = 0;

    correctionFactor.setSatelliteAltitude(SATELLITE_ALTITUDE + 500);
    for(int i = 0; i < static_cast<int>(correctionFactor.getVector().size()); i++){
        vectorSum += correctionFactor.getVector()[i];
    }

    QCOMPARE(correctionFactor.getSatelliteAltitude(), 1322.5);
    QCOMPARE(vectorSum, 1789.2422367565114);
}

void testMain::testSetSatelliteSwath(){
    CorrectionFactor correctionFactor(IMAGE_WIDTH);
    double vectorSum = 0;

    correctionFactor.setSatelliteSwath(SATELLITE_SWATH + 500);
    for(int i = 0; i < static_cast<int>(correctionFactor.getVector().size()); i++){
        vectorSum += correctionFactor.getVector()[i];
    }

    QCOMPARE(correctionFactor.getSatelliteSwath(), 3300);
    QCOMPARE(vectorSum, 3136.0679240160866);

}

void testMain::testGetRectifiedWidth(){
    CorrectionFactor correctionFactor(IMAGE_WIDTH);
    QCOMPARE(correctionFactor.getRectifiedWidth(), 2791);
}

void testMain::testGetEarthRadius(){
    CorrectionFactor correctionFactor(IMAGE_WIDTH);
    QCOMPARE(correctionFactor.getEarthRadius(), EARTH_RADIUS);
}

void testMain::testGetSatelliteAltitude(){
    CorrectionFactor correctionFactor(IMAGE_WIDTH);
    QCOMPARE(correctionFactor.getSatelliteAltitude(), SATELLITE_ALTITUDE);
}

void testMain::testGetSatelliteSwath(){
    CorrectionFactor correctionFactor(IMAGE_WIDTH);
    QCOMPARE(correctionFactor.getSatelliteSwath(), SATELLITE_SWATH);
}

void testMain::testGetDefaultEarthRadius(){
    CorrectionFactor correctionFactor(IMAGE_WIDTH);
    correctionFactor.setEarthRadius(3000);
    QCOMPARE(correctionFactor.getDefaultEarthRadius(), EARTH_RADIUS);
}

void testMain::testGetDefaultSatelliteAltitude(){
    CorrectionFactor correctionFactor(IMAGE_WIDTH);
    correctionFactor.setSatelliteAltitude(1000);
    QCOMPARE(correctionFactor.getDefaultSatelliteAltitude(), SATELLITE_ALTITUDE);
}

void testMain::testGetDefaultSatelliteSwath(){
    CorrectionFactor correctionFactor(IMAGE_WIDTH);
    correctionFactor.setSatelliteSwath(5000);
    QCOMPARE(correctionFactor.getDefaultSatelliteSwath(), SATELLITE_SWATH);
}

void testMain::testGetVector(){
    CorrectionFactor correctionFactor(IMAGE_WIDTH);
    double vectorSum = 0;

    for(int i = 0; i < static_cast<int>(correctionFactor.getVector().size()); i++){
        vectorSum += correctionFactor.getVector()[i];
    }

    QCOMPARE(vectorSum, 2790.3388488805076);
}

void testMain::testSetInputFilePath(){
    FileManager fileManager;
    fileManager.setInputFilePath(&INPUT_PATH);

    QCOMPARE(fileManager.getInputFilePath(), INPUT_PATH);
    QCOMPARE(fileManager.getInputFileName(), INPUT_FILE);
    QVERIFY_EXCEPTION_THROWN(fileManager.setInputFilePath(&ERR_PATH), string);
    QVERIFY_EXCEPTION_THROWN(fileManager.setInputFilePath(&ERR_PATH_2), string);
}

void testMain::testSetOutputFilePath(){
    FileManager fileManager;
    fileManager.setOutputFilePath(&OUTPUT_PATH);

    QCOMPARE(fileManager.getOutputFilePath(), OUTPUT_PATH);
    QCOMPARE(fileManager.getOutputFileName(), OUTPUT_FILE);
    QVERIFY_EXCEPTION_THROWN(fileManager.setOutputFilePath(&ERR_PATH), string);
    QVERIFY_EXCEPTION_THROWN(fileManager.setOutputFilePath(&ERR_PATH_2), string);
}

void testMain::testOpen(){
    FileManager fileManager;
    fileManager.setInputFilePath(&TEST_IMAGE_INPUT_PATH);
    fileManager.open();

    QCOMPARE(*fileManager.getImagePtr(), TEST_IMAGE);
    fileManager.setInputFilePath(&INPUT_PATH);
    QVERIFY_EXCEPTION_THROWN(fileManager.open(), string);
}

void testMain::testSave(){
    FileManager fileManager;
    fileManager.setOutputFilePath(&TEST_IMAGE_OUTPUT_PATH);
    *fileManager.getRectImagePtr() = TEST_IMAGE;
    try {
        fileManager.save();
    }  catch (exception &e) {
        QFAIL("Save failed");
    }

    QVERIFY(QFile::exists(QString::fromStdString(TEST_IMAGE_OUTPUT_PATH)));
    QFile(QString::fromStdString(TEST_IMAGE_OUTPUT_PATH)).remove();
    QVERIFY(!QFile::exists(QString::fromStdString(TEST_IMAGE_OUTPUT_PATH)));
    //Cannot find a way to force a failed save...
}

void testMain::testGetInputFileName(){
    FileManager fileManager;
    fileManager.setInputFilePath(&INPUT_PATH);
    QCOMPARE(fileManager.getInputFileName(), INPUT_FILE);
}

void testMain::testGetInputFilePath(){
    FileManager fileManager;
    fileManager.setInputFilePath(&INPUT_PATH);
    QCOMPARE(fileManager.getInputFilePath(), INPUT_PATH);
}

void testMain::testGetOutputFileName(){
    FileManager fileManager;
    fileManager.setOutputFilePath(&OUTPUT_PATH);
    QCOMPARE(fileManager.getOutputFileName(), OUTPUT_FILE);
}

void testMain::testGetOutputFilePath(){
    FileManager fileManager;
    fileManager.setOutputFilePath(&OUTPUT_PATH);
    QCOMPARE(fileManager.getOutputFilePath(), OUTPUT_PATH);
}

void testMain::testGetImagePtr(){
    FileManager fileManager;
    fileManager.setInputFilePath(&TEST_IMAGE_INPUT_PATH);
    fileManager.open();
    QCOMPARE(*fileManager.getImagePtr(), TEST_IMAGE);
}

void testMain::testGetRectImagePtr(){
    FileManager fileManager;
    fileManager.setInputFilePath(&TEST_IMAGE_INPUT_PATH);
    fileManager.open();
    QCOMPARE(*fileManager.getImagePtr(), TEST_IMAGE);
    *fileManager.getRectImagePtr() = TEST_IMAGE;
    QCOMPARE(*fileManager.getRectImagePtr(), TEST_IMAGE);
}

void testMain::testRunRT(){
    CorrectionFactor correctionFactor(TEST_IMAGE.width());
    QImage testImageWork(correctionFactor.getRectifiedWidth(),
                                  TEST_IMAGE.height(), TEST_IMAGE.format());
    int rows_completed;

    RectifyThread *rectifyThread = new RectifyThread(&TEST_IMAGE, &testImageWork,
                                                     correctionFactor.getRectifiedWidth(),
                                                     correctionFactor.getVector(),
                                                     TEST_IMAGE.height(),
                                                     0, &rows_completed);

    QSignalSpy testRowSpy(rectifyThread, SIGNAL(rowCompleted()));

    QThreadPool::globalInstance()->start(rectifyThread);

    do{
        if(testRowSpy.count() == TEST_IMAGE.height()){
            QCOMPARE(TEST_IMAGE_RECTIFIED, testImageWork);
        }
    }while (testRowSpy.count() != TEST_IMAGE.height());
}

void testMain::testSetOriginalImage(){
    ThreadManager threadManager;
    threadManager.setOriginalImage(&TEST_IMAGE);
    QCOMPARE(TEST_IMAGE, *threadManager.originalImage);
    QCOMPARE(&TEST_IMAGE, threadManager.originalImage);
}

void testMain::testSetRectImage(){
    ThreadManager threadManager;
    CorrectionFactor correctionFactor(TEST_IMAGE.width());
    QImage testImageWork(correctionFactor.getRectifiedWidth(),
                                  TEST_IMAGE.height(), TEST_IMAGE.format());
    threadManager.setRectImage(&testImageWork);
    QCOMPARE(testImageWork, *threadManager.rectifiedImage);
    QCOMPARE(&testImageWork, threadManager.rectifiedImage);
}

void testMain::testSetCorrectionFactorVector(){
    ThreadManager threadManager;
    CorrectionFactor correctionFactor(TEST_IMAGE.width());
    threadManager.setCorrectionFactorVector(correctionFactor.getVector());
    QCOMPARE(correctionFactor.getVector(), threadManager.correctionFactorVector);
}

void testMain::testSetRectifiedWidth(){
    ThreadManager threadManager;
    CorrectionFactor correctionFactor(TEST_IMAGE.width());
    threadManager.setRectifiedWidth(correctionFactor.getRectifiedWidth());
    QCOMPARE(correctionFactor.getRectifiedWidth(), threadManager.rectifiedWidth);
}

void testMain::testPrepare(){
    ThreadManager threadManager;
    CorrectionFactor correctionFactor(TEST_IMAGE.width());
    QImage testImageWork(correctionFactor.getRectifiedWidth(),
                                  TEST_IMAGE.height(), TEST_IMAGE.format());
    threadManager.setCorrectionFactorVector(correctionFactor.getVector());
    threadManager.setOriginalImage(&TEST_IMAGE);
    threadManager.setRectImage(&testImageWork);
    threadManager.setRectifiedWidth(correctionFactor.getRectifiedWidth());
    threadManager.prepare();
    QCOMPARE(threadManager.numberThreads, std::thread::hardware_concurrency());
    QCOMPARE(threadManager.workers.size(), std::thread::hardware_concurrency());
    QCOMPARE(threadManager.workerRows, ceil(TEST_IMAGE.height() / std::thread::hardware_concurrency()));
    QCOMPARE(threadManager.endRow, TEST_IMAGE.height());
    QCOMPARE(threadManager.progress, 0);
    QCOMPARE(threadManager.rowsCompleted, 0);
}

void testMain::testRunTM(){
    ThreadManager threadManager;
    CorrectionFactor correctionFactor(TEST_IMAGE.width());
    QImage testImageWork(correctionFactor.getRectifiedWidth(),
                                  TEST_IMAGE.height(), TEST_IMAGE.format());
    threadManager.setCorrectionFactorVector(correctionFactor.getVector());
    threadManager.setOriginalImage(&TEST_IMAGE);
    threadManager.setRectImage(&testImageWork);
    threadManager.setRectifiedWidth(correctionFactor.getRectifiedWidth());
    threadManager.prepare();
    QSignalSpy testProgressSpy(&threadManager, SIGNAL(progressMade(int)));
    QSignalSpy testDoneSpy(&threadManager, SIGNAL(processingDone()));
    threadManager.run();
    testDoneSpy.wait(120000);
    QCOMPARE(testProgressSpy.count(), 100);
    QCOMPARE(testDoneSpy.count(), 1);
    QCOMPARE(testImageWork, TEST_IMAGE_RECTIFIED);
}


QTEST_MAIN(testMain)
#include "tst_testmain.moc"
