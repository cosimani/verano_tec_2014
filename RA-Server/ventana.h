#ifndef VENTANA_H
#define VENTANA_H

#include <QWidget>
#include <QDesktopWidget>
#include "hiloCaptura.h"
#include "escena.h"

#include "QKinect.h"
#include "enfrentador.h"
#include "servidor_ros.h"

namespace Ui {
    class Ventana;
}

class Ventana : public QWidget
{
    Q_OBJECT

public:
    explicit Ventana(QWidget *parent = 0);
    ~Ventana();
    Escena* getEscena();


private:
    Ui::Ventana *ui;
    HiloCaptura* hiloCaptura;
    Escena * escena;
    QKinect * kinect;

    ServidorROS servidorROS_GetImage;
    ServidorROS servidorROS_SendMsg;

    int valorServoVertical;

    Enfrentador enfrentador;


protected:
    void closeEvent(QCloseEvent *);
    void keyPressEvent(QKeyEvent *);

private slots:
    void slot_encenderCamara();
    void slot_apagarCamara();
    void slot_llevarHaciaProyector();

    void slot_SendMsg_recibido(QString,int);

};

#endif // VENTANA_H
