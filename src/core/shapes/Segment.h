#pragma once
#include "../LineShape.h"
#include <QLineF>

class Segment : public LineShape {
public:
    static int count;
    Segment(const QPointF& a, const QPointF& b);
    QString typeName() const override;
    double length() const override;
    int vertexCount() const override;
    void moveBy(const QPointF& d) override;
    void moveTo(const QPointF& p) override;
    void rotate(double degrees, const QPointF& center) override;
    void draw(QPainter& p) const override;
    QRectF boundingRect() const override;
    bool hitTest(const QPointF& p) const override;
    QJsonObject toJson() const override;
    QPointF p1() const;
    QPointF p2() const;
private:
    QPointF m_p1;
    QPointF m_p2;
};
