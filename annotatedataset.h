#ifndef ANNOTATEDATASET_H
#define ANNOTATEDATASET_H

/*
 * 这个类用于标注图片数据，先set_prompt_level设置提示信息等级，默认3，所有信息都提示；设置输入，输出文件路径，否则
 * 默认值，设置最大类被数目，否则默认3；init初始化，然后可以start开始标注，标注第一轮标注所有图片第一类，然后图片
 * 从头开始，标注第二类，类推
 * ganyd 20200807
*/

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>

class AnnotateDataset
{
public:
    enum PromptLevel{ //信息提示等级
        fatal = 1,
        debug = 2,
        info = 3,
        none,
    };

public:
    AnnotateDataset();

    void set_input_path(std::string str);

    void set_output_path(std::string str);

    void set_maxclass_num(int num);

    int init();

    void start();

    void set_prompt_level(PromptLevel n);

private:

private:
    std::string input_path = "../data/uav0000186_00250_v/";
    std::string output_path = "D:/MyStudyProj/mulTrack/output/";
    int maxclass_num = 3;
    std::vector<cv::String> all_pic_path;

    int width = 0;
    int height = 0;
    int channels = 0;
    int total_pic_num = 0;

    int prompt_level = none;

};

#endif // ANNOTATEDATASET_H
