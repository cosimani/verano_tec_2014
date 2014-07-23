#ifndef VENTANA_H
#define VENTANA_H

#include <QWidget>
#include "hiloCaptura.h"
#include "escena.h"
#include "servidor_ros.h"

#include "QKinect.h"
#include "enfrentador.h"


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

    ServidorROS servidorROS_GetImage;
    ServidorROS servidorROS_SendMsg;

    QKinect * kinect;
    Enfrentador enfrentador;

    int valorServoVertical;

protected:
    void closeEvent(QCloseEvent *);
    void keyPressEvent(QKeyEvent *);

private slots:
    void slot_encenderCamara();
    void slot_apagarCamara();

    void slot_SendMsg_recibido(QString,int);

};

#endif // VENTANA_H
