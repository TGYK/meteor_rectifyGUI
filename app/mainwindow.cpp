//============================================================================
// Name        : mainwindow.cpp
// Author      : TGYK
// Date        : 12/14/2020
// E-Mail      : tgyk@tgyk.net
// Description : This class is responsible for the graphical interface
//               presented to the user. It handles button and slider inputs,
//               as well as updating various graphic displays based on signals
//               from other classes. This class also handles the preparation
//               of other classes and overall program flow.
//============================================================================

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow){
    //Setup ui
    ui->setupUi(this);

    //Set window title
    this->setWindowTitle("meteor_rectifyGUI V" + QString::fromStdString(this->version));

    //Set sliders to default value
    ui->radiusSlider->setMinimum(this->correctionFactor.getDefaultEarthRadius());
    ui->radiusSlider->setValue(this->correctionFactor.getDefaultEarthRadius());
    ui->altitudeSlider->setMinimum(this->correctionFactor.getDefaultSatelliteAltitude());
    ui->altitudeSlider->setValue(this->correctionFactor.getDefaultSatelliteAltitude());
    ui->swathSlider->setValue(this->correctionFactor.getDefaultSatelliteSwath());

    //Disable ui elements to prevent modification until image is opened
    ui->radiusSlider->setDisabled(true);
    ui->altitudeSlider->setDisabled(true);
    ui->swathSlider->setDisabled(true);
    ui->sliderResetButton->setDisabled(true);
    ui->saveButton->setDisabled(true);
    ui->rectifyButton->setDisabled(true);

    //Align image in center of frame to be viewed more friendly
    ui->imageView->setAlignment(Qt::AlignHCenter);

    //Connect slot responsible for updating progressbar to signal from threadmanager
    QObject::connect(&threadManager, SIGNAL(progressMade(int)), this, SLOT(updateProgress(int)), Qt::DirectConnection);
    QObject::connect(&threadManager, SIGNAL(processingDone()), this, SLOT(updateImage()));

    //Print in logbox about startup
    ui->logBox->append("meteor_rectifyGUI V" + QString::fromStdString(this->version) + " successfully started.");
    ui->logBox->append("Please open an image");
}

MainWindow::~MainWindow(){
    //DEATH AND DESTRUCTION
    //Also, delete the ui to free up memory when done
    delete ui;
}

void MainWindow::resetClicked(){
    //Reset sliders to default values
    ui->radiusSlider->setValue(this->correctionFactor.getDefaultEarthRadius());
    ui->altitudeSlider->setValue(this->correctionFactor.getDefaultSatelliteAltitude());
    ui->swathSlider->setValue(this->correctionFactor.getDefaultSatelliteSwath());

    //Update class vars in CorrectionFactor
    this->correctionFactor.setEarthRadius(ui->radiusSlider->value());
    this->correctionFactor.setSatelliteAltitude(ui->altitudeSlider->value());
    this->correctionFactor.setSatelliteSwath(ui->swathSlider->value());

    //Prepare new threads based on new correction factor.
    threadManager.setCorrectionFactorVector(correctionFactor.getVector());
    threadManager.setRectifiedWidth(correctionFactor.getRectifiedWidth());

    //Reset image alignment
    ui->imageView->setAlignment(Qt::AlignHCenter);

    //Reset image
    ui->imageView->setPixmap(QPixmap::fromImage(*fileManager.getImagePtr()).QPixmap::scaledToHeight(ui->imageView->height()));

    //Reset progress bar
    ui->rectifyProgress->setValue(0);

    //Print to logbox about the event
    ui->logBox->append("Sliders reset to default values");

    //Re-prepare threads
    threadManager.prepare();
}

void MainWindow::rectifyClicked(){
    //Call threadManager to start rectification
    threadManager.run();

    //Print to logbox about the event
    ui->logBox->append("Rectifying image...");

    //Enable ui elements for saving
    ui->saveButton->setDisabled(false);
}

void MainWindow::openClicked(){
    QString filePath;
    //If lineEdit is empty, open file dialoge to current directory
    //Otherwise, open to string
    if(ui->openLineEdit->text() == "") {
        filePath = QFileDialog::getOpenFileName(this,
            tr("Open Image"), "", tr("Image Files (*.png)"));
    } else {
        filePath = QFileDialog::getOpenFileName(this,
            tr("Open Image"), ui->openLineEdit->text(), tr("Image Files (*.png)"));
    }

    //Verify we got something back from the file dialog
    if(filePath.isNull()){
        //Print to logbox if nothing selected during file dialog
        ui->logBox->append("No image selected for open");
        return;
    }

    //Set some working strings
    string inputFilePath = filePath.toStdString();
    string outputFilePath = inputFilePath.substr(0, inputFilePath.length() - 4) + "-rectified.png";

    //Update the UI to reflect these strings
    ui->openLineEdit->setText(filePath);
    ui->saveLineEdit->setText(QString().fromStdString(outputFilePath));

    //Have fileManager deal with these strings to open
    try {
        fileManager.setInputFilePath(&inputFilePath);
    }  catch (exception &e) {
        QMessageBox msgBox;
        msgBox.setText(QString().fromStdString(e.what()));
        msgBox.exec();
        return;
    }
    try {
        fileManager.setOutputFilePath(&outputFilePath);
    }  catch (exception &e) {
        QMessageBox msgBox;
        msgBox.setText(QString().fromStdString(e.what()));
        msgBox.exec();
        return;
    }
    try {
        fileManager.open();
    }  catch (exception &e) {
        QMessageBox msgBox;
        msgBox.setText(QString().fromStdString(e.what()));
        msgBox.exec();
        return;
    }

    //Print to logbox
    ui->logBox->append(QString::fromStdString(fileManager.getInputFileName()) + " opened.");

    //If the opening is successful, update the CorrectionFactor object to recalculate the factor array
    this->correctionFactor.setImageWidth(fileManager.getImagePtr()->width());

    //Display the unrectified image
    ui->imageView->setPixmap(QPixmap::fromImage(*fileManager.getImagePtr()).QPixmap::scaledToHeight(ui->imageView->height()));

    //Reset progress bar
    ui->rectifyProgress->setValue(0);

    //Set threadmanager variables.. Likely a better way to do this.
    threadManager.setOriginalImage(fileManager.getImagePtr());
    threadManager.setRectImage(fileManager.getRectImagePtr());
    threadManager.setCorrectionFactorVector(correctionFactor.getVector());
    threadManager.setRectifiedWidth(correctionFactor.getRectifiedWidth());

    //Call threadmanager to setup threads
    threadManager.prepare();

    //Enable ui elements after image is opened
    ui->radiusSlider->setDisabled(false);
    ui->altitudeSlider->setDisabled(false);
    ui->swathSlider->setDisabled(false);
    ui->sliderResetButton->setDisabled(false);
    ui->rectifyButton->setDisabled(false);
}

void MainWindow::saveClicked(){
    //Get output path from fileDialog
    QString outputFilePath = QFileDialog::getSaveFileName(this,
        tr("Save"), ui->saveLineEdit->text(), tr("Image Files (*.png)"));

    //Check for blank string
    if(outputFilePath.isNull()){
        //Print to logbox if nothing selected during file dialog
        ui->logBox->append("No image selected for save");
        return;
    }

    //Convert to string
    string filePath = outputFilePath.toStdString();

    //Set the new output path
    try {
        fileManager.setOutputFilePath(&filePath);
    }  catch (exception &e) {
        QMessageBox msgBox;
        msgBox.setText(QString().fromStdString(e.what()));
        msgBox.exec();
        return;
    }

    //Save the file
    try {
        fileManager.save();
    }  catch (exception &e) {
        QMessageBox msgBox;
        msgBox.setText(QString().fromStdString(e.what()));
        msgBox.exec();
        return;
    }
    ui->logBox->append("Image " + QString::fromStdString(fileManager.getOutputFileName()) + " saved.");
}

void MainWindow::updateSlider(){
    //Update class vars in CorrectionFactor
    this->correctionFactor.setEarthRadius(ui->radiusSlider->value());
    this->correctionFactor.setSatelliteAltitude(ui->altitudeSlider->value());
    this->correctionFactor.setSatelliteSwath(ui->swathSlider->value());

    //Prepare new threads based on new correction factor.
    threadManager.setCorrectionFactorVector(correctionFactor.getVector());
    threadManager.setRectifiedWidth(correctionFactor.getRectifiedWidth());
    this->threadManager.prepare();
}


void MainWindow::updateProgress(int progress){
    //Update progressbar based on incoming progress by emitting a signal to the progress bar's slot..
    emit setProgressValue(progress);

    //Print to logbox
    if(progress == 100){
        ui->logBox->append("Image successfully rectified");
    }
}

void MainWindow::updateImage(){
    //Change the imageview to the new image, scaling based on imageView constraints
    if(QPixmap::fromImage(*fileManager.getRectImagePtr()).QPixmap::scaledToHeight(ui->imageView->height()).width() > ui->imageView->width()){
        //Align image in center of frame to be viewed more friendly
        ui->imageView->setAlignment(Qt::AlignVCenter);
        ui->imageView->setPixmap(QPixmap::fromImage(*fileManager.getRectImagePtr()).QPixmap::scaledToWidth(ui->imageView->width()));
    }else {
        //Align image in center of frame to be viewed more friendly
        ui->imageView->setAlignment(Qt::AlignHCenter);
        ui->imageView->setPixmap(QPixmap::fromImage(*fileManager.getRectImagePtr()).QPixmap::scaledToHeight(ui->imageView->height()));
    }
}

