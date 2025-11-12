#include "MainWindow.h"
#include <QMenuBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QColorDialog>
#include <QInputDialog>
#include <QToolBar>
#include <QDockWidget>
#include <QPushButton>
#include <QSpinBox>
#include <QComboBox>
#include <QSlider>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    m_canvas = new Canvas(this);
    setCentralWidget(m_canvas);
    m_toolbar = addToolBar("工具");
    auto file = menuBar()->addMenu("文件");
    auto edit = menuBar()->addMenu("编辑");
    auto insert = menuBar()->addMenu("绘制");
    auto view = menuBar()->addMenu("视图");

    file->addAction("保存", this, &MainWindow::save);
    file->addAction("加载", this, &MainWindow::load);

    auto aSeg = insert->addAction("线段", this, &MainWindow::setModeSegment);
    auto aPl = insert->addAction("多段折线", this, &MainWindow::setModePolyline);
    auto aTri = insert->addAction("三角形", this, &MainWindow::setModeTriangle);
    auto aRect = insert->addAction("矩形", this, &MainWindow::setModeRectangle);
    auto aPoly = insert->addAction("多边形", this, &MainWindow::setModePolygon);
    auto aCir = insert->addAction("圆形", this, &MainWindow::setModeCircle);
    auto aEll = insert->addAction("椭圆形", this, &MainWindow::setModeEllipse);
    m_actSelect = insert->addAction("选择", this, &MainWindow::setModeSelect);

    edit->addAction("颜色", this, &MainWindow::chooseColor);
    edit->addAction("线型", this, &MainWindow::choosePen);
    edit->addAction("左旋转15°", this, &MainWindow::rotateLeft);
    edit->addAction("右旋转15°", this, &MainWindow::rotateRight);
    edit->addAction("删除选中", this, &MainWindow::deleteSelected);
    edit->addAction("清空所有", this, &MainWindow::clearAll);
    edit->addAction("移动到...", this, &MainWindow::moveToDialog);

    m_actDelete = m_toolbar->addAction("删除选中", this, &MainWindow::deleteSelected);
    m_actClear = m_toolbar->addAction("清空所有", this, &MainWindow::clearAll);
    m_toolbar->addAction("选择", this, &MainWindow::setModeSelect);
    m_toolbar->addSeparator();
    m_toolbar->addAction("线段", this, &MainWindow::setModeSegment);
    m_toolbar->addAction("折线", this, &MainWindow::setModePolyline);
    m_toolbar->addAction("三角形", this, &MainWindow::setModeTriangle);
    m_toolbar->addAction("矩形", this, &MainWindow::setModeRectangle);
    m_toolbar->addAction("多边形", this, &MainWindow::setModePolygon);
    m_toolbar->addAction("圆", this, &MainWindow::setModeCircle);
    m_toolbar->addAction("椭圆", this, &MainWindow::setModeEllipse);

    view->addAction("显示/隐藏属性标签", this, &MainWindow::toggleLabels);

    statusBar()->showMessage("就绪");
    connect(m_canvas, &Canvas::statsChanged, this, [this](QString s){ statusBar()->showMessage(s); });
    connect(m_canvas, &Canvas::selectionChanged, this, &MainWindow::onSelectionChanged);

    m_props = new QDockWidget("属性", this);
    QWidget* panel = new QWidget(m_props);
    QVBoxLayout* v = new QVBoxLayout;
    QHBoxLayout* l1 = new QHBoxLayout; l1->addWidget(new QLabel("颜色")); m_colorBtn = new QPushButton("选择"); l1->addWidget(m_colorBtn); v->addLayout(l1);
    QHBoxLayout* l2 = new QHBoxLayout; l2->addWidget(new QLabel("线宽")); m_widthSpin = new QSpinBox; m_widthSpin->setRange(1, 20); m_widthSpin->setValue(2); l2->addWidget(m_widthSpin); v->addLayout(l2);
    QHBoxLayout* l3 = new QHBoxLayout; l3->addWidget(new QLabel("线型")); m_styleCombo = new QComboBox; m_styleCombo->addItems({"实线","虚线","点线","点划线"}); l3->addWidget(m_styleCombo); v->addLayout(l3);
    QHBoxLayout* l4 = new QHBoxLayout; l4->addWidget(new QLabel("旋转")); m_rotSlider = new QSlider(Qt::Horizontal); m_rotSlider->setRange(-180, 180); m_rotSlider->setValue(0); l4->addWidget(m_rotSlider); v->addLayout(l4);
    QHBoxLayout* l5 = new QHBoxLayout; QPushButton* rotL = new QPushButton("左旋15°"); QPushButton* rotR = new QPushButton("右旋15°"); l5->addWidget(rotL); l5->addWidget(rotR); v->addLayout(l5);
    QPushButton* delBtn = new QPushButton("删除选中"); v->addWidget(delBtn);
    QPushButton* clrBtn = new QPushButton("清空所有"); v->addWidget(clrBtn);
    panel->setLayout(v);
    m_props->setWidget(panel);
    addDockWidget(Qt::RightDockWidgetArea, m_props);
    connect(m_colorBtn, &QPushButton::clicked, this, &MainWindow::chooseColor);
    connect(m_widthSpin, qOverload<int>(&QSpinBox::valueChanged), this, &MainWindow::onPenWidthChanged);
    connect(m_styleCombo, qOverload<int>(&QComboBox::currentIndexChanged), this, &MainWindow::onStyleChanged);
    connect(m_rotSlider, &QSlider::valueChanged, this, &MainWindow::onRotateChanged);
    connect(rotL, &QPushButton::clicked, this, &MainWindow::rotateLeft);
    connect(rotR, &QPushButton::clicked, this, &MainWindow::rotateRight);
    connect(delBtn, &QPushButton::clicked, this, &MainWindow::deleteSelected);
    connect(clrBtn, &QPushButton::clicked, this, &MainWindow::clearAll);
}

void MainWindow::setModeSegment() { m_canvas->setMode(Canvas::DrawSegment); }
void MainWindow::setModePolyline() { m_canvas->setMode(Canvas::DrawPolyline); }
void MainWindow::setModeTriangle() { m_canvas->setMode(Canvas::DrawTriangle); }
void MainWindow::setModeRectangle() { m_canvas->setMode(Canvas::DrawRectangle); }
void MainWindow::setModePolygon() { m_canvas->setMode(Canvas::DrawPolygon); }
void MainWindow::setModeCircle() { m_canvas->setMode(Canvas::DrawCircle); }
void MainWindow::setModeEllipse() { m_canvas->setMode(Canvas::DrawEllipse); }
void MainWindow::setModeSelect() { m_canvas->setMode(Canvas::Select); }

void MainWindow::toggleLabels() { m_labelsOn = !m_labelsOn; m_canvas->setShowLabels(m_labelsOn); }

void MainWindow::chooseColor() { QColor c = QColorDialog::getColor(Qt::black, this, "选择颜色"); if (c.isValid()) m_canvas->setColor(c); }

void MainWindow::choosePen() { bool ok = false; int w = QInputDialog::getInt(this, "线宽", "宽度", 2, 1, 20, 1, &ok); if (!ok) return; QStringList styles = {"实线","虚线","点线","点划线","自定义"}; QString s = QInputDialog::getItem(this, "线型", "样式", styles, 0, false, &ok); if (!ok) return; Qt::PenStyle ps = Qt::SolidLine; if (s == "虚线") ps = Qt::DashLine; else if (s == "点线") ps = Qt::DotLine; else if (s == "点划线") ps = Qt::DashDotLine; QPen pen = QPen(Qt::black, w); pen.setStyle(ps); m_canvas->setPen(pen); }

void MainWindow::rotateLeft() { m_canvas->rotateSelected(-15); }
void MainWindow::rotateRight() { m_canvas->rotateSelected(15); }

void MainWindow::deleteSelected() { m_canvas->deleteSelected(); }

void MainWindow::clearAll() { m_canvas->clearAll(); }

void MainWindow::moveToDialog() {
    bool okx=false, oky=false;
    double x = QInputDialog::getDouble(this, "移动到", "X", 100.0, -10000, 10000, 1, &okx);
    if (!okx) return;
    double y = QInputDialog::getDouble(this, "移动到", "Y", 100.0, -10000, 10000, 1, &oky);
    if (!oky) return;
    m_canvas->moveSelectedTo(QPointF(x,y));
}

void MainWindow::onPenWidthChanged(int v) {
    QPen pen = m_canvas->selected() ? m_canvas->selected()->pen() : QPen(Qt::black, v);
    pen.setWidth(v);
    m_canvas->setPen(pen);
}

void MainWindow::onStyleChanged(int idx) {
    Qt::PenStyle ps = Qt::SolidLine;
    if (idx==1) ps = Qt::DashLine; else if (idx==2) ps = Qt::DotLine; else if (idx==3) ps = Qt::DashDotLine;
    QPen pen = m_canvas->selected() ? m_canvas->selected()->pen() : QPen(Qt::black, m_widthSpin->value());
    pen.setStyle(ps);
    m_canvas->setPen(pen);
}

void MainWindow::onRotateChanged(int val) {
    int delta = val - m_lastRot;
    m_lastRot = val;
    m_canvas->rotateSelected(delta);
}

void MainWindow::onSelectionChanged() {
    auto s = m_canvas->selected();
    m_lastRot = 0; m_rotSlider->setValue(0);
    if (!s) return;
    QPen pen = s->pen();
    m_widthSpin->setValue(pen.width());
    int idx = 0; if (pen.style()==Qt::DashLine) idx=1; else if (pen.style()==Qt::DotLine) idx=2; else if (pen.style()==Qt::DashDotLine) idx=3;
    m_styleCombo->setCurrentIndex(idx);
}

void MainWindow::save() { QString path = QFileDialog::getSaveFileName(this, "保存", "shapes.json", "JSON (*.json)"); if (!path.isEmpty()) m_canvas->saveToFile(path); }
void MainWindow::load() { QString path = QFileDialog::getOpenFileName(this, "加载", {}, "JSON (*.json)"); if (!path.isEmpty()) m_canvas->loadFromFile(path); }
