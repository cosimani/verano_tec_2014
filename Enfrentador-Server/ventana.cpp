#include "ventana.h"
#include "ui_ventana.h"
#include <QMessageBox>
#include <QDebug>
#include <QKeyEvent>


Ventana::Ventana(QWidget *parent) : QWidget(parent), ui(new Ui::Ventana), valorServoVertical(0)  {
    ui->setupUi(this);

    escena = new Escena;
    hiloCaptura = new HiloCaptura(escena);
    ui->layoutEscena->addWidget(escena);
    escena->setFocus();  // Para que el Escape dentro de Escena sea capturada y poder cerrar la aplicacion

    connect(ui->pbCambiarCamara, SIGNAL(clicked()), hiloCaptura, SLOT(slot_cambiarCamara()));

    this->slot_encenderCamara();

    servidorROS_GetImage.iniciarServicio("/TUBot/navegacion_por_imagenes", ServidorROS::IMAGE_NAVEGACION);
    servidorROS_SendMsg.iniciarServicio("/TUBot/control_por_string", ServidorROS::STRING_MSG);
    connect(&servidorROS_SendMsg, SIGNAL(signal_SendMsg_recibido(QString,int)),
            this, SLOT(slot_SendMsg_recibido(QString,int)));


    kinect = QKinect::instance();
    kinect->setAngle(valorServoVertical);
}

Ventana::~Ventana()  {
    delete ui;
    delete hiloCaptura;
}


Escena* Ventana::getEscena()  {
    return escena;
}


void Ventana::slot_encenderCamara()  {
    escena->iniciarTemporizador();
    hiloCaptura->start();
    escena->setFocus();
}

void Ventana::slot_apagarCamara()  {
    escena->detenerTemporizador();
    hiloCaptura->finalizarHilo();
    escena->setFocus();
}


/**
  * Metodo que se ejecuta antes de cerrar la ventana (por ejemplo, cuando pulsamos X)
  */
void Ventana::closeEvent(QCloseEvent *)  {
    this->slot_apagarCamara();
    hiloCaptura->wait();  // Esperamos que el hilo finalice nates de cerrar la aplicacion.
    qDebug() << "closeEvent";
}

void Ventana::keyPressEvent(QKeyEvent *keyEvent)  {
    int key = keyEvent->key();

    switch(key)  {
    case Qt::Key_Escape:
        this->slot_apagarCamara();
        hiloCaptura->wait();  // Esperamos que el hilo finalice nates de cerrar la aplicacion.
        this->close();
        break;
    default: ;
    }
}

// Aqui es donde este servidor es controlado remotamente por el cliente. Los servicios son:
// - Llevar y traer desde el proyector: ("Proyector", 1) y ("Proyector", 0)
void Ventana::slot_SendMsg_recibido(QString cadenaRecibida, int valor)  {

    qDebug() << "Cadena Recibida = " << cadenaRecibida << valor;

    // La cadena Enfrentar enviada por el cliente es para que el robot se enfrente a una pared.
    // El valor es a la distancia que se tiene que enfrentar (permitido: 800 - 1800)
    if (cadenaRecibida.contains("Enfrentar", Qt::CaseInsensitive))  {
        if (valor <= 800)  {
            enfrentador.enfrentar(800);
        }
        else if (valor > 800 && valor < 1800)  {
            enfrentador.enfrentar(valor);
        }
        else if (valor >= 1800)  {
            enfrentador.enfrentar(1800);
        }
    }

    if (cadenaRecibida.contains("ServoKinect", Qt::CaseInsensitive))  {
        valorServoVertical += valor;
        if (valorServoVertical<-30)
            valorServoVertical = -30;
        if (valorServoVertical>30)
            valorServoVertical = 30;

        kinect->setAngle(valorServoVertical);
    }
}
