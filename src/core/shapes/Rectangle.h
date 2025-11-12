#pragma once
#include "../AreaShape.h"

class Rectangle : public AreaShape {
public:
    static int count;
    Rectangle(const QPointF& a, const QPointF& b);
    QString typeName() const override;
    double length() const override;
    int vertexCount() const override;
    double area() const override;
    void moveBy(const QPointF& d) override;
    void moveTo(const QPointF& p) override;
    void rotate(double degrees, const QPointF& center) override;
    void draw(QPainter& p) const override;
    QRectF boundingRect() const override;
    bool hitTest(const QPointF& p) const override;
    QJsonObject toJson() const override;
private:
    QPointF m_a, m_b, m_c, m_d;
};
