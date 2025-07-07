#include <opencv2/opencv.hpp>
#include <iostream>
#include <set>
#include <utility>

using namespace cv;
using namespace std;

const int WIDTH = 32;
const int HEIGHT = 18;

struct CompareByMeanGray{
	bool operator()(const pair<Mat, double>& a, const pair<Mat, double>& b) const {
		return a.second < b.second;
	};
};

Mat FindMosaic(set<pair<Mat, double>, CompareByMeanGray> &photosSet, double grayValue) {
	auto it = photosSet.lower_bound(make_pair(Mat(), grayValue));
	if (it == photosSet.end()) {
		--it; // Si no hay un valor mayor, tomar el menor
	}
	return it->first;
}

int main(){

	//Cargar las imagenes mosaicos, pasar a escala de grises, obtener su valor de gris y ordenarlas en un set
	pair<Mat,double> photos[10];

	for(int i = 0; i<10; i++){
		stringstream ss;
		ss << "photos/" << i+1 << ".jpeg";
		Mat photo = imread(ss.str(), IMREAD_COLOR);
		if(photo.empty()){
			cerr << "Error al cargar la imagen: " << ss.str() << endl;
			return -1;
		}
		Mat copy;
		cvtColor(photo, copy, COLOR_BGR2GRAY);
		double meanGray = mean(copy)[0];
		photos[i] = make_pair(photo, meanGray);
	}

	set<pair<Mat,double>, CompareByMeanGray> photosSet;
	for(int i = 0; i<10; i++){
		photosSet.insert(photos[i]);
	}


	





	Mat original = imread("photos/image.jpg", IMREAD_COLOR);
	imshow("Ventana", original);
	waitKey(0);

	Mat result = Mat::zeros(original.size(), original.type());
	Mat photoMosaic = Mat::zeros(original.size(), original.type());
	int numBlockRows = ceil((double)original.rows / HEIGHT);
	int numBlockCols = ceil((double)original.cols / WIDTH);
	double** grayValues = new double*[numBlockRows];
	for (int i = 0; i < numBlockRows; ++i) {
		grayValues[i] = new double[numBlockCols];
	}

	for(int i = 0; i<original.rows; i+=HEIGHT){
		for(int j = 0; j<original.cols; j+=WIDTH){
			int blockWidth = min(WIDTH, original.cols-j);
			int blockHeight = min(HEIGHT, original.rows-i);

			Rect roiRect(j, i, blockWidth, blockHeight);
			if(roiRect.x < 0 || roiRect.y < 0 || roiRect.x + roiRect.width > original.cols || roiRect.y + roiRect.height > original.rows) {
				cerr << "Error: ROI fuera de los límites de la imagen." << endl;
				return -1;
				//Depuracion
			}
			Mat roi = original(roiRect);
			Mat grayBlock;

			cvtColor(roi, grayBlock, COLOR_BGR2GRAY);

			Scalar meanGray = mean(grayBlock);
			double grayValue = meanGray[0];
			grayValues[i/HEIGHT][j/WIDTH] = grayValue;
			Mat mosaic = FindMosaic(photosSet, grayValue);
			resize(mosaic, mosaic, Size(blockWidth, blockHeight));
			mosaic.copyTo(photoMosaic(roiRect));

			Mat filledBlock(roi.size(), CV_8UC3, Scalar(grayValue, grayValue, grayValue));
			filledBlock.copyTo(result(roiRect));

			
			imshow("Result", result);

		}
	}
	waitKey(0);
	imshow("Photo Mosaic", photoMosaic);
	waitKey(0);
	for (int i = 0; i < numBlockRows; ++i) {
		delete[] grayValues[i];
	}
	delete[] grayValues;

}
