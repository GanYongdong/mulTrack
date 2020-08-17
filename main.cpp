#include "mainwindow.h"
#include <QApplication>
#include <annotatedataset.h>
#include <pictamper.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

//    AnnotateDataset ann;
//    ann.set_prompt_level(AnnotateDataset::info);
//    ann.init();
//    ann.start();

//    picTamper pic_tamper_ins1;

    return a.exec();
}
