#include "drawTrack.h"
#include <QDebug>

drawTrack::drawTrack()
{

}

cv::Mat drawTrack::track_update(cv::Mat img, cv::Point point)
{
    if (img.empty()) {
        qDebug() << "[drawTrack] track_update's img is empty" << endl;
        return img;
    }

    point.x = point.x > img.cols ? img.cols : point.x;
    point.x = point.x < 0 ? 0 : point.x;
    point.y = point.y > img.rows ? img.rows : point.y;
    point.y = point.y < 0 ? 0 : point.y;

    pointVec.push_back(point);

    for (size_t i = 0; i < pointVec.size() - 1; i++) {
        cv::line(img, pointVec[i], pointVec[i+1], cv::Scalar(0,0,255), 3);
    }

    return img;
}

void drawTrack::track_clear()
{
    pointVec.clear();
}
