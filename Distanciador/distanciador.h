#ifndef DISTANCIADOR_H
#define DISTANCIADOR_H

#include <QThread>
#include <vector>
#include <QKinect.h>


// Distanciador ejecuta un hilo que siempre se esta ejecutando y crea una matrix de
// FREENECT_FRAME_DEPTH_W * FREENECT_FRAME_DEPTH_H para almacenar en cada celda la distancia en milimetros hacia ese pixel.
class Distanciador : public QThread  {
    Q_OBJECT

public:
    Distanciador(QObject * parent = 0);
    virtual ~Distanciador();
    void detenerDistanciador();

    int getDistancia(int x, int y, int pixelesDeLado = 1);

private:
    bool activo;

    QKinect * kinect;  // Conexion con la Webcam
    std::vector<uint16_t> m_depth11;

    std::vector< std::vector<uint16_t> > m_distancias;


protected:
    void run();
};


#endif // DISTANCIADOR_H
