#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>
#include <libKalman/predictpoint.h>
#include <fstream>
#include <drawTrack.h>
#include <motionpredict.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void testFunc();

private slots:
    void on_openCameraAct_triggered();

    void on_openLocalVideoAct_triggered();

    void on_ImgSequenceAct_triggered();

    void on_selectObjectBtn_clicked();

    void on_stopBtn_clicked();

    void on_startBtn_clicked();

    void on_screenshotBtn_clicked();

    void on_pauseOrContinueBtn_clicked();

    void on_saveVideoBtn_clicked();

    void on_showTrackBtn_clicked();

public:
    std::vector<cv::Rect> objectRectVec; //多目标框vector
    std::vector<cv::Rect> objectRectVecInit;


private:
    Ui::MainWindow *ui;

    int iUseDevice = -1; //0:camera 1:localVideo 2:imgSeq
    int iCameraID = 0; //默认摄像头编号
    bool bStartFlag = false;
    bool bPauseFlag = false;
    bool bHadSetObjectFlag = false;
    bool bScreenshotFlag = false;
    bool bSaveVideoFlag = false;

    int frameWidth = 0;
    int frameHeight = 0;

    //视频读取
    cv::VideoCapture videoReader;
    cv::Mat frame; //帧图像
    std::string openVideoPath = "D:/DataSet/track_test_sequence/qiu3.mp4";
    //std::string openVideoPath = "D:/DataSet/track_test_sequence/2.mp4";
    //std::string openImgSeqPath = "D:/DataSet/track_test_sequence/OBT50/Biker/img/";
    //std::string openImgSeqPath = "D:/DataSet/track_test_sequence/OBT50/BlurFace/img/";
    //std::string openImgSeqPath = "D:/DataSet/track_test_sequence/OBT50/CarScale/img/";
    //std::string openImgSeqPath = "D:/DataSet/track_test_sequence/OBT50/Dog/img/";
    //std::string openImgSeqPath = "D:/DataSet/track_test_sequence/OBT50/Skater/img/";
    //std::string openImgSeqPath = "D:/DataSet/track_test_sequence/OBT50/Human3/img/";
    //std::string openImgSeqPath = "D:/DataSet/VOT2016/blanket/"; //遮挡
    //std::string openImgSeqPath = "D:/DataSet/VOT2016/bmx/"; //旋转
    //std::string openImgSeqPath = "D:/DataSet/VOT2016/bolt2/"; //多人 赛跑
    //std::string openImgSeqPath = "D:/DataSet/\drone_sequence/uav0000074_11856_v/"; //多人 车 夜景
    //std::string openImgSeqPath = "../data/uav0000186_00250_v/"; //多车
    std::string openImgSeqPath = "../data/uav0000186_00250_vtamp2/"; //修改后的车图像序列文件夹

    bool useVideoCaptuere = true;

    //视频保存
    cv::VideoWriter videoWriter;
    std::string saveVideoPath = "../output.avi";
    double fps = 30.0;

    //跟踪器
    std::string trackerTypes[7] = { "BOOSTING", "MIL", "KCF", "TLD","MEDIANFLOW", "MOSSE", "CSRT" };
    std::string trackerType = trackerTypes[2];
    cv::Rect2d objectRect; //跟踪框
    //cv::Rect2d objectRect2;
    //cv::Ptr<cv::Tracker> tracker;

    //多目标跟踪器
    cv::MultiTracker *mul_tracker;//多目标跟踪器
    std::vector<cv::Ptr<cv::Tracker>> trackers; //指向各跟踪器的指针vector
    std::vector<cv::Rect2d> objectRect2dVec;//存储各个目标框
    int objectNumber = 3;//目标个数

    //预测
    //cv::Point currentPoint;
    //cv::Point predictPoint;
    int filterMethod = 2; //0-不用预测 1-kalman 2-速度预测
    //predict_point predict;
    //MotionPredict predictor;
    //cv::Point predictPoint;
    std::vector<MotionPredict> predictorVec;
    //std::vector<cv::Point> predictPointVec;

    //输出文件
    std::ofstream outFile;
    bool isOutputFile = true;

    //轨迹显示
    drawTrack draw_track_predict;
    drawTrack draw_track_true;
    bool isShowTrack = false;

    //状态栏显示
    QLabel *statusLabel; // 先创建一个QLabel对象

    //其他标志位
    bool isShowFPS = true;
    bool isShowType = false;
    bool isShowFailure = false;

};

#endif // MAINWINDOW_H
