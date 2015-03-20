#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>
#include <QMainWindow>
#include <QSpinBox>
#include <QColorDialog>
#include <QColor>
#include <QList>
#include <QSlider>

#include "conversion.h"

enum ColorSrc {
    NONE,
    RGB,
    HSL,
    CMYK,
    LUV
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void displaySample(QColor color);
    void displayWarning(bool warning);

    QColorDialog *dialog;

public slots:
    void updateColor(const QColor color, ColorSrc src = NONE);
    
private:
    Ui::MainWindow *ui;
    QList<QSlider*> m_sliders;

    bool m_lock;

    void generateSpinBoxes(QList<QSlider *> sliders);

private slots:
    void rgbUpdated();
    void hslUpdated();
    void cmykUpdated();
    void luvUpdated();
};

#endif // MAINWINDOW_H
