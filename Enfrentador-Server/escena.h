#ifndef ESCENA_H
#define ESCENA_H

#include <QGLWidget>
#include <QGLFunctions>
#include <QImage>
#include <QKeyEvent>
#include <QTimer>
#include <QPoint>
#include <QVector>
#include <QRect>
#include <opencv/cv.h>

#include "configuracion.h"

//#define RESOLUTION_W 1280
//#define RESOLUTION_H 1024

//#define RESOLUTION_W 640
//#define RESOLUTION_H 480

#define RESOLUTION_W 320
#define RESOLUTION_H 240



// Heredar tambien de QGLFunctions es para poder utilizar algunas funciones que estan disponibles en la extensiones
// de OpenGL. Algunos funciones como glGenBuffers. Para habilitarlas hay que llamar al
// metodo initializeGLFunctions() de QGLFunctions, y se recomienda hacerlo en initializeGL().
class Escena : public QGLWidget, protected QGLFunctions  {
    Q_OBJECT

public:
    Escena(QWidget * parent = 0);
    virtual ~Escena();
    void setImage(cv::Mat imageCamara);

    void iniciarTemporizador();
    void detenerTemporizador();


private:
    QImage imageCamara;        // QImage para camara

    void prepararImagenCamara(QRect recorte = QRect());


    cv::Mat iplImageCamara;     // IplImage para camara
    unsigned char* textureCamara;
    void createTextureFromCurrentImg();

    GLuint idTextura[50];  // Limitamos por ahora un maximo de 50 texturas

    GLuint texturaParaIplImage;

    unsigned char* texturaCamara;

    bool imagenNueva;

    QTimer * timer;

    int paraIntercambiar;

    bool realidadActiva;


protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void keyPressEvent(QKeyEvent *e)  {
        switch(e->key())  {
        case Qt::Key_Escape:
            if (!realidadActiva)  {
                this->detenerTemporizador();
                this->close();
            }
            break;

        default:
            break;
        }
    }


private slots:
    void slot_actualizarEscena();

};

#endif // ESCENA_H
