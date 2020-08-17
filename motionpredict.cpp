#include "motionpredict.h"
#include <QDebug>

MotionPredict::MotionPredict()
{

}

void MotionPredict::init(int _width, int _height)
{
    width = _width;
    height = _height;
}

void MotionPredict::correct(cv::Point point, bool bIsRealValue)
{
    pointVec.push_back(point);
    if (pointVec.size() > 5) {
        std::vector<cv::Point>::iterator it = pointVec.begin();//获取第一个元素的位置
        pointVec.erase(it);//删除第一个元素
    }

    if (bIsRealValue == true) {
        the_number_of_consecutive_predictions = 0;
    }
}

cv::Point MotionPredict::predict()
{
    the_number_of_consecutive_predictions++;

    cv::Point retPoint(-1, -1);
    if (pointVec.size() < 5) {
        qDebug() << "MotionPredict.predict -- pointVec.size() < 10" << Qt::endl;
        return retPoint;
    }

    if (the_number_of_consecutive_predictions > the_max_number_of_consecutive_predictions) {
        qDebug() << "MotionPredict.predict -- over max predict steps" << Qt::endl;
        return retPoint;
    }


    cv::Point2d s1 = pointVec.at(3) - pointVec.at(1);
    cv::Point2d s2 = pointVec.at(4) - pointVec.at(3);
    //cv::Point2d s3 = pointVec.at(3) - pointVec.at(2);
    //cv::Point2d s4 = pointVec.at(4) - pointVec.at(3);

    cv::Point2d v1 = s2 - s1;
    //cv::Point2d v2 = s3 - s2;
    //cv::Point2d v3 = s4 - s3;

    //cv::Point2d a1 = v2 - v1;
    //cv::Point2d a2 = v3 - v2;

    //cv::Point2d ea = a2 - a1;

    cv::Point2d lastP(pointVec.at(2).x, pointVec.at(2).y);
    cv::Point2d retPointed = (v1 + s2) * 0.01 + lastP;

    retPoint = cv::Point((int)retPointed.x, (int)retPointed.y);

    retPoint.x = retPoint.x < 0 ? 0 : retPoint.x;
    retPoint.x = retPoint.x > width ? width : retPoint.x;
    retPoint.y = retPoint.y < 0 ? 0 : retPoint.y;
    retPoint.y = retPoint.y > height ? height : retPoint.y;

    //qDebug() << "MotionPredict.predict -- predict " << retPoint.x << " " << retPoint.y << Qt::endl;

    return retPoint;
}

cv::Point MotionPredict::predict2()
{
    qDebug() << "this" << Qt::endl;
    the_number_of_consecutive_predictions++;

    cv::Point retPoint(-1, -1);
    if (pointVec.size() < 10) {
        qDebug() << "MotionPredict.predict -- pointVec.size() < 3" << Qt::endl;
        return retPoint;
    }

    if (the_number_of_consecutive_predictions > the_max_number_of_consecutive_predictions) {
        qDebug() << "MotionPredict.predict -- over max predict steps" << Qt::endl;
        return retPoint;
    }

    cv::Point2d s1 = pointVec.at(1) - pointVec.at(0);
    cv::Point2d s2 = pointVec.at(2) - pointVec.at(1);
    //cv::Point2d s3 = pointVec.at(3) - pointVec.at(2);
    //cv::Point2d s4 = pointVec.at(4) - pointVec.at(3);

    cv::Point2d v1 = s2 - s1;
    //cv::Point2d v2 = s3 - s2;
    //cv::Point2d v3 = s4 - s3;

    //cv::Point2d a1 = v2 - v1;
    //cv::Point2d a2 = v3 - v2;

    //cv::Point2d ea = a2 - a1;

    cv::Point2d lastP(pointVec.at(2).x, pointVec.at(2).y);
    cv::Point2d retPointed = (v1 + s2) * 0.02 + lastP;

    retPoint = cv::Point((int)retPointed.x, (int)retPointed.y);

    retPoint.x = retPoint.x < 0 ? 0 : retPoint.x;
    retPoint.x = retPoint.x > width ? width : retPoint.x;
    retPoint.y = retPoint.y < 0 ? 0 : retPoint.y;
    retPoint.y = retPoint.y > height ? height : retPoint.y;

    return retPoint;
}
