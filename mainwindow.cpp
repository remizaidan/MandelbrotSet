#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "mandelbrotthememanager.h"
#include "mandelbrotthemedialog.h"

#include <QFuture>
#include <QtConcurrent>
#include <QPixmap>
#include <QCloseEvent>
#include <QImageWriter>
#include <QFileDialog>

#include <QDebug>

/**
 * Initialize the GUI and connect signals to slots.
 *
 * @param parent Parent widget. Default is 0.
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_mandelbrotSet = ui->mandelbrotView->getMandelbrotSet();
    m_themeManager = m_mandelbrotSet->getThemeManager();

    QStringList themes = m_themeManager->getThemes();
    QString currentTheme = m_themeManager->getCurrentThemeName();
    ui->themeCombo->addItems(themes);
    ui->themeCombo->setCurrentText(currentTheme);

    m_currentDir = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation).at(0);

    connect(m_mandelbrotSet, SIGNAL(progressChanged(int)),
            this, SLOT(onProgressChanged(int)));
    connect(m_mandelbrotSet, SIGNAL(pixmapReady()),
            this, SLOT(doneUpdate()));
    connect(ui->mandelbrotView, SIGNAL(updateNeeded(double, double, double, double)),
            this, SLOT(startUpdate(double, double, double, double)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_exitButton_clicked()
{
    close();
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_saveButton_clicked()
{
    save();
}

void MainWindow::on_actionSave_triggered()
{
    save();
}

void MainWindow::on_unzoomButton_clicked()
{
    ui->mandelbrotView->unzoom();
}

/**
 * @param arg1 Order of the mandelbrot set.
 */
void MainWindow::on_orderSpinBox_valueChanged(double arg1)
{
    m_mandelbrotSet->setOrder(arg1);
}

/**
 * @param arg1 Precision parameter of the mandelbrot set.
 */
void MainWindow::on_precisionSpinBox_valueChanged(int arg1)
{
    if(arg1 <= 10) ui->precisionSpinBox->setSingleStep(1);
    else if(arg1 < 100) ui->precisionSpinBox->setSingleStep(10);
    else if(arg1 < 1000) ui->precisionSpinBox->setSingleStep(100);
    else if(arg1 < 10000) ui->precisionSpinBox->setSingleStep(1000);
    else if(arg1 < 100000) ui->precisionSpinBox->setSingleStep(10000);
    else ui->precisionSpinBox->setSingleStep(100000);
    m_mandelbrotSet->setPrecision(arg1);
}

void MainWindow::on_updateButton_clicked()
{
    if(ui->updateButton->text() == "Update") {
        double xmin = m_mandelbrotSet->getXmin();
        double xmax = m_mandelbrotSet->getXmax();
        double ymin = m_mandelbrotSet->getYmin();
        double ymax = m_mandelbrotSet->getYmax();
        startUpdate(xmin, xmax, ymin, ymax);
    }else{
        m_mandelbrotSet->cancel();
        doneUpdate();
    }
}

/**
 * @param arg1 Current text: designate the name of a theme.
 */
void MainWindow::on_themeCombo_currentTextChanged(const QString &arg1)
{
    m_themeManager->setTheme(arg1);
}

void MainWindow::on_editThemeButton_clicked()
{
    MandelbrotThemeDialog dlg;
    dlg.setThemeManager(m_themeManager);
    dlg.exec();
    if(dlg.result() == QDialog::Accepted) {
        m_themeManager->copyFrom(&dlg.getThemeManager());
        ui->themeCombo->blockSignals(true);
        ui->themeCombo->clear();
        ui->themeCombo->addItems(m_themeManager->getThemes());
        ui->themeCombo->setCurrentText(m_themeManager->getCurrentThemeName());
        ui->themeCombo->blockSignals(false);
    }
}


/**
 * The canculation of the Mandelbrot set happens in a concurent thread,
 * leaving the GUI active with these changes:
 * - Disable the save button and menu item.
 * - Change the "Update" button into a "Stop" button,
 * allowing to abort the calculation at any time.
 *
 * @param xmin Start of the x coordinate range in the complex plane.
 * @param xmax End of the x coordinate range in the complex plane.
 * @param ymin Start of the y coordinate range in the complex plane.
 * @param ymax End of the y coordinate range in the complex plane.
 */
void MainWindow::startUpdate(double xmin, double xmax, double ymin, double ymax)
{
    ui->saveButton->setEnabled(false);
    ui->actionSave->setEnabled(false);
    ui->updateButton->setText("Stop");
    QtConcurrent::run(ui->mandelbrotView, &MandelbrotView::update,
                      xmin, xmax, ymin, ymax);
}

/**
 * When the calculation of the Mandelbrot set is done,
 * resets the GUI to its original state:
 * - Enable the save button and menu item.
 * - Change the "Stop" button into an "Update" button,
 * allowing to start a new calculation.
 */
void MainWindow::doneUpdate()
{
    if(!ui->mandelbrotView->getPixmap().isNull()) {
        ui->saveButton->setEnabled(true);
        ui->actionSave->setEnabled(true);
    }
    QString zoomText;
    zoomText.sprintf("x %g", m_mandelbrotSet->getZoomFactor());
    ui->scaleLabel->setText(zoomText);
    ui->updateButton->setText("Update");
}


/**
 * @param progress Progress of the current calculation in percent.
 */
void MainWindow::onProgressChanged(int progress)
{
    ui->progressBar->setValue(progress);
}

void MainWindow::save()
{

    QPixmap pixmap = ui->mandelbrotView->getPixmap();

    QList<QByteArray> supportedFormats = QImageWriter::supportedImageFormats();

    QString filter = "Images (";
    for(QString ss : supportedFormats) {
        filter += " *."+ss;
    }
    filter += " ) ;; All Files (*.*)";

    QFileDialog dlg;
    dlg.setOption(QFileDialog::DontUseNativeDialog,true);
    dlg.setDirectory(m_currentDir);
    dlg.setFileMode(QFileDialog::AnyFile);
    dlg.setNameFilter(filter);
    dlg.exec();

    switch(dlg.result()) {
    case QDialog::Accepted: {

        bool isSaved = pixmap.save(dlg.selectedFiles()[0]);

        if(!isSaved) {
            qDebug() << "Error Saving image";
        }
    }
    }
}

/**
 * @param event Close event.
 */
void MainWindow::closeEvent(QCloseEvent *event)
{
    m_mandelbrotSet->cancel();
    doneUpdate();

    event->accept();
}

