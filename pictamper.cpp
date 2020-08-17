#include "pictamper.h"
#include <opencv2/opencv.hpp>
#include <QDebug>
#include <iostream>

//加入椒盐噪声
void salt(cv::Mat image, int n)
{
    int i,j;
    for (int k=0; k<n; k++)
    {
        // rand()是随机数生成器
        i= rand()%image.cols;
        j= rand()%image.rows;

        //value
        int value = rand()%2;
        value = value == 0 ? 0 : 255;

        if (image.type() == CV_8UC1)
        { // 灰度图像
            image.at<uchar>(j,i)= value;
        }
        else if (image.type() == CV_8UC3)
        { // 彩色图像
            image.at<cv::Vec3b>(j,i)[0]= value;
            image.at<cv::Vec3b>(j,i)[1]= value;
            image.at<cv::Vec3b>(j,i)[2]= value;
        }
    }
}


picTamper::picTamper()
{
    //获取输入路径下所有文件名称
    cv::String path = "D:/MyStudyProj/mulTrack/data/uav0000186_00250_vtamp2/*.jpg";
    std::vector<cv::String> all_pic_path;
    cv::glob(path, all_pic_path, false);

    qDebug() << "[picTamper] init with " << QString::fromStdString(all_pic_path[0]) << Qt::endl;


    //初始化读取第一帧临时图片
    cv::Mat frameTmp = cv::imread(all_pic_path[0]);
    if (frameTmp.empty()) { //图片非空
        qFatal("[picTamper] frameTmp is empty");
    }

    //记录图像序列的 宽度 高度 通道数 图片总数
    int width = frameTmp.cols;
    int height = frameTmp.rows;
    int channels = frameTmp.channels();
    int total_pic_num = static_cast<int>(all_pic_path.size());
    qDebug() << "[picTamper] width=" << width << " height=" << height
             << " channels=" << channels << " total_pic_num=" << total_pic_num << Qt::endl;

    //遍历需要修改的几张图片
    for (int i = static_cast<int>(total_pic_num*0.15); i < static_cast<int>(total_pic_num*0.33); i++) {
        //计数
        static int count = 0;

        //读取帧图片
        cv::Mat frame = cv::imread(all_pic_path[i]);
        if (frame.empty()) { //图片非空
            qFatal("[picTamper] frameTmp is empty");
        }

        //修改处理
        double y_dev = static_cast<double>(0.6*(1+0.01*count));
        count++;
        cv::Mat frameROT(frame,cv::Rect(cv::Point(static_cast<int>(width*0.3), static_cast<int>(height*y_dev)),
                                      cv::Point(static_cast<int>(width*0.53), static_cast<int>(height*(y_dev+0.07)))));
        //cv::GaussianBlur(frameROT, frameROT, cv::Size(13, 13), 10, 100); //方法1 高斯滤波
        salt(frameROT, 30000); //方法2 椒盐噪声

        //实时显示
        cv::imshow("frame", frame);
        cv::waitKey(100);

        //写到本地，覆盖原图片
        cv::imwrite(all_pic_path[i], frame);
    }
    cv::destroyAllWindows(); //删除所有cv窗口
}
