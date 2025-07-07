#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(){
	
	Mat image = imread("photos/image.jpg", IMREAD_COLOR);

	imshow("Ventana", image);
	waitKey(0);


	Mat roi = image(Rect(50,50,200,500));
	
	imshow("Ventana", roi);
	waitKey(0);

}
