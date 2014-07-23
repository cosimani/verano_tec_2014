#ifndef HILOCAPTURA_H
#define HILOCAPTURA_H

#include <QThread>
#include <QImage>
#include <QStringList>

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/photo/photo.hpp>

#include <vector>

#include "configuracion.h"

#include <QKinect.h>

#include "aruco.h"
#include "cvdrawingutils.h"

#include "facedetector.h"
#include "persona.h"

#include "adminDB.h"

#include "distanciador.h"


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

    FaceDetector faceDetector;
    QVector<Persona> vPersonas;
    void leerBaseCargarPersonas();

    Mat frame;  // Con este objeto se trabajara para cada frame de la camara, tanto para kinect como webcam

    AdminDB adminDB;

    bool detectorQRactivo;
    bool detectorRostroActivo;
    bool detectorPlanosActivo;
    bool detectorMarcadoresActivo;

    // Los sigueintes int almacenan una cantidad de frames de margen que una vez detectado (por ejemplo, un marker)
    // el mismo sera tenido en cuenta como identificado en el mismo lugar. Eta cantidad se decrementara con cada frame
    int markerDetectado;
    int qrDetectado;

    int valorMatizMinRojo, valorMatizMaxRojo, valorMatizMinVerde, valorMatizMaxVerde;
    int valorSaturacionRojo, valorBrilloRojo, valorSaturacionVerde, valorBrilloVerde;
    bool rojoSiVerdeNo;

    Distanciador distanciador;

protected:
    void run();

private slots:

public slots:
    void slot_cambiarCamara();

    void slot_sMatizMinRojo(int valor)  {  this->valorMatizMinRojo = valor;  }
    void slot_sMatizMaxRojo(int valor)  {  this->valorMatizMaxRojo = valor;  }
    void slot_sMatizMinVerde(int valor)  {  this->valorMatizMinVerde = valor;  }
    void slot_sMatizMaxVerde(int valor)  {  this->valorMatizMaxVerde = valor;  }
    void slot_sSaturacionRojo(int valor)  {  this->valorSaturacionRojo = valor;  }
    void slot_sBrilloRojo(int valor)  {  this->valorBrilloRojo = valor;  }
    void slot_sSaturacionVerde(int valor)  {  this->valorSaturacionVerde = valor;  }
    void slot_sBrilloVerde(int valor)  {  this->valorBrilloVerde = valor;  }
    void slot_cbRojoVerde(bool valor)  {  this->rojoSiVerdeNo = valor;  }

signals:
    void signal_distanciaActualizada(int);

};

#endif // HILOCAPTURA_H
