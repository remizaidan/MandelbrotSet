#ifndef MANDELBROTTHEMEDIALOG_H
#define MANDELBROTTHEMEDIALOG_H

#include <QDialog>

namespace Ui {
class MandelbrotThemeDialog;
}

#include<mandelbrotthememanager.h>

/**
 * @brief The MandelbrotThemeDialog class
 *
 * This dialog provides the GUI for interacting with the color themes.
 */
class MandelbrotThemeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MandelbrotThemeDialog(QWidget *parent = 0);
    ~MandelbrotThemeDialog();

    void setThemeManager(MandelbrotThemeManager *themeManager);
    const MandelbrotThemeManager &getThemeManager();

private slots:

    void on_nameCombo_currentIndexChanged(const QString &arg1);

    void onNameComboTextEdited(const QString &arg1);

    void on_typeCombo_currentTextChanged(const QString &arg1);

    void on_convergedColorButton_clicked();

    void on_divergedColorButton_clicked();

    void on_startColorButton_clicked();

    void on_endColorButton_clicked();

    void on_nColorsSpinBox_valueChanged(int arg1);

    void on_paletteTypeCombo_currentTextChanged(const QString &arg1);

    void on_newThemeButton_clicked();

    void on_seedSpinBox_valueChanged(int arg1);

    void on_seedButton_clicked();

protected:

    QPixmap getColorButtonIcon(const QColor &color);
    QPixmap getPalettePreviewPixmap();
    bool getColor(QColor &color);
    void showHideWidgets();
    void loadThemeInfo();

    Ui::MandelbrotThemeDialog *ui;

    MandelbrotThemeManager m_themeManager;
};

#endif // MANDELBROTTHEMEDIALOG_H
