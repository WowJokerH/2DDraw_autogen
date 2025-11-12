#pragma once
#include "../AreaShape.h"

class Ellipse : public AreaShape {
public:
    static int count;
    Ellipse(const QPointF& c, double rx, double ry);
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
    QPointF m_c;
    double m_rx;
    double m_ry;
    double m_angle = 0.0;
};
