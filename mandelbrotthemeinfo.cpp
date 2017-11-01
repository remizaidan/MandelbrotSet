#include "mandelbrotthemeinfo.h"

#include <QDebug>

MandelbrotThemeInfo::MandelbrotThemeInfo()
{

    m_name = "Default";
    m_themeType = BiColor;
    m_paletteType = Linear;
    m_startColor = QColor(0,0,0);
    m_endColor = QColor(255,255,255);
    m_nColors = 100;
    m_convergedColor = QColor(0,0,0);
    m_divergedColor = QColor(255,255,255);
    m_randomSeed = 1234;

}

QStringList MandelbrotThemeInfo::s_themeTypes = QStringList();
QStringList MandelbrotThemeInfo::s_paletteTypes = QStringList();

const QString &MandelbrotThemeInfo::getName() const
{
    return m_name;
}

MandelbrotThemeInfo::ThemeType MandelbrotThemeInfo::getThemeType() const
{
    return m_themeType;
}

MandelbrotThemeInfo::PaletteType MandelbrotThemeInfo::getPaletteType() const
{
    return m_paletteType;
}

const QColor &MandelbrotThemeInfo::getStartColor() const
{
    return m_startColor;
}

const QColor &MandelbrotThemeInfo::getEndColor() const
{
    return m_endColor;
}

int MandelbrotThemeInfo::getNColors() const
{
    return m_nColors;
}

const QColor &MandelbrotThemeInfo::getConvergedColor() const
{
    return m_convergedColor;
}

const QColor &MandelbrotThemeInfo::getDivergedColor() const
{
    return m_divergedColor;
}

int MandelbrotThemeInfo::getRandomSeed() const
{
    return m_randomSeed;
}

void MandelbrotThemeInfo::setName(const QString &name)
{
    m_name = name;
}

void MandelbrotThemeInfo::setThemeType(MandelbrotThemeInfo::ThemeType type)
{
    m_themeType = type;
}

void MandelbrotThemeInfo::setPaletteType(MandelbrotThemeInfo::PaletteType type)
{
    m_paletteType = type;
}

void MandelbrotThemeInfo::setStartColor(const QColor &c)
{
    m_startColor = c;
}

void MandelbrotThemeInfo::setEndColor(const QColor &c)
{
    m_endColor = c;
}

void MandelbrotThemeInfo::setNColors(int n)
{
    m_nColors = n;
}

void MandelbrotThemeInfo::setConvergedColor(const QColor &c)
{
    m_convergedColor = c;
}

void MandelbrotThemeInfo::setDivergedColor(const QColor &c)
{
    m_divergedColor = c;
}

void MandelbrotThemeInfo::setRandomSeed(int seed)
{
    m_randomSeed = seed;
}

QStringList MandelbrotThemeInfo::getThemeTypes()
{
    if(s_themeTypes.empty()) {
        s_themeTypes.push_back(getThemeTypeName(BiColor));
        s_themeTypes.push_back(getThemeTypeName(EscapeTime));
        s_themeTypes.push_back(getThemeTypeName(Continuous));
        s_themeTypes.push_back(getThemeTypeName(Histogram));
        s_themeTypes.push_back(getThemeTypeName(Distance));
    }
    return s_themeTypes;
}

QString MandelbrotThemeInfo::getThemeTypeName(ThemeType type)
{
    switch(type) {
    case BiColor: return "Bi-Color";
    case EscapeTime: return "Escape Time";
    case Continuous: return "Continuous Escape Time";
    case Histogram: return "Escape Time Histogram";
    case Distance: return "Distance";
    default: return "NoTheme";
    }
    return "Unknown";
}

MandelbrotThemeInfo::ThemeType MandelbrotThemeInfo::getThemeType(QString name)
{
    if(name == "Bi-Color") return BiColor;
    if(name == "Escape Time") return EscapeTime;
    if(name == "Distance") return Distance;
    if(name == "Continuous") return Continuous;
    if(name == "Histogram") return Histogram;
    return NoTheme;
}

QStringList MandelbrotThemeInfo::getPaletteTypes()
{
    if(s_paletteTypes.empty()) {
        s_paletteTypes.push_back(getPaletteTypeName(Linear));
        s_paletteTypes.push_back(getPaletteTypeName(Logarithmic));
        s_paletteTypes.push_back(getPaletteTypeName(Exponential));
        s_paletteTypes.push_back(getPaletteTypeName(Trigonometric));
        s_paletteTypes.push_back(getPaletteTypeName(Randomized));
        s_paletteTypes.push_back(getPaletteTypeName(Rainbow));
    }
    return s_paletteTypes;
}

QString MandelbrotThemeInfo::getPaletteTypeName(PaletteType type)
{
    switch (type) {
    case Linear: return "Linear";
    case Logarithmic: return "Logarithmic";
    case Exponential: return "Exponential";
    case Trigonometric: return "Trigonometric";
    case Randomized: return "Randomized";
    case Rainbow: return "Rainbow";
    case NoPalette: return "NoPalette";
    }
    return "Unknown";
}

MandelbrotThemeInfo::PaletteType MandelbrotThemeInfo::getPaletteType(QString name)
{
    if(name == "Linear") return Linear;
    if(name == "Logarithmic") return Logarithmic;
    if(name == "Exponential") return Exponential;
    if(name == "Trigonometric") return Trigonometric;
    if(name == "Randomized") return Randomized;
    if(name == "Rainbow") return Rainbow;
    return NoPalette;
}
