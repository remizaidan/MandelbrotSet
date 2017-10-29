#ifndef MANDELBROTSET_H
#define MANDELBROTSET_H

#include <QObject>
#include <QPixmap>

class MandelbrotThemeManager;

/**
 * @brief The MandelbrotSet class.
 *
 * This class handles the maths behind the Mandelbrot Set.
 * The convergence/divergence information is converted into a
 * color value by the MandelbrotThemeManager class to fill an image.
 *
 * A convergence test is performed for the following series for
 * different points \f$c\f$ in the complex plane:
 * \f{
 * z_0 = 0
 * z_{n+1} = z_{n}^a + c
 * \f}
 * The parameter \f$a\f$ is called the order of the set.
 * Strictly speaking, this formula handles the Multibrot set which is
 * the generalization of the Mandelbrot set. The latter being the
 * particular case where \f$a = 2\f$.
 *
 * To construct an image, a different color is assigned for points that
 * converge and points that diverge. In the latter case, a continuous color
 * can be assigned based on how fast and how far the series diverge.
 * The details are documented in MandelbrotThemeManager.
 */
class MandelbrotSet : public QObject
{
    Q_OBJECT

public:

    /** Contructor */
    MandelbrotSet();

    /** Desctructor */
    ~MandelbrotSet();

    /**
     * @brief The MandelbrotPoint structure.
     *
     * This structure holds information about the convergence/divergence
     * of a point in the complex plane.
     */
    struct MandelbrotPoint {
        int n; //!< Number of iteration before divergence. -1 in case of convergence.
        double r; //!< Radius of the first point in the series larger than 2.
        double de; //!< Estimate of the distance to the boundary of the set.
    };

    /** Fill an image: the image dimensions are mapped to the given range. */
    bool fillPixmap(QImage &image, double xmin, double xmax, double ymin, double ymax);

    /** Set the precision: i.e. the number of iterations before calling a convergence. */
    void setPrecision(int precision);

    /** Set the order of the set. */
    void setOrder(double order);

    /** Get the lower boundary in the x coordinate system */
    double getXmin() const;

    /** Get the upper boundary in the x coordinate system */
    double getXmax() const;

    /** Get the lower boundary in the y coordinate system */
    double getYmin() const;

    /** Get the upper boundary in the y coordinate system */
    double getYmax() const;

    /** Get the order of the set. */
    double getOrder() const;

    /** Get the precision: i.e. the number of iterations before calling a convergence. */
    int getPrecision() const;

    /** Get the zoom factor. */
    double getZoomFactor() const;

    /** Get the internal buffer. */
    const std::vector<MandelbrotPoint> &getBuffer() const;

    /** Get a pointer to the theme manager. */
    MandelbrotThemeManager *getThemeManager();

public slots:

    /** Cancel ongoing calculation. */
    void cancel();

signals:

    /** Signal a change in the rogress for the ongoing calculation. */
    void progressChanged(int progress);

    /** Signal the end of the calculation */
    void pixmapReady();

protected:

    /** Performs a convergence test on a given point in the complex plane. */
    MandelbrotPoint doConvergenceTest(double x0, double y0);

    bool m_canceled; //!< Holds if the ongoing calculation should be canceled.
    int m_precision; //!< Precision: this is the number of iteration before calling a convergence.
    double m_order; //!< Order of the set.
    std::vector<MandelbrotPoint> m_buffer; //!< buffer containing convergence test data.
    bool m_isBufferUpToDate; //!< Holds whether a buffer need to be updated.
    MandelbrotThemeManager *m_themeManager; //!< The theme manager.
    int m_width; //!< Width in pixels of the last filled image.
    int m_height; //!< Height in pixels of the last filled image.
    double m_xmin; //!< Lower boundary along the x (real) axis.
    double m_xmax; //!< Upper boundary along the x (real) axis.
    double m_ymin; //!< Lower boundary along the y (imaginary) axis.
    double m_ymax; //!< Upper boundary along the y (imaginary) axis.
};

#endif // MANDELBROTSET_H
