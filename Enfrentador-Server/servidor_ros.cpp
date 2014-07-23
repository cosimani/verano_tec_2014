#include "servidor_ros.h"
#include <QDebug>
#include <QImage>
#include "sensor_msgs/fill_image.h"

ServidorROS::ServidorROS() : servidorEscuchando(false)  {

    m_rgb.resize(640 * 480 * 3);
    kinect = QKinect::instance();
}

void ServidorROS::run()  {
    while(servidorEscuchando && ros::ok())  {
        ros::spinOnce();
    }
}

// Una vez enviada la solicitud, este publicador debe esperar una respuesta con la informacion
// Desde aqui iniciamos el hilo poniendo a escuchar a este publicador
void ServidorROS::spinQt()  {
    this->servidorEscuchando = true;
    this->start();
}

void ServidorROS::iniciarServicio(QString nombreServicio, Servicio tipoServicio)  {
    switch (tipoServicio)  {
    case IMAGE_NAVEGACION:
        service = node.advertiseService(nombreServicio.toStdString(), &ServidorROS::callback_image_navegacion, this);
        break;
    case STRING_MSG:
        service = node.advertiseService(nombreServicio.toStdString(), &ServidorROS::callback_string_msg, this);
        break;
    default:;
    }

    this->spinQt();
}

// callback donde el cliente envia dos parametros (string y entero). La idea es que el cliente pueda controlar
// ciertas caracteristicas del servidor (el servidor se encuentra en el turtlebot). Por ejemplo, el cliente podria
// enviar ("identificar personas", 1) o ("velocidad", 22). El servidor devolvera un entero como valor de error.
bool ServidorROS::callback_string_msg(tu::SendMsg::Request &req, tu::SendMsg::Response &res)  {

    QString cadenaRecibida = req.cadena.data.c_str();

    // Por si alguien le interesa a este servidor conocer el string e int que envio el cliente
    emit signal_SendMsg_recibido(cadenaRecibida, req.valor);

    return true;
}


// callback donde el cliente envia dos enteros (ancho y alto de la imagen) y este servidor le devuelve la
// imagen levantada desde el kinect que tiene el servidor conectado.
bool ServidorROS::callback_image_navegacion(tu::GetImage::Request &req, tu::GetImage::Response &res)  {

    kinect->getRGB(m_rgb);

    // Primero llevamos la imagen a QImage en su resolucion dada por el kinect
    // Luego de esto la redimensionaremos segun los parametros del clienteROS
    QImage imageRGB(m_rgb.data(), 640, 480, QImage::Format_RGB888);

    // Esta signal es por si el mismo servidor quiere publicar esta imagen en algun lado
    emit signal_GetImage_imagenEnviada(imageRGB);

    // La redimensionamos al tamano pedido
    QImage imageRedimensionada = imageRGB.scaled(req.ancho, req.alto);

    // Llenamos el mensaje de imagen con la imagen redimensionado
    sensor_msgs::fillImage(res.image,
                           "rgb8",
                           imageRedimensionada.height(),
                           imageRedimensionada.width(),
                           3*imageRedimensionada.width(),
                           imageRedimensionada.bits());

    // Por si alguien le interesa saber los dos enteros que el cliente envio
    emit signal_GetImage_recibido(req.ancho, req.alto);

    return true;
}

void ServidorROS::detenerServicio()  {
    service.shutdown();

    this->servidorEscuchando = false;
}

