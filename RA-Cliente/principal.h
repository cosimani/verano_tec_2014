#ifndef PRINCIPAL_H
#define PRINCIPAL_H

#include <QWidget>
#include <QTimer>
#include <QAbstractButton>

#include <ros/ros.h>

#include "controladorDesplazamiento.h"
#include "tu/get_image.h"
#include "cliente_ros.h"

namespace Ui {
class Principal;
}

class Principal : public QWidget
{
    Q_OBJECT
    
public:
    explicit Principal(QWidget *parent = 0);
    ~Principal();
    
private:
    Ui::Principal *ui;

    ControladorDesplazamiento controlDesplazamiento;
    double lineal, angular;

    ClienteROS_image clienteROS_image;
    ClienteROS_string clienteROS_string;

    QImage image;
    cv::Mat imageMat;
    QTimer timerActualizarRobot;

    void pedirImagen(int ancho, int alto);

    int estadoKey;
    bool haceFaltaActualizarConCero;
    int contPbProyector;

protected:
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);
    void paintEvent(QPaintEvent *);

private slots:
    void timeout();

    void slot_abrirConfiguracion();
    void slot_actualizarRobot();
    void slot_enfrentar();
    void slot_proyectar();

};

#endif // PRINCIPAL_H

