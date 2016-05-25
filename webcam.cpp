#include <iostream>
#include <opencv2/core/core.hpp>
#include <cv.h>
#include <highgui.h>
#include <stdio.h>

using namespace std;
using namespace cv;

void writeSequence(FILE *out, unsigned char * y_buf, signed char * u_buf, signed char * v_buf, int width, int height)
{

	FILE *pc;
	int Yfull = width*height;
	int UVfull = Yfull / 4;
	pc = fopen("pc.yuv", "wb");
	//while ((fread(y_buf, 1, Yfull, out) != NULL)) //系統開始	
	{
		fwrite(y_buf, sizeof(unsigned char)*Yfull, 1, pc);
		fwrite(y_buf, 1, Yfull, out);
		fwrite(u_buf, 1, UVfull, out);
		fwrite(v_buf, 1, UVfull, out);

	}
}


void onMouse(int Event, int x, int y, int flags, void* param)
{
	printf("( %d, %d) ", x, y);
	printf("The Event is : %d ", Event);
	printf("The flags is : %d ", flags);
	printf("The param is : %d\n", param);
}


void main()
{
	
	FILE *out;
	out = fopen("test.yuv", "wb");
	VideoCapture capture(0);
	//VideoWriter writer("VideoTest.avi", CV_FOURCC('M', 'J', 'P', 'G'), 25.0, Size(640, 480));
	Mat frame;
	int framecount = 0;

	//cvSetMouseCallback("webcam", onMouse, NULL);

	while (capture.isOpened())
	{
		capture >> frame;
		//writer << frame;
		namedWindow("webcam", CV_WINDOW_AUTOSIZE);
		imshow("webcam", frame);
		
		static unsigned char * y_buf = NULL;	//輸入Y
		static signed char * u_buf = NULL;	//輸入U
		static signed char * v_buf = NULL;	//輸入V
		int cols = frame.cols;//width
		int rows = frame.rows;//length
		int Yfull = frame.cols*frame.rows;
		int UVfull = Yfull / 4;
		y_buf = (unsigned char *)malloc(Yfull);
		u_buf = (signed char *)malloc(UVfull);
		v_buf = (signed char *)malloc(UVfull);
		for (int i = 0; i < cols; i++)
		{
			for (int j = 0; j < rows; j++)
			{
				y_buf[j*cols + i] = frame.at<Vec3b>(j, i)[2] * 0.299 + frame.at<Vec3b>(j, i)[1] * 0.587 + frame.at<Vec3b>(j, i)[0] * 0.114;
				u_buf[(j / 2)*(cols / 2) + (i / 2)] = frame.at<Vec3b>(j, i)[2] * -0.169 + frame.at<Vec3b>(j, i)[1] * -0.332 + frame.at<Vec3b>(j, i)[0] * 0.500 + 128.0;
				v_buf[(j / 2)*(cols / 2) + (i / 2)] = frame.at<Vec3b>(j, i)[2] * 0.500 + frame.at<Vec3b>(j, i)[1] * -0.419 + frame.at<Vec3b>(j, i)[0] * -0.0813 + 128.0;
			}
		}
		writeSequence(out ,y_buf, u_buf, v_buf, cols, rows);
		framecount++;
		cout << framecount << endl;
		if (waitKey(20) == 27)
		{
			break;
		}
	}
}