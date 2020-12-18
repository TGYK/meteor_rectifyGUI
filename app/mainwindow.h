//============================================================================
// Name        : mainwindow.h
// Author      : TGYK
// Date        : 12/14/2020
// E-Mail      : tgyk@tgyk.net
// Description : This is the class definition of MainWindow. Special note is
//               the slots used to capture GUI events and process them, and
//               the signal used to send the progress to the GUI progress bar
//               for updating.
//============================================================================

#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include "filemanager.h"
#include "correctionfactor.h"
#include "threadmanager.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void resetClicked();
    void rectifyClicked();
    void openClicked();
    void saveClicked();
    void updateSlider();
    void updateProgress(int progress);
    void updateImage();

private:
    int progress = 0;
    string version = "1.0";
    Ui::MainWindow *ui;
    FileManager fileManager;
    CorrectionFactor correctionFactor;
    ThreadManager threadManager;
signals:
    void setProgressValue(int progress);
};
#endif // MAINWINDOW_H
