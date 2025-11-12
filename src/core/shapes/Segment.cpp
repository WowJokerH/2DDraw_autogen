#include "Segment.h"
#include <QtMath>
#include <QJsonArray>

int Segment::count = 0;

Segment::Segment(const QPointF& a, const QPointF& b) : m_p1(a), m_p2(b) { count++; totalCount++; }

QString Segment::typeName() const { return "Segment"; }

double Segment::length() const { return QLineF(m_p1, m_p2).length(); }

int Segment::vertexCount() const { return 2; }

void Segment::moveBy(const QPointF& d) { m_p1 += d; m_p2 += d; }

void Segment::moveTo(const QPointF& p) { QPointF c = (m_p1 + m_p2) * 0.5; QPointF d = p - c; moveBy(d); }

void Segment::rotate(double degrees, const QPointF& center) { QTransform t; t.translate(center.x(), center.y()); t.rotate(degrees); t.translate(-center.x(), -center.y()); m_p1 = t.map(m_p1); m_p2 = t.map(m_p2); }

void Segment::draw(QPainter& p) const { QPen old = p.pen(); p.setPen(m_pen); p.drawLine(m_p1, m_p2); p.setPen(old); }

QRectF Segment::boundingRect() const { return QRectF(m_p1, m_p2).normalized(); }

bool Segment::hitTest(const QPointF& p) const {
    const QPointF a = m_p1, b = m_p2;
    double dx = b.x() - a.x();
    double dy = b.y() - a.y();
    double l2 = dx*dx + dy*dy;
    if (l2 == 0.0) return QLineF(a, p).length() <= m_pen.widthF() + 4.0;
    double t = ((p.x()-a.x())*dx + (p.y()-a.y())*dy) / l2;
    if (t < 0.0) t = 0.0; else if (t > 1.0) t = 1.0;
    QPointF proj(a.x() + t*dx, a.y() + t*dy);
    double d = QLineF(proj, p).length();
    return d <= m_pen.widthF() + 4.0;
}

QJsonObject Segment::toJson() const { QJsonObject o; o["type"] = typeName(); o["p1x"] = m_p1.x(); o["p1y"] = m_p1.y(); o["p2x"] = m_p2.x(); o["p2y"] = m_p2.y(); o["color"] = m_color.name(); o["width"] = m_pen.widthF(); o["style"] = (int)m_pen.style(); return o; }

QPointF Segment::p1() const { return m_p1; }
QPointF Segment::p2() const { return m_p2; }
