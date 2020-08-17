#include "annotatedataset.h"
#include <QDebug>
#include <iterator>
#include <regex>
#include <fstream>

AnnotateDataset::AnnotateDataset()
{
}

/*
 * 设置读取图片的路径
 */
void AnnotateDataset::set_input_path(std::string str)
{
    input_path = str;

    if (prompt_level >= info) {
        qInfo() << "[AnnotateDataset] input_path: " << QString::fromStdString(input_path) << Qt::endl;
    }
}

/*
 * 设置输出文件的路径
 */
void AnnotateDataset::set_output_path(std::string str)
{
    output_path = str;

    if (prompt_level >= info) {
        qInfo() << "[AnnotateDataset] output_path: " << QString::fromStdString(output_path) << Qt::endl;
    }
}

/*
 * 设置最大类别数目。默认3
 */
void AnnotateDataset::set_maxclass_num(int num)
{
    maxclass_num = num;
    if (prompt_level >= info) {
        qInfo() << "[AnnotateDataset] maxclass_num: " << maxclass_num << Qt::endl;
    }
}

/*
 * 初始化标注程序，获取视频基本信息
 */
int AnnotateDataset::init()
{
    //输入路径非空
    if (input_path.empty()) {
        if (prompt_level >= fatal) {
            qFatal("[AnnotateDataset] could not find the dir of input path.");
        }
        return 0;
    }

    //输出路径非空
    if (output_path.empty()) {
        if (prompt_level >= fatal) {
            qFatal("[AnnotateDataset] could not find the dir of output path.");
        }
        return 0;
    }

    //获取输入路径下所有文件名称
    cv::String path = input_path + "/*.jpg";
    cv::glob(path, all_pic_path, false);
    if (prompt_level >= debug) {
        qDebug() << "[AnnotateDataset] init with " << QString::fromStdString(all_pic_path[0]) << Qt::endl;
    }

    //初始化读取第一帧临时图片
    cv::Mat frameTmp = cv::imread(all_pic_path[0]);
    if (frameTmp.empty()) { //图片非空
        if (prompt_level >= fatal) {
            qFatal("[AnnotateDataset] frameTmp is empty");
        }
        return 0;
    }

    //记录图像序列的 宽度 高度 通道数 图片总数
    width = frameTmp.cols;
    height = frameTmp.rows;
    channels = frameTmp.channels();
    total_pic_num = static_cast<int>(all_pic_path.size());
    if (prompt_level >= debug) {
        qDebug() << "[AnnotateDataset] width=" << width << " height=" << height
                 << " channels=" << channels << " total_pic_num=" << total_pic_num << Qt::endl;
    }

    return 1;
}

/*
 * 开始标注
*/
void AnnotateDataset::start()
{
    // 对每个类别循环一次所有图片，每次大循环只标注一类，按照提示操作
    for (int k = 1; k <= maxclass_num; k++) {
        if (prompt_level >= info) {
            qDebug() << "[AnnotateDataset] Please mark the No." << k << " category, 'enter' key to start the next frame, 'c' key to remark the current frame" << Qt::endl;
        }

        std::string winName = "请标注类别：" + std::to_string(k);
        cv::namedWindow(winName);
        cv::Scalar scalar;
        std::string save_csv_file_path;

        //根据不同类别，给框不同颜色和保存在不同文件中，之后另外再合并
        switch (k) {
        case 1:
            scalar = cv::Scalar(255,0,0);
            save_csv_file_path = output_path + "class1.csv";
            break;
        case 2:
            scalar = cv::Scalar(0,255,0);
            save_csv_file_path = output_path + "class2.csv";
            break;
        case 3:
            scalar = cv::Scalar(0,0,255);
            save_csv_file_path = output_path + "class3.csv";
            break;
        default:
            if (prompt_level >= fatal) {
                qFatal("[AnnotateDataset] class over max class num");
            }
            break;
        }
        if (prompt_level >= debug) {
            qDebug() << "save_csv_file_path: " << QString::fromStdString(save_csv_file_path) << Qt::endl;
        }
        std::ofstream save_csv_file(save_csv_file_path); //打开输出文件

        // 循环遍历所有图片
        for (int i = 0; i < total_pic_num; i++) {
            cv::Mat frame = cv::imread(all_pic_path[i]); //读取帧图片
            cv::Rect rect(0,0,0,0); //默认保存标注的框

            while (rect.x == 0 && rect.y ==0 && rect.width == 0 && rect.height == 0) {
                rect = cv::selectROI(winName, frame); //标注
            }

            if (prompt_level >= debug) {
                qDebug() << rect.x << " " << rect.y << " " << rect.width << " " << rect.height << Qt::endl;
            }

            cv::rectangle(frame, rect, scalar);
            cv::imwrite(all_pic_path[i], frame);

            // 将rect的左上角坐标转换成rect中心坐标，同时把x y w h都计算占图像宽度或者高度的比例
            double save_x = static_cast<double>(rect.x + 0.5*rect.width) / width;
            double save_y = static_cast<double>(rect.y + 0.5*rect.height) / height;
            double save_w = static_cast<double>(rect.width) / width;
            double save_h = static_cast<double>(rect.height) / height;

            // 保存label数据到文件
            save_csv_file << k << "," << all_pic_path[i] << "," << save_x << "," << save_y << "," << save_w << "," << save_h << std::endl;
            if (prompt_level >= debug) {
                qDebug() << save_x << " " << save_y << " " << save_w << " " << save_h << Qt::endl;
            }

            // 显示进度，第几帧/总帧数
            if (prompt_level >= info && i%1==0) {
                qDebug() << "[AnnotateDataset] " << i << "/" << total_pic_num << Qt::endl;
            }

            cv::waitKey(10);
        }
        save_csv_file.close(); //关闭输出文件
    }

    //结束，删除所有窗
    cv::destroyAllWindows();
}

/*
 * 设置提示信息的等级
*/
void AnnotateDataset::set_prompt_level(AnnotateDataset::PromptLevel n)
{
    prompt_level = n;
    if (prompt_level > 0) {
        qInfo() << "prompt_level: " << prompt_level << Qt::endl;
    }
}
