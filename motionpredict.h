#ifndef MOTIONPREDICT_H
#define MOTIONPREDICT_H

/***
 * 运动预测器类
 * 2020.8.12
***/

#include <iostream>
#include <opencv2/opencv.hpp>

class MotionPredict
{
public:
    MotionPredict();

    void init(int _width, int _height);

    void correct(cv::Point point, bool bIsRealValue = true);

    cv::Point predict();
    cv::Point predict2();

public:
    std::vector<cv::Point> pointVec;

    int width = 0;
    int height = 0;

    int the_number_of_consecutive_predictions = 0;

    int the_max_number_of_consecutive_predictions = 30;
};

#endif // MOTIONPREDICT_H
