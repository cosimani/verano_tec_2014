#ifndef ENFRENTADOR_H
#define ENFRENTADOR_H

#include <QThread>
#include <vector>
#include <QKinect.h>
#include "controladorDesplazamiento.h"

class Enfrentador : public QThread  {
    Q_OBJECT

public:
    Enfrentador(QObject * parent = 0);
    virtual ~Enfrentador();
    void finalizarHilo();
    void enfrentar(int distanciaHaciaPared);

private:
    bool activo;

    QKinect * kinect;  // Conexion con la Webcam
    std::vector<uint16_t> m_depth11;

    ControladorDesplazamiento controlDesplazamiento;

    int distanciaHaciaPared;

protected:
    void run();
};

#endif // ENFRENTADOR_H
