#pragma once
#include <QWidget>
#include <QList>
#include <memory>
#include <QJsonDocument>
#include <QFile>
#include "../core/Shape.h"
#include "../core/shapes/Segment.h"
#include "../core/shapes/Polyline.h"
#include "../core/shapes/Triangle.h"
#include "../core/shapes/Rectangle.h"
#include "../core/shapes/Polygon.h"
#include "../core/shapes/Circle.h"
#include "../core/shapes/Ellipse.h"

class Canvas : public QWidget {
    Q_OBJECT
public:
    enum Mode { None, DrawSegment, DrawPolyline, DrawTriangle, DrawRectangle, DrawPolygon, DrawCircle, DrawEllipse, Select };
    explicit Canvas(QWidget* parent = nullptr);
    void setMode(Mode m);
    void setShowLabels(bool b);
    void setPen(const QPen& pen);
    void setColor(const QColor& c);
    void rotateSelected(double degrees);
    void saveToFile(const QString& path);
    void loadFromFile(const QString& path);
    std::shared_ptr<Shape> selected() const;
    void deleteSelected();
    void moveSelectedBy(const QPointF& d);
    void moveSelectedTo(const QPointF& p);
    void clearAll();
signals:
    void statsChanged(QString s);
    void selectionChanged();
protected:
    void paintEvent(QPaintEvent*) override;
    void mousePressEvent(QMouseEvent*) override;
    void mouseMoveEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;
    void keyPressEvent(QKeyEvent*) override;
private:
    QList<std::shared_ptr<Shape>> m_shapes;
    std::shared_ptr<Shape> m_current;
    std::shared_ptr<Shape> m_selected;
    QList<std::shared_ptr<Shape>> m_selection;
    Mode m_mode = None;
    bool m_drag = false;
    QPointF m_lastPos;
    bool m_showLabels = true;
    QPen m_pen = QPen(Qt::black, 2);
    QColor m_color = Qt::black;
    void updateStats();
    QVector<QPointF> m_tempPts;
    QPointF m_hoverPos;
    bool m_hoverValid = false;
    bool m_boxSelecting = false;
    QPointF m_boxStart;
    QRectF m_boxRect;
    double minSize() const { return 1.0; }
};
