#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QWidget>

#include <QtWidgets>
class GraphicsView: public QGraphicsView {
  public:
    GraphicsView(QWidget* parent = nullptr);
    //是否绘制背景
    enum BACK_GROUND_TYPE {
        UNDEFINED,
        GRID,
        STRIPES,
        CUSTOM1,
    };
    //设置当前图片
    void SetCurrentImage(const QString &imagePath);
    void SetBackGroundType(BACK_GROUND_TYPE type = BACK_GROUND_TYPE::UNDEFINED);
  private:
    void drawGridBackGround(QPainter *painter, const QRectF &rect);
  protected:
    virtual void wheelEvent(QWheelEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void drawBackground(QPainter *painter, const QRectF &rect) override;
    virtual void resizeEvent(QResizeEvent* event) override;
    virtual void leaveEvent(QEvent* event) override;

  private:
    QGraphicsPixmapItem *_pixmapItem = nullptr;
    QCursor _rotateCursor;
    BACK_GROUND_TYPE _backgroundType = BACK_GROUND_TYPE::UNDEFINED;
};

#endif // GRAPHICSVIEW_H
