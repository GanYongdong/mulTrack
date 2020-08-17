#-------------------------------------------------
#
# Project created by QtCreator 2020-04-04T13:37:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mulTrack
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
        drawRect.cpp \
        libKalman/Kalman2D.cpp \
        libKalman/Matrix.cpp \
        libKalman/Random.cpp \
        libKalman/predictpoint.cpp \
        drawTrack.cpp \
        motionpredict.cpp \
        annotatedataset.cpp \
        mymsg.cpp \
        pictamper.cpp

HEADERS += \
        mainwindow.h \
        drawRect.h \
        libKalman/Kalman2D.h \
        libKalman/Matrix.h \
        libKalman/Random.h \
        libKalman/predictpoint.h \
        drawTrack.h \
        motionpredict.h \
        annotatedataset.h \
        mymsg.h \
        pictamper.h

FORMS += \
        mainwindow.ui


#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../lib_install/opencv/opencv420/build_with_contrib2/install/x64/vc16/lib/ -lopencv_world420
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../lib_install/opencv/opencv420/build_with_contrib2/install/x64/vc16/lib/ -lopencv_world420d
#else:unix: LIBS += -L$$PWD/../../../lib_install/opencv/opencv420/build_with_contrib2/install/x64/vc16/lib/ -lopencv_world420
#INCLUDEPATH += $$PWD/../../../lib_install/opencv/opencv420/build_with_contrib2/install/include
#DEPENDPATH += $$PWD/../../../lib_install/opencv/opencv420/build_with_contrib2/install/include
#INCLUDEPATH += $$PWD/../../../lib_install/opencv/opencv420/build_with_contrib2/install/include/opencv
#DEPENDPATH += $$PWD/../../../lib_install/opencv/opencv420/build_with_contrib2/install/include/opencv


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../Software/opencv/opencv440/build_with_contrib/install/x64/vc16/lib/ -lopencv_world440
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../Software/opencv/opencv440/build_with_contrib/install/x64/vc16/lib/ -lopencv_world440d
else:unix: LIBS += -L$$PWD/../../../Software/opencv/opencv440/build_with_contrib/install/x64/vc16/lib/ -lopencv_world440
INCLUDEPATH += $$PWD/../../../Software/opencv/opencv440/build_with_contrib/install/include
DEPENDPATH += $$PWD/../../../Software/opencv/opencv440/build_with_contrib/install/include
INCLUDEPATH += $$PWD/../../../Software/opencv/opencv440/build_with_contrib/install/include/opencv2
DEPENDPATH += $$PWD/../../../Software/opencv/opencv440/build_with_contrib/install/include/opencv2
