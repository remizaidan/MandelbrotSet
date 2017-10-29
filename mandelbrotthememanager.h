#ifndef MANDELBROTTHEMEMANAGER_H
#define MANDELBROTTHEMEMANAGER_H

#include <QColor>
#include <QString>

#include <map>

#include "mandelbrotset.h"
#include "mandelbrotthemeinfo.h"
#include "mandelbrottheme.h"

/**
 * @brief The MandelbrotThemeManager class
 *
 * This class handles a menu of several available themes.
 */
class MandelbrotThemeManager
{
public:

    /** Constructor */
    MandelbrotThemeManager();

    /** Get a color for a convergence test result using the current theme */
    QColor getColor(const MandelbrotSet::MandelbrotPoint &p);

    /** Copy from another instance. */
    void copyFrom(const MandelbrotThemeManager *other);

    /** Get the list of available themes by name */
    const QStringList &getThemes() const;

    /** Get the name of the current theme */
    const QString &getCurrentThemeName() const;

    /** Get the settings for the current theme */
    const MandelbrotThemeInfo &getCurrentThemeInfo() const;

    /** Get the current theme */
    const MandelbrotTheme &getCurrentTheme() const;

    /** Get the modifiable current theme */
    MandelbrotTheme &getCurrentTheme();

    /** Set the current theme by name */
    void setTheme(QString themeName);

    /** Adds a theme */
    bool addTheme(const MandelbrotThemeInfo &themeInfo);

    /** Rename an existing theme */
    void renameCurrentTheme(QString newName);

protected:

    std::map<QString, MandelbrotTheme> m_themesMap;
    QStringList m_themesList;
    QString m_currentTheme;
};

#endif // MANDELBROTTHEMEMANAGER_H
