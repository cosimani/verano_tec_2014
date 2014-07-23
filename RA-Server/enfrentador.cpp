#include "enfrentador.h"
#include <QDebug>

Enfrentador::Enfrentador(QObject * parent) : activo(false), distanciaHaciaPared(900)  {
    kinect = QKinect::instance();
    m_depth11.resize(320 * 240, 0);

    qDebug() << "Enfrentador instanciado - Por defecto no enfrenta";
}

Enfrentador::~Enfrentador()  {

}

void Enfrentador::run()  {

    if (!kinect->kinectDetected)  {
        qDebug() << "La camara del Kinect no pudo ser iniciada para obtener depth";
        return;
    }

    while(activo)  {
        QThread::msleep(200);

        kinect->getDepth16Bit(m_depth11);

        int sumaDistanciaMiliIzq = 0, promediarEntreCuantosIzq = 0;
        int sumaDistanciaMiliDer = 0, promediarEntreCuantosDer = 0;
        int sumaDistanciaCentro = 0, promediarDistanciaCentroEntre = 0;

        for (unsigned int i11=0 ; i11<m_depth11.size() ; i11+=2)  {

            int distanceMilimetros = 0;

            if (m_depth11.at(i11) < 2047)
                distanceMilimetros = 1.0 / (m_depth11.at(i11) * -0.0030711016 + 3.3309495161) * 1000;

            if (i11>=76200 && i11<=76218)  {  // El borde comienza en 76160
                if ( distanceMilimetros != 0)  {
                    promediarEntreCuantosDer++;
                    sumaDistanciaMiliDer += distanceMilimetros;
                }
            }
            else if (i11>=76772 && i11<=76790)  {
                if ( distanceMilimetros != 0)  {
                    promediarEntreCuantosIzq++;
                    sumaDistanciaMiliIzq += distanceMilimetros;
                }
            }
            else if (i11>=75825 && i11<=75865)  {  // Distancia al centro (el centro es 75845)
                if ( distanceMilimetros != 0)  {
                    promediarDistanciaCentroEntre++;
                    sumaDistanciaCentro += distanceMilimetros;
                }
            }
        }

        int promedioDerecha = 0;
        int promedioIzquierda = 0;
        int promedioCentro = 0;
        int margen = 20;
        int margenDistanciaPared = 50;
        float rotacion = 0.35;
        float velocidadLineal = 0.1;
        int distanciaHaciaPared = this->distanciaHaciaPared;

        if ( promediarEntreCuantosIzq != 0 && promediarEntreCuantosDer != 0 )  {
            promedioDerecha = sumaDistanciaMiliDer / promediarEntreCuantosDer;
            promedioIzquierda = sumaDistanciaMiliIzq / promediarEntreCuantosIzq;
        }

        if ( promediarDistanciaCentroEntre != 0 )  {
             promedioCentro = sumaDistanciaCentro / promediarDistanciaCentroEntre;
        }

        int distanciaParedMasomenos = (promedioDerecha+promedioIzquierda) / 2;

        if (promedioCentro > distanciaHaciaPared + margenDistanciaPared )  {
            controlDesplazamiento.publish(0, velocidadLineal);
        }
        else if (promedioCentro <= distanciaHaciaPared - margenDistanciaPared )  {
            controlDesplazamiento.publish(0, -velocidadLineal);
        }
        else  {  // Entra este else cuando ya se corrigio la distancia hacia la pared.

            if ( distanciaParedMasomenos < 700)  {
                margen = 20;
                rotacion = 0.4;
            }
            else if ( distanciaParedMasomenos > 700 && distanciaParedMasomenos < 1500)  {
                margen = 40;
                rotacion = 0.35;
            }
            else if ( distanciaParedMasomenos > 1500 )  {
                margen = 50;
                rotacion = 0.35;
            }

            if (promedioDerecha == 0 || promedioIzquierda == 0)  {
                qDebug() << "uno de los dos es cero - NO corregimos posicion";
            }
            else if ( qAbs(promedioIzquierda - promedioDerecha) > margen )  {
                if ( (promedioIzquierda - promedioDerecha) > 0 )  {
                    controlDesplazamiento.publish(rotacion, 0);
                    qDebug() << "corregimos hacia derecha";
                }
                else  {
                    controlDesplazamiento.publish(-rotacion, 0);
                    qDebug() << "corregimos hacia izquierda";
                }                
            }
            else  {  // Entra a este else cuando ya esta el enfrentamiento corregido. Desactivamos el hilo
                this->activo = false;  // Con este valor, el proximo ciclo del while sera el ultimo
            }
        }
    }
}

void Enfrentador::finalizarHilo()  {
    this->activo = false;
}

void Enfrentador::enfrentar(int distanciaPared)  {
    this->distanciaHaciaPared = distanciaPared;
    this->activo = true;
    this->start();
}

