#include "escena.h"
#include <QPainter>
#include <QDebug>
#include <QMessageBox>
#include <QKeyEvent>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <QMouseEvent>
#include <vector>
#include <QDir>
#include <QGridLayout>
#include <QPushButton>
#include <QStringList>

#include <QProcess>


Escena::Escena(QWidget * parent) : QGLWidget(parent), imagenNueva(false), realidadActiva(true)
{

//    this->setFixedSize(320, 240);
    this->resize(RESOLUTION_W * 1.4f, RESOLUTION_H * 1.4f);

    // Este timer esta porque no se puede llamar a actualizacion de escena desde el hilo.
    timer = new QTimer(this);

    //    timer->setInterval(10);  // Desactivamo este timer total no mostraremos nada en la maquina del robot

    connect(timer, SIGNAL(timeout()), this, SLOT(slot_actualizarEscena()));

    this->paraIntercambiar = 0;
}

void Escena::iniciarTemporizador()  {
    timer->start();

}

void Escena::detenerTemporizador()  {
    timer->stop();

}


Escena::~Escena()  {

}

/**
  * Este metodo es llamado desde el hilo. Debe tener total independencia con respecto al momento en que se
  * actualiza la escena, es por ello que tenemos un timer para actualizar.
  */
void Escena::setImage(cv::Mat image)  {

    if(!imagenNueva)  {

        this->iplImageCamara = image;

//        this->imageCamara = Util::getUtil()->IplImage2QImage(&image);
        this->imagenNueva = true;
    }
}

void Escena::initializeGL()  {
    // A esto lo pide la clase QGLFunctions
    this->initializeGLFunctions();

//    glClearColor(1, 1, 1, 0);
    glClearColor(0, 0, 0, 0);

    glShadeModel(GL_SMOOTH); // Type of shading for the polygons
    glEnable(GL_DEPTH_TEST);

    glGenTextures(1, idTextura);

}

void Escena::resizeGL(int w, int h){
    glViewport(0, 0, w, h);
}

void Escena::paintGL()  {
//    glutWireTorus(0.01,1.5,1,75);
    // We clear both the color and the depth buffer so to draw the next frame
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
//    glOrtho(0, 320, 0, 240, 1, 1000);
    glOrtho(0, RESOLUTION_W, 0, RESOLUTION_H, 1, 1000);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    glBindTexture(GL_TEXTURE_2D, idTextura[0]);

    glEnable(GL_TEXTURE_2D);
    glColor3f(1, 1, 1);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 1);     glVertex3f(0, 0, -999);
        glTexCoord2f(0, 0);     glVertex3f(0, RESOLUTION_H, -999);
        glTexCoord2f(1, 0);     glVertex3f(RESOLUTION_W, RESOLUTION_H, -999);
        glTexCoord2f(1, 1);     glVertex3f(RESOLUTION_W, 0, -999);
    glEnd();

    // Si la textura se deshabilitara al final de este metodo. El color que seteemos se visualiza mal.
    glDisable(GL_TEXTURE_2D);


    glFlush();
    //    glutSwapBuffers();  // No funciono, siempre se me cierra el programa


}

// Es el reemplazo de preparaImagenCamara() pero que no pasa la imagen por QImage, va directo
void Escena::createTextureFromCurrentImg()  {
    glBindTexture( GL_TEXTURE_2D, idTextura[0] );

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // El septimo parametro puede invertir formato RGB a BGR.
    // Se puede usar GL_BGR o GL_RGB
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iplImageCamara.size().width, iplImageCamara.size().height,0, GL_RGB, GL_UNSIGNED_BYTE, iplImageCamara.data);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iplImageCamara.size().width, iplImageCamara.size().height,0, GL_RGB, GL_UNSIGNED_BYTE, iplImageCamara.data);
}


void Escena::prepararImagenCamara(QRect recorte)  {

    // Entra a este if cuando se quiere recortar la imagen de la camara segun el Qrect. Como para hacer zoom
    if (!recorte.isNull())  {
        imageCamara = imageCamara.copy(recorte.x(), recorte.y(), recorte.width(), recorte.height());
    }

    imageCamara = QGLWidget::convertToGLFormat(imageCamara);
    texturaCamara = imageCamara.bits();

    glBindTexture(GL_TEXTURE_2D, idTextura[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, imageCamara.width(), imageCamara.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texturaCamara);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}


void Escena::slot_actualizarEscena()  {
    if (imagenNueva)  {

        if (realidadActiva)  {
//            this->prepararImagenCamara();
            this->createTextureFromCurrentImg();
        }

        imagenNueva = false;
        this->updateGL();
    }
}

