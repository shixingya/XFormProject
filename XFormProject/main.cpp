#pragma execution_character_set("utf-8")
#include "mainwindow.h"
#include "global.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "XFormProject_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow w;
    QString windowTitle;
    windowTitle.append(DisplayConstants::APP_NAME)
               .append("_").append(DisplayConstants::APP_VERSION);
    w.setWindowTitle(windowTitle);
    w.setWindowIcon(QIcon(DisplayConstants::APP_ICON_PATH));
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    auto screenWidth = screenGeometry.width();
    auto screenHeight = screenGeometry.height();
    auto windowWidth = screenWidth * 0.9;
    auto windowHeight = screenHeight * 0.8;
    auto windowX = (screenWidth - windowWidth) / 2;
    auto windowY = (screenHeight - windowHeight) / 2;
    w.resize(windowWidth, windowHeight);
    w.move(windowX, windowY);
    w.show();
    return a.exec();
}
