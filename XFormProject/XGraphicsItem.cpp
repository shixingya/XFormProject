#include "XGraphicsItem.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

XGraphicsItem::XGraphicsItem(QGraphicsItem *parent)
    : QGraphicsObject(parent) {
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setAcceptHoverEvents(true);
}

void XGraphicsItem::setVertices(const QVector<QPointF> &vertices) {
    m_vertices = vertices;
    updateShape();
    updateControlHandles();
}

QVector<QPointF> XGraphicsItem::vertices() const {
    return m_vertices;
}

QRectF XGraphicsItem::boundingRect() const  {
    return m_boundingRect;
}

QPainterPath XGraphicsItem::shape() const  {
    return m_shape;
}

bool XGraphicsItem::contains(const QPointF &point) const  {
    return m_shape.contains(point);
}

bool XGraphicsItem::isHighlighted() const {
    return m_highlighted;
}

void XGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)  {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setRenderHint(QPainter::Antialiasing);
    if (isHighlighted()) {
        painter->setPen(QPen(Qt::darkGreen, 2));
        painter->setBrush(QColor(0, 0, 255, 120));
    } else {
        painter->setPen(Qt::green);
        painter->setBrush(QColor(0, 0, 255, 200));
    }
    painter->drawPath(m_shape);
}

void XGraphicsItem::updateShape() {
    m_boundingRect = QRectF();
    int size = m_vertices.size();
    if (size < 3) {
        m_shape = QPainterPath();
        return;
    }
    m_shape = QPainterPath();
    m_shape.moveTo(m_vertices.first());
    for (int i = 1; i < size; ++i) {
        m_shape.lineTo(m_vertices[i]);
    }
    m_shape.lineTo(m_vertices.first());
    m_boundingRect = m_shape.boundingRect();
    update();
}

void XGraphicsItem::updateControlHandles() {
    m_controlHandles.clear();
    int size = m_vertices.size();
    auto createEndPoint = [&](int index, const QPointF & pos) {
        ControlHandlePtr handle = std::make_shared<ControlHandle>(this);
        handle->setPos(pos);
        handle->SetIndex(index);
        handle->SetHandleType(ControlHandle::END_POINT);
        m_controlHandles.append(std::move(handle));
    };
    auto createMidPoint = [&](int index, const QPointF & p1, const QPointF & p2) {
        QPointF midPoint = (p1 + p2) / 2;
        ControlHandlePtr handle = std::make_shared<ControlHandle>(this);
        handle->setPos(midPoint);
        handle->SetIndex(index);
        handle->SetHandleType(ControlHandle::MID_POINT);
        m_controlHandles.append(std::move(handle));
    };
    for (int index = 0; index < size; ++index) {
        QPointF p1 = m_vertices[index];
        createEndPoint(index, p1);
        QPointF p2 = m_vertices[(index + 1) % size];
        if (index + 1 == size) {
            p2 = m_vertices[0];
        }
        createMidPoint(index, p1, p2);
    }
    for(auto item : m_controlHandles ) {
        //注意下面必须用QueuedConnection，用DirectConnection不行
        //因为在OnPositionChanged会把本体删除，
        //ControlHandle发送信号后后续执行会崩溃。
        QObject::connect(item.get(), &ControlHandle::positionChanged,
                         this, &XGraphicsItem::OnPositionChanged, Qt::QueuedConnection);
        QObject::connect(item.get(), &ControlHandle::hideMiddlePoints,
                         this, &XGraphicsItem::OnHideMiddlePoints, Qt::QueuedConnection);
        QObject::connect(item.get(), &ControlHandle::showMiddlePoints,
                         this, &XGraphicsItem::OnShowMiddlePoints, Qt::QueuedConnection);
    }
    update();
}

void XGraphicsItem::modifyVertices(int index, const QPointF &vertex) {
    if (index < 0 || index >= m_vertices.size())
        return;
    m_vertices[index] = vertex;
    updateShape();
}

void XGraphicsItem::insertVertex(int index, const QPointF &vertex) {
    if (index < 0 || index > m_vertices.size())
        return;
    m_vertices.insert(index, vertex);
    updateShape();
    updateControlHandles();
}

void XGraphicsItem::removeVertex(int index) {
    if (index < 0 || index >= m_vertices.size())
        return;
    m_vertices.remove(index);
    updateShape();
    updateControlHandles();
}

void XGraphicsItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)  {
    Q_UNUSED(event);
    m_highlighted = true;
    update();
}

void XGraphicsItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)  {
    Q_UNUSED(event);
    m_highlighted = false;
    update();
}

void moveVertex(QVector<QPointF>& vertices, int index, const QPointF& newPos) {
    // 获取原始顶点和目标位置
    QPointF oldPos = vertices[index];
    QPointF translation = newPos - oldPos;
    // 计算缩放比例
    qreal scaleX = newPos.x() / oldPos.x();
    qreal scaleY = newPos.y() / oldPos.y();
    // 移动顶点并按比例缩放其他顶点
    for (int i = 0; i < vertices.size(); i++) {
        if (i == index) {
            vertices[i] = newPos;
        } else {
            QPointF vertex = vertices[i] - oldPos;
            vertex.setX(vertex.x() * scaleX);
            vertex.setY(vertex.y() * scaleY);
            vertex += newPos;
            vertices[i] = vertex;
        }
    }
}
void XGraphicsItem::OnHideMiddlePoints() {
    for(auto item : m_controlHandles) {
        if(item->GetHandleType() == ControlHandle::MID_POINT) {
            item->hide();
        }
    }
}
void XGraphicsItem::OnShowMiddlePoints() {
    updateControlHandles();
    for(auto item : m_controlHandles) {
        if(item->GetHandleType() == ControlHandle::MID_POINT) {
            item->show();
        }
    }
}
void XGraphicsItem::OnPositionChanged(QPointF scenePos) {
    auto handle = (ControlHandle*)(sender());
    QString objName;
    if (handle) {
        int index = handle->GetIndex();
        objName = handle->objectName();
        if (index >= 0) {
            if(handle->GetHandleType() == ControlHandle::END_POINT) {
                if(handle->IsResizing()) {
                    moveVertex(m_vertices, index, mapFromScene(scenePos));
                }
                modifyVertices(index, mapFromScene(scenePos));
            } else {
                insertVertex(index + 1, mapFromScene(scenePos));
            }
        } else {
            qDebug("error");
        }
    }
}
