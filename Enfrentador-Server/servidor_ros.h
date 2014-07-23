#ifndef SERVIDOR_ROS_H
#define SERVIDOR_ROS_H

#include "ros/ros.h"
#include "tu/get_image.h"
#include "tu/send_msg.h"
#include "QKinect.h"
#include <vector>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <QImage>
#include <QThread>

class ServidorROS : public QThread  {
    Q_OBJECT

public:
    // IMAGE_NAVEGACION - Servicio que recibe 2 enteros para ancho y alto, y retorna una imaen de ese ancho y alto
    //                  - Pensado para navegacion remota y que las imagenes sean de la resolucion pedida
    enum Servicio  {  IMAGE_NAVEGACION, STRING_MSG  };
    ServidorROS();
    ~ServidorROS()  {
        servidorEscuchando = false;
        service.shutdown();
    }

    void iniciarServicio(QString nombreServicio, Servicio tipoServicio);
    void detenerServicio();

protected:
    void run();

private:
    ros::NodeHandle node;

    ros::ServiceServer service;

    QKinect * kinect;
    std::vector<uint8_t> m_rgb;
    cv::Mat imagenEnviada;
    QImage image;

    bool servidorEscuchando;

    bool callback_image_navegacion(tu::GetImage::Request  &req, tu::GetImage::Response &res);
    bool callback_string_msg(tu::SendMsg::Request &req, tu::SendMsg::Response &res);

public slots:
    void spinQt();

signals:
    void signal_GetImage_recibido(int, int);
    void signal_GetImage_imagenEnviada(QImage image);

    void signal_SendMsg_recibido(QString, int);
};


#endif // SERVIDOR_ROS_H
