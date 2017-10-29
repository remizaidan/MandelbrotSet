#include "mandelbrotset.h"
#include "mandelbrotthememanager.h"

#include <complex>

#include <QDebug>

typedef std::complex<double> Complex;

MandelbrotSet::MandelbrotSet()
{
    m_order = 2;
    m_precision = 500;
    m_themeManager = new MandelbrotThemeManager();
    m_isBufferUpToDate = false;
    m_canceled = false;

    m_xmin = -2;
    m_xmax = 2;
    m_ymin = -2;
    m_ymax = 2;
}

MandelbrotSet::~MandelbrotSet()
{
    delete m_themeManager;
}

void MandelbrotSet::setPrecision(int precision)
{
    if(m_precision < 1) return;

    m_precision = precision;
    m_isBufferUpToDate = false;
}

void MandelbrotSet::setOrder(double order)
{
    if(m_order < 0) return;

    m_order = order;
    m_isBufferUpToDate = false;
}

bool MandelbrotSet::fillPixmap(QImage &image, double xmin, double xmax, double ymin, double ymax)
{
    m_canceled = false;

    int w = image.width();
    int h = image.height();

    bool isBufferUpToDate = m_isBufferUpToDate;
    if( (w != m_width || h != m_height ||
         xmin != m_xmin || xmax != m_xmax ||
         ymin != m_ymin || ymax != m_ymax) ){
        isBufferUpToDate = false;
    }

    std::vector<MandelbrotPoint> buffer = m_buffer;

    int progress = -1;
    if(!isBufferUpToDate) {

        buffer.resize(w*h);

        for(int i=0; i<w; i++){
            for(int j=0; j<h; j++){

                if(m_canceled) {
                    emit(progressChanged(0));
                    return false;
                }

                double x = xmin + i * (xmax - xmin) / w;
                double y = ymin + (h-j-1) * (ymax - ymin) / h;

                buffer[i*h+j] = doConvergenceTest(x, y);
            }

            int prog = 100*i/w;
            if(prog!=progress){
                progress = prog;
                emit(progressChanged(progress));
            }
        }
    }

    emit(progressChanged(100));

    m_themeManager->getCurrentTheme().setMandelbrotSet(this);

    progress = -1;
    for(int i=0; i<w; i++){
        for(int j=0; j<h; j++){

            if(m_canceled) {
                emit(progressChanged(0));
                return false;
            }

            MandelbrotPoint p = buffer[i*h+j];

            image.setPixelColor(i, j, m_themeManager->getColor(p));

            int prog = 100*i/w;
            if(prog!=progress){
                progress = prog;
                emit(progressChanged(progress));
            }
        }
    }

    m_width = w;
    m_height = h;
    m_xmin = xmin;
    m_xmax = xmax;
    m_ymin = ymin;
    m_ymax = ymax;
    m_buffer = buffer;
    m_isBufferUpToDate = true;

    emit(progressChanged(100));
    emit(pixmapReady());

    return true;
}

double MandelbrotSet::getXmin() const
{
    return m_xmin;
}

double MandelbrotSet::getXmax() const
{
    return m_xmax;
}

double MandelbrotSet::getYmin() const
{
    return m_ymin;
}

double MandelbrotSet::getYmax() const
{
    return m_ymax;
}

double MandelbrotSet::getOrder() const
{
    return m_order;
}

int MandelbrotSet::getPrecision() const
{
    return m_precision;
}

double MandelbrotSet::getZoomFactor() const
{
    double zoomFactor = 1;
    if(m_xmax - m_xmin > m_ymax - m_ymin) {
        zoomFactor = 4. / (m_xmax - m_xmin);
    }else{
        zoomFactor = 4. / (m_ymax - m_ymin);
    }
    return zoomFactor;
}

const std::vector<MandelbrotSet::MandelbrotPoint> &MandelbrotSet::getBuffer() const
{
    return m_buffer;
}

MandelbrotThemeManager *MandelbrotSet::getThemeManager()
{
    return m_themeManager;
}

void MandelbrotSet::cancel()
{
    m_canceled = true;
}

MandelbrotSet::MandelbrotPoint MandelbrotSet::doConvergenceTest(double x0, double y0)
{

    MandelbrotPoint p;
    p.de = 0;

    double rcut2 = 4;
    int niterDist = 1000;
    double rcutDist = 1000;
    bool hasExitedDist = false;

    Complex c(x0, y0);
    Complex z(0,0);
    Complex dz(1,0);
    Complex one(1,0);
    p.n = -1;
    bool hasExited = false;
    for(int i=0; i<m_precision; i++){
        dz = m_order * std::pow(z, m_order-1) * dz + one;
        z = std::pow(z, m_order) + c;
        double r2 = std::norm(z);
        if(!hasExited && r2 > rcut2){
            p.n = i;
            p.r = sqrt(r2);
            hasExited = true;
        }
        double dr = std::abs(dz);
        if(!hasExitedDist && (i > niterDist || dr > rcutDist)) {
            double r = sqrt(r2);
            p.de = r * log(r)/dr;
            if(std::isnan(p.de)) {
                qDebug() << p.de << r << log(r) << dr;
            }
            hasExitedDist = true;
        }
        if(hasExited && hasExitedDist){
            break;
        }
    }
    return p;
}

