#ifndef drawRect_H
#define drawRect_H

/***
 * 鼠标拖动绘制矩形框
***/

#include <QMainWindow>
#include <QImage>
#include <QPixmap>
#include <QLabel>
#include <QPainter>
#include <QMouseEvent>

class drawRect : public QMainWindow
{
    Q_OBJECT

public:
    drawRect(QWidget *parent = 0);
    ~drawRect();
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    QImage src;
    QPixmap pixmap;
    QLabel label;
    QPainter *painter;
    QPoint start;
    QPoint end;
    bool is_start;
    QString str_dian;
    QString str_width;
    QString str_height;
};

#endif // drawRect_H
