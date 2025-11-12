#pragma once
#include <QColor>
#include <QPen>
#include <QPainter>
#include <QPointF>
#include <QJsonObject>
#include <QString>
#include <QRectF>

class Shape {
public:
    virtual ~Shape() {}
    virtual QString typeName() const = 0;
    virtual double length() const = 0;
    virtual int vertexCount() const = 0;
    virtual double area() const { return 0.0; }
    virtual void moveBy(const QPointF& d) = 0;
    virtual void moveTo(const QPointF& p) = 0;
    virtual void rotate(double degrees, const QPointF& center) = 0;
    virtual void draw(QPainter& p) const = 0;
    virtual QRectF boundingRect() const = 0;
    virtual bool hitTest(const QPointF& p) const = 0;
    virtual QJsonObject toJson() const = 0;
    virtual void setColor(const QColor& c) { m_color = c; m_pen.setColor(c); }
    virtual void setPen(const QPen& pen) { m_pen = pen; }
    virtual QColor color() const { return m_color; }
    virtual QPen pen() const { return m_pen; }
protected:
    QColor m_color = Qt::black;
    QPen m_pen = QPen(Qt::black, 2);
};
