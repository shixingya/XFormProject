QT       += core gui network concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
# 设置工程名称
TARGET = NVDumpTracer
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
win32 {
    QMAKE_CXXFLAGS += /std:c++17
    SYS_LIB_ROOT_PATH = D:/luster_dev
    VISION_LAB_LIB_PATH = D:/luster_visionlab
    OPENCV_PATH = $${SYS_LIB_ROOT_PATH}/opencv_4.6.0/no_cuda
    INCLUDEPATH += $${OPENCV_PATH}/include
    LIBS += -L$${OPENCV_PATH}/x64/vc16/lib

    # Debug环境配置
    CONFIG(debug, debug|release) {
        # Debug环境特定配置
        LIBS += -lopencv_core460d -lopencv_imgproc460d -lopencv_highgui460d -lopencv_imgcodecs460d
    }

    # Release环境配置
    CONFIG(release, debug|release) {
        # Release环境特定配置
        LIBS += -lopencv_core460 -lopencv_imgproc460 -lopencv_highgui460 -lopencv_imgcodecs460
    }

    # 定义图标文件的路径
    ICON = ./app.ico

    # 配置生成的可执行文件的图标
    win32:RC_ICONS += $$ICON
    QMAKE_TARGET_BUNDLE = $$ICON
}

SOURCES += \
    FileDeletionThread.cpp \
    FileUploadWorker.cpp \
    InnerImpl.cpp \
    ReportCenter.cpp \
    ScreenCapture.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    FileDeletionThread.h \
    FileUploadWorker.h \
    InnerImpl.h \
    ReportCenter.h \
    ScreenCapture.h \
    contants/constants.h \
    contants/constants_codes.h \
    contants/constants_displays.h \
    global.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    XFormProject_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources/res.qrc
