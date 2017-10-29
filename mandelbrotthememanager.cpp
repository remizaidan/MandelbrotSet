#include "mandelbrotthememanager.h"

#include <QDebug>

MandelbrotThemeManager::MandelbrotThemeManager()
{
    MandelbrotThemeInfo themeInfo;
    themeInfo.setName("Black & White");
    themeInfo.setThemeType(MandelbrotThemeInfo::BiColor);
    themeInfo.setConvergedColor(QColor(0, 0, 0));
    themeInfo.setDivergedColor(QColor(255, 255, 255));
    addTheme(themeInfo);

    themeInfo.setName("White & Black");
    themeInfo.setConvergedColor(QColor(255, 255, 255));
    themeInfo.setDivergedColor(QColor(0, 0, 0));
    addTheme(themeInfo);

    setTheme("Black & White");
}

void MandelbrotThemeManager::copyFrom(const MandelbrotThemeManager *other)
{
    m_themesMap = other->m_themesMap;
    m_themesList = other->m_themesList;
    m_currentTheme = other->m_currentTheme;
}

const QStringList &MandelbrotThemeManager::getThemes() const
{
    return m_themesList;
}

const QString &MandelbrotThemeManager::getCurrentThemeName() const
{
    return m_currentTheme;
}

const MandelbrotThemeInfo &MandelbrotThemeManager::getCurrentThemeInfo() const
{
    return getCurrentTheme().getThemeInfo();
}

const MandelbrotTheme &MandelbrotThemeManager::getCurrentTheme() const
{
    std::map<QString, MandelbrotTheme>::const_iterator itr = m_themesMap.find(m_currentTheme);
    return itr->second;
}

MandelbrotTheme &MandelbrotThemeManager::getCurrentTheme()
{
    return m_themesMap[m_currentTheme];
}


QColor MandelbrotThemeManager::getColor(const MandelbrotSet::MandelbrotPoint &p)
{
    return getCurrentTheme().getColor(p);
}


void MandelbrotThemeManager::setTheme(QString themeName)
{
    std::map<QString, MandelbrotTheme>::iterator itr = m_themesMap.find(themeName);
    if(itr != m_themesMap.end()) {
        m_currentTheme = itr->first;
    }
}

bool MandelbrotThemeManager::addTheme(const MandelbrotThemeInfo &themeInfo)
{
    std::map<QString, MandelbrotTheme>::iterator itr = m_themesMap.find(themeInfo.getName());
    if(itr != m_themesMap.end()) {
        MandelbrotTheme &theme = itr->second;
        theme.setThemeInfo(themeInfo);
        return false;
    }else{
        m_themesMap.insert(std::make_pair(themeInfo.getName(),
                                          MandelbrotTheme(themeInfo)));
        m_themesList.push_back(themeInfo.getName());
        return true;
    }
}

void MandelbrotThemeManager::renameCurrentTheme(QString newName)
{
    std::map<QString, MandelbrotTheme>::iterator mapItr = m_themesMap.find(m_currentTheme);
    if(mapItr != m_themesMap.end()) {
      std::swap(m_themesMap[newName], mapItr->second);
      m_themesMap.erase(mapItr);
    }

    QStringList::iterator listItr = std::find(m_themesList.begin(), m_themesList.end(), m_currentTheme);
    if(listItr != m_themesList.end()) {
        *listItr = newName;
    }

    m_currentTheme = newName;
}

