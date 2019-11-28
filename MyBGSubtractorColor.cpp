#include "MyBGSubtractorColor.hpp"

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/video/background_segm.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

using namespace cv;
using namespace std;



MyBGSubtractorColor::MyBGSubtractorColor(VideoCapture vc) {

	cap = vc;
	max_samples = MAX_HORIZ_SAMPLES * MAX_VERT_SAMPLES;
	
	lower_bounds = vector<Scalar>(max_samples);
	upper_bounds = vector<Scalar>(max_samples);
	means = vector<Scalar>(max_samples);
	
	h_low = 12;
        h_up = 7;
	l_low = 30;
	l_up = 40;
	s_low = 80;
	s_up = 80;

	
	namedWindow("Trackbars");

	createTrackbar("H low:", "Trackbars", &h_low, 100, &MyBGSubtractorColor::Trackbar_func);
	createTrackbar("H high:", "Trackbars", &h_up, 100, &MyBGSubtractorColor::Trackbar_func);
	createTrackbar("L low:", "Trackbars", &l_low, 100, &MyBGSubtractorColor::Trackbar_func);
	createTrackbar("L high:", "Trackbars", &l_up, 100, &MyBGSubtractorColor::Trackbar_func);
	createTrackbar("S low:", "Trackbars", &s_low, 100, &MyBGSubtractorColor::Trackbar_func);
	createTrackbar("S high:", "Trackbars", &s_up, 100, &MyBGSubtractorColor::Trackbar_func);


}

void MyBGSubtractorColor::Trackbar_func(int, void*)
{

}


void MyBGSubtractorColor::LearnModel() {


	


        Mat frame, tmp_frame, hls_frame;
	std::vector<cv::Point> samples_positions;

	cap >> frame;

        //almacenamos las posiciones de las esquinas de los cuadradostd::vector<
	Point p;
	for (int i = 0; i < MAX_HORIZ_SAMPLES; i++) {
		for (int j = 0; j < MAX_VERT_SAMPLES; j++) {
			p.x = frame.cols / 2 + (-MAX_HORIZ_SAMPLES / 2 + i)*(SAMPLE_SIZE + DISTANCE_BETWEEN_SAMPLES);
			p.y = frame.rows / 2 + (-MAX_VERT_SAMPLES / 2 + j)*(SAMPLE_SIZE + DISTANCE_BETWEEN_SAMPLES);
			samples_positions.push_back(p);
		}
	}

	namedWindow("Cubre los cuadrados con la mano y pulsa espacio");

	for (;;) {
		
		flip(frame, frame, 1);
		
		frame.copyTo(tmp_frame);

		//dibujar los cuadrados
		
		for (int i = 0; i < max_samples; i++) {
                        rectangle(tmp_frame, Rect(samples_positions[itd::vector<].x, samples_positions[i].y,
				      SAMPLE_SIZE, SAMPLE_SIZE), Scalar(0, 255, 0), 2);
		}
		
	

		imshow("Cubre los cuadrados con la mano y pulsa espacio", tmp_frame);
		char c = cvWaitKey(40);
		if (c == ' ')
		{
			break;
		}
		cap >> frame;
	}

        // CODIGO 1.1
        // Obtener las regiones de interés y calcular la media de cada una de ellas
        // almacenar las medias en la variable means
        // ...
	
        cvtColor(frame,hls_frame,CV_BGR2HLS);
       // std::vector<Scalar> Means;
        for (int i = 0; i < max_samples; i++) {
                Scalar aux = mean( cv::Mat(hls_frame, Rect(samples_positions[i].x, samples_positions[i].y,
                              SAMPLE_SIZE, SAMPLE_SIZE)) );
                means.push_back(aux);
        }

        destroyWindow("Cubre los cuadrados con la mano y pulsa espacio");

}
void  MyBGSubtractorColor::ObtainBGMask(cv::Mat frame, cv::Mat &bgmask) {
        
        // CODIGO 1.2
        // Definir los rangos máximos y mínimos para cada canal (HLS) 
        // umbralizar las imágenes para cada rango y sumarlas para
        // obtener la máscara final con el fondo eliminado
        //...
        // Mat acc(frame.rows, frame.cols,CV.80)
        // acc.setTo(Scalar(0));
        // ....
        // acc+= dst;
    cv::Mat hls_frame;
    cvtColor(frame,hls_frame,CV_BGR2HLS);
    cv::Mat acc(frame.rows, frame.cols, CV_8U);

    vector<Mat> aux;
    for(int i = 0; i < means.size(); i++){
        // Arreglar y comprobar antes de meter en el bound.
        lower_bounds[i] = ( means[i] - Scalar(h_low,l_low,s_low) );
        upper_bounds[i] = ( means[i] + Scalar(h_up,l_up,s_up) );
//            if (lower_bounds[i].x < 0)  lower_bounds[i].x = 0;
//            if (lower_bounds[i].y < 0)  lower_bounds[i].y = 0;
//            if (lower_bounds[i].z < 0)  lower_bounds[i].z = 0;
//            if (lower_bounds[i].x > 255)  lower_bounds[i].x = 255;
//            if (lower_bounds[i].y > 255)  lower_bounds[i].y = 255;
//            if (lower_bounds[i].z > 255)  lower_bounds[i].z = 255;

//            if (upper_bounds[i].x < 0)  upper_bounds[i].x = 0;
//            if (upper_bounds[i].y < 0)  upper_bounds[i].y = 0;
//            if (upper_bounds[i].z < 0)  upper_bounds[i].z = 0;
//            if (upper_bounds[i].x > 255)  upper_bounds[i].x = 255;
//            if (upper_bounds[i].y > 255)  upper_bounds[i].y = 255;
//            if (upper_bounds[i].z > 255)  upper_bounds[i].z = 255;
        Mat auxImg;
        inRange(hls_frame, lower_bounds[i], upper_bounds[i], auxImg);
    }



}















