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
    setRenderHint(QPainter::TextAntialiasing);
    setViewportUpdateMode(FullViewportUpdate);
    setCacheMode(CacheBackground);
    setMouseTracking(true); // 开启鼠标追踪
    label = new QLabel(this); // 创建标签用于显示鼠标位置
    label->setStyleSheet("background-color: white; padding: 5px;");
    label->hide();
}


void GraphicsView::mousePressEvent(QMouseEvent* event) {
    QGraphicsView::mousePressEvent(event);
}

void GraphicsView::mouseMoveEvent(QMouseEvent* event) {
    QGraphicsView::mouseMoveEvent(event);
    QPointF scenePos = mapToScene(event->pos());
    label->setText(QString("Mouse Position: (%1, %2)").arg(scenePos.x()).arg(scenePos.y()));
    label->move(event->pos().x(), event->pos().y() + 30);
    label->show();
}

void GraphicsView::mouseReleaseEvent(QMouseEvent* event) {
    QGraphicsView::mouseReleaseEvent(event);
}

void GraphicsView::drawGridBackGround(QPainter* painter, const QRectF& rect) {
    // 设置网格大小和颜色
    const int gridSize = this->width() / 100;
    const QColor oddColor = QColor("#8F8F8F");
    const QColor evenColor = QColor("#707070");
    // 计算网格起始位置
    int startX = static_cast<int>(rect.left()) - (static_cast<int>(rect.left()) % gridSize) - gridSize;
    int startY = static_cast<int>(rect.top()) - (static_cast<int>(rect.top()) % gridSize) - gridSize;
    // 计算网格终止位置
    int rows = static_cast<int>(rect.right()) + gridSize;
    int cols = static_cast<int>(rect.bottom()) + gridSize;
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
void GraphicsView::leaveEvent(QEvent* event)  {
    QGraphicsView::leaveEvent(event);
    label->hide();
}
void GraphicsView::SetBackGroundType(BACK_GROUND_TYPE type) {
    _backgroundType = type;
}
void GraphicsView::SetCurrentImage(const QString &imagePath) {
    if (!_pixmapItem) {
        _pixmapItem = new QGraphicsPixmapItem();
        this->scene()->addItem(_pixmapItem);
        _pixmapItem->setZValue(-1);
    }
    QPixmap pixmap = QPixmap(imagePath);
    _pixmapItem->setPixmap(pixmap.scaled(this->size(),
                                         Qt::KeepAspectRatio, Qt::SmoothTransformation));
    // 将图片居中显示
    QPointF center = this->viewport()->rect().center();
    _pixmapItem->setPos(center.x() - _pixmapItem->boundingRect().width() / 2,
                        center.y() - _pixmapItem->boundingRect().height() / 2);
    // 设置场景的矩形大小与 GraphicsView 的大小相匹配
    this->scene()->setSceneRect(QRectF(QPointF(0, 0), this->size()));
}
void GraphicsView::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Equal && event->modifiers() & Qt::ControlModifier) {
        // 按下Ctrl和"+"键时放大视图
        scale(1.1, 1.1);
        event->accept();
    } else if (event->key() == Qt::Key_Minus && event->modifiers() & Qt::ControlModifier) {
        // 按下Ctrl和"-"键时缩小视图
        scale(1.0 / 1.1, 1.0 / 1.1);
        event->accept();
    } else {
        // 将键盘事件传递给父类处理
        QGraphicsView::keyPressEvent(event);
    }
}
void GraphicsView::wheelEvent(QWheelEvent* event) {
    if (event->modifiers() == Qt::ControlModifier) {
        event->ignore(); // 忽略滚轮事件，防止触发默认的缩放行为
        int delta = event->angleDelta().y();
        qreal scaleFactor = (delta > 0) ? 1.1 : 0.9;
        // 获取当前的缩放因子
        qreal currentScaleFactor = transform().m11();
        // 设置最小和最大缩放因子
        qreal minScaleFactor = 0.1;
        qreal maxScaleFactor = 10.0;
        // 计算缩放后的缩放因子
        qreal newScaleFactor = currentScaleFactor * scaleFactor;
        qDebug() << "newScaleFactor ======= " << newScaleFactor;
        // 确保缩放因子在允许范围内
        if (newScaleFactor < minScaleFactor)
            newScaleFactor = minScaleFactor;
        else if (newScaleFactor > maxScaleFactor)
            newScaleFactor = maxScaleFactor;
        // 计算缩放比例
        qreal scaleRatio = newScaleFactor / currentScaleFactor;
        // 执行缩放操作
        scale(scaleRatio, scaleRatio);
    } else {
        QGraphicsView::wheelEvent(event);
    }
}
void GraphicsView::resizeEvent(QResizeEvent * event) {
    QGraphicsView::resizeEvent(event);
    if (scene()) {
        QRectF sceneRect = scene()->itemsBoundingRect();
        this->fitInView(sceneRect, Qt::KeepAspectRatio);
    }
}
