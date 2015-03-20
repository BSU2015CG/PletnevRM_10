#ifndef CONVERSION_H
#define CONVERSION_H

#include <QColor>
#include <QDebug>
#include <math.h>

bool HSLtoRGB(int h, int s, int l, QColor &out);
bool RGBtoHSL(int &h, int &s, int &l, QColor rgb);

bool CMYKtoRGB(int c, int m, int y, int k, QColor &out);
bool RGBtoCMYK(int &c, int &m, int &y, int &k, QColor rgb);

bool LUVtoRGB(int l, int u, int v, QColor &out);
bool RGBtoLUV(int &l, int &u, int &v, QColor rgb);

#endif // CONVERSION_H
