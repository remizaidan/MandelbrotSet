#include "mandelbrottheme.h"
#include <math.h>

#include <QDebug>

MandelbrotTheme::MandelbrotTheme()
{
    initPalette();
}

MandelbrotTheme::MandelbrotTheme(const MandelbrotThemeInfo &themeInfo)
{
    m_themeInfo = themeInfo;
    initPalette();
}

const MandelbrotThemeInfo &MandelbrotTheme::getThemeInfo() const
{
    return m_themeInfo;
}


void MandelbrotTheme::setThemeInfo(const MandelbrotThemeInfo &themeInfo)
{
    m_themeInfo = themeInfo;
    initPalette();
}

void MandelbrotTheme::setMandelbrotSet(MandelbrotSet *mandelbrotSet)
{
    m_mandelbrotSet = mandelbrotSet;

    if(m_themeInfo.getThemeType() == MandelbrotThemeInfo::Histogram) {
        double max = 0;
        m_histogram.clear();
        const std::vector<MandelbrotSet::MandelbrotPoint> &buffer = m_mandelbrotSet->getBuffer();
        int buffSize = buffer.size();
        for(int i=0; i<buffSize; i++) {
            MandelbrotSet::MandelbrotPoint p = buffer[i];
            if(p.n >= 0) {
                if(m_histogram.find(p.n) == m_histogram.end()) m_histogram[p.n] = 1;
                else m_histogram[p.n] += 1;
                if(m_histogram[p.n] > max) max = m_histogram[p.n];
            }
        }

        for(std::map<int, double>::iterator itr=m_histogram.begin();
            itr!=m_histogram.end(); itr++) {
            itr->second /= max;
        }
    }
}

QColor MandelbrotTheme::getColor(const MandelbrotSet::MandelbrotPoint &p)
{

    switch(m_themeInfo.getThemeType()) {
    case MandelbrotThemeInfo::BiColor:
        if(p.n < 0) {
            return m_themeInfo.getConvergedColor();
        }else{
            return m_themeInfo.getDivergedColor();
        }
    case MandelbrotThemeInfo::EscapeTime:
        if(p.n < 0) {
            return m_themeInfo.getConvergedColor();
        }else{
            return getPaletteInvertedColor(p.n);
        }
    case MandelbrotThemeInfo::Distance:
        if(p.n < 0) {
            return m_themeInfo.getConvergedColor();
        }else{
            double n = m_palette.size()*(p.de)/4;
            const QColor &color1 = getPaletteColor((int)floor(n));
            const QColor &color2 = getPaletteColor((int)floor(n)+1);

            double r1 = color1.red();
            double g1 = color1.green();
            double b1 = color1.blue();

            double r2 = color2.red();
            double g2 = color2.green();
            double b2 = color2.blue();

            double r = r1 + (r2 - r1) * (n - floor(n));
            double g = g1 + (g2 - g1) * (n - floor(n));
            double b = b1 + (b2 - b1) * (n - floor(n));

            return QColor(r, g, b);
        }
    case MandelbrotThemeInfo::Continuous:
        if(p.n < 0) {
            return m_themeInfo.getConvergedColor();
        }else{
            double n = p.n + 1 - log( log(p.r) / log(2) ) / log(m_mandelbrotSet->getOrder());
            const QColor &color1 = getPaletteInvertedColor(floor(n));
            const QColor &color2 = getPaletteInvertedColor(floor(n)+1);

            double r1 = color1.red();
            double g1 = color1.green();
            double b1 = color1.blue();

            double r2 = color2.red();
            double g2 = color2.green();
            double b2 = color2.blue();

            double r = r1 + (r2 - r1) * (n - floor(n));
            double g = g1 + (g2 - g1) * (n - floor(n));
            double b = b1 + (b2 - b1) * (n - floor(n));

            return QColor(r, g, b);
        }
    case MandelbrotThemeInfo::Histogram:
        if(p.n < 0) {
            return m_themeInfo.getConvergedColor();
        }else{
            return getPaletteColor((int)(99*m_histogram[p.n]));
        }
    default:
        return QColor(0,0,0);
    }
}

const QColor &MandelbrotTheme::getPaletteColor(int i) const
{
    static QColor defaultColor = QColor(0,0,0);
    if(m_palette.size() == 0) {
        return defaultColor;
    }
    return m_palette[i % m_palette.size()];
}

const QColor &MandelbrotTheme::getPaletteInvertedColor(int i) const
{

    static QColor defaultColor = QColor(0,0,0);
    if(m_palette.size() == 0) {
        return defaultColor;
    }
    return m_palette[m_palette.size() - (i % m_palette.size()) - 1];
}
void MandelbrotTheme::initPalette()
{

    m_palette.resize(m_themeInfo.getNColors());

    MandelbrotThemeInfo::PaletteType type = m_themeInfo.getPaletteType();
    switch (type) {
    case MandelbrotThemeInfo::Linear:
    case MandelbrotThemeInfo::Logarithmic:
    case MandelbrotThemeInfo::Exponential:
    case MandelbrotThemeInfo::Trigonometric:
        for(unsigned int i=0; i<m_palette.size(); i++) {

            double f = (1. + i) / m_palette.size();
            if(type == MandelbrotThemeInfo::Logarithmic) f = log(1+i) / log(m_palette.size());
            if(type == MandelbrotThemeInfo::Exponential) f = exp(1+i) / exp(m_palette.size());
            if(type == MandelbrotThemeInfo::Trigonometric) f = cos(f*M_PI-M_PI/2);

            int r1 = m_themeInfo.getStartColor().red();
            int g1 = m_themeInfo.getStartColor().green();
            int b1 = m_themeInfo.getStartColor().blue();

            int r2 = m_themeInfo.getEndColor().red();
            int g2 = m_themeInfo.getEndColor().green();
            int b2 = m_themeInfo.getEndColor().blue();

            int r = r1 + f * (r2 - r1);
            int g = g1 + f * (g2 - g1);
            int b = b1 + f * (b2 - b1);

            m_palette[i].setRgb(r, g, b);
        }
        break;
    case MandelbrotThemeInfo::Rainbow:
        for(unsigned int i=0; i<m_palette.size(); i++) {

            double f = (1. + i) / m_palette.size();

            double r = f - 3./6.;
            double g = f - 5./6.;
            double b = f - 1./6.;
            while(r < -0.5) r++;
            while(g < -0.5) g++;
            while(b < -0.5) b++;
            while(r > 0.5) r--;
            while(g > 0.5) g--;
            while(b > 0.5) b--;
            r = 6. * fabs(r) - 1.;
            g = 6. * fabs(g) - 1.;
            b = 6. * fabs(b) - 1.;
            if(r < 0) r = 0;
            if(r > 1) r = 1;
            if(g < 0) g = 0;
            if(g > 1) g = 1;
            if(b < 0) b = 0;
            if(b > 1) b = 1;

            r *= 255;
            g *= 255;
            b *= 255;

            m_palette[i].setRgb((int)r, (int)g, (int)b);
        }
        break;
    case MandelbrotThemeInfo::Randomized:
        srand(m_themeInfo.getRandomSeed());
        for(unsigned int i=0; i<m_palette.size(); i++) {

          int r = rand() % 256;
          int g = rand() % 256;
          int b = rand() % 256;

          m_palette[i].setRgb(r, g, b);
        }
        break;
    default:
        break;
    }
}

