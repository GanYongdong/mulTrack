#ifndef PREDICTPOINT_H
#define PREDICTPOINT_H

#include <opencv2/opencv.hpp>

class predict_point
{
public:
    predict_point();

    void predictInit(int step_);
    void corrent(cv::Point realPoint, bool isTracked = true);
    cv::Point predict();
    //cv::Point predict_wihoutCorrent();

private:
    cv::Point laster_point;
    cv::Point last_point;
    cv::Point current_point;

    int step;

    int reStartFlag = 1;

    //std::vector<cv::Point> pointVec;

    //cv::Point s1, s2, a;
};

#endif // PREDICTPOINT_H
