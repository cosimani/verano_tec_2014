#include "hiloCaptura.h"
#include "util.h"
#include "escena.h"
#include <QDebug>


using namespace cv;
using namespace aruco;


HiloCaptura::HiloCaptura(QObject * parent) : activo(false), camaraActiva(0), detectorQRactivo(false),
    detectorRostroActivo(false), detectorPlanosActivo(false), markerDetectado(0), qrDetectado(0)
{
    this->escena = (Escena *)parent;

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


    camParam = new CameraParameters;
    mDetector = new MarkerDetector;
    markers = new vector<Marker>;

    this->leerBaseCargarPersonas();

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

    try  {
        // En este archivo hay algunos buenos parametros para tocar. Sobre todo nframes
        // Hace que se detecte de mejor manera el marcador. Por defecto estaba en 5.
        // En 15 mejora mucho. En 50 es casi lo mismo que con 50.
        camParam->readFromXMLFile(Configuracion::carpetaDeTrabajo.toStdString() + "/intrinsics.yml");
    }
    catch (cv::Exception e)  {
        qDebug() << "Exception:" << e.what();
        qDebug() << "Ojo que puede ser que no encuentre el archivo intrinsics.yml";
        qDebug() << "En mi caso lo tuve que ubicar en la carpeta build-desktop fuera de Debug y Release.";
    }

    float markerSize = 0.05f;  // Con esto se indica que el marcador es de 5 cm

    while(activo)  {

        QThread::msleep(20);

        if (Configuracion::tipoCamara.contains("kinect", Qt::CaseInsensitive))  {
            kinect->getRGB(m_rgb);

//            kinect->publicarCurrentDepthMode();

            // NOTA: La siguiente forma de trabajar la imagen no produce out_of_range pero va consumiendo
            // recursos de memoria hasta producir un Out Of Memory
//            QImage imageRGB(m_rgb.data(), RESOLUTION_W, RESOLUTION_H, QImage::Format_RGB888);
//            IplImage * image = Util::qImage2IplImage(imageRGB, false);
//            Mat frame(image);  // el true es para que copie los datos del plImage a la Mat
//            this->frame = frame;


            // NOTA: La siguiente forma de trabajar la imagen SI produce out_of_range pero NO consume
            // recursos de memoria crecientemente
            // Creamos la Mat pero aca tiene que estar en BGR, por eso convertimos con CV_RGB2BGR
            Mat frame(Size(RESOLUTION_W, RESOLUTION_H), CV_8UC3, (void*)m_rgb.data());
            this->frame = frame;
        }
        else  {  // Aqui para cualquier webcam
            cap->operator >>(frame);
        }


        if (detectorQRactivo)  {
            QVector<CodigoQR> vQrDetectado = detectorQR.detectarCodigos(frame, true);
            for (int i=0 ; i<vQrDetectado.size() ; i++)  {
                qDebug() << vQrDetectado.at(i).data << i;
            }
        }

        if ( markerDetectado-- <= 0)  {
            camParam->resize(frame.size());
            mDetector->detect(frame, *markers, *camParam, markerSize);

            escena->setMarker(markers);
            escena->setCameraParam(camParam);

            if ( markers->size() > 0 )
                markerDetectado = 4;
        }

        escena->setImage(frame);
    }
}

void HiloCaptura::finalizarHilo()  {
    this->activo = false;
}


// Este metodo lee la base de datos y carga el vector de Personas. Esto es para poder identificarlos si aparecen
// en la escena y obtener sus datos de perfil.
void HiloCaptura::leerBaseCargarPersonas()  {
    if ( ! adminDB.conectar("tubot"))  {
        qDebug() << "Conexion a la base fallida";
        return;
    }

    QString comando_sql = "SELECT nombre, apellido, mail, url_linkedin FROM personas";
    QVector<QStringList> vDatos = adminDB.ejecutarComandoSelect(comando_sql);



    for (int i=0 ; i<vDatos.size() ; i++)  {
        qDebug() << vDatos.at(i);
        Persona persona;
        persona.setNombre(vDatos.at(i).at(0) + " " + vDatos.at(i).at(1));
        persona.setMail(vDatos.at(i).at(2));
        persona.setLinkedin(vDatos.at(i).at(3));

        this->vPersonas.push_back(persona);
    }

    escena->setPersonas(vPersonas);
}
