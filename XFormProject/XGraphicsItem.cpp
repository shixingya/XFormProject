#include "XGraphicsItem.h"
ControlHandle::ControlHandle(QGraphicsItem *parent)
    : QGraphicsObject(parent) {
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    setAcceptHoverEvents(true);
    static int index = 0;
    index++;
    setObjectName(QString("control_handle %1").arg(index));
}
ControlHandle::~ControlHandle() {
    qDebug() << "delete objName:" << this->objectName();
}
int ControlHandle::GetIndex() const {
    return index;
}

void ControlHandle::SetIndex(int idx) {
    index = idx;
}

QRectF ControlHandle::boundingRect() const  {
    return QRectF(-5, -5, 10, 10);
}

QPainterPath ControlHandle::shape() const  {
    QPainterPath path;
    path.addEllipse(boundingRect());
    return path;
}
void ControlHandle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)  {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->eraseRect(boundingRect());
    painter->setPen(Qt::yellow);
    painter->setBrush(Qt::darkGray);
    painter->drawEllipse(boundingRect());
    painter->drawRect(boundingRect());
    painter->setPen(Qt::red);
    painter->drawText(QPointF(0, 0), QString::number(index));
}
void ControlHandle::mousePressEvent(QGraphicsSceneMouseEvent *event)  {
    if (event->button() == Qt::LeftButton) {
        isSelected = true;
        previousPos = event->scenePos();
    }
    QGraphicsItem::mousePressEvent(event);
}
void ControlHandle::mouseMoveEvent(QGraphicsSceneMouseEvent *event)  {
    if (isSelected) {
        qreal distance = QLineF(previousPos, event->scenePos()).length();
        if (abs(distance) > MIN_STEP_DISTANCE) {
            qDebug() << "------------------------";
            emit positionChanged(event->scenePos());
        }
    }
    QGraphicsItem::mouseMoveEvent(event);
}
void ControlHandle::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        isSelected = false;
    }
    QGraphicsItem::mouseReleaseEvent(event);
}
void ControlHandle::hoverEnterEvent(QGraphicsSceneHoverEvent * event)  {
    Q_UNUSED(event);
    setOpacity(0.5);
}
void ControlHandle::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)  {
    Q_UNUSED(event);
    setOpacity(1.0);
}
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
        painter->setPen(QPen(Qt::red, 2));
        painter->setBrush(QColor(0, 0, 255, 120));
    } else {
        painter->setPen(Qt::black);
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
    for (int i = 0; i < size; ++i) {
        QPointF p1 = m_vertices[i];
        QPointF p2 = m_vertices[(i + 1) % size];
        if (i + 1 == size) {
            p2 = m_vertices[0];
        }
        QPointF midPoint = (p1 + p2) / 2;
        ControlHandlePtr handle = std::make_shared<ControlHandle>(this);
        handle->setPos(midPoint);
        handle->SetIndex(i);
        handle->update();
        m_controlHandles.append(std::move(handle));
        //注意下面必须用QueuedConnection，用DirectConnection不行
        //因为在OnPositionChanged会把本体删除，
        //ControlHandle发送信号后后续执行会崩溃。
        QObject::connect(m_controlHandles[i].get(), &ControlHandle::positionChanged,
                         this, &XGraphicsItem::OnPositionChanged, Qt::QueuedConnection);
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
void XGraphicsItem::OnPositionChanged(QPointF scenePos) {
    auto handle = (ControlHandle*)(sender());
    QString objName;
    if (handle) {
        int index = handle->GetIndex();
        objName = handle->objectName();
        if (index >= 0) {
            insertVertex(index + 1, mapFromScene(scenePos));
        } else {
            qDebug("error");
        }
    }
}
