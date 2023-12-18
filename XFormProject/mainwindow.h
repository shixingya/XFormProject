#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE
class ROIImageItem;
class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

  private slots:
    void on_pushButton_clicked();

  private:
    Ui::MainWindow *ui_;
    QGraphicsScene *scene_ = nullptr;

    ROIImageItem *_ROIImageItem = nullptr;
};
#endif // MAINWINDOW_H
