#include "facedetector.h"
#include "configuracion.h"

FaceDetector::FaceDetector()  {
    face_cascade_name = Configuracion::carpetaDeTrabajo.toStdString() + "/haarcascade_frontalface_alt.xml";

    if( !face_cascade.load( face_cascade_name ) )  {
        printf("--(!)Error loading face cascade\n");
    }

    cascade = 0;
    cvReleaseHaarClassifierCascade(&cascade);
    QString qstringArchivo = Configuracion::carpetaDeTrabajo + "/haarcascade_frontalface_alt_osi.xml";
    const char* archivo = qstringArchivo.toStdString().c_str();
    cascade = (CvHaarClassifierCascade*)cvLoad(archivo, 0, 0, 0 );

}

Cara FaceDetector::detectBiggestFace(const Mat & frame)  {
    std::vector<Rect> faces;
    std::vector<int> areas;
    Mat frame_gray;
    faces.clear();
    areas.clear();

    cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray );

    //-- Detect faces
    face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(30, 30) );

    int indMax=0;

    for ( size_t i = 0; i < faces.size(); i++ )  {
        //find biggest face;
        int area;
        area = faces.at(i).width * faces.at(i).height;
        areas.push_back(area);
        //Mat faceROI = frame_gray( faces[i] );
    }
    for(size_t i = 0; (i < areas.size() && faces.empty() == false); i++)  {
        if(areas.at(i) > areas.at(indMax))
            indMax = i;
    }
    //qDebug() << "indMax = " << indMax;
    Cara face;
    Point center;
    int diametro = 0;

    if(faces.empty() == false)  {
        center.x = faces.at(indMax).x + faces.at(indMax).width/2;
        center.y = faces.at(indMax).y + faces.at(indMax).height/2;
        diametro =  faces.at(indMax).width;
        face.width = faces.at(indMax).width/2;
        face.heigth = faces.at(indMax).height/2;
    }
    face.center = center;
    face.diametro = diametro;
    return face;
}

void FaceDetector::drawFace(Cara face, Mat &frame, int width, int heigth)  {
    ellipse( frame, face.center, Size( width, heigth ), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );
}

// Function to detect and draw any faces that is present in an image
Cara FaceDetector::detectaCara(IplImage* img )  {

    Cara cara;

    int i;

    // Clear the memory storage which was used before
    static CvMemStorage* storage0;
    storage0 = cvCreateMemStorage(0);

    // Find whether the cascade is loaded, to find the faces. If yes, then:
    if( cascade )  {

        // There can be more than one face in an image. So create a growable sequence of faces.
        // Detect the objects and store them in the sequence
        CvSeq* faces = cvHaarDetectObjects( img, cascade, storage0, 1.2, 2, CV_HAAR_DO_CANNY_PRUNING, cvSize(50, 50) );
        // El ultimo parametro es el tamano minimo de cara detectada. Mientras mas grande es este valor, mejor sera
        // el desempeno en tiempo real, pero la persona debera estar mas cerca para ser detectada.

        // Si no se detectan caras, entonces devolvemos QPoint(0, 0)
        if (faces->total<=0)  {
            return cara;
        }

        qDebug() << "Caras detectadas = " << faces->total;

        int areaMax = 0;

        // Loop the number of faces found.
        for( i = 0; i < (faces ? faces->total : 0); i++ )  {
            // Create a new rectangle for drawing the face
            CvRect* rectanguloCara = (CvRect*)cvGetSeqElem( faces, i );

            int area = rectanguloCara->width * rectanguloCara->height;

            if (area > areaMax)  {
                areaMax = area;

                cara.width = rectanguloCara->width/2;
                cara.heigth = rectanguloCara->height/2;
                cara.diametro = cara.width;
                cara.center.x = rectanguloCara->x + cara.width;
                cara.center.y = rectanguloCara->y + cara.heigth;
            }
        }
    }

    // No hay que destruir los datos de la imagen aqui, ya que trabajamos con punteros y el objeto apuntado es
    // el mismo. El que libera la memoria es HiloCaptura::run()
//    cvReleaseImage(&img);

    cvClearMemStorage( storage0 );

    return cara;
}
