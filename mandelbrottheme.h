#ifndef MANDELBROTTHEME_H
#define MANDELBROTTHEME_H

#include "mandelbrotthemeinfo.h"
#include "mandelbrotset.h"

/**
 * @brief The MandelbrotTheme class
 *
 * This class handles a single theme which assigns a pixel color
 * to the result of the convergence test of the corresponding point.
 *
 * The theme settings are stored in a separate MandelbrotThemeInfo class.
 */
class MandelbrotTheme
{
public:

    /** Constructor */
    MandelbrotTheme();

    /** Constructor using a theme info. */
    MandelbrotTheme(const MandelbrotThemeInfo &themeInfo);

    /** Get the theme settings. */
    const MandelbrotThemeInfo &getThemeInfo() const;

    /** Set the theme settings. */
    void setThemeInfo(const MandelbrotThemeInfo &themeInfo);

    /** Set a pointer to the set being drawn. */
    void setMandelbrotSet(MandelbrotSet *mandelbrotSet);

    /** Get a color for a given point based on the result of the convergence test. */
    QColor getColor(const MandelbrotSet::MandelbrotPoint &p);

    /** Get a color from the color palette. */
    const QColor &getPaletteColor(int i) const;

    /** Get a color from the inverted color palette. */
    const QColor &getPaletteInvertedColor(int i) const;

protected:

    /** Construct a palette based on the theme settings. */
    void initPalette();

    MandelbrotThemeInfo m_themeInfo; //!< Theme settings.
    std::vector<QColor> m_palette; //!< Color palette.
    std::map<int, double> m_histogram; //!< Escape time histogram.

    MandelbrotSet *m_mandelbrotSet; //!< The mandelbrot set being drawn.
};

#endif // MANDELBROTTHEME_H
