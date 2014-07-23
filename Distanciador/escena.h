#ifndef ESCENA_H
#define ESCENA_H

#include <QGLWidget>
#include <QGLFunctions>
#include <QImage>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QTimer>
#include <QPoint>
#include <QVector>
#include <QRect>
#include <opencv/cv.h>

#include "configuracion.h"

#include "QKinect.h"

//#define RESOLUTION_W 1280
//#define RESOLUTION_H 1024

#define RESOLUTION_W 640
#define RESOLUTION_H 480

class Ventana;


// Heredar tambien de QGLFunctions es para poder utilizar algunas funciones que estan disponibles en la extensiones
// de OpenGL. Algunos funciones como glGenBuffers. Para habilitarlas hay que llamar al
// metodo initializeGLFunctions() de QGLFunctions, y se recomienda hacerlo en initializeGL().
class Escena : public QGLWidget, protected QGLFunctions  {
    Q_OBJECT

public:
    Escena(QWidget * parent = 0);
    virtual ~Escena();
    void setImage(cv::Mat & imageCamara);
    void iniciarTemporizador();
    void detenerTemporizador();

private:
    Ventana * ventana;
    cv::Mat iplImageCamara;     // IplImage para camara
    unsigned char* textureCamara;
    void createTextureFromCurrentImg();

    GLuint idTextura[50];  // Limitamos por ahora un maximo de 50 texturas

    GLuint texturaParaIplImage;

    unsigned char* texturaCamara;
    bool imagenNueva;
    bool camaraActiva;
    QTimer * timer;
    int paraIntercambiar;

    QKinect * kinect;

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

private slots:
    void slot_actualizarEscena();

protected:
    void mousePressEvent(QMouseEvent *e);

};

#endif // ESCENA_H
