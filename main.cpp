#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/video/background_segm.hpp>

#include <stdio.h>
#include <string>
#include <iostream>

#include "MyBGSubtractorColor.hpp"
#include "HandGesture.hpp"

using namespace std;
using namespace cv;



int main(int argc, char** argv)
{

	Mat frame, bgmask, out_frame;
	


	//Abrimos la webcam

	VideoCapture cap;
	cap.open(0);
	if (!cap.isOpened())
	{
		printf("\nNo se puede abrir la cámara\n");
		return -1;
	}
        int cont = 0;
        while (frame.empty()&& cont < 2000 ) {

                cap >> frame;
                ++cont;
        }
        if (cont >= 2000) {
                printf("No se ha podido leer un frame válido\n");
                exit(-1);
        }

	// Creamos las ventanas que vamos a usar en la aplicación

	namedWindow("Reconocimiento");
	namedWindow("Fondo");

        // creamos el objeto para la substracción de fondo
        MyBGSubtractorColor Prueba(cap);

	// creamos el objeto para el reconocimiento de gestos

	// iniciamos el proceso de obtención del modelo del fondo
        Prueba.LearnModel();

	for (;;)
	{
		cap >> frame;
                flip(frame, frame, 1);
		if (frame.empty())
		{
			printf("Leído frame vacío\n");
			continue;
		}
		int c = cvWaitKey(40);
		if ((char)c == 'q') break;

		// obtenemos la máscara del fondo con el frame actual
                Prueba.ObtainBGMask(frame, bgmask);
                // CODIGO 2.1
                // limpiar la máscara del fondo de ruido
                //medianBlur(bgmask, bgmask, 5);
                int dilatation_size = 4;
                Mat element = getStructuringElement(MORPH_RECT,
                                                    Size(2*dilatation_size+1,2*dilatation_size+1),
                                                    Point(dilatation_size,dilatation_size));

                //erode(bgmask,bgmask,element);
                dilate(bgmask,bgmask,element);

		// deteccion de las características de la mano
                vector<vector<Point> > contours;
                findContours(bgmask,contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
                vector<vector<Point> > hull(contours.size());
                vector<vector<int> > hull_int(contours.size());
                vector<vector<Vec4i> > defects(contours.size());

                for(int i=0; i<contours.size(); i++)
                {
                   convexHull(Mat(contours[i]),hull[i],false);
                   convexHull(Mat(contours[i]),hull_int[i],false);
                    if(hull_int[i].size() > 3)
                    {
                        convexityDefects(contours[i],hull_int[i],defects[i]);
                    }
                }



                for(int i=0; i<contours.size(); i++)
                {
                    drawContours(frame, contours, i, cv::Scalar(255,0,0),2,8,vector<Vec4i>(),0,Point());
                    drawContours(frame, hull, i, cv::Scalar(0,0,255),2,8,vector<Vec4i>(),0,Point());
                    for(const Vec4i& v : defects[i])
                    {
                        int f_int = v[2];
                        Point f(contours[i][f_int]);
                        circle(frame, f, 4, Scalar(0,255,0), 2);
                    }
                }

                // mostramos el resultado de la sobstracción de fondo
                imshow("Fondo", bgmask);

                // mostramos el resultado del reconocimento de gestos           
                imshow("Reconocimiento", frame);
		
	}
	
	destroyWindow("Reconocimiento");
	destroyWindow("Fondo");
	cap.release();
	return 0;
}


getCenter(Mat bgmaskO )
{
	Mat &bgmaskR;
	Mat element = getStructuringElement(MORPH_RECT,
			Size(10*dilatation_size+1,10*dilatation_size+1),
			Point(dilatation_size,dilatation_size));

	erode(bgmask0, bgmaskR, element);

	medianBlur(bgmaskR, bgmaskR, 10);
	erode(bgmaskR, bgmaskR, element);
	
	vector<vector<Point> > contours;
    findContours(bgmaskR,contours, CV_RETR_EXTERNAL,
			CV_CHAIN_APPROX_SIMPLE);

	return contours[0][0];
}

getAngle(Point point1, Point point2){

	Point aux = point2-point1;
	return getAngle(aux.x,aux.y);
			
}




