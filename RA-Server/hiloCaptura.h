#ifndef HILOCAPTURA_H
#define HILOCAPTURA_H

#include <QThread>
#include <QImage>
#include <QStringList>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <vector>

#include "configuracion.h"

#include <QKinect.h>

#include "aruco.h"
#include "cvdrawingutils.h"

#include "detectorQR.h"

#include "persona.h"

#include "adminDB.h"


using namespace std;
using namespace cv;
using namespace aruco;

class Escena;


class HiloCaptura : public QThread  {
    Q_OBJECT

public:
    HiloCaptura(QObject * parent = 0);
    virtual ~HiloCaptura();
    void finalizarHilo();

    void detectarQR(bool detectorQRactivo)  {  this->detectorQRactivo = detectorQRactivo;  }
    void detectarRostro(bool detectorRostroActivo)  {  this->detectorRostroActivo = detectorRostroActivo;  }
    void detectarPlanos(bool detectorPlanosActivo)  {  this->detectorPlanosActivo = detectorPlanosActivo;  }

private:
    Escena * escena;
    bool activo;
    int camaraActiva;

    VideoCapture * cap;  // Conexion con la Webcam

    CameraParameters *camParam;
    MarkerDetector *mDetector;
    vector<Marker> *markers;


    QKinect * kinect;
    std::vector<uint8_t> m_rgb;

    DetectorQR detectorQR;

    QVector<Persona> vPersonas;
    void leerBaseCargarPersonas();

    Mat frame;  // Con este objeto se trabajara para cada frame de la camara, tanto para kinect como webcam

    AdminDB adminDB;

    bool detectorQRactivo;
    bool detectorRostroActivo;
    bool detectorPlanosActivo;

    // Los sigueintes int almacenan una cantidad de frames de margen que una vez detectado (por ejemplo, un marker)
    // el mismo sera tenido en cuenta como identificado en el mismo lugar. Eta cantidad se decrementara con cada frame
    int markerDetectado;
    int qrDetectado;


protected:
    void run();

private slots:

public slots:
    void slot_cambiarCamara();

};

#endif // HILOCAPTURA_H
