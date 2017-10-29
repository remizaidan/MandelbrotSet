#ifndef MANDELBROTTHEMEINFO_H
#define MANDELBROTTHEMEINFO_H

#include <QString>
#include <QColor>

/**
 * @brief The MandelbrotThemeInfo class
 *
 * This class holds various settings used to define a color theme.
 */
class MandelbrotThemeInfo
{
public:

    /** Constructor */
    MandelbrotThemeInfo();

    /**
     * @brief The ThemeType enum
     *
     * Various theme types.
     */
    enum ThemeType {
        BiColor,      //!< Only two colors: converged/diverged
        EscapeTime,   //!< Use Escape Time algorithm
        Distance,     //!< Use distance to the set boundary estimation
        Continuous,   //!< Use a normalized iteration count
        Histogram,    //!< Use a histogram algorithm
        NoTheme       //!< Not a valid theme
    };

    /**
     * @brief The PaletteType enum
     *
     * Various color gradient palette types.
     */
    enum PaletteType {
        Linear,         //!< Use a linear gradient between two colors.
        Logarithmic,    //!< Use a logarithmic gradient between two colors.
        Exponential,    //!< Use an exonential gradient between two colors.
        Trigonometric,  //!< Use a trigonometric (cosine) gradient between two colors.
        Randomized,     //!< Use a randomized palette.
        Rainbow,        //!< Use a preset rainbow palette.
        NoPalette       //!< Not a valid palette.
    };

    const QString &getName() const;
    ThemeType getThemeType() const;
    PaletteType getPaletteType() const;
    const QColor &getStartColor() const;
    const QColor &getEndColor() const;
    int getNColors() const;
    const QColor &getConvergedColor() const;
    const QColor &getDivergedColor() const;
    int getRandomSeed() const;

    void setName(const QString &name);
    void setThemeType(ThemeType type);
    void setPaletteType(PaletteType type);
    void setStartColor(const QColor &c);
    void setEndColor(const QColor &c);
    void setNColors(int n);
    void setConvergedColor(const QColor &c);
    void setDivergedColor(const QColor &c);
    void setRandomSeed(int seed);

    static QStringList getThemeTypes();
    static QString getThemeTypeName(ThemeType type);
    static ThemeType getThemeType(QString name);

    static QStringList getPaletteTypes();
    static QString getPaletteTypeName(PaletteType type);
    static PaletteType getPaletteType(QString name);

protected:

    QString m_name;
    ThemeType m_themeType;
    PaletteType m_paletteType;
    QColor m_startColor;
    QColor m_endColor;
    int m_nColors;
    QColor m_convergedColor;
    QColor m_divergedColor;
    int m_randomSeed;

    static QStringList s_themeTypes;
    static QStringList s_paletteTypes;

};

#endif // MANDELBROTTHEMEINFO_H
