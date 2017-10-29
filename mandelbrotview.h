#ifndef MANDELBROTVIEW_H
#define MANDELBROTVIEW_H

#include <QWidget>
#include <QPixmap>

namespace Ui {
class MandelbrotView;
}
class QShowEvent;

class MandelbrotSet;

/**
 * @brief The MandelbrotView class
 *
 * This widget displays the mandelbrot set along with two axes.
 * It also allows to interact with the set for zooming in.
 */
class MandelbrotView : public QWidget
{
    Q_OBJECT

public:
    explicit MandelbrotView(QWidget *parent = 0);
    ~MandelbrotView();

    MandelbrotSet *getMandelbrotSet();

    QPixmap getPixmap();

    void update(double xmin, double xmax, double ymin, double ymax);
    void unzoom();

signals:

    void updateNeeded(double xmin, double xmax, double ymin, double ymax);

protected:
    void showEvent(QShowEvent *event);

    void mousePressEvent(QMouseEvent *mouseEvent);
    void mouseMoveEvent(QMouseEvent *mouseEvent);
    void mouseReleaseEvent(QMouseEvent *mouseEvent);

    void constrainInPixmap(QPoint &point);
    void updatePixmap();
    void zoom();


private:
    Ui::MandelbrotView *ui;


    MandelbrotSet *m_mandelbrotSet;
    QPixmap m_pixmap;
    bool m_isBusy;
    bool m_isPressed;
    QPoint m_startZoom;
    QPoint m_endZoom;
};

#endif // MANDELBROTVIEW_H
