#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPolygonF>
#include <QPainter>
#include <QPointF>
#include <QRectF>

class XGraphicsItem : public QGraphicsItem {
  public:
    XGraphicsItem(QGraphicsItem *parent = nullptr)
        : QGraphicsItem(parent) {
        setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges | ItemIsFocusable);
        setAcceptHoverEvents(true);  // 启用悬停事件
    }

    QRectF boundingRect() const override {
        return m_polygon.boundingRect();  // 根据实际的多边形形状设置包围矩形范围;
    }

    // 重写 shape() 函数，返回图形项的形状
    QPainterPath shape() const override {
        QPainterPath path;
        path.addPolygon(m_polygon);
        return path;
    }

    // 重写 contains() 函数，判断给定的点是否在图形项的范围内
    bool contains(const QPointF &point) const override {
        return m_polygon.containsPoint(point, Qt::WindingFill);
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override {
        Q_UNUSED(option);
        Q_UNUSED(widget);
        painter->setRenderHint(QPainter::Antialiasing);
        if (m_highlighted) {
            painter->setPen(QPen(Qt::red, 2)); // 高亮时使用红色画笔
            painter->setBrush(QColor(0, 0, 255, 20));  // 蓝色遮罩，透明度为50
        } else {
            painter->setPen(Qt::black);
            painter->setBrush(Qt::gray);
        }
        painter->drawPolygon(m_polygon);
    }

    int type() const override {
        return UserType + 1;
    }

    void setPolygon(const QPolygonF &polygon) {
        prepareGeometryChange();
        m_polygon = polygon;
    }

    void setPoints(const QVector<QPointF> &points) {
        prepareGeometryChange();
        m_points = points;
    }

  protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override {
        if (change == ItemPositionChange && scene()) {
            // Restrict the item's position within the scene
            QPointF newPos = value.toPointF();
            QRectF sceneRect = scene()->sceneRect();
            if (!sceneRect.contains(newPos)) {
                newPos.setX(qMin(sceneRect.right(), qMax(newPos.x(), sceneRect.left())));
                newPos.setY(qMin(sceneRect.bottom(), qMax(newPos.y(), sceneRect.top())));
                return newPos;
            }
        }
        return QGraphicsItem::itemChange(change, value);
    }
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override {
        Q_UNUSED(event);
        setCursor(Qt::PointingHandCursor);  // 设置鼠标悬停时的光标形状
        m_highlighted = true;
        update();
    }

    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override {
        Q_UNUSED(event);
        unsetCursor();  // 恢复默认的光标形状
        m_highlighted = false;
        update();
    }

  private:
    QRectF m_boundingRect;
    QPolygonF m_polygon;
    QVector<QPointF> m_points;
    QTransform m_transform; // 仿射矩形的变换矩阵
    bool m_highlighted = false;
};
