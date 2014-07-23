#ifndef HILOCAPTURA_H
#define HILOCAPTURA_H

#include <QThread>
#include <QImage>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <vector>

#include "configuracion.h"

#include <QKinect.h>

#include "controladorDesplazamiento.h"

using namespace std;
using namespace cv;

class Escena;

class HiloCaptura : public QThread  {
    Q_OBJECT

public:
    HiloCaptura(QObject * parent = 0);
    virtual ~HiloCaptura();
    void finalizarHilo();


private:
    Escena * escena;
    bool activo;

    QKinect * kinect;  // Conexion con la Webcam
    Mat frame;
    std::vector<uint8_t> m_rgb;
    std::vector<uint16_t> m_depth16;
    std::vector<uint16_t> m_depth11;
    std::vector<uint8_t> m_depthRaw;
    std::vector<uint8_t> depth;

    std::vector<uint8_t> m_depth32;

    ControladorDesplazamiento controlDesplazamiento;


protected:
    void run();

private slots:

public slots:
    void slot_cambiarCamara();

};

#endif // HILOCAPTURA_H
