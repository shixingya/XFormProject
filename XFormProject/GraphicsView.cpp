#pragma execution_character_set("utf-8")
#include "GraphicsView.h"
QLabel *label = nullptr;
GraphicsView::GraphicsView(QWidget* parent) : QGraphicsView(parent) {
    //setDragMode(QGraphicsView::ScrollHandDrag);
    QPixmap pix(":/image/rotate.png");
    QSize size(18, 18);
    pix = pix.scaled(size, Qt::KeepAspectRatio);
    _rotateCursor = QCursor(pix, pix.height() / 8, pix.width() / 8);
    // 设置视图属性
    setRenderHint(QPainter::Antialiasing);
    setRenderHint(QPainter::TextAntialiasing);
    setViewportUpdateMode(FullViewportUpdate);
    setCacheMode(CacheBackground);
    setMouseTracking(true); // 开启鼠标追踪
    label = new QLabel(this); // 创建标签用于显示鼠标位置
    label->setStyleSheet("background-color: white; padding: 5px;");
    label->hide();
}

void GraphicsView::wheelEvent(QWheelEvent* event) {
    if (event->modifiers() == Qt::ControlModifier) {
        int delta = event->angleDelta().y();
        qreal scaleFactor = (delta > 0) ? 1.2 : 1.0 / 1.2;
        scale(scaleFactor, scaleFactor);
        event->accept();
    } else {
        QGraphicsView::wheelEvent(event);
    }
}

void GraphicsView::mousePressEvent(QMouseEvent* event) {
    QGraphicsView::mousePressEvent(event);
}

void GraphicsView::mouseMoveEvent(QMouseEvent* event) {
    QGraphicsView::mouseMoveEvent(event);
    QPointF scenePos = mapToScene(event->pos());
    label->setText(QString("Mouse Position: (%1, %2)").arg(scenePos.x()).arg(scenePos.y()));
    label->move(event->globalPos().x() + 10, event->globalPos().y() + 10);
    label->show();
}

void GraphicsView::mouseReleaseEvent(QMouseEvent* event) {
    QGraphicsView::mouseReleaseEvent(event);
}

void GraphicsView::drawGridBackGround(QPainter * painter, const QRectF & rect) {
    // 设置格子大小和颜色
    static int gridSize = 20;
    const int discardCnt = 1;
    // 设置格子的颜色
    QColor oddColor, evenColor;
    oddColor.setNamedColor("#8F8F8F");
    evenColor.setNamedColor("#707070");
    // 计算网格起始位置
    int startX = static_cast<int>(rect.left()) - (static_cast<int>(rect.left()) % gridSize) - gridSize * discardCnt;
    int startY = static_cast<int>(rect.top()) - (static_cast<int>(rect.top()) % gridSize) - gridSize * discardCnt;
    // 计算网格终止位置
    int rows = static_cast<int>(rect.right()) + gridSize * discardCnt;
    int cols = static_cast<int>(rect.bottom()) + gridSize * discardCnt;
    // 绘制网格背景
    painter->save();
    painter->setPen(Qt::NoPen);
    for (int row = startX; row < rows; row += gridSize) {
        for (int col = startY; col < cols; col += gridSize) {
            QColor cellColor = ((row / gridSize) + (col / gridSize)) % 2 == 0 ? oddColor : evenColor;
            painter->setBrush(cellColor);
            painter->drawRect(row, col, gridSize, gridSize);
        }
    }
    painter->restore();
}
void GraphicsView::drawBackground(QPainter* painter, const QRectF& rect) {
    QGraphicsView::drawBackground(painter, rect);
    switch(_backgroundType) {
    case GRID:
        drawGridBackGround(painter, rect);
        break;
    default:
        break;
    }
}
void  GraphicsView::resizeEvent(QResizeEvent* event)  {
    QGraphicsView::resizeEvent(event);
    fitInView(sceneRect(), Qt::KeepAspectRatio);
}
void GraphicsView::leaveEvent(QEvent* event)  {
    QGraphicsView::leaveEvent(event);
    label->hide();
}
void GraphicsView::SetBackGroundType(BACK_GROUND_TYPE type) {
    _backgroundType = type;
}
void GraphicsView::SetCurrentImage(const QString &imagePath) {
    if(!_pixmapItem) {
        _pixmapItem = new QGraphicsPixmapItem(imagePath);
        this->scene()->addItem(_pixmapItem);
        _pixmapItem->setZValue(-1);
    }
    _pixmapItem->setPixmap(QPixmap::fromImage(QImage(imagePath)));
}
