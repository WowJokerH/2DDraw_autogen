#include "Triangle.h"
#include <QJsonArray>

int Triangle::count = 0;

Triangle::Triangle(const QPointF& a, const QPointF& b, const QPointF& c) : m_a(a), m_b(b), m_c(c) { count++; totalCount++; }

QString Triangle::typeName() const { return "Triangle"; }

double Triangle::length() const { return QLineF(m_a, m_b).length() + QLineF(m_b, m_c).length() + QLineF(m_c, m_a).length(); }

int Triangle::vertexCount() const { return 3; }

double Triangle::area() const { double s = m_a.x()*(m_b.y()-m_c.y()) + m_b.x()*(m_c.y()-m_a.y()) + m_c.x()*(m_a.y()-m_b.y()); return qAbs(s) * 0.5; }

void Triangle::moveBy(const QPointF& d) { m_a += d; m_b += d; m_c += d; }

void Triangle::moveTo(const QPointF& p) { QPointF c = (m_a + m_b + m_c) / 3.0; QPointF d = p - c; moveBy(d); }

void Triangle::rotate(double degrees, const QPointF& center) { QTransform t; t.translate(center.x(), center.y()); t.rotate(degrees); t.translate(-center.x(), -center.y()); m_a = t.map(m_a); m_b = t.map(m_b); m_c = t.map(m_c); }

void Triangle::draw(QPainter& p) const { QPen old = p.pen(); p.setPen(m_pen); QPointF pts[3] = { m_a, m_b, m_c }; p.drawPolygon(pts, 3); p.setPen(old); }

QRectF Triangle::boundingRect() const {
    qreal minx = m_a.x();
    qreal maxx = m_a.x();
    qreal miny = m_a.y();
    qreal maxy = m_a.y();
    for (auto pt : {m_b, m_c}) {
        if (pt.x() < minx) minx = pt.x();
        if (pt.x() > maxx) maxx = pt.x();
        if (pt.y() < miny) miny = pt.y();
        if (pt.y() > maxy) maxy = pt.y();
    }
    return QRectF(QPointF(minx, miny), QPointF(maxx, maxy)).normalized();
}

bool Triangle::hitTest(const QPointF& p) const { QPolygonF poly; poly << m_a << m_b << m_c; return poly.containsPoint(p, Qt::OddEvenFill); }

QJsonObject Triangle::toJson() const { QJsonObject o; o["type"] = typeName(); QJsonArray arr; QJsonArray a1; a1.append(m_a.x()); a1.append(m_a.y()); arr.append(a1); QJsonArray a2; a2.append(m_b.x()); a2.append(m_b.y()); arr.append(a2); QJsonArray a3; a3.append(m_c.x()); a3.append(m_c.y()); arr.append(a3); o["points"] = arr; o["color"] = m_color.name(); o["width"] = m_pen.widthF(); o["style"] = (int)m_pen.style(); return o; }
