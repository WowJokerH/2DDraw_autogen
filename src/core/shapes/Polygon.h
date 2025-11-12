#pragma once
#include "../AreaShape.h"
#include <QVector>

class Polygon : public AreaShape {
public:
    static int count;
    Polygon();
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
    void addPoint(const QPointF& p);
    const QVector<QPointF>& points() const;
private:
    QVector<QPointF> m_pts;
};
