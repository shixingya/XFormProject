#include <QtWidgets>
#include <memory>
#include <QThread>
const int MIN_STEP_DISTANCE = 100;

class ControlHandle : public QGraphicsObject {
    Q_OBJECT
  public:
    ControlHandle(QGraphicsItem *parent = nullptr);
    virtual ~ControlHandle();
    int GetIndex() const;
    void SetIndex(int idx);

    QRectF boundingRect() const override;

    QPainterPath shape() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

  protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event)override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent * event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent * event) override;
  signals:
    void positionChanged(QPointF scenePos);
  private:
    int index = -1;
    QPointF previousPos;
    bool isSelected = false;
};

typedef std::shared_ptr<ControlHandle> ControlHandlePtr;
class XGraphicsItem : public QGraphicsObject {
    Q_OBJECT
  public:
    XGraphicsItem(QGraphicsItem *parent = nullptr);
    void setVertices(const QVector<QPointF> &vertices) ;
    QVector<QPointF> vertices() const ;
    QRectF boundingRect() const override ;
    QPainterPath shape() const override ;
    bool contains(const QPointF &point) const override;
    bool isHighlighted() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

  protected:
    void updateShape();
    void updateControlHandles();
    void modifyVertices(int index, const QPointF &vertex);
    void insertVertex(int index, const QPointF &vertex);
    void removeVertex(int index);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

  private slots:
    void OnPositionChanged(QPointF scenePos);

  private:
    QVector<QPointF> m_vertices;
    QRectF m_boundingRect;
    QPainterPath m_shape;
    QVector<ControlHandlePtr> m_controlHandles;
    bool m_highlighted = false;
};
