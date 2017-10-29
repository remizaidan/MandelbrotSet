#include "axiswidget.h"

#include <QPainter>
#include <math.h>


/**
 * This class inerits from QLabel and draws the axis elements on a QPixmap.
 *
 * @param parent Parent widget
 */
AxisWidget::AxisWidget(QWidget *parent) : QLabel(parent)
{
    m_minValue = -2;
    m_maxValue = 2;
    m_value = m_minValue;
    m_orientation = Qt::Horizontal;
    m_length = size().width();
    m_tickSpacing = 1./25.;
    m_zoomFactor = 1;
}

AxisWidget::~AxisWidget()
{

}

/**
 * Changing the length triggers a redraw.
 *
 * @param length Length of the axis in pixels.
 */
void AxisWidget::setLength(int length)
{
    m_length = length;
    draw();
}

/**
 * This method can be used to track the mouse movement:
 * the parent widget might call this method in a mouseMoveEvent() handler.
 *
 * Changing the cursor position triggers a redraw.
 *
 * @param pos Position of the cursor in pixels.
 */
void AxisWidget::setCursorPos(int pos)
{
    m_value = m_minValue + (m_maxValue - m_minValue) * (double)pos / (double)m_length;
    if(m_value < m_minValue) m_value = m_minValue;
    if(m_value > m_maxValue) m_value = m_maxValue;

    draw();
}

/**
 * The zoom factor is used to recalculate the tick spacing after a zoom on the range.
 *
 * @param zoomFactor Zoom factor.
 */
void AxisWidget::setZoomFactor(double zoomFactor)
{
    m_zoomFactor = zoomFactor;
    draw();
}

/**
 * @param orientation Orientation: Qt::Vertical or Qt::Horizontal.
 */
void AxisWidget::setOrientation(Qt::Orientation orientation)
{
    m_orientation = orientation;
    draw();
}

void AxisWidget::draw()
{

    // Compute the size based on the orientation and the size of the widget.
    QSize s;
    switch(m_orientation) {
    case Qt::Horizontal:
        s.setWidth(m_length);
        s.setHeight(size().height());
        break;
    case Qt::Vertical:
        s.setWidth(size().width());
        s.setHeight(m_length);
        break;
    }

    // Create the pixmap
    QPixmap pixmap(s);
    pixmap.fill(QColor(255,255,208));

    // Paint the axis elements
    QPainter painter(&pixmap);
    drawBorders(&painter, s);
    drawTicksAndLabels(&painter, s);
    drawCursor(&painter, s);

    // Sets the pixmap as this label's pixmap
    setPixmap(pixmap);
    update();
}

/**
 * @param painter Painter to draw on the pixmap.
 * @param s Size of the pixmap.
 */
void AxisWidget::drawBorders(QPainter *painter, const QSize &s)
{

    painter->setPen(QColor(0,0,0));

    switch(m_orientation) {
    case Qt::Vertical:
        painter->drawLine(s.width()-1, 0, s.width()-1, s.height()-1);
        break;
    case Qt::Horizontal:
        painter->drawLine(0, s.height()-1, s.width()-1, s.height()-1);
        break;
    default:
        break;
    }
}

/**
 * @param painter Painter to draw on the pixmap.
 * @param s Size of the pixmap.
 */
void AxisWidget::drawTicksAndLabels(QPainter *painter, const QSize &s)
{

    painter->setPen(QColor(0,0,0));
    QFont font = painter->font();
    font.setPointSize(8);
    painter->setFont(font);

    double dx = m_tickSpacing / m_zoomFactor;

    double precision = pow(10, floor(log10(2./(m_maxValue - m_minValue)))+2);

    int itick = 0;
    for(int i=0; i<m_length; i++) {

        double x = ((double)i/(double)m_length) * (m_maxValue - m_minValue);

        bool drawTick = x >= itick*dx;

        if(drawTick) {

            int tickSize = 5;
            if(itick % 5 == 0) tickSize = 10;
            if(itick % 10 == 0) tickSize = 15;

            switch(m_orientation) {
            case Qt::Vertical:
                painter->drawLine(s.width()-tickSize-1, i, s.width()-1, i);
                break;
            case Qt::Horizontal:
                painter->drawLine(i, s.height()-tickSize-1, i, s.height()-1);
                break;
            default:
                break;
            }

            if(itick%10 == 0) {

                QString slabel;
                slabel.sprintf("%g", round(m_minValue + itick*dx, precision));

                switch(m_orientation) {
                case Qt::Vertical:
                    painter->save();
                    painter->translate(s.width()-tickSize-3, i+12);
                    painter->rotate(-90);
                    painter->drawText(0, 0, slabel);
                    painter->restore();
                    break;
                case Qt::Horizontal:
                    painter->drawText(i-8, s.height()-tickSize-2, slabel);
                    break;
                default:
                    break;
                }
            }

            itick++;
        }
    }
}

/**
 * @param painter Painter to draw on the pixmap.
 * @param s Size of the pixmap.
 */
void AxisWidget::drawCursor(QPainter *painter, const QSize &s)
{

    painter->setPen(QColor(0,0,0));
    painter->setBrush(QColor(0,0,0));

    int i = m_length * (m_value - m_minValue) / (m_maxValue - m_minValue);

    QPolygon polygon;
    switch (m_orientation) {
    case Qt::Vertical:
        polygon << QPoint(s.width()-1, i)
                << QPoint(s.width()-8, i-3)
                << QPoint(s.width()-8, i+3);
        break;
    case Qt::Horizontal:
        polygon << QPoint(i, s.height()-1)
                << QPoint(i-3, s.height()-8)
                << QPoint(i+3, s.height()-8);
        break;
    default:
        break;
    }

    painter->drawPolygon(polygon);
}

/**
 * @param x Value to be rounded.
 * @param power power to which to round.
 * @return rounded value.
 */
double AxisWidget::round(double x, double power)
{
    double sign = x >= 0 ? 1 : -1;

    x *= sign;
    x *= power;
    x += 0.5;
    x = (int)x;
    x /= power;
    x *= sign;

    return x;
}

/**
 * @param x Cursor position in the coordinate system.
 */
void AxisWidget::setValue(double x)
{
    m_value = x;
    draw();
}

/**
 * @param xmin Lower bound of the coordinate range.
 * @param xmax Upper bound of the coordinate range.
 */
void AxisWidget::setRange(double xmin, double xmax)
{
    m_minValue = xmin;
    m_maxValue = xmax;
    draw();
}

