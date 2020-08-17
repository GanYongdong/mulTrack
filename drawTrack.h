#ifndef DRAW_TRACK_H
#define DRAW_TRACK_H

/***
 * 在图像上绘制轨迹
 * 2020.8.12
***/

#include <iostream>
#include <opencv2/opencv.hpp>

class drawTrack
{
public:
    drawTrack();

    cv::Mat track_update(cv::Mat img, cv::Point point);

    void track_clear();

private:
    std::vector<cv::Point> pointVec;
};

#endif // DRAW_TRACK_H
