#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

Mat srcImage, src_gray;
int thresh = 200;
int max_thresh = 255;

const char* source_windows = "webcam";
const char* corners_window = "Corners image";

void cornerHarris_demo(int, void*);

/** @function main */
int main(int argc, char** argv)
{
	VideoCapture capture(0);
	//VideoWriter writer("VideoTest.avi", CV_FOURCC('M', 'J', 'P', 'G'), 25.0, Size(640, 480));
	int framecount = 0;


	while (capture.isOpened())
	{
		capture >> srcImage;
		cvtColor(srcImage, src_gray, COLOR_BGR2GRAY);
		namedWindow(source_windows, CV_WINDOW_AUTOSIZE);
		createTrackbar("Threshold: ", source_windows, &thresh, max_thresh, cornerHarris_demo);
		imshow(source_windows, srcImage);
		
		cornerHarris_demo(0, 0);
		
		if (waitKey(20) == 27)
		{
			break;
		}
	}

	//waitKey(0);
	return 0;
}

void cornerHarris_demo(int, void*)
{
	Mat dst, dst_norm, dst_norm_scaled;
	dst = Mat::zeros(srcImage.size(), CV_32FC1);

	int blocksize = 2;
	int apertureSize = 3;
	double k = 0.04;

	cornerHarris(src_gray, dst, blocksize, apertureSize, k, BORDER_DEFAULT);

	normalize(dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
	convertScaleAbs(dst_norm, dst_norm_scaled);

	for (int i = 0; i < dst_norm.cols; i++){
		for (int j = 0; j < dst_norm.rows; j++){
			if ((int)dst_norm.at<float>(j, i)>thresh){
				circle(dst_norm_scaled, Point(i, j), 5, Scalar(0), 2, 8, 0);
			}
		}
	}

	namedWindow(corners_window, WINDOW_AUTOSIZE);
	imshow(corners_window, dst_norm_scaled);
}

