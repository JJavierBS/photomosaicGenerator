#include <opencv2/opencv.hpp>
#include <iostream>
#include <set>
#include <utility>

using namespace cv;
using namespace std;

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

Mat generate(string mainImagePath, set<string> tilesPaths, int WIDTH, int HEIGHT) {

	//Cargar las imagenes mosaicos, pasar a escala de grises, obtener su valor de gris y ordenarlas en un set
	set<pair<Mat,double>, CompareByMeanGray> photosSet;

	for(string tilePath : tilesPaths){
		Mat photo = imread(tilePath, IMREAD_COLOR);
		if(photo.empty()){
			cerr << "Error al cargar la imagen: " << tilePath << endl;
			return Mat();
		}
		Mat copy;
		cvtColor(photo, copy, COLOR_BGR2GRAY);
		double meanGray = mean(copy)[0];
		photosSet.insert(make_pair(photo, meanGray));
	}

	Mat original = imread(mainImagePath, IMREAD_COLOR);
	imshow("Ventana", original);
	waitKey(0);

	Mat result = Mat::zeros(original.size(), original.type());
	Mat photoMosaic = Mat::zeros(original.size(), original.type());
	int numBlockRows = ceil((double)original.rows / HEIGHT);
	int numBlockCols = ceil((double)original.cols / WIDTH);

	for(int i = 0; i<original.rows; i+=HEIGHT){
		for(int j = 0; j<original.cols; j+=WIDTH){
			int blockWidth = min(WIDTH, original.cols-j);
			int blockHeight = min(HEIGHT, original.rows-i);

			Rect roiRect(j, i, blockWidth, blockHeight);
			if(roiRect.x < 0 || roiRect.y < 0 || roiRect.x + roiRect.width > original.cols || roiRect.y + roiRect.height > original.rows) {
				cerr << "Error: ROI fuera de los límites de la imagen." << endl;
				return Mat();
				//Depuracion
			}
			Mat roi = original(roiRect);
			Mat grayBlock;

			cvtColor(roi, grayBlock, COLOR_BGR2GRAY);

			Scalar meanGray = mean(grayBlock);
			double grayValue = meanGray[0];
			Mat mosaic = FindMosaic(photosSet, grayValue);
			resize(mosaic, mosaic, Size(blockWidth, blockHeight));
			mosaic.copyTo(photoMosaic(roiRect));

			Mat filledBlock(roi.size(), CV_8UC3, Scalar(grayValue, grayValue, grayValue));
			filledBlock.copyTo(result(roiRect));

			
			//imshow("Result", result);

		}
	}

	return photoMosaic;
}
