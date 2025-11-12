#include "Canvas.h"
#include <QPainter>
#include <QMouseEvent>
#include <QInputDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>

Canvas::Canvas(QWidget* parent) : QWidget(parent) { setMinimumSize(800, 600); setFocusPolicy(Qt::StrongFocus); }

void Canvas::setMode(Mode m) { m_mode = m; m_current.reset(); m_hoverValid = false; }

void Canvas::setShowLabels(bool b) { m_showLabels = b; update(); }

void Canvas::setPen(const QPen& pen) { 
    m_pen = pen; 
    m_pen.setColor(m_color); // 确保 pen 使用当前颜色
    if (m_selected) m_selected->setPen(m_pen); 
    if (m_current) m_current->setPen(m_pen); 
    update(); 
}

void Canvas::setColor(const QColor& c) { 
    m_color = c; 
    m_pen.setColor(m_color); // 同步更新 pen 的颜色
    if (m_selected) m_selected->setColor(m_color); 
    if (m_current) m_current->setColor(m_color); 
    update(); 
}

void Canvas::rotateSelected(double degrees) { if (!m_selection.isEmpty()) { for (auto& s : m_selection) { QPointF c = s->boundingRect().center(); s->rotate(degrees, c); } update(); } }

void Canvas::saveToFile(const QString& path) { QJsonArray arr; for (auto& s : m_shapes) arr.append(s->toJson()); QJsonDocument doc(arr); QFile f(path); if (f.open(QIODevice::WriteOnly)) f.write(doc.toJson()); }

void Canvas::loadFromFile(const QString& path) { QFile f(path); if (!f.open(QIODevice::ReadOnly)) return; QJsonDocument doc = QJsonDocument::fromJson(f.readAll()); m_shapes.clear(); if (doc.isArray()) { for (auto v : doc.array()) { QJsonObject o = v.toObject(); QString t = o.value("type").toString(); if (t == "Segment") { QPointF a(o.value("p1x").toDouble(), o.value("p1y").toDouble()); QPointF b(o.value("p2x").toDouble(), o.value("p2y").toDouble()); auto s = std::make_shared<Segment>(a,b); s->setColor(QColor(o.value("color").toString())); QPen pen = QPen(s->color(), o.value("width").toDouble()); pen.setStyle((Qt::PenStyle)o.value("style").toInt()); s->setPen(pen); m_shapes.append(s); }
            else if (t == "Polyline") { auto s = std::make_shared<Polyline>(); QJsonArray pts = o.value("points").toArray(); for (auto pv : pts) { QJsonArray a = pv.toArray(); s->addPoint(QPointF(a[0].toDouble(), a[1].toDouble())); } s->setColor(QColor(o.value("color").toString())); QPen pen = QPen(s->color(), o.value("width").toDouble()); pen.setStyle((Qt::PenStyle)o.value("style").toInt()); s->setPen(pen); m_shapes.append(s); }
            else if (t == "Triangle") { QJsonArray pts = o.value("points").toArray(); QPointF a(pts[0].toArray()[0].toDouble(), pts[0].toArray()[1].toDouble()); QPointF b(pts[1].toArray()[0].toDouble(), pts[1].toArray()[1].toDouble()); QPointF c(pts[2].toArray()[0].toDouble(), pts[2].toArray()[1].toDouble()); auto s = std::make_shared<Triangle>(a,b,c); s->setColor(QColor(o.value("color").toString())); QPen pen = QPen(s->color(), o.value("width").toDouble()); pen.setStyle((Qt::PenStyle)o.value("style").toInt()); s->setPen(pen); m_shapes.append(s); }
            else if (t == "Rectangle") { QJsonArray pts = o.value("points").toArray(); QPointF a(pts[0].toArray()[0].toDouble(), pts[0].toArray()[1].toDouble()); QPointF c(pts[2].toArray()[0].toDouble(), pts[2].toArray()[1].toDouble()); auto s = std::make_shared<Rectangle>(a,c); s->setColor(QColor(o.value("color").toString())); QPen pen = QPen(s->color(), o.value("width").toDouble()); pen.setStyle((Qt::PenStyle)o.value("style").toInt()); s->setPen(pen); m_shapes.append(s); }
            else if (t == "Polygon") { auto s = std::make_shared<Polygon>(); QJsonArray pts = o.value("points").toArray(); for (auto pv : pts) { QJsonArray a = pv.toArray(); s->addPoint(QPointF(a[0].toDouble(), a[1].toDouble())); } s->setColor(QColor(o.value("color").toString())); QPen pen = QPen(s->color(), o.value("width").toDouble()); pen.setStyle((Qt::PenStyle)o.value("style").toInt()); s->setPen(pen); m_shapes.append(s); }
            else if (t == "Circle") { QPointF c(o.value("cx").toDouble(), o.value("cy").toDouble()); double r = o.value("r").toDouble(); auto s = std::make_shared<Circle>(c,r); s->setColor(QColor(o.value("color").toString())); QPen pen = QPen(s->color(), o.value("width").toDouble()); pen.setStyle((Qt::PenStyle)o.value("style").toInt()); s->setPen(pen); m_shapes.append(s); }
            else if (t == "Ellipse") { QPointF c(o.value("cx").toDouble(), o.value("cy").toDouble()); double rx = o.value("rx").toDouble(); double ry = o.value("ry").toDouble(); auto s = std::make_shared<Ellipse>(c,rx,ry); s->rotate(o.value("angle").toDouble(), c); s->setColor(QColor(o.value("color").toString())); QPen pen = QPen(s->color(), o.value("width").toDouble()); pen.setStyle((Qt::PenStyle)o.value("style").toInt()); s->setPen(pen); m_shapes.append(s); }
        }
    }
    update();
    updateStats();
}

std::shared_ptr<Shape> Canvas::selected() const { return m_selected; }

void Canvas::paintEvent(QPaintEvent*) { QPainter painter(this); painter.fillRect(rect(), Qt::white); for (auto& s : m_shapes) { s->draw(painter); if (m_showLabels) {
            QRectF br = s->boundingRect(); QString txt = QString("L:%1").arg(s->length(),0,'f',2); if (s->area() > 0) txt += QString(" A:%1").arg(s->area(),0,'f',2); painter.setPen(QPen(Qt::darkGray)); painter.drawText(br.adjusted(4,4,-4,-4).bottomRight(), txt);
        }
    }
    if (m_current) m_current->draw(painter);
    if (m_mode == DrawTriangle) {
        if (m_tempPts.size() >= 1) {
            painter.setPen(QPen(m_color, m_pen.width(), m_pen.style()));
            painter.setBrush(Qt::NoBrush);
            qreal r = m_pen.width();
            painter.drawEllipse(QRectF(m_tempPts[0] - QPointF(r,r), QSizeF(2*r,2*r)));
        }
        if (m_tempPts.size() >= 2) {
            painter.setPen(QPen(m_color, m_pen.width(), m_pen.style()));
            painter.drawLine(m_tempPts[0], m_tempPts[1]);
        }
    }
    if (m_mode == DrawPolygon && m_current) {
        auto poly = std::static_pointer_cast<Polygon>(m_current);
        int n = poly->points().size();
        if (n >= 1) {
            painter.setPen(QPen(m_color, m_pen.width(), m_pen.style()));
            painter.setBrush(Qt::NoBrush);
            qreal r = m_pen.width();
            painter.drawEllipse(QRectF(poly->points()[0] - QPointF(r,r), QSizeF(2*r,2*r)));
        }
        if (n >= 2) {
            painter.setPen(QPen(m_color, m_pen.width(), m_pen.style()));
            painter.drawPolyline(poly->points().data(), n);
        }
    }
    if (m_mode == DrawPolygon && m_current && m_hoverValid) {
        auto poly = std::static_pointer_cast<Polygon>(m_current);
        if (!poly->points().isEmpty()) {
            painter.setPen(QPen(Qt::gray, 1, Qt::DashLine));
            painter.drawLine(poly->points().last(), m_hoverPos);
        }
    }
    if (m_mode == DrawPolyline && m_current && m_hoverValid) {
        auto pl = std::static_pointer_cast<Polyline>(m_current);
        if (!pl->points().isEmpty()) {
            painter.setPen(QPen(Qt::gray, 1, Qt::DashLine));
            painter.drawLine(pl->points().last(), m_hoverPos);
        }
    }
    if (!m_selection.isEmpty()) {
        QPen selPen(QColor(30,144,255), 1, Qt::DashLine);
        painter.setPen(selPen);
        QBrush oldBrush = painter.brush();
        painter.setBrush(Qt::NoBrush);
        const QSizeF hs(6,6);
        for (auto& s : m_selection) {
            QRectF br = s->boundingRect();
            painter.drawRect(br);
            painter.setBrush(QColor(30,144,255));
            painter.drawRect(QRectF(br.topLeft()-QPointF(hs.width()/2,hs.height()/2), hs));
            painter.drawRect(QRectF(br.topRight()-QPointF(hs.width()/2,hs.height()/2), hs));
            painter.drawRect(QRectF(br.bottomLeft()-QPointF(hs.width()/2,hs.height()/2), hs));
            painter.drawRect(QRectF(br.bottomRight()-QPointF(hs.width()/2,hs.height()/2), hs));
            painter.setBrush(Qt::NoBrush);
        }
        painter.setBrush(oldBrush);
    }
    if (m_boxSelecting) {
        painter.setPen(QPen(QColor(0,0,0,128), 1, Qt::DashLine));
        painter.setBrush(QColor(0,0,255,40));
        painter.drawRect(m_boxRect);
    }
}

void Canvas::mousePressEvent(QMouseEvent* e) {
    m_lastPos = e->position();
    if (m_mode == Select) {
        if (e->button() == Qt::RightButton) {
            m_selected.reset();
            m_selection.clear();
            m_boxSelecting = true;
            m_boxStart = m_lastPos;
            m_boxRect = QRectF(m_boxStart, m_boxStart);
            update();
            return;
        }
        std::shared_ptr<Shape> hit;
        for (auto it = m_shapes.crbegin(); it != m_shapes.crend(); ++it) { if ((*it)->hitTest(m_lastPos)) { hit = *it; break; } }
        if (!hit) { m_selected.reset(); m_selection.clear(); m_boxSelecting = true; m_boxStart = m_lastPos; m_boxRect = QRectF(m_boxStart, m_boxStart); update(); return; }
        if (!m_selection.contains(hit)) { m_selection.clear(); m_selected = hit; m_selection.append(hit); emit selectionChanged(); }
        else { m_selected = hit; }
        m_drag = true;
        update();
        return;
    }
    if (e->button() == Qt::LeftButton) {
        if (m_mode == DrawSegment) m_current = std::make_shared<Segment>(m_lastPos, m_lastPos);
        else if (m_mode == DrawPolyline) { if (!m_current) m_current = std::make_shared<Polyline>(); std::static_pointer_cast<Polyline>(m_current)->addPoint(m_lastPos); m_hoverValid = false; }
        else if (m_mode == DrawTriangle) { m_tempPts.append(m_lastPos); if (m_tempPts.size()==1) { m_current = std::make_shared<Triangle>(m_lastPos, m_lastPos, m_lastPos); m_current->setPen(m_pen); m_current->setColor(m_color); } else if (m_tempPts.size()==2) { m_current = std::make_shared<Triangle>(m_tempPts[0], m_tempPts[1], m_lastPos); m_current->setPen(m_pen); m_current->setColor(m_color); } else if (m_tempPts.size()==3) { auto t = std::make_shared<Triangle>(m_tempPts[0], m_tempPts[1], m_tempPts[2]); t->setPen(m_pen); t->setColor(m_color); if (t->area() > 0.5) { m_shapes.append(t); m_selected = t; m_selection.clear(); m_selection.append(t); emit selectionChanged(); } m_current.reset(); m_hoverValid = false; m_tempPts.clear(); update(); updateStats(); return; } }
        else if (m_mode == DrawRectangle) m_current = std::make_shared<Rectangle>(m_lastPos, m_lastPos);
        else if (m_mode == DrawPolygon) { if (!m_current) m_current = std::make_shared<Polygon>(); std::static_pointer_cast<Polygon>(m_current)->addPoint(m_lastPos); m_hoverValid = false; }
        else if (m_mode == DrawCircle) m_current = std::make_shared<Circle>(m_lastPos, 0.0);
        else if (m_mode == DrawEllipse) m_current = std::make_shared<Ellipse>(m_lastPos, 0.0, 0.0);
        if (m_current) { m_current->setPen(m_pen); m_current->setColor(m_color); }
        update();
    } else if (e->button() == Qt::RightButton) {
        if (m_mode == DrawTriangle) {
            if (m_tempPts.size() < 3) { m_current.reset(); m_hoverValid = false; m_tempPts.clear(); update(); }
        }
        if (m_mode == DrawPolyline && m_current) {
            auto pl = std::static_pointer_cast<Polyline>(m_current);
            if (pl->vertexCount() >= 2 && pl->length() > minSize()) {
                auto created = m_current;
                m_shapes.append(created);
                m_selected = created;
                m_selection.clear(); m_selection.append(created);
                emit selectionChanged();
            }
            m_current.reset(); m_hoverValid = false; update(); updateStats();
        }
        if (m_mode == DrawPolygon && m_current) {
            auto pg = std::static_pointer_cast<Polygon>(m_current);
            if (pg->vertexCount() >= 3 && pg->area() > 0.5) {
                auto created = m_current;
                m_shapes.append(created);
                m_selected = created;
                m_selection.clear(); m_selection.append(created);
                emit selectionChanged();
            }
            m_current.reset(); m_hoverValid = false; update(); updateStats();
        }
    }
}

void Canvas::mouseMoveEvent(QMouseEvent* e) {
    QPointF pos = e->position();
    m_hoverPos = pos; m_hoverValid = true;
    if (m_drag && !m_selection.isEmpty()) { QPointF d = pos - m_lastPos; for (auto& s : m_selection) s->moveBy(d); m_lastPos = pos; update(); return; }
    if (m_boxSelecting) { m_boxRect = QRectF(m_boxStart, pos).normalized(); update(); return; }
    if (!m_current && m_mode!=DrawPolyline && m_mode!=DrawPolygon && !(m_mode==DrawTriangle && m_tempPts.size()>=1)) return;
    if (m_mode == DrawSegment) { m_current = std::make_shared<Segment>(m_lastPos, pos); m_current->setPen(m_pen); m_current->setColor(m_color); }
    else if (m_mode == DrawTriangle && m_tempPts.size()==2) { m_current = std::make_shared<Triangle>(m_tempPts[0], m_tempPts[1], pos); m_current->setPen(m_pen); m_current->setColor(m_color); }
    else if (m_mode == DrawRectangle) { m_current = std::make_shared<Rectangle>(m_lastPos, pos); m_current->setPen(m_pen); m_current->setColor(m_color); }
    else if (m_mode == DrawCircle) { double r = QLineF(m_lastPos, pos).length(); m_current = std::make_shared<Circle>(m_lastPos, r); m_current->setPen(m_pen); m_current->setColor(m_color); }
    else if (m_mode == DrawEllipse) { double rx = qAbs(pos.x()-m_lastPos.x()); double ry = qAbs(pos.y()-m_lastPos.y()); m_current = std::make_shared<Ellipse>(m_lastPos, rx, ry); m_current->setPen(m_pen); m_current->setColor(m_color); }
    update();
}

void Canvas::mouseReleaseEvent(QMouseEvent* e) {
    if (m_drag) { m_drag = false; return; }
    if (m_boxSelecting) {
        m_boxSelecting = false;
        m_selected.reset();
        m_selection.clear();
        for (auto& s : m_shapes) {
            QRectF br = s->boundingRect();
            if (m_boxRect.intersects(br) || m_boxRect.contains(br)) m_selection.append(s);
        }
        for (auto it = m_shapes.crbegin(); it != m_shapes.crend(); ++it) { if (m_selection.contains(*it)) { m_selected = *it; break; } }
        update();
        emit selectionChanged();
        return;
    }
    if (!m_current && m_mode!=DrawTriangle) return;
    if (m_mode == DrawSegment || m_mode == DrawRectangle || m_mode == DrawCircle || m_mode == DrawEllipse) {
        if (m_current && m_current->length() > minSize()) {
            auto created = m_current;
            m_shapes.append(created);
            m_selected = created;
            m_selection.clear(); m_selection.append(created);
            emit selectionChanged();
        }
        m_current.reset(); m_hoverValid = false; update(); updateStats();
    }
}

void Canvas::updateStats() {
    int seg=0, pl=0, tri=0, rect=0, poly=0, cir=0, ell=0;
    for (auto& s : m_shapes) {
        QString t = s->typeName();
        if (t=="Segment") seg++;
        else if (t=="Polyline") pl++;
        else if (t=="Triangle") tri++;
        else if (t=="Rectangle") rect++;
        else if (t=="Polygon") poly++;
        else if (t=="Circle") cir++;
        else if (t=="Ellipse") ell++;
    }
    QString msg = QString("线段:%1 多折线:%2 三角形:%3 矩形:%4 多边形:%5 圆:%6 椭圆:%7").arg(seg).arg(pl).arg(tri).arg(rect).arg(poly).arg(cir).arg(ell);
    emit statsChanged(msg);
}

void Canvas::deleteSelected() {
    if (m_selection.isEmpty()) return;
    for (auto& s : m_selection) {
        for (int i = 0; i < m_shapes.size(); ++i) {
            if (m_shapes[i] == s) { m_shapes.removeAt(i); break; }
        }
    }
    m_selection.clear();
    m_selected.reset();
    update();
    updateStats();
}

void Canvas::moveSelectedBy(const QPointF& d) { if (!m_selection.isEmpty()) { for (auto& s : m_selection) s->moveBy(d); update(); } }

void Canvas::moveSelectedTo(const QPointF& p) { if (m_selected) { m_selected->moveTo(p); update(); } }

void Canvas::clearAll() { m_shapes.clear(); m_selection.clear(); m_selected.reset(); m_current.reset(); m_tempPts.clear(); update(); updateStats(); }

void Canvas::keyPressEvent(QKeyEvent* e) {
    if (m_selection.isEmpty()) return;
    int step = e->modifiers() & Qt::ShiftModifier ? 5 : 1;
    if (e->key() == Qt::Key_Delete) { deleteSelected(); return; }
    if (e->key() == Qt::Key_Left) { moveSelectedBy(QPointF(-step,0)); return; }
    if (e->key() == Qt::Key_Right) { moveSelectedBy(QPointF(step,0)); return; }
    if (e->key() == Qt::Key_Up) { moveSelectedBy(QPointF(0,-step)); return; }
    if (e->key() == Qt::Key_Down) { moveSelectedBy(QPointF(0,step)); return; }
}
