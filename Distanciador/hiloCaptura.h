#ifndef HILOCAPTURA_H
#define HILOCAPTURA_H

#include <QThread>
#include <QImage>
#include <QStringList>

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/photo/photo.hpp>

#include <vector>

#include "ventana.h"

#include "configuracion.h"

#include <QKinect.h>

#include "distanciador.h"


using namespace std;
using namespace cv;

class Ventana;

class HiloCaptura : public QThread  {
    Q_OBJECT

public:
    HiloCaptura(QObject * parent = 0);
    virtual ~HiloCaptura();
    void finalizarHilo();

    Distanciador distanciador;


private:
    Ventana * ventana;
    bool activo;
    int camaraActiva;

    VideoCapture * cap;  // Conexion con la Webcam

    QKinect * kinect;
    std::vector<uint8_t> m_rgb;

    Mat frame;  // Con este objeto se trabajara para cada frame de la camara, tanto para kinect como webcam


protected:
    void run();

private slots:

public slots:
    void slot_cambiarCamara();

signals:
    void signal_distanciaActualizada(int);

};

#endif // HILOCAPTURA_H
