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
    w.show();
    return a.exec();
}
