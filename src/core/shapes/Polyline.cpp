#include "Polyline.h"
#include <QJsonArray>

int Polyline::count = 0;

Polyline::Polyline() { count++; totalCount++; }

QString Polyline::typeName() const { return "Polyline"; }

double Polyline::length() const { double s = 0.0; for (int i = 1; i < m_pts.size(); ++i) s += QLineF(m_pts[i-1], m_pts[i]).length(); return s; }

int Polyline::vertexCount() const { return m_pts.size(); }

void Polyline::moveBy(const QPointF& d) { for (auto& pt : m_pts) pt += d; }

void Polyline::moveTo(const QPointF& p) { if (m_pts.isEmpty()) return; QPointF c; for (auto& pt : m_pts) c += pt; c /= m_pts.size(); QPointF d = p - c; moveBy(d); }

void Polyline::rotate(double degrees, const QPointF& center) { QTransform t; t.translate(center.x(), center.y()); t.rotate(degrees); t.translate(-center.x(), -center.y()); for (auto& pt : m_pts) pt = t.map(pt); }

void Polyline::draw(QPainter& p) const { QPen old = p.pen(); p.setPen(m_pen); if (m_pts.size() >= 2) p.drawPolyline(m_pts.data(), m_pts.size()); p.setPen(old); }

QRectF Polyline::boundingRect() const {
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

bool Polyline::hitTest(const QPointF& p) const {
    auto distSeg = [](const QPointF& a, const QPointF& b, const QPointF& p){
        double dx = b.x() - a.x();
        double dy = b.y() - a.y();
        double l2 = dx*dx + dy*dy;
        if (l2 == 0.0) return QLineF(a, p).length();
        double t = ((p.x()-a.x())*dx + (p.y()-a.y())*dy) / l2;
        if (t < 0.0) t = 0.0; else if (t > 1.0) t = 1.0;
        QPointF proj(a.x() + t*dx, a.y() + t*dy);
        return QLineF(proj, p).length();
    };
    for (int i = 1; i < m_pts.size(); ++i) {
        if (distSeg(m_pts[i-1], m_pts[i], p) <= m_pen.widthF() + 4.0) return true;
    }
    return false;
}

QJsonObject Polyline::toJson() const { QJsonObject o; o["type"] = typeName(); QJsonArray arr; for (auto& pt : m_pts) { QJsonArray a; a.append(pt.x()); a.append(pt.y()); arr.append(a); } o["points"] = arr; o["color"] = m_color.name(); o["width"] = m_pen.widthF(); o["style"] = (int)m_pen.style(); return o; }

void Polyline::addPoint(const QPointF& p) { m_pts.append(p); }

const QVector<QPointF>& Polyline::points() const { return m_pts; }
