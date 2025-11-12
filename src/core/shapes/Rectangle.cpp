#include "Rectangle.h"
#include <QJsonArray>

int Rectangle::count = 0;

Rectangle::Rectangle(const QPointF& a, const QPointF& b) {
    count++; totalCount++;
    m_a = a;
    m_c = b;
    m_b = QPointF(b.x(), a.y());
    m_d = QPointF(a.x(), b.y());
}

QString Rectangle::typeName() const { return "Rectangle"; }

double Rectangle::length() const { return QLineF(m_a, m_b).length() + QLineF(m_b, m_c).length() + QLineF(m_c, m_d).length() + QLineF(m_d, m_a).length(); }

int Rectangle::vertexCount() const { return 4; }

double Rectangle::area() const { double w = QLineF(m_a, m_b).length(); double h = QLineF(m_a, m_d).length(); return w * h; }

void Rectangle::moveBy(const QPointF& d) { m_a += d; m_b += d; m_c += d; m_d += d; }

void Rectangle::moveTo(const QPointF& p) { QPointF c = (m_a + m_b + m_c + m_d) / 4.0; QPointF d = p - c; moveBy(d); }

void Rectangle::rotate(double degrees, const QPointF& center) { QTransform t; t.translate(center.x(), center.y()); t.rotate(degrees); t.translate(-center.x(), -center.y()); m_a = t.map(m_a); m_b = t.map(m_b); m_c = t.map(m_c); m_d = t.map(m_d); }

void Rectangle::draw(QPainter& p) const { QPen old = p.pen(); p.setPen(m_pen); QPointF pts[4] = { m_a, m_b, m_c, m_d }; p.drawPolygon(pts, 4); p.setPen(old); }

QRectF Rectangle::boundingRect() const {
    qreal minx = m_a.x();
    qreal maxx = m_a.x();
    qreal miny = m_a.y();
    qreal maxy = m_a.y();
    for (auto pt : {m_b, m_c, m_d}) {
        if (pt.x() < minx) minx = pt.x();
        if (pt.x() > maxx) maxx = pt.x();
        if (pt.y() < miny) miny = pt.y();
        if (pt.y() > maxy) maxy = pt.y();
    }
    return QRectF(QPointF(minx, miny), QPointF(maxx, maxy)).normalized();
}

bool Rectangle::hitTest(const QPointF& p) const { QPolygonF poly; poly << m_a << m_b << m_c << m_d; return poly.containsPoint(p, Qt::OddEvenFill); }

QJsonObject Rectangle::toJson() const { QJsonObject o; o["type"] = typeName(); QJsonArray arr; for (auto pt : {m_a,m_b,m_c,m_d}) { QJsonArray a; a.append(pt.x()); a.append(pt.y()); arr.append(a); } o["points"] = arr; o["color"] = m_color.name(); o["width"] = m_pen.widthF(); o["style"] = (int)m_pen.style(); return o; }
