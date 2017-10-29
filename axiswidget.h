#ifndef AXISWIDGET_H
#define AXISWIDGET_H

#include <QLabel>

/**
 * @brief The AxisWidget class
 *
 * Diplays a vertical or horizontal axis with ticks, labels and a mobile cursor.
 */
class AxisWidget : public QLabel
{
    Q_OBJECT
public:

    /** Constructor */
    explicit AxisWidget(QWidget *parent = nullptr);

    /** Destructor */
    ~AxisWidget();

    /** Set the length of the axis in pixels. */
    void setLength(int length);

    /** Set the cursor position in pixels. */
    void setCursorPos(int pos);

    /** Set the zoom factor. */
    void setZoomFactor(double zoomFactor);

    /** Set the orientation. */
    void setOrientation(Qt::Orientation orientation);

    /** Draw or redraw the axis .*/
    void draw();

    /** Set the position on the coordinate system. */
    void setValue(double x);

    /** Set the coordinate system range. */
    void setRange(double xmin, double xmax);

protected:

    /** Draw the borders of the axis. */
    void drawBorders(QPainter *painter, const QSize &s);

    /** Draw the ticks and labels. */
    void drawTicksAndLabels(QPainter *painter, const QSize &s);

    /** Draw the cursor. */
    void drawCursor(QPainter *painter, const QSize &s);

    /** Round to the nearest power. */
    double round(double x, double power);

    int m_length; //!< Length of the axis in pixels.
    double m_minValue; //!< Lower boundary in the coordinate system.
    double m_maxValue; //!< Upper boundary in the coordinate system.
    double m_value; //!< Position of the cursor in the coordinate system.
    double m_tickSpacing; //!< Spacing of the ticks in the coordinate system.
    double m_zoomFactor; //!< Zoom factor.
    Qt::Orientation m_orientation; //!< Orientation of the axis.

};

#endif // AXISWIDGET_H
