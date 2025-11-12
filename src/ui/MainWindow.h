#pragma once
#include <QMainWindow>
#include "Canvas.h"
class QToolBar;
class QDockWidget;
class QPushButton;
class QSpinBox;
class QComboBox;
class QSlider;
class QAction;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
private slots:
    void setModeSegment();
    void setModePolyline();
    void setModeTriangle();
    void setModeRectangle();
    void setModePolygon();
    void setModeCircle();
    void setModeEllipse();
    void setModeSelect();
    void toggleLabels();
    void chooseColor();
    void choosePen();
    void rotateLeft();
    void rotateRight();
    void save();
    void load();
    void deleteSelected();
    void clearAll();
    void moveToDialog();
    void onPenWidthChanged(int);
    void onStyleChanged(int);
    void onRotateChanged(int);
    void onSelectionChanged();
private:
    Canvas* m_canvas;
    bool m_labelsOn = true;
    QToolBar* m_toolbar = nullptr;
    QDockWidget* m_props = nullptr;
    QPushButton* m_colorBtn = nullptr;
    QSpinBox* m_widthSpin = nullptr;
    QComboBox* m_styleCombo = nullptr;
    QSlider* m_rotSlider = nullptr;
    int m_lastRot = 0;
    QAction* m_actSelect = nullptr;
    QAction* m_actDelete = nullptr;
    QAction* m_actClear = nullptr;
};
