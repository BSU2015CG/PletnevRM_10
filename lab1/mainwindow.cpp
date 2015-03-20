#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_lock = false;

    dialog = new QColorDialog();
    connect(dialog, SIGNAL(colorSelected(const QColor)), this, SLOT(updateColor(const QColor)));
    connect(ui->pushButton_selectColor, SIGNAL(clicked()), dialog, SLOT(open()));

    m_sliders.append(ui->horizontalSlider_hlsH);
    m_sliders.append(ui->horizontalSlider_hlsL);
    m_sliders.append(ui->horizontalSlider_hlsS);
    m_sliders.append(ui->horizontalSlider_luvL);
    m_sliders.append(ui->horizontalSlider_luvU);
    m_sliders.append(ui->horizontalSlider_luvV);
    m_sliders.append(ui->horizontalSlider_cmykC);
    m_sliders.append(ui->horizontalSlider_cmykM);
    m_sliders.append(ui->horizontalSlider_cmykY);
    m_sliders.append(ui->horizontalSlider_cmykK);
    m_sliders.append(ui->horizontalSlider_rgbR);
    m_sliders.append(ui->horizontalSlider_rgbG);
    m_sliders.append(ui->horizontalSlider_rgbB);
    for (int i=0; i<m_sliders.size(); i++)
        m_sliders[i]->setRange(0, 255);
    ui->horizontalSlider_hlsH->setRange(0, 359);
    ui->horizontalSlider_luvL->setRange(0, 100);
    ui->horizontalSlider_luvU->setRange(0, 100);
    ui->horizontalSlider_luvV->setRange(0, 100);
    generateSpinBoxes(m_sliders);

    connect(ui->horizontalSlider_hlsH, SIGNAL(valueChanged(int)), this, SLOT(hslUpdated()));
    connect(ui->horizontalSlider_hlsS, SIGNAL(valueChanged(int)), this, SLOT(hslUpdated()));
    connect(ui->horizontalSlider_hlsL, SIGNAL(valueChanged(int)), this, SLOT(hslUpdated()));
    connect(ui->horizontalSlider_cmykC, SIGNAL(valueChanged(int)), this, SLOT(cmykUpdated()));
    connect(ui->horizontalSlider_cmykM, SIGNAL(valueChanged(int)), this, SLOT(cmykUpdated()));
    connect(ui->horizontalSlider_cmykY, SIGNAL(valueChanged(int)), this, SLOT(cmykUpdated()));
    connect(ui->horizontalSlider_cmykK, SIGNAL(valueChanged(int)), this, SLOT(cmykUpdated()));
    connect(ui->horizontalSlider_rgbR, SIGNAL(valueChanged(int)), this, SLOT(rgbUpdated()));
    connect(ui->horizontalSlider_rgbG, SIGNAL(valueChanged(int)), this, SLOT(rgbUpdated()));
    connect(ui->horizontalSlider_rgbB, SIGNAL(valueChanged(int)), this, SLOT(rgbUpdated()));
    connect(ui->horizontalSlider_luvL, SIGNAL(valueChanged(int)), this, SLOT(luvUpdated()));
    connect(ui->horizontalSlider_luvU, SIGNAL(valueChanged(int)), this, SLOT(luvUpdated()));
    connect(ui->horizontalSlider_luvV, SIGNAL(valueChanged(int)), this, SLOT(luvUpdated()));

    updateColor(QColor(16, 128, 240));
    //displayWarning(true);
}

MainWindow::~MainWindow()
{
    delete ui;

    delete dialog;
}

void MainWindow::displayWarning(bool warning)
{
    if (warning) {
        ui->label_colorSample->setText("cannot\ndisplay color");
        QFont font = ui->label_colorSample->font();
        font.setPointSize(9);
        font.setBold(true);
        ui->label_colorSample->setFont(font);
    } else {
        ui->label_colorSample->setText("");
    }
}

void MainWindow::hslUpdated()
{
    int h = ui->horizontalSlider_hlsH->value();
    int s = ui->horizontalSlider_hlsS->value();
    int l = ui->horizontalSlider_hlsL->value();
    QColor col;
    HSLtoRGB(h, s, l, col);
    updateColor(col, HSL);
}

void MainWindow::cmykUpdated()
{
    int c = ui->horizontalSlider_cmykC->value();
    int m = ui->horizontalSlider_cmykM->value();
    int y = ui->horizontalSlider_cmykY->value();
    int k = ui->horizontalSlider_cmykK->value();
    QColor col;
    CMYKtoRGB(c, m, y, k, col);
    updateColor(col, CMYK);
}

void MainWindow::luvUpdated()
{
    int l = ui->horizontalSlider_luvL->value();
    int u = ui->horizontalSlider_luvU->value();
    int v = ui->horizontalSlider_luvV->value();
    QColor col;
    if (LUVtoRGB(l, u, v, col))
        updateColor(col, LUV);
    else
        displayWarning(true);
}

void MainWindow::rgbUpdated()
{
    int r = ui->horizontalSlider_rgbR->value();
    int g = ui->horizontalSlider_rgbG->value();
    int b = ui->horizontalSlider_rgbB->value();
    updateColor(QColor(r, g, b), RGB);
}

void MainWindow::updateColor(const QColor color, ColorSrc src)
{
    if (!m_lock) {
        m_lock = true;
        qDebug() << "updateColor(" << color.name() << ")";

        // HSL
        if (src != HSL) {
            int hue, saturation, lightness;
            RGBtoHSL(hue, saturation, lightness, color);
            ui->horizontalSlider_hlsH->setValue(hue);
            ui->horizontalSlider_hlsS->setValue(saturation);
            ui->horizontalSlider_hlsL->setValue(lightness);
            qDebug() << "HSL:" << hue << saturation << lightness;
        }

        // Luv
        if (src != LUV) {
            int l, u, v;
            RGBtoLUV(l, u, v, color);
            ui->horizontalSlider_luvL->setValue(l);
            ui->horizontalSlider_luvU->setValue(u);
            ui->horizontalSlider_luvV->setValue(v);
            qDebug() << "Luv:" << l << u << v;
        }

        // CMYK
        if (src != CMYK) {
            int cyan, magenta, yellow, key;
            RGBtoCMYK(cyan, magenta, yellow, key, color);
            ui->horizontalSlider_cmykC->setValue(cyan);
            ui->horizontalSlider_cmykM->setValue(magenta);
            ui->horizontalSlider_cmykY->setValue(yellow);
            ui->horizontalSlider_cmykK->setValue(key);
            qDebug() << "CMYK:" << cyan << magenta << yellow << key;
        }

        // RGB
        if (src != RGB) {
            int red = color.red();
            int green = color.green();
            int blue = color.blue();
            ui->horizontalSlider_rgbR->setValue(red);
            ui->horizontalSlider_rgbG->setValue(green);
            ui->horizontalSlider_rgbB->setValue(blue);
            qDebug() << "RGB:" << red << green << blue;
        }

        displaySample(color);
        displayWarning(false);
        m_lock = false;
    }
}

void MainWindow::displaySample(QColor color)
{
    QString hex = color.name().toUpper();

    ui->lineEdit_colorHex->setText(hex);
    ui->label_colorSample->setStyleSheet("QLabel { background-color : " + hex + "; }");
}

void MainWindow::generateSpinBoxes(QList<QSlider*> sliders)
{
    for (int i=0; i<sliders.size(); i++) {
        QSpinBox *sb = new QSpinBox(ui->centralWidget);
        sb->setRange(sliders[i]->minimum(), sliders[i]->maximum());

        QRect rect = sliders[i]->geometry();
        int offset = 10;
        int w = 56;
        int h = 24;
        int y = rect.top() + (rect.height() - h) / 2;
        int x = rect.right() + offset;
        sb->setGeometry(x, y, w, h);

        connect(sliders[i], SIGNAL(valueChanged(int)), sb, SLOT(setValue(int)));
        connect(sb, SIGNAL(valueChanged(int)), sliders[i], SLOT(setValue(int)));
        //sliders[i]->setRange(0, 255);
    }
}
