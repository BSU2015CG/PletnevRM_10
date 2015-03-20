#include "conversion.h"

bool HSLtoRGB(int h, int s, int l, QColor &out)
{
    int c = ((255 - abs((2 * l) - 255)) * s) / 255;
    int x = c * (1 - fabs(((h % 120) / 60.0) - 1));
    int m = l - c/2;

    int r = 0;
    int g = 0;
    int b = 0;
    if ((h >= 0) && (h < 60)) {
        r = c;
        g = x;
    }
    if ((h >= 60) && (h < 120)) {
        r = x;
        g = c;
    }
    if ((h >= 120) && (h < 180)) {
        g = c;
        b = x;
    }
    if ((h >= 180) && (h < 240)) {
        g = x;
        b = c;
    }
    if ((h >= 240) && (h < 300)) {
        r = x;
        b = c;
    }
    if ((h >= 300) && (h < 360)) {
        r = c;
        b = x;
    }
    r += m;
    g += m;
    b += m;

    out.setRgb(r, g, b);
    return true;
}

bool RGBtoHSL(int &h, int &s, int &l, QColor rgb)
{
    int r = rgb.red();
    int g = rgb.green();
    int b = rgb.blue();
    int max = qMax(qMax(r, g), b);
    int min = qMin(qMin(r, g), b);
    int d = max - min;

    if (d == 0)
        h = 0;
    else {
        if (max == r)
            h = 60 * (g - b) / d;
        if (max == g)
            h = 60 * (b - r) / d + 120;
        if (max == b)
            h = 60 * (r - g) / d + 240;
        h = h % 360;
    }

    l = (max + min) / 2;

    if (max == 255)
        s = 255;
    else
        s = 255 * d / (double)(255 - abs(2 * l - 255));
    s = qMin(s, 255);

    return true;
}

bool CMYKtoRGB(int c, int m, int y, int k, QColor &out)
{
    int r = ((255 - c) * (255 - k)) / 255;
    int g = ((255 - m) * (255 - k)) / 255;
    int b = ((255 - y) * (255 - k)) / 255;
    out.setRgb(r, g, b);
    return true;
}

bool RGBtoCMYK(int &c, int &m, int &y, int &k, QColor rgb)
{
    int r = rgb.red();
    int g = rgb.green();
    int b = rgb.blue();
    k = 255 - qMax(qMax(r, g), b);
    if (k != 255) {
        c = (255 * (255 - r - k)) / (255 - k);
        m = (255 * (255 - g - k)) / (255 - k);
        y = (255 * (255 - b - k)) / (255 - k);
    } else {
        c = 0;
        m = 0;
        y = 0;
    }
    return true;
}

bool LUVtoRGB(int l, int u, int v, QColor &out)
{
    double x = 9. * l * u / (4. * v);
    double y = l;
    double z = (-3. * l * u - 20. * l * v + 1200. * l) / (4. * v);
    x /= 100.;
    y /= 100.;
    z /= 100.;
    double r = x * 3.2406 + y * -1.5372 + z * -0.4986;
    double g = x * -0.9689 + y * 1.8758 + z * 0.0415;
    double b = x * 0.0557 + y * -0.2040 + z * 1.0570;

    if (r > 0.0031308)
        r = 1.055 * pow(r, 1 / 2.4) - 0.055;
    else
        r = 12.92 * r;

    if (g > 0.0031308)
        g = 1.055 * pow(g, 1 / 2.4) - 0.055;
    else
        g = 12.92 * g;
    if (b > 0.0031308)
        b = 1.055 * pow(b, 1 / 2.4) - 0.055;
    else
        b = 12.92 * b;

    if (r < 0 || r >= 1 || g < 0 || g >= 1 || b < 0 || b >= 1)
        return false;
    else
        out.setRgb(r*255, g*255, b*255);

    return true;
}

bool RGBtoLUV(int &l, int &u, int &v, QColor rgb)
{
    double r = rgb.red() / 255.;
    double g = rgb.green() / 255.;
    double b = rgb.blue() / 255.;

    if (r > 0.04045)
        r = pow((r + 0.055) / 1.055, 2.4);
    else
        r = r / 12.92;

    if (g > 0.04045)
        g = pow((g + 0.055) / 1.055, 2.4);
    else
        g = g / 12.92;

    if (b > 0.04045)
        b = pow((b + 0.055) / 1.055, 2.4);
    else
        b = b / 12.92;

    r *= 100;
    g *= 100;
    b *= 100;
    double x = r * 0.4124 + g * 0.3576 + b * 0.1805;
    double y = r * 0.2126 + g * 0.7152 + b * 0.0722;
    double z = r * 0.0193 + g * 0.1192 + b * 0.9505;

    l = y;
    u = 400 * x / (x + 15 * y + 3 * z);
    v = 900 * y / (x + 15 * y + 3 * z);

    return true;
}
