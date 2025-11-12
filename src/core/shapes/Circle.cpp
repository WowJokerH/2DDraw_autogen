#include "Circle.h"
#include <QtMath>

int Circle::count = 0;

Circle::Circle(const QPointF& c, double r) : m_c(c), m_r(r) { count++; totalCount++; }

QString Circle::typeName() const { return "Circle"; }

double Circle::length() const { return 2 * M_PI * m_r; }

int Circle::vertexCount() const { return 0; }

double Circle::area() const { return M_PI * m_r * m_r; }

void Circle::moveBy(const QPointF& d) { m_c += d; }

void Circle::moveTo(const QPointF& p) { m_c = p; }

void Circle::rotate(double, const QPointF&) {}

void Circle::draw(QPainter& p) const { QPen old = p.pen(); p.setPen(m_pen); p.drawEllipse(m_c, m_r, m_r); p.setPen(old); }

QRectF Circle::boundingRect() const { return QRectF(m_c.x()-m_r, m_c.y()-m_r, m_r*2, m_r*2); }

bool Circle::hitTest(const QPointF& p) const { double d = QLineF(m_c, p).length(); return qAbs(d - m_r) <= m_pen.widthF() + 4.0; }

QJsonObject Circle::toJson() const { QJsonObject o; o["type"] = typeName(); o["cx"] = m_c.x(); o["cy"] = m_c.y(); o["r"] = m_r; o["color"] = m_color.name(); o["width"] = m_pen.widthF(); o["style"] = (int)m_pen.style(); return o; }
