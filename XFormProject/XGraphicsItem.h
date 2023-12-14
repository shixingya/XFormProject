#include <QtWidgets>
#include <memory>
#include <QThread>
const int MIN_STEP_DISTANCE = 100;
class Tester {
  public:
    Tester(const QString &name): name_(name) {
    }
    ~Tester() {
        //qDebug() << "===== ====== ====== " << name_;
    }
  private:
    QString name_;
};

class ControlHandle : public QGraphicsObject {
    Q_OBJECT
  public:
    ControlHandle(QGraphicsItem *parent = nullptr)
        : QGraphicsObject(parent) {
        setFlag(QGraphicsItem::ItemIsMovable, true);
        setFlag(QGraphicsItem::ItemIsSelectable, true);
        setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
        setAcceptHoverEvents(true);
        static int index = 0;
        index++;
        setObjectName(QString("control_handle %1").arg(index));
    }
    virtual ~ControlHandle() {
        qDebug() << "delete objName:" << this->objectName();
    }
    int GetIndex() const {
        return index;
    }

    void SetIndex(int idx) {
        index = idx;
    }

    QRectF boundingRect() const override {
        return QRectF(-5, -5, 10, 10);
    }

    QPainterPath shape() const override {
        QPainterPath path;
        path.addEllipse(boundingRect());
        return path;
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override {
        Q_UNUSED(option);
        Q_UNUSED(widget);
        painter->setPen(Qt::yellow);
        painter->setBrush(Qt::darkGray);
        painter->drawEllipse(boundingRect());
        painter->setPen(Qt::red);
        painter->drawText(QPointF(0, 0), QString::number(index));
    }

  protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override {
        if (change == ItemSelectedChange && scene()) {
            isSelected = value.toBool();
            if (isSelected) {
                previousPos = pos();
                qDebug() << "===============current Index:" << GetIndex()
                         << "=============== Pos:" << previousPos
                         << "========" << this->objectName();
            }
        } else if (change == ItemPositionChange && scene()) {
            if(GetIndex() < 0) {
                return QGraphicsItem::itemChange(change, value);
            }
            Tester tester(this->objectName());
            QPointF newPos = value.toPointF();
            qDebug() << "call -------- " << this->objectName() << " " << newPos
                     << " " << GetIndex();
            if(isSelected) {
                // 获取当前线程的ID
                QThread* currentThread = QThread::currentThread();
                auto threadId = currentThread->currentThreadId();
                qreal distance = QLineF(previousPos, newPos).length();
                if(distance > MIN_STEP_DISTANCE) {
                    qDebug() << "call begin tID-" << threadId << " item Name =" << this->objectName();
                    emit positionChanged(newPos);
                    qDebug() << "call end tID- " << newPos << threadId;
                }
            }
        }
        return QGraphicsItem::itemChange(change, value);
    }

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override {
        Q_UNUSED(event);
        setOpacity(0.5);
    }

    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override {
        Q_UNUSED(event);
        setOpacity(1.0);
    }

  signals:
    void positionChanged(QPointF newPos);

  private:
    int index = -1;
    QPointF previousPos;
    bool isSelected = false;
};

typedef std::shared_ptr<ControlHandle> ControlHandlePtr;
class XGraphicsItem : public QGraphicsObject {
    Q_OBJECT
  public:
    XGraphicsItem(QGraphicsItem *parent = nullptr)
        : QGraphicsObject(parent) {
        setFlag(QGraphicsItem::ItemIsSelectable, true);
        setFlag(QGraphicsItem::ItemIsMovable, true);
        setAcceptHoverEvents(true);
    }

    void setVertices(const QVector<QPointF> &vertices) {
        m_vertices = vertices;
        updateShape();
        updateControlHandles();
    }

    QVector<QPointF> vertices() const {
        return m_vertices;
    }

    QRectF boundingRect() const override {
        return m_boundingRect;
    }

    QPainterPath shape() const override {
        return m_shape;
    }

    bool contains(const QPointF &point) const override {
        return m_shape.contains(point);
    }

    bool isHighlighted() const {
        return m_highlighted;
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override {
        Q_UNUSED(option);
        Q_UNUSED(widget);
        painter->setRenderHint(QPainter::Antialiasing);
        if (isHighlighted()) {
            painter->setPen(QPen(Qt::red, 2));
            painter->setBrush(QColor(0, 0, 255, 20));
        } else {
            painter->setPen(Qt::black);
            painter->setBrush(Qt::gray);
        }
        painter->drawPath(m_shape);
    }

  protected:
    void updateShape() {
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

    void updateControlHandles() {
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
            m_controlHandles.append(std::move(handle));
            //注意下面必须用QueuedConnection，用DirectConnection不行
            //因为在OnPositionChanged会把本体删除，
            //ControlHandle发送信号后后续执行会崩溃。
            QObject::connect(m_controlHandles[i].get(), &ControlHandle::positionChanged,
                             this, &XGraphicsItem::OnPositionChanged, Qt::QueuedConnection);
        }
    }

    void modifyVertices(int index, const QPointF &vertex) {
        if (index < 0 || index >= m_vertices.size())
            return;
        m_vertices[index] = vertex;
        updateShape();
    }

    void insertVertex(int index, const QPointF &vertex) {
        if (index < 0 || index > m_vertices.size())
            return;
        m_vertices.insert(index, vertex);
        updateShape();
        updateControlHandles();
    }

    void removeVertex(int index) {
        if (index < 0 || index >= m_vertices.size())
            return;
        m_vertices.remove(index);
        updateShape();
        updateControlHandles();
    }

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override {
        Q_UNUSED(event);
        m_highlighted = true;
        update();
    }

    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override {
        Q_UNUSED(event);
        m_highlighted = false;
        update();
    }

  private slots:
    void OnPositionChanged(QPointF newPos) {
        auto handle = (ControlHandle*)(sender());
        QString objName;
        if (handle) {
            int index = handle->GetIndex();
            objName = handle->objectName();
            if (index >= 0) {
                insertVertex(index + 1, newPos);
            } else {
                qDebug("error");
            }
        }
        // 获取当前线程的ID
        QThread* currentThread = QThread::currentThread();
        auto threadId = currentThread->currentThreadId();
        qDebug() << "OnPositionChanged-" << threadId << " obj " << objName;
    }

  private:
    QVector<QPointF> m_vertices;
    QRectF m_boundingRect;
    QPainterPath m_shape;
    QVector<ControlHandlePtr> m_controlHandles;
    bool m_highlighted = false;
};
