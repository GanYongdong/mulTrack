#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QTime>
#include <QDate>

void waitMSec(int ms)
{
//    QTime t;
//    t.start();
//    while(t.elapsed()<ms)
//        QCoreApplication::processEvents();
    QTime dieTime = QTime::currentTime().addMSecs(ms);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    showMaximized(); //最大化

    //状态栏
    statusLabel = new QLabel();
    ui->statusBar->addWidget(statusLabel);

    //测试函数
    //testFunc();

    //禁用一些控件并使能一些控件
    ui->openCameraAct->setDisabled(false);
    ui->openLocalVideoAct->setDisabled(false);
    ui->startBtn->setDisabled(true);
    ui->selectObjectBtn->setDisabled(true);
    ui->pauseOrContinueBtn->setDisabled(true);
    ui->screenshotBtn->setDisabled(true);
    ui->saveVideoBtn->setDisabled(true);
    ui->stopBtn->setDisabled(true);

    setWindowFlags(Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::testFunc()
{
    cv::Mat src = cv::imread("D:/DataSet/Pictures/8.jpg");
    cv::imshow("src", src);
    cv::waitKey(0);
}

/*
 * 按下打开摄像头按钮功能
*/
void MainWindow::on_openCameraAct_triggered()
{
    videoReader.open(iCameraID);

    if (!videoReader.isOpened()) {
        qDebug() << "[MainWindow]: camera 0 can not open" << Qt::endl;
        return;
    }
    useVideoCaptuere = true;
    iUseDevice = 0;//确定使用相机

    //todo: videoReader设置 分辨率 帧率 曝光等

    //禁用一些控件并使能一些控件
    ui->startBtn->setDisabled(true);
    ui->openCameraAct->setDisabled(true);
    ui->openLocalVideoAct->setDisabled(true);
    ui->selectObjectBtn->setDisabled(false);
    ui->pauseOrContinueBtn->setDisabled(true);
    ui->screenshotBtn->setDisabled(true);
    ui->stopBtn->setDisabled(false);

    while (!bStartFlag) {
        videoReader >> frame; //获取帧

        //转Qimage显示
        QImage image(frame.data, frame.cols, frame.rows, static_cast<int>(frame.step), QImage::Format_RGB888);
        ui->frameLabel->setPixmap(QPixmap::fromImage(image.rgbSwapped().scaled(ui->frameLabel->size(),
                                                                               Qt::KeepAspectRatio,Qt::SmoothTransformation)));
        cv::waitKey(1);
        ui->frameLabel->setScaledContents(true); //注释掉则不铺满窗口，采用比例伸缩

        waitMSec(10); //延时10ms
    }
}

/*
 * 按下打开本地文件按钮功能
*/
void MainWindow::on_openLocalVideoAct_triggered()
{
    videoReader.open(openVideoPath);

    if (!videoReader.isOpened()) {
        qDebug() << "[MainWindow]: local video can not open" << Qt::endl;
        return;
    }
    useVideoCaptuere = true;
    iUseDevice = 1;//确定使用本地视频

    //todo: videoReader设置

    //禁用一些控件并使能一些控件
    ui->startBtn->setDisabled(true);
    ui->openCameraAct->setDisabled(true);
    ui->openLocalVideoAct->setDisabled(true);
    ui->selectObjectBtn->setDisabled(false);
    ui->pauseOrContinueBtn->setDisabled(true);
    ui->screenshotBtn->setDisabled(true);
    ui->stopBtn->setDisabled(false);

    cv::VideoCapture cap_temp(openVideoPath);
    while (!bStartFlag) {
        cap_temp >> frame;

        QImage image(frame.data, frame.cols, frame.rows, static_cast<int>(frame.step), QImage::Format_RGB888);
        ui->frameLabel->setPixmap(QPixmap::fromImage(image.rgbSwapped().scaled(ui->frameLabel->size(),
                                                                               Qt::KeepAspectRatio,Qt::SmoothTransformation)));
        cv::waitKey(1);
        ui->frameLabel->setScaledContents(true); //注释掉则不铺满窗口，采用比例伸缩

        waitMSec(10);
    }
    cap_temp.release();
}


void MainWindow::on_ImgSequenceAct_triggered()
{
    useVideoCaptuere = false;
    iUseDevice = 2;//确定使用本地视频

    //禁用一些控件并使能一些控件
    ui->startBtn->setDisabled(true);
    ui->openCameraAct->setDisabled(true);
    ui->openLocalVideoAct->setDisabled(true);
    ui->selectObjectBtn->setDisabled(false);
    ui->pauseOrContinueBtn->setDisabled(true);
    ui->screenshotBtn->setDisabled(true);
    ui->stopBtn->setDisabled(false);

    std::string this_framePath = openImgSeqPath + "0000001.jpg";
    frame = cv::imread(this_framePath);

    QImage image(frame.data, frame.cols, frame.rows, static_cast<int>(frame.step), QImage::Format_RGB888);
    ui->frameLabel->setPixmap(QPixmap::fromImage(image.rgbSwapped().scaled(ui->frameLabel->size(),
                                                                           Qt::KeepAspectRatio,Qt::SmoothTransformation)));
    cv::waitKey(1);
    //ui->frameLabel->setScaledContents(true); //注释掉则不铺满窗口，采用比例伸缩

}

/*
 * 按下选择初始区域按钮功能
*/
void MainWindow::on_selectObjectBtn_clicked()
{
    if (iUseDevice == 0 || iUseDevice == 1) {
        videoReader.read(frame);
    } else if (iUseDevice == 2) {
        std::string this_framePath = openImgSeqPath + "0000001.jpg";
        frame = cv::imread(this_framePath);
    }

    frameWidth = frame.cols;
    frameHeight = frame.rows;
    qDebug() << "[MainWindow] frame size:" << frameWidth << "x" << frameHeight << Qt::endl;

    //手动在图像上画矩形框
    objectRectVec.clear();
    cv::selectROIs("请框选初始目标区域", frame, objectRectVec, false);
    cv::destroyWindow("请框选初始目标区域");
    objectRectVecInit = objectRectVec;
    objectNumber = static_cast<int>(objectRectVec.size()); //目标个数
    if (objectRectVec.size() < 1) {
        qDebug() << "did not set object" << Qt::endl;
    }
    else {
        qDebug() << "the num of object:" << objectRectVec.size() << Qt::endl;
    }
    bHadSetObjectFlag = true; //已经设置好了目标位置

    //跟踪器初始化
    mul_tracker = new cv::MultiTracker();
    qDebug() << "objectRectVec:" << Qt::endl;
    for (auto i = 0; i < static_cast<int>(objectRectVec.size()); i++) {
        cv::Ptr<cv::Tracker> trackerTmp;
        if (trackerType == "BOOSTING")
            trackerTmp = cv::TrackerBoosting::create();
        if (trackerType == "MIL")
            trackerTmp = cv::TrackerMIL::create();
        if (trackerType == "KCF")
            trackerTmp = cv::TrackerKCF::create();
        if (trackerType == "TLD")
            trackerTmp = cv::TrackerTLD::create();
        if (trackerType == "MEDIANFLOW")
            trackerTmp = cv::TrackerMedianFlow::create();
        if (trackerType == "MOSSE")
            trackerTmp = cv::TrackerMOSSE::create();
        if (trackerType == "CSRT")
            trackerTmp = cv::TrackerCSRT::create();

        objectRect2dVec.push_back(objectRectVec[i]);
        trackers.push_back(trackerTmp);

        qDebug() << objectRectVec[i].tl().x << "," << objectRectVec[i].tl().y << "," << objectRectVec[i].width << "," << objectRectVec[i].height << Qt::endl;
    }
    mul_tracker->add(trackers, frame, objectRect2dVec);//添加到多目标检测器
    qDebug() << "[MainWindow]: mulTracker init success, size:" << objectNumber << Qt::endl;

    //输出文件初始化
    if (isOutputFile) {
        outFile.open("../output/dataOutputFile.txt", std::ios::out);
        if (!outFile.is_open()) {
            qDebug() << "outputfile init failed" << Qt::endl;
        }
    }

    //禁用一些控件并使能一些控件
    ui->startBtn->setDisabled(false);
    ui->openCameraAct->setDisabled(true);
    ui->openLocalVideoAct->setDisabled(true);
    ui->selectObjectBtn->setDisabled(true);
    ui->pauseOrContinueBtn->setDisabled(true);
    ui->screenshotBtn->setDisabled(true);
    ui->stopBtn->setDisabled(false);
}

/*
 * 按下开始按钮功能
*/
void MainWindow::on_startBtn_clicked()
{
    bStartFlag = true;

    //禁用一些控件并使能一些控件
    ui->startBtn->setDisabled(true);
    ui->openCameraAct->setDisabled(true);
    ui->openLocalVideoAct->setDisabled(true);
    ui->selectObjectBtn->setDisabled(true);
    ui->pauseOrContinueBtn->setDisabled(false);
    ui->screenshotBtn->setDisabled(false);
    ui->stopBtn->setDisabled(false);
    ui->saveVideoBtn->setDisabled(false);

    /*//1.kalman filter setup
    const int stateNum = 4;                         //状态值4×1向量(x,y,△x,△y)
    const int measureNum = 2;                       //测量值2×1向量(x,y)
    cv::KalmanFilter KF(stateNum, measureNum, 0);	//构建卡尔曼滤波器模型
    cv::Mat measurement = cv::Mat::zeros(measureNum, 1, CV_32F);
    cv::RNG rng;
    KF.transitionMatrix = (cv::Mat_<float>(4, 4) << 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1);  //转移矩阵A
    cv::setIdentity(KF.measurementMatrix);                                 //测量矩阵H
    cv::setIdentity(KF.processNoiseCov, cv::Scalar::all(1e-5));            //系统噪声方差矩阵Q 1e-5
    cv::setIdentity(KF.measurementNoiseCov, cv::Scalar::all(1e-1));        //测量噪声方差矩阵R
    cv::setIdentity(KF.errorCovPost, cv::Scalar::all(1));                  //后验错误估计协方差矩阵P
    rng.fill(KF.statePost, cv::RNG::UNIFORM, 0, frameHeight > frameWidth ? frameWidth : frameHeight); //初始状态值x(0)
    //2.speed predict setup
    predict.predictInit(1);
    static int lost_count = 0;
    static cv::Rect last_lost_objectRect;
    static cv::Rect last_predict_objectRect;
    int frameID = 1;*/

    for (int i = 0; i < objectNumber; i++) {
        MotionPredict predictor;
        predictor.init(frameWidth, frameHeight);
        predictorVec.push_back(predictor);
    }

    on_saveVideoBtn_clicked();

    int count = 1;
    while (((iUseDevice == 0 || iUseDevice == 1) && bStartFlag && videoReader.read(frame))
           || (iUseDevice == 2) ) {

        if (iUseDevice == 2) { // 如果是图像序列，这样获取图片

            char num2stringTmp[128];
            sprintf(num2stringTmp, "%07d", count++);
            std::string num2string = num2stringTmp;

            std::string this_framePath = openImgSeqPath + num2string + ".jpg";
            frame = cv::imread(this_framePath);

            if (frame.empty()) {
                break;
            }
        }

        //开始计时
        double timer = (double)cv::getTickCount();

        //跟新跟踪器
        bool ok = mul_tracker->update(frame);

        //todo: 增加方框预测函数，考虑用卡尔曼滤波/粒子滤波

        //计算FPS
        float fps = cv::getTickFrequency() / ((double)cv::getTickCount() - timer);
        static float fps_ave = 0;
        if (fps_ave != 0){
            fps_ave = (fps_ave + fps) / 2;
        } else {
            fps_ave = fps;
        }

        objectRectVec.clear();

        //绘制目标并更新状态栏信息
        if (ok)
        {
            //如果跟踪到目标则遍历每个目标
            for (auto j = 0; j < static_cast<int>(mul_tracker->getObjects().size()); j++) {
                //获取单个矩形框
                objectRect = mul_tracker->getObjects()[j];
                objectRectVec.push_back(objectRect);

                //获取中心点
                cv::Point point_center_temp;
                point_center_temp.x = floor(objectRect.x + objectRect.width / 2);
                point_center_temp.y = floor(objectRect.y + objectRect.height / 2);

                //更新预测器实际位置
                predictorVec.at(j).correct(point_center_temp);

                //绘制矩形框
                cv::rectangle(frame, mul_tracker->getObjects()[j], cv::Scalar(255, 0, 0), 2, 1);
            }

            //状态栏信息显示
            std::string str3;
            for (auto j = 0; j < static_cast<int>(mul_tracker->getObjects().size()); j++) {
                objectRect = mul_tracker->getObjects()[j];
                str3 = str3 + std::to_string(static_cast<int>(objectRect.x)) + "," + std::to_string(static_cast<int>(objectRect.y)) + "," +
                                    std::to_string(static_cast<int>(objectRect.width)) + "," + std::to_string(static_cast<int>(objectRect.height)) + ",";
            }
            QString qstr3 = QString::fromStdString(str3); //设置显示的内容
            statusLabel->setText(qstr3);
        }
        else {
            qDebug() << "this : " << static_cast<int>(mul_tracker->getObjects().size()) << Qt::endl;
            qDebug() << "[1] : " << mul_tracker->getObjects()[0].x << "," << mul_tracker->getObjects()[0].y
                     << "[2] : " << mul_tracker->getObjects()[1].x << "," << mul_tracker->getObjects()[1].y
                     << "[3] : " << mul_tracker->getObjects()[2].x << "," << mul_tracker->getObjects()[2].y << Qt::endl;

            if (filterMethod == 2) { //如果使用运动预测
                for (int j = 0; j < objectNumber; j++) {
                    if (j == 0) {
                        cv::Point pointPredict = predictorVec.at(j).predict();//预测点位置

                        //qDebug() << "predict:" << count << "per " << j << " - " << pointPredict.x << "," << pointPredict.y << Qt::endl;
                        cv::Rect rectPredict(pointPredict.x - floor(objectRectVecInit.at(j).width / 2), pointPredict.y - floor(objectRectVecInit.at(j).height / 2), objectRectVecInit.at(j).width, objectRectVecInit.at(j).height);

                        if (pointPredict.x != -1 || pointPredict.y != -1) { //预测到了目标的话
                            cv::rectangle(frame, rectPredict, cv::Scalar(255, 0, 0), 2, 1); //画框
                            predictorVec.at(j).correct(pointPredict, false); //用预测的点去更新预测器
                            objectRectVec.push_back(rectPredict);
                            isShowFailure = false;

                            //状态栏信息显示
                            std::string str3;
                            for (int j = 0; j < static_cast<int>(objectRectVec.size()); j++) {
                                objectRect = objectRectVec.at(j);
                                str3 = str3 + std::to_string(static_cast<int>(objectRect.x)) + "," + std::to_string(static_cast<int>(objectRect.y)) + "," +
                                                    std::to_string(static_cast<int>(objectRect.width)) + "," + std::to_string(static_cast<int>(objectRect.height)) + ",";
                            }
                            QString qstr3 = QString::fromStdString(str3); //设置显示的内容
                            statusLabel->setText(qstr3);

                        } else {
                            isShowFailure = true;
                        }
                    }
                    else {
                        //如果跟踪到目标则遍历每个目标
                        for (auto j = 0; j < static_cast<int>(mul_tracker->getObjects().size()); j++) {
                            if (j == 0) {
                                continue;
                            }
                            //获取单个矩形框
                            objectRect = mul_tracker->getObjects()[j];
                            objectRectVec.push_back(objectRect);

                            //获取中心点
                            cv::Point point_center_temp;
                            point_center_temp.x = floor(objectRect.x + objectRect.width / 2);
                            point_center_temp.y = floor(objectRect.y + objectRect.height / 2);

                            //更新预测器实际位置
                            predictorVec.at(j).correct(point_center_temp);

                            //绘制矩形框
                            cv::rectangle(frame, mul_tracker->getObjects()[j], cv::Scalar(255, 0, 0), 2, 1);
                        }

                        //状态栏信息显示
                        std::string str3;
                        for (auto j = 0; j < static_cast<int>(mul_tracker->getObjects().size()); j++) {
                            objectRect = mul_tracker->getObjects()[j];
                            str3 = str3 + std::to_string(static_cast<int>(objectRect.x)) + "," + std::to_string(static_cast<int>(objectRect.y)) + "," +
                                                std::to_string(static_cast<int>(objectRect.width)) + "," + std::to_string(static_cast<int>(objectRect.height)) + ",";
                        }
                        QString qstr3 = QString::fromStdString(str3); //设置显示的内容
                        statusLabel->setText(qstr3);
                    }

                }
            }

            //没有就输出跟踪失败
            if (isShowFailure) {
                cv::putText(frame, "Tracking failure detected", cv::Point(100, 80), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(0, 0, 255), 2);

                //状态栏信息显示
                QString qstr3 = "Tracking failure detected"; //设置显示的内容
                statusLabel->setText(qstr3);
            }

            if (count == 30) {
                qDebug() << "per:" << count << Qt::endl;
                cv::imwrite("../output/123.png", frame);
                cv::waitKey(1000);
            }
        }

        //输出文件
        if (isOutputFile) {
            if (!isShowFailure) {
                for (int j = 0; j < static_cast<int>(objectRectVec.size()); j++) {
                    objectRect = objectRectVec.at(j);

                    // 将rect的左上角坐标转换成rect中心坐标，同时把x y w h都计算占图像宽度或者高度的比例
                    double save_x = static_cast<double>(objectRect.x + 0.5*objectRect.width) / frameWidth;
                    double save_y = static_cast<double>(objectRect.y + 0.5*objectRect.height) / frameHeight;
                    double save_w = static_cast<double>(objectRect.width) / frameWidth;
                    double save_h = static_cast<double>(objectRect.height) / frameHeight;

                    outFile << save_x << " " << save_y << " " << save_w << " " << save_h;
                }
                outFile << std::endl;
                //outFile << objectRect.x << " " << objectRect.y << " " << objectRect.width << " " << objectRect.height << std::endl;
            } else {
                outFile << "0" << " " << "0" << " " << "0" << " " << "0" << std::endl;
            }
        }

        //展示检测算法类型
        if (isShowType) {
            cv::putText(frame, trackerType + " Tracker", cv::Point(100, 20), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(50, 170, 50), 2);
        }

        //表示FPS
        if (isShowFPS) {
            //cv::putText(frame, "FPS : " + std::to_string(int(fps)), cv::Point(100, 50), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(50, 170, 50), 2);
            cv::putText(frame, "fps_ave : " + std::to_string(int(fps_ave)), cv::Point(100, 100), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(40, 40, 160), 2);
        }

        //显示frame
        QImage image(frame.data, frame.cols, frame.rows, static_cast<int>(frame.step), QImage::Format_RGB888);
        ui->frameLabel->setPixmap(QPixmap::fromImage(image.rgbSwapped().scaled(ui->frameLabel->size(),
                                                                               Qt::KeepAspectRatio,Qt::SmoothTransformation)));
        cv::waitKey(1);
        //ui->frameLabel->setScaledContents(true);//注释掉则不铺满窗口，采用比例伸缩

        if (bSaveVideoFlag) { //保存视频
            videoWriter << frame;
        }

        while (bScreenshotFlag) { //非截图状态
            QTime dieTime = QTime::currentTime().addMSecs(2);

            while( QTime::currentTime() < dieTime )

            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        }

        while (bPauseFlag && bStartFlag) { //非暂停状态
            QTime dieTime = QTime::currentTime().addMSecs(10);

            while( QTime::currentTime() < dieTime )

            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        }

        //延时一下，速度太快，不易观察效果，可以注释，打开也不影响帧率计算
        waitMSec(20);
    }

    //on_stopBtn_clicked(); //自动释放
    qDebug() << "finish" << Qt::endl;
}

/*
 * 截图功能
*/
void MainWindow::on_screenshotBtn_clicked()
{
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyyMMdd_hhmmss");
    std::string  screenshotPicPath = "../" + current_date.toStdString() + ".png";

    bScreenshotFlag = true;
    cv::imwrite(screenshotPicPath, frame);
    qDebug() << "screenshot a frame" << Qt::endl;
    bScreenshotFlag = false;
}

/*
 * 按下暂停按钮功能
*/
void MainWindow::on_pauseOrContinueBtn_clicked()
{
    if (bPauseFlag) {
        bPauseFlag = false;
    }else {
        bPauseFlag = true;
    }
}

/*
 * 按下保存视频功能
*/
void MainWindow::on_saveVideoBtn_clicked()
{
    ui->saveVideoBtn->setDisabled(true);

    videoWriter.open(saveVideoPath, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, cv::Size(frameWidth, frameHeight));
    if (videoWriter.isOpened()) {
        bSaveVideoFlag = true;
        qDebug() << "[MainWindow] begin save video" << Qt::endl;
    } else {
        qDebug() << "[MainWindow] failed to save video" << Qt::endl;
        ui->saveVideoBtn->setDisabled(false);
    }
}

/*
 * 按下停止按钮功能
*/
void MainWindow::on_stopBtn_clicked()
{
    //禁用一些控件并使能一些控件
    ui->openCameraAct->setDisabled(false);
    ui->openLocalVideoAct->setDisabled(false);
    ui->startBtn->setDisabled(true);
    ui->selectObjectBtn->setDisabled(true);
    ui->pauseOrContinueBtn->setDisabled(true);
    ui->screenshotBtn->setDisabled(true);
    ui->saveVideoBtn->setDisabled(true);
    ui->stopBtn->setDisabled(true);

    if (!bStartFlag) {
        qDebug() << "[MainWindow]: not start, could not stop" << Qt::endl;
        return;
    }
    bStartFlag = false;

    if (iUseDevice != -1) {
        videoReader.release();//释放
        iUseDevice = -1;
    }

    if (bHadSetObjectFlag) {//释放跟踪器
        bHadSetObjectFlag = false;
        for (int j = 0; j < objectNumber; j++) {
            trackers.at(j).release();
        }
        objectRect2dVec.clear();
        trackers.clear();
        mul_tracker->clear();
        mul_tracker->~MultiTracker();
    }

    if (bSaveVideoFlag) {
        bSaveVideoFlag = false;
        videoWriter.release();//释放视频保存器
    }

    if (isOutputFile) {
        outFile.close();
    }

    cv::Mat blankMat(1920, 1080, CV_8UC3, cv::Scalar::all(255));
    QImage image(blankMat.data, blankMat.cols, blankMat.rows, static_cast<int>(blankMat.step), QImage::Format_RGB888);
    ui->frameLabel->setPixmap(QPixmap::fromImage(image.rgbSwapped().scaled(ui->frameLabel->size(),
                                                                           Qt::KeepAspectRatio,Qt::SmoothTransformation)));
    cv::waitKey(1);
    ui->frameLabel->setScaledContents(true); //注释掉则不铺满窗口，采用比例伸缩
}


void MainWindow::on_showTrackBtn_clicked()
{
    if (isShowTrack) {
        isShowTrack = false;
    } else {
        draw_track_predict.track_clear();
        isShowTrack = true;
    }
}
