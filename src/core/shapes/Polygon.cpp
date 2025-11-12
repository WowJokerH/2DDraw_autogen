#include "Polygon.h"
#include <QJsonArray>

int Polygon::count = 0;

Polygon::Polygon() { count++; totalCount++; }

QString Polygon::typeName() const { return "Polygon"; }

double Polygon::length() const { double s = 0.0; if (m_pts.size() < 2) return 0.0; for (int i = 1; i < m_pts.size(); ++i) s += QLineF(m_pts[i-1], m_pts[i]).length(); s += QLineF(m_pts.last(), m_pts.first()).length(); return s; }

int Polygon::vertexCount() const { return m_pts.size(); }

double Polygon::area() const { if (m_pts.size() < 3) return 0.0; double s = 0.0; for (int i = 0; i < m_pts.size(); ++i) { QPointF a = m_pts[i]; QPointF b = m_pts[(i+1)%m_pts.size()]; s += a.x()*b.y() - b.x()*a.y(); } return qAbs(s) * 0.5; }

void Polygon::moveBy(const QPointF& d) { for (auto& pt : m_pts) pt += d; }

void Polygon::moveTo(const QPointF& p) { if (m_pts.isEmpty()) return; QPointF c; for (auto& pt : m_pts) c += pt; c /= m_pts.size(); QPointF d = p - c; moveBy(d); }

void Polygon::rotate(double degrees, const QPointF& center) { QTransform t; t.translate(center.x(), center.y()); t.rotate(degrees); t.translate(-center.x(), -center.y()); for (auto& pt : m_pts) pt = t.map(pt); }

void Polygon::draw(QPainter& p) const { QPen old = p.pen(); p.setPen(m_pen); if (m_pts.size() >= 3) p.drawPolygon(m_pts.data(), m_pts.size()); p.setPen(old); }

QRectF Polygon::boundingRect() const {
    if (m_pts.isEmpty()) return QRectF();
    qreal minx = m_pts[0].x();
    qreal maxx = m_pts[0].x();
    qreal miny = m_pts[0].y();
    qreal maxy = m_pts[0].y();
    for (auto& pt : m_pts) {
        if (pt.x() < minx) minx = pt.x();
        if (pt.x() > maxx) maxx = pt.x();
        if (pt.y() < miny) miny = pt.y();
        if (pt.y() > maxy) maxy = pt.y();
    }
    return QRectF(QPointF(minx, miny), QPointF(maxx, maxy)).normalized();
}

bool Polygon::hitTest(const QPointF& p) const { QPolygonF poly(m_pts); return poly.containsPoint(p, Qt::OddEvenFill); }

QJsonObject Polygon::toJson() const { QJsonObject o; o["type"] = typeName(); QJsonArray arr; for (auto& pt : m_pts) { QJsonArray a; a.append(pt.x()); a.append(pt.y()); arr.append(a); } o["points"] = arr; o["color"] = m_color.name(); o["width"] = m_pen.widthF(); o["style"] = (int)m_pen.style(); return o; }

void Polygon::addPoint(const QPointF& p) { m_pts.append(p); }

const QVector<QPointF>& Polygon::points() const { return m_pts; }
