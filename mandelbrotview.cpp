#include "mandelbrotview.h"
#include "ui_mandelbrotview.h"
#include "mandelbrotset.h"

#include <math.h>

#include <QPainter>
#include <QMouseEvent>

#include <QDebug>

MandelbrotView::MandelbrotView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MandelbrotView)
{
    ui->setupUi(this);

    ui->horizontalAxis->setOrientation(Qt::Horizontal);
    ui->verticalAxis->setOrientation(Qt::Vertical);

    ui->centralView->setCursor(QCursor(QPixmap(":/cursor/rc/CrossCursor.png"), 7, 7));

    m_mandelbrotSet = new MandelbrotSet();
    m_isBusy = false;

    m_isPressed = false;

}

MandelbrotView::~MandelbrotView()
{
    delete ui;
}

MandelbrotSet *MandelbrotView::getMandelbrotSet()
{
    return m_mandelbrotSet;
}

QPixmap MandelbrotView::getPixmap()
{
    return m_pixmap;
}


void MandelbrotView::update(double xmin, double xmax, double ymin, double ymax)
{
    if(m_isBusy) return;

    m_isBusy = true;

    QSize viewSize = ui->centralView->size();
    QSize pixmapSize;

    double viewAspectRatio = (double)(viewSize.width())/viewSize.height();
    double realAspectRatio = (xmax - xmin) / (ymax - ymin);

    if(viewAspectRatio > realAspectRatio) {
        pixmapSize.setWidth(viewSize.height() * realAspectRatio);
        pixmapSize.setHeight(viewSize.height());
    }else{
        pixmapSize.setWidth(viewSize.width());
        pixmapSize.setHeight(viewSize.width() / realAspectRatio);
    }

    QPixmap pixmap(pixmapSize);
    pixmap.fill(QColor(0, 0, 0));
    QImage image = pixmap.toImage();

    if(m_pixmap.isNull()) {
        ui->horizontalAxis->setLength(pixmapSize.width());
        ui->verticalAxis->setLength(pixmapSize.height());
        ui->horizontalAxis->setRange(xmin, xmax);
        ui->verticalAxis->setRange(ymin, ymax);
        ui->horizontalAxis->setZoomFactor(m_mandelbrotSet->getZoomFactor());
        ui->verticalAxis->setZoomFactor(m_mandelbrotSet->getZoomFactor());
    }

    bool finished = m_mandelbrotSet->fillPixmap(image, xmin, xmax, ymin, ymax);
    if(finished) {
        ui->horizontalAxis->setLength(pixmapSize.width());
        ui->verticalAxis->setLength(pixmapSize.height());
        ui->horizontalAxis->setRange(xmin, xmax);
        ui->verticalAxis->setRange(ymin, ymax);
        ui->horizontalAxis->setZoomFactor(m_mandelbrotSet->getZoomFactor());
        ui->verticalAxis->setZoomFactor(m_mandelbrotSet->getZoomFactor());

        pixmap = QPixmap::fromImage(image);
        ui->centralView->setPixmap(pixmap);
        m_pixmap = pixmap;
    }
    m_isBusy = false;
}

void MandelbrotView::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);

    double xmin = m_mandelbrotSet->getXmin();
    double xmax = m_mandelbrotSet->getXmax();
    double ymin = m_mandelbrotSet->getYmin();
    double ymax = m_mandelbrotSet->getYmax();

    emit updateNeeded(xmin, xmax, ymin, ymax);
}

void MandelbrotView::mousePressEvent(QMouseEvent *mouseEvent)
{
    if(m_isBusy) return;
    if(m_pixmap.isNull()) return;

    m_isPressed = true;
    m_startZoom = ui->centralView->mapFromParent(mouseEvent->pos());
    m_endZoom = ui->centralView->mapFromParent(mouseEvent->pos());
    constrainInPixmap(m_startZoom);
    constrainInPixmap(m_endZoom);
    updatePixmap();
}

void MandelbrotView::mouseMoveEvent(QMouseEvent *mouseEvent)
{
    QPoint pos = ui->centralView->mapFromParent(mouseEvent->pos());
    ui->horizontalAxis->setCursorPos(pos.x());
    ui->verticalAxis->setCursorPos(pos.y());

    if(m_isBusy) return;
    if(!m_isPressed) return;

    m_endZoom = pos;
    constrainInPixmap(m_endZoom);
    updatePixmap();
}

void MandelbrotView::mouseReleaseEvent(QMouseEvent *mouseEvent)
{
    if(m_isBusy) return;
    if(!m_isPressed) return;

    m_isPressed = false;

    m_endZoom = ui->centralView->mapFromParent(mouseEvent->pos());
    constrainInPixmap(m_endZoom);
    updatePixmap();

    if( fabs(m_endZoom.x() - m_startZoom.x()) >= 5 &&
            fabs(m_endZoom.y() - m_startZoom.y()) >= 5) {
        zoom();
    }
}

void MandelbrotView::zoom()
{

    double xmin = m_mandelbrotSet->getXmin();
    double xmax = m_mandelbrotSet->getXmax();
    double ymin = m_mandelbrotSet->getYmin();
    double ymax = m_mandelbrotSet->getYmax();

    double dx = xmax - xmin;
    double dy = ymax - ymin;
    double startZoomX = m_startZoom.x();
    double startZoomY = m_pixmap.height() - m_startZoom.y() - 1;
    double endZoomX = m_endZoom.x();
    double endZoomY = m_pixmap.height() - m_endZoom.y() - 1;
    double newXmin = xmin + (dx * startZoomX) / m_pixmap.width();
    double newYmin = ymin + (dy * startZoomY) / m_pixmap.height();
    double newXmax = xmin + (dx * endZoomX) / m_pixmap.width();
    double newYmax = ymin + (dy * endZoomY) / m_pixmap.height();
    if(newXmin > newXmax) {
        std::swap(newXmax, newXmin);
    }
    if(newYmin > newYmax) {
        std::swap(newYmax, newYmin);
    }

    emit updateNeeded(newXmin, newXmax, newYmin, newYmax);
}

void MandelbrotView::unzoom()
{
    emit updateNeeded(-2, 2, -2, 2);
}

void MandelbrotView::constrainInPixmap(QPoint &point)
{
    if(point.x() < 0) point.setX(0);
    if(point.x() >= m_pixmap.width()) point.setX(m_pixmap.width()-1);
    if(point.y() < 0) point.setY(0);
    if(point.y() >= m_pixmap.height()) point.setY(m_pixmap.height()-1);
}

void MandelbrotView::updatePixmap()
{

    QPixmap pixmap(m_pixmap);

    if(m_isPressed) {
        QPainter painter(&pixmap);

        painter.setPen(QColor(255,255,255));
        QRect rect;
        rect.setTopLeft(m_startZoom);
        rect.setBottomRight(m_endZoom-QPoint(1,1));
        painter.drawRect(rect);

        painter.setPen(QPen(QBrush(QColor(0,0,0)), 1, Qt::DashLine));
        painter.drawRect(rect);
    }
    ui->centralView->setPixmap(pixmap);
}
