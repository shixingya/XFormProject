#pragma execution_character_set("utf-8")
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "XGraphicsItem.h"
XGraphicsItem *CreateRectAngleItem(qreal x, qreal y, qreal w, qreal h) {
    XGraphicsItem *resultItem = new XGraphicsItem();
    QPolygonF polygon;
    polygon << QPointF(x, y) << QPointF(x + w, y) << QPointF(x + w, y + h)
            << QPointF(x, y + h);
    resultItem->setPolygon(polygon);
    return resultItem;
}
XGraphicsItem *CreatePolygonItem(const QPolygonF& polygon) {
    XGraphicsItem *resultItem = new XGraphicsItem();
    resultItem->setPolygon(polygon);
    return resultItem;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui_(new Ui::MainWindow) {
    ui_->setupUi(this);
    scene_ = new QGraphicsScene(nullptr);
    ui_->graphicsView->setRenderHint(QPainter::Antialiasing);
    // 创建一个矩形
    XGraphicsItem *rectangleItem = CreateRectAngleItem(0, 0, 100, 100);
    scene_->addItem(rectangleItem);
    //创建一个三角形
    XGraphicsItem *triangleItem = CreatePolygonItem(
                                      QPolygonF({QPointF(0, 0), QPointF(100, 0), QPointF(50, 100)}));
    triangleItem->setPos(150, 0);
    scene_->addItem(triangleItem);
    //创建一个多边形
    XGraphicsItem *polygonItem = CreatePolygonItem(QPolygonF({QPointF(0, 0), QPointF(44, 13), QPointF(100, 0), QPointF(100, 100), QPointF(0, 100)}));
    polygonItem->setPos(0, 150);
    scene_->addItem(polygonItem);
    // 创建一个仿射矩形
    XGraphicsItem *affineRectangleItem = CreatePolygonItem(
            QPolygonF({QPointF(0, 0), QPointF(100, 0), QPointF(75, 100), QPointF(25, 100)}));
    affineRectangleItem->setPos(150, 150);
    scene_->addItem(affineRectangleItem);
    // 创建一个圆形
    // 设置圆心和半径
    QPointF center(50.0, 50.0);
    qreal radius = 150.0;
    // 计算圆上的点坐标
    const int numPoints = 100;  // 圆上的点数
    const qreal angleIncrement = 2.0 * M_PI / numPoints;
    QPolygonF polygonCircle;
    for (int i = 0; i < numPoints; ++i) {
        qreal angle = i * angleIncrement;
        qreal x = center.x() + radius * std::cos(angle);
        qreal y = center.y() + radius * std::sin(angle);
        polygonCircle << QPointF(x, y);
    }
    XGraphicsItem *circleItem = CreatePolygonItem(polygonCircle);
    circleItem->setPos(0, 300);
    scene_->addItem(circleItem);
    // 创建一个不规则点云
    XGraphicsItem *pointCloudItem = CreatePolygonItem(
                                        QPolygonF({QPointF(0, 0), QPointF(10, 20), QPointF(39, 40), QPointF(60, 60)}));
    pointCloudItem->setPos(150, 300);
    scene_->addItem(pointCloudItem);
    ui_->graphicsView->setScene(scene_);
}

MainWindow::~MainWindow() {
    delete ui_;
}

