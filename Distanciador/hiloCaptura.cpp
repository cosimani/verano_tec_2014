#include "hiloCaptura.h"
#include "util.h"
#include "escena.h"
#include <QDebug>

using namespace cv;

HiloCaptura::HiloCaptura(QObject * parent) : activo(false), camaraActiva(0)
{
    this->ventana = (Ventana *)parent;

    if (Configuracion::tipoCamara.contains("kinect", Qt::CaseInsensitive))  {
        m_rgb.resize(RESOLUTION_W * RESOLUTION_H * 3);
        kinect = QKinect::instance();
    }
    else if (Configuracion::tipoCamara.contains("webcam1", Qt::CaseInsensitive))  {
        cap = new VideoCapture(0);qDebug() << "webcam1";
        cap->set(CV_CAP_PROP_FRAME_WIDTH, RESOLUTION_W);
        cap->set(CV_CAP_PROP_FRAME_HEIGHT, RESOLUTION_H);
    }
    else if (Configuracion::tipoCamara.contains("webcam2", Qt::CaseInsensitive))  {
        cap = new VideoCapture(1);qDebug() << "webcam2";
        cap->set(CV_CAP_PROP_FRAME_WIDTH, RESOLUTION_W);
        cap->set(CV_CAP_PROP_FRAME_HEIGHT, RESOLUTION_H);
    }
    else if (Configuracion::tipoCamara.contains("webcam3", Qt::CaseInsensitive))  {
        cap = new VideoCapture(2);
        cap->set(CV_CAP_PROP_FRAME_WIDTH, RESOLUTION_W);
        cap->set(CV_CAP_PROP_FRAME_HEIGHT, RESOLUTION_H);
    }


    this->frame.create(Size(RESOLUTION_W, RESOLUTION_H), CV_8UC3);

}

HiloCaptura::~HiloCaptura()  {
    delete cap;
}


void HiloCaptura::slot_cambiarCamara()  {
    this->finalizarHilo();
    this->wait(500);  // Esperamos 500 milisegundos maximo para que finalice el hilo y podamos cambiar de camara

    if (camaraActiva == 2)
        camaraActiva = 0;
    else if (camaraActiva == 1)
        camaraActiva = 2;
    else if (camaraActiva == 0)
        camaraActiva = 1;

//    (this->camaraActiva == 0) ? this->camaraActiva = 2 : this->camaraActiva = 0;
    this->start();
}

void HiloCaptura::run()  {

    this->activo = true;

    if (Configuracion::tipoCamara.contains("kinect", Qt::CaseInsensitive))  {
        if (!kinect->kinectDetected)  {
            qDebug() << "La camara del Kinect no pudo ser iniciada!!";
            return;
        }
    }
    else  {  // Aqui para cualquier webcam
        if( !cap->isOpened() )  {
            qDebug() << "La camara con VideoCapture no pudo ser iniciada!!";
            return;
        }
    }

    while(activo)  {

        QThread::msleep(20);

        if (Configuracion::tipoCamara.contains("kinect", Qt::CaseInsensitive))  {
            kinect->getRGB(m_rgb);

            Mat frame(Size(RESOLUTION_W, RESOLUTION_H), CV_8UC3, (void*)m_rgb.data());
            this->frame = frame;
        }
        else  {  // Aqui para cualquier webcam
            cap->operator >>(frame);
        }

        ventana->escena->setImage(frame);
    }
}

void HiloCaptura::finalizarHilo()  {
    this->activo = false;
}

