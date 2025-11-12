#include "Ellipse.h"
#include <QtMath>
#include <QPainterPath>

int Ellipse::count = 0;

Ellipse::Ellipse(const QPointF& c, double rx, double ry) : m_c(c), m_rx(rx), m_ry(ry) { count++; totalCount++; }

QString Ellipse::typeName() const { return "Ellipse"; }

double Ellipse::length() const { double h = qPow((m_rx - m_ry), 2) / qPow((m_rx + m_ry), 2); return M_PI * (m_rx + m_ry) * (1 + (3*h)/(10 + qSqrt(4 - 3*h))); }

int Ellipse::vertexCount() const { return 0; }

double Ellipse::area() const { return M_PI * m_rx * m_ry; }

void Ellipse::moveBy(const QPointF& d) { m_c += d; }

void Ellipse::moveTo(const QPointF& p) { m_c = p; }

void Ellipse::rotate(double degrees, const QPointF&) { m_angle += degrees; }

void Ellipse::draw(QPainter& p) const { QPen old = p.pen(); p.setPen(m_pen); QTransform t; t.translate(m_c.x(), m_c.y()); t.rotate(m_angle); t.translate(-m_c.x(), -m_c.y()); QPainterPath path; path.addEllipse(m_c, m_rx, m_ry); QPainterPath tp = t.map(path); p.drawPath(tp); p.setPen(old); }

QRectF Ellipse::boundingRect() const { QRectF r(m_c.x()-m_rx, m_c.y()-m_ry, m_rx*2, m_ry*2); return r; }

bool Ellipse::hitTest(const QPointF& p) const { QTransform t; t.translate(m_c.x(), m_c.y()); t.rotate(-m_angle); t.translate(-m_c.x(), -m_c.y()); QPointF rp = t.map(p); double v = qPow((rp.x()-m_c.x())/m_rx, 2) + qPow((rp.y()-m_c.y())/m_ry, 2); return qAbs(v - 1.0) <= 0.05; }

QJsonObject Ellipse::toJson() const { QJsonObject o; o["type"] = typeName(); o["cx"] = m_c.x(); o["cy"] = m_c.y(); o["rx"] = m_rx; o["ry"] = m_ry; o["angle"] = m_angle; o["color"] = m_color.name(); o["width"] = m_pen.widthF(); o["style"] = (int)m_pen.style(); return o; }
