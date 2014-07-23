#include "distanciador.h"
#include <QDebug>

Distanciador::Distanciador(QObject * ) : activo(true)  {
    kinect = QKinect::instance();

    m_depth11.resize(FREENECT_FRAME_DEPTH_W * FREENECT_FRAME_DEPTH_H, 0);

    m_distancias.resize(FREENECT_FRAME_DEPTH_W);
    for (unsigned int i=0 ; i<m_distancias.size() ; i++)  {
        m_distancias.at(i).resize(FREENECT_FRAME_DEPTH_H, 0);
    }

    this->start();
}

Distanciador::~Distanciador()  {

}

void Distanciador::run()  {

    if (!kinect->kinectDetected)  {
        qDebug() << "La camara del Kinect no pudo ser iniciada para Distanciador";
        return;
    }

    while(activo)  {
        QThread::msleep(200);

        kinect->getDepth16Bit(m_depth11);

        for (unsigned int row=0 ; row<FREENECT_FRAME_DEPTH_H ; row++)  {
            for (unsigned int col=0 ; col<FREENECT_FRAME_DEPTH_W ; col++)  {
                int distanciaMilimetros = 0;

                int valorEnBruto = m_depth11.at(col + (FREENECT_FRAME_DEPTH_W * row));

                if ( valorEnBruto < 2047 )
                    distanciaMilimetros = 1.0 / (valorEnBruto * -0.0030711016 + 3.3309495161) * 1000;

                m_distancias.at(col).at(row) = distanciaMilimetros;
            }
        }
    }
}

void Distanciador::detenerDistanciador()  {
    this->activo = false;
}

// Este metodo recibe un punto en la imagen y devuelve la distancia. El segundo parametro es la cantidad de pixels que
// tiene cada lado del cuadrado que se quiere calcular la distancia.
// Con este segundo parametro se hace un promedio de las distancias entre todos estos pixeles de distancia
int Distanciador::getDistancia(int x, int y, int pixelesDeLado)  {

//    qDebug() << "Pedido de distancia = " << x << y << " Distancia=" << m_distancias.at(x).at(y);

    if (pixelesDeLado == 1)
        return m_distancias.at(x).at(y);
    else if (pixelesDeLado <= 0)
        return 0;

    // Se utilizara la mediana para calcular el pixel central del cuadrado. Entonces el valor pixelesDeLado es importante
    // para saber si es impar o par para calcular la mediana.

    int contadorValoresDistintosDeCero = 0;
    int sumaDeValoresDistintosDeCero = 0;
    int valor = 0, valorAnterior = 0;
    int margen = 100;  // esto define un marguen de 100 milimetros. Diferencia mayor a esta se descarta porque podría
    // estar tomando un valor que modificaria demasiado el promedio y quizas sea una medida realmente que no se desea tomar

    int sumarEstoParaCalcularLaMediana = 0;

    if (pixelesDeLado % 2 == 0)  // par
        sumarEstoParaCalcularLaMediana = 1;
    else  // impar
        sumarEstoParaCalcularLaMediana = 0;


    for ( int i=(x-pixelesDeLado/2+sumarEstoParaCalcularLaMediana) ; i<(x+pixelesDeLado/2) ; i++ )  {  // desplazado en x
        for ( int j=(y-pixelesDeLado/2+sumarEstoParaCalcularLaMediana) ; j<(y+pixelesDeLado/2) ; j++ )  {  // y

            int col = i;
            int row = j;

            if (col < 0)  col = 0;
            if (row < 0)  row = 0;
            if (col >= FREENECT_FRAME_DEPTH_W) col = FREENECT_FRAME_DEPTH_W-1;
            if (row >= FREENECT_FRAME_DEPTH_H) row = FREENECT_FRAME_DEPTH_H-1;

            valor = m_distancias.at(col).at(row);

            if ( valor != 0 )  {
                if ( qAbs(valor-valorAnterior) < margen || sumaDeValoresDistintosDeCero == 0)  {
                    contadorValoresDistintosDeCero++;
                    sumaDeValoresDistintosDeCero += valor;
                }
            }

            valorAnterior = valor;
        }
    }

    if ( contadorValoresDistintosDeCero > 0 )
        return sumaDeValoresDistintosDeCero/contadorValoresDistintosDeCero;

    return 0;
}


