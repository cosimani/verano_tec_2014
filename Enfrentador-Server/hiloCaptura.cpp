#include "hiloCaptura.h"
#include "escena.h"
#include <QDebug>
#include <bitset>

using namespace cv;

HiloCaptura::HiloCaptura(QObject * parent) : activo(false)  {
    this->escena = (Escena *)parent;

    m_rgb.resize(RESOLUTION_W * RESOLUTION_H * 3, 0);
    kinect = QKinect::instance();

    m_depth16.resize(RESOLUTION_W * RESOLUTION_H, 0);
    m_depth11.resize(RESOLUTION_W * RESOLUTION_H, 0);

    m_depthRaw.resize(RESOLUTION_W * RESOLUTION_H, 0);
    depth.resize(RESOLUTION_W * RESOLUTION_H, 0);

    m_depth32.resize(RESOLUTION_W * RESOLUTION_H * 3, 0);
}

HiloCaptura::~HiloCaptura()  {

}


void HiloCaptura::slot_cambiarCamara()  {
    this->finalizarHilo();
    this->wait(500);  // Esperamos 500 milisegundos maximo para que finalice el hilo y podamos cambiar de camara

    this->start();
}

void HiloCaptura::run()  {

    this->activo = true;

    if (!kinect->kinectDetected)  {
        qDebug() << "La camara del Kinect no pudo ser iniciada!!";
        return;
    }

    // Hilo que no necesitamos. Se podria eliminar.
    while(activo)  {

        QThread::msleep(20);

    }
}

void HiloCaptura::finalizarHilo()  {
    this->activo = false;
}

