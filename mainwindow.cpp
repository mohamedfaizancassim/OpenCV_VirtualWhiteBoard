#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QtMultimedia/QCamera"
#include "QtMultimedia/QCameraInfo"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "QThread"
#include "QFileDialog"
#include "QMessageBox"
#include <string.h>
#include "QImage"
#include "vector"

//Global Variables
cv::VideoCapture vidCap; //Holds the video capture device infor
bool isRecording=false; //Flag for recording to continue

//SlideBar Variables
std::atomic_int varLH(100);
std::atomic_int varLS(100);
std::atomic_int varLV(100);
std::atomic_int varUH(179);
std::atomic_int varUS(254);
std::atomic_int varUV(254);


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Populate Cameras
    //Get all connected cameras
    const QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    //Add all cameras to UI Combobox
    for(auto cam : cameras)
    {
        ui->cmbAvCameras->addItem(cam.description());
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_cmdImageSnap_clicked()
{
    vidCap.open(ui->cmbAvCameras->currentIndex());
    cv::Mat frame;
    if(vidCap.isOpened())
    {
        vidCap>>frame;
        cv::imshow("Captured Image",frame);
    }
    vidCap.release();
}

void MainWindow::Record_Video()
{
     //Load the image caoture device
     vidCap.open(ui->cmbAvCameras->currentIndex());

     auto filename=QFileDialog::getSaveFileName(this,tr("Save video as..."),tr("Video Files (*.avi)"));


    if(vidCap.isOpened())
    {
        //Get Frame Width and Height
        int vid_height=vidCap.get(cv::CAP_PROP_FRAME_HEIGHT);
        int vid_width=vidCap.get(cv::CAP_PROP_FRAME_WIDTH);
        //Stores the frame object.
        cv::Mat videoFrame;
        //Initialise VideoWriter object
        cv::VideoWriter vidWriter(filename.toStdString(),cv::VideoWriter::fourcc('M','J','P','G'),10,cv::Size(vid_width,vid_height));
        while(isRecording)
        {
            //Grab frame from video capture device
            vidCap>>videoFrame;
            //Break if frame empty
            if(videoFrame.empty())break;

            //Convert frame from BGR to HSV
            cv::Mat hsv_frame;
            cv::cvtColor(videoFrame,hsv_frame,cv::COLOR_BGR2HSV);


            //Create a mask based on Lower and Upper bounds
            cv::Mat mask;
            int _varLH=varLH.load();
            int _varLS=varLS.load();
            int _varLV=varLV.load();
            int _varUH=varUH.load();
            int _varUS=varUS.load();
            int _varUV=varUV.load();

            std::cout<<_varLH<<" "<<_varLS<<" "<<_varLV<<std::endl;

            cv::inRange(hsv_frame,cv::Scalar(_varLH,_varLS,_varLV),cv::Scalar(_varUH,_varUS,_varUV),mask);

            //Write frame to Video Writer
            cv::imshow("Mask Output",mask);
            vidWriter<<hsv_frame;
        }

        //Show message box with video location!
        QMessageBox savedVidLocation;
        savedVidLocation.setText("Your video is saved in: "+filename);
        savedVidLocation.show();



        //Release vidCap and vidWriter objects.
        vidWriter.release();
        vidCap.release();
    }



}
void MainWindow::on_cmdRecord_clicked()
{
    QThread *recordVideo=QThread::create([this]{Record_Video();});
    isRecording=true;
    recordVideo->start();


}

void MainWindow::on_cmdStopRecord_clicked()
{
    isRecording=false;
}

void MainWindow::on_hslideLH_sliderMoved(int position)
{
    varLH=position;
}

void MainWindow::on_hslideLS_sliderMoved(int position)
{
    varLS=position;
}

void MainWindow::on_hslideLV_sliderMoved(int position)
{
    varLV=position;
}
