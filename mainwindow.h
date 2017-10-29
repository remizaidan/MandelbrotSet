#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProgressDialog>

#include "mandelbrotset.h"

namespace Ui {
class MainWindow;
}
class QCloseEvent;
class QShowEvent;

/**
 * @brief The MainWindow class
 *
 * Manages the Main GUI. It is composed of two main sections:
 * - A view widget to display the generated Mandelbrot Set.
 * - An interactive section to change the settings
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    /** Constructor */
    explicit MainWindow(QWidget *parent = 0);

    /** Destructor */
    ~MainWindow();

private slots:

    /** Respond to exit button: close the main window, thus exiting the application. */
    void on_exitButton_clicked();

    /** Respond to exit menu item: close the main window, thus exiting the application. */
    void on_actionExit_triggered();

    /** Respond to save button: save the current displayed image. */
    void on_saveButton_clicked();

    /** Respond to save menu item: save the current displayed image. */
    void on_actionSave_triggered();

    /** Respond to unzoom button: reset the zoom to 1. */
    void on_unzoomButton_clicked();

    /** Respond to the order spin box: change the order of the mandelbrot set. */
    void on_orderSpinBox_valueChanged(double arg1);

    /** Respond to the precision spin box: change the presision of the mandelbrot set. */
    void on_precisionSpinBox_valueChanged(int arg1);

    /** Respond to the update button: update the mandelbrot set if settings have changed. */
    void on_updateButton_clicked();

    /** Respond to the theme combo box: change the selected theme. */
    void on_themeCombo_currentTextChanged(const QString &arg1);

    /** Respond to the Edit/New theme button: edit a theme or add a new theme. */
    void on_editThemeButton_clicked();

    /** Respond to the updateNeeded() signal: starts updating the mandelbrot set. */
    void startUpdate(double xmin, double xmax, double ymin, double ymax);

    /** Respond to the pixmapReady() signal: update the GUI when a new set is ready. */
    void doneUpdate();

    /** Respond to the progressChanged() signal: update the progress bar. */
    void onProgressChanged(int progress);

private:

    /** Save the current displayed image. */
    void save();

    /** Handles closeEvent: stop any calculation in progress before exiting. */
    void closeEvent(QCloseEvent *event);

    Ui::MainWindow *ui; //!< User interface object.

    MandelbrotSet *m_mandelbrotSet; //!< A Mandelbrot set.
    MandelbrotThemeManager *m_themeManager; //!< A theme manager.
    QString m_currentDir; //!< Last open directory by the save file dialog.

};

#endif // MAINWINDOW_H
