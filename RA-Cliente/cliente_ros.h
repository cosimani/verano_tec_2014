#ifndef SERVIDOR_ROS_H
#define SERVIDOR_ROS_H

#include "ros/ros.h"
#include "tu/get_image.h"
#include "tu/send_msg.h"

#include <QObject>
#include <QDebug>
#include <QImage>

#include <opencv/cv.h>
#include <opencv/highgui.h>


class ClienteROS  {

public:
    ClienteROS()  {  }
    ~ClienteROS()  {  this->desvincularServicio();  }

    void desvincularServicio()  {  cliente.shutdown();  }

protected:
    ros::NodeHandle node;
    ros::ServiceClient cliente;
};


// Este es un cliente que envia 2 enteros y obtiene como respuesta otro entero que es la suma
class ClienteROS_image : public ClienteROS  {

public:
    ClienteROS_image()  {  }
    ~ClienteROS_image()  {  this->desvincularServicio();  }

    void vincularServicio(QString servicio)  {
        cliente = node.serviceClient<tu::GetImage>(servicio.toStdString());
    }

    // Devolvemos un puntero porque parece que el constructor copia no
    void enviarMensaje(int ancho, int alto, cv::Mat & imagenRecibida)  {
        tu::GetImage srv;
        srv.request.ancho = ancho;
        srv.request.alto = alto;

        if (cliente.call(srv))  {
            cv::Mat image(cv::Size(srv.response.image.width, srv.response.image.height),
                          CV_8UC3,
                          (void*)srv.response.image.data.data());

            image.copyTo(imagenRecibida);
        }
    }
};

//QImage imageRGB(m_rgb.data(), RESOLUTION_W, RESOLUTION_H, QImage::Format_RGB888);
//IplImage * image = Util::qImage2IplImage(imageRGB, false);
//Mat frame(image);  // el true es para que copie los datos del plImage a la Mat



// Este es un cliente que envia  y obtiene como respuesta una imagen
class ClienteROS_string  : public ClienteROS  {

public:
    ClienteROS_string()  {  }
    ~ClienteROS_string()  {  this->desvincularServicio();  }

    void vincularServicio(QString servicio)  {
        cliente = node.serviceClient<tu::SendMsg>(servicio.toStdString());
    }

    int enviarMensaje(QString cadena, int valor)  {
        tu::SendMsg srv;
        srv.request.cadena.data = cadena.toStdString();
        srv.request.valor = valor;

        if (cliente.call(srv))  {
            return srv.response.valor;
        }
        return -1;
    }
};


#endif // SERVIDOR_ROS_H
