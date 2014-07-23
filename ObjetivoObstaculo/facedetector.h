#ifndef FACEDETECTOR_H
#define FACEDETECTOR_H

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "cara.h"
#include <QDebug>

using namespace cv;

class FaceDetector
{
private:
    String face_cascade_name;
    CascadeClassifier face_cascade;

    CvHaarClassifierCascade * cascade;

public:
    FaceDetector();
    Cara detectBiggestFace(const Mat &frame);
    void drawFace(Cara face, Mat &frame, int width, int heigth);

    Cara detectaCara(IplImage *img );
};

#endif // FACEDETECTOR_H
