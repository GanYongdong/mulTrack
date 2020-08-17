#include "predictpoint.h"

predict_point::predict_point()
{

}

void predict_point::predictInit(int step_)
{
    step = step_;
}

void predict_point::corrent(cv::Point realPoint, bool isTracked)
{
    if (reStartFlag > 0 && isTracked == true) {
        reStartFlag++;
    }

    if (isTracked == false) {
        reStartFlag = 1;

        cv::Point s1 = last_point - laster_point;
        cv::Point s2 = current_point - last_point;

        cv::Point a = s2 - s1;

        cv::Point s3 = s2 + a*step*0.1;

        laster_point = last_point;
        last_point = current_point;
        //current_point = realPoint;
        current_point = current_point + s3;

    } else {
        laster_point = last_point;
        last_point = current_point;
        current_point = realPoint;
    }

}

cv::Point predict_point::predict()
{
    cv::Point retP = current_point;
    if (reStartFlag == 0 || reStartFlag >= 3) {
        reStartFlag = 0;


        cv::Point s1 = last_point - laster_point;
        cv::Point s2 = current_point - last_point;

        cv::Point a = s2 - s1;

        cv::Point s3 = s2 + a*step*0.05;
        retP = current_point + s3;
    }

    return retP;
}

