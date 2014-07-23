#include "principal.h"
#include "ui_principal.h"

#include <QKeyEvent>
#include <QDebug>
#include <QPainter>


Principal::Principal(QWidget *parent) : QWidget(parent),  ui(new Ui::Principal),
    lineal(0), angular(0), estadoKey(0), haceFaltaActualizarConCero(false)
{
    ui->setupUi(this);

    clienteROS_image.vincularServicio("/NavegarRobotServer/navegacion_por_imagenes");
    clienteROS_string.vincularServicio("/NavegarRobotServer/control_por_string");

    pedirImagen(ui->leAncho->text().toInt(), ui->leAlto->text().toInt());

    connect(ui->pbConfiguracion, SIGNAL(clicked()), this, SLOT(slot_abrirConfiguracion()));
    connect(ui->pbCerrar, SIGNAL(clicked()), this, SLOT(slot_abrirConfiguracion()));

    ui->wConfiguracion->hide();

    connect(&timerActualizarRobot, SIGNAL(timeout()), this, SLOT(slot_actualizarRobot()));
    timerActualizarRobot.start(100);

    connect(ui->cbNoImagen, SIGNAL(toggled(bool)), this, SLOT(timeout()));

    connect(ui->pbEnfrentar, SIGNAL(clicked()), this, SLOT(slot_enfrentar()));

}

Principal::~Principal()  {
    delete ui;
}

void Principal::keyReleaseEvent(QKeyEvent *e)  {

    // Esto es para que no detecte repetidamente el release event de las teclas
    if(e->isAutoRepeat())
        return;

    int a;
    switch(e->key())  {
    case Qt::Key_Up:
        a = 8;
        estadoKey = estadoKey & ~a;
        break;
    case Qt::Key_Down:
        a = 4;
        estadoKey = estadoKey & ~a;
        break;
    case Qt::Key_Left:
        a = 2;
        estadoKey = estadoKey & ~a;
        break;
    case Qt::Key_Right:
        a = 1;
        estadoKey = estadoKey & ~a;
        break;

    default:;
    }
}

void Principal::keyPressEvent(QKeyEvent *e)  {

    switch(e->key())  {
    case Qt::Key_Up:
        estadoKey = estadoKey | 8;
        break;
    case Qt::Key_Down:
        estadoKey = estadoKey | 4;
        break;
    case Qt::Key_Left:
        estadoKey = estadoKey | 2;
        break;
    case Qt::Key_Right:
        estadoKey = estadoKey | 1;
        break;

    default:;
    }
}

void Principal::slot_actualizarRobot()  {

    switch(estadoKey)  {
    case 0:
    case 3:
    case 12:
    case 15:
        angular = angular * 0.8;
        if (qAbs(angular) < 0.005)
            angular = 0;

        lineal = lineal * 0.9;  // Va desacelerando el 90 porciento del valor
        if (qAbs(lineal) < 0.001)
            lineal = 0;

        break;
    case 1:
    case 13:
        angular -= 0.1;
        break;
    case 2:
    case 14:
        angular += 0.1;
        break;
    case 4:
    case 7:
        lineal -= 0.01;
        break;
    case 8:
    case 11:
        lineal += 0.01;
        break;
    case 5:
        lineal -= 0.01;
        angular -= 0.1;
        break;
    case 6:
        lineal -= 0.01;
        angular += 0.1;
        break;
    case 9:
        lineal += 0.01;
        angular -= 0.1;
        break;
    case 10:
        lineal += 0.01;
        angular += 0.1;
        break;
    default:;
    }

    // Estos if son para que si ya se le mando un cero cero, entonces que no siga enviandole, total ya esta detenido
    if (angular != 0 || lineal != 0)
        haceFaltaActualizarConCero = true;

    if (haceFaltaActualizarConCero)  {
        controlDesplazamiento.publish(angular, lineal);
        qDebug() << "publish: lineal = " << lineal << " angular = " << angular << " estadoKey = " << estadoKey;
    }

    if (angular == 0 && lineal == 0)
        haceFaltaActualizarConCero = false;


}


void Principal::pedirImagen(int ancho, int alto)  {

    clienteROS_image.enviarMensaje(ancho, alto, imageMat);

    qDebug() << "Se recbio una imagen de " << imageMat.cols;

    QImage image(imageMat.data, imageMat.cols, imageMat.rows, QImage::Format_RGB888);
    if ( ! image.isNull())  {
        image.convertToFormat(QImage::Format_ARGB32_Premultiplied);

        this->image = image.scaled(this->width(), this->height() - ui->pbConfiguracion->height());

        this->repaint();
    }

    if (ui->cbNoImagen->isChecked())  {
        // mseg de retardo para que no pida tantas imagenes al ServidorROS
        QTimer::singleShot(ui->leInterarribo->text().toInt(), this, SLOT(timeout()));
    }
}

void Principal::paintEvent(QPaintEvent *)  {
    QPainter painter(this);
    painter.drawImage(0, 0, image);
}


void Principal::timeout()  {
    pedirImagen(ui->leAncho->text().toInt(), ui->leAlto->text().toInt());
}

void Principal::slot_abrirConfiguracion()  {
    if (ui->wConfiguracion->isVisible())  {
        ui->wConfiguracion->hide();
        ui->pbConfiguracion->show();
    }
    else  {
        ui->wConfiguracion->show();
        ui->pbConfiguracion->hide();
    }
}

void Principal::slot_enfrentar()  {
    clienteROS_string.enviarMensaje("Enfrentar", ui->leEnfrentar->text().toInt());
}

