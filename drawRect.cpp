#include "drawRect.h"
#include <iostream>

drawRect::drawRect(QWidget *parent)
    : QMainWindow(parent)
{
    src.load("/home/username/Pictures/1920x1080_2.jpg");//加载QImage
    pixmap = QPixmap::fromImage(src);//转换成QPixmap
    label.setPixmap(pixmap);//label设置Pixmap
    painter = new QPainter(this);//在当前drawRect创建画笔painter
    is_start = false;//是不是起点的标志位
    update();//更新图像,内部调用了paintEvent()
}

drawRect::~drawRect()
{

}

void drawRect::paintEvent(QPaintEvent *event)
{
    painter->begin(this);//开始画

    painter->drawPixmap(0, 0, 1920, 1080, pixmap);//在(0,0),宽1920,高1080的位置绘制pixmap图像

    painter->setPen(QPen(Qt::red, 1, Qt::SolidLine));//设置画笔为红色,粗细为1
    painter->drawRect(QRect(start.x(), start.y(), end.x() - start.x(), end.y() - start.y()));//绘制矩形

    painter->setPen(QPen(Qt::green, 1, Qt::SolidLine));//设置画笔为蓝色,粗细为1
    str_dian = QString::number(start.x()) + "," + QString::number(start.y());//设置显示的顶点坐标,坐标位置根据起点start
    painter->drawText(start.x(), start.y(), str_dian);//将顶点坐标画出来

    str_width = QString::number(end.x() - start.x());//设置显示的矩形宽度
    painter->drawText((start.x() + end.x()) / 2, start.y(), str_width);//[0.5*(x1 + x2), y]

    str_height = QString::number(end.y() - start.y());//设置显示的矩形高度,
    painter->drawText(start.x(), (start.y() + end.y()) / 2, str_height);//[x, 0.5*(y1 + y2)]

    painter->end();//结束绘画
}

//鼠标按下事件,每次鼠标按下都会触发该函数
void drawRect::mousePressEvent(QMouseEvent *event)
{
    if(is_start == false && event->button() == Qt::LeftButton)//如果当前没有开始而且鼠标左键点下
    {
        start = event->pos();//获取当前坐标为start
        is_start = true;//设置起点已经有了
    }
}

//鼠标释放事件,每次鼠标释放都会触发该函数
void drawRect::mouseReleaseEvent(QMouseEvent *event)
{
    if(is_start == true && event->button() == Qt::LeftButton)//如果起点已经有了而且鼠标坐标松开
    {
        end = event->pos();//获取当前位置为end
        is_start = false;//结束,起点复位为false
        update();
    }
}

//鼠标移动时间,每次鼠标移动都会触发该函数
void drawRect::mouseMoveEvent(QMouseEvent *event)
{
    if(is_start == true)//如果起点有了
    {
        end = event->pos();//获取当前位置为end
        update();//更新显示
    }
}




