#ifndef PHOTOMOSAICGENERATOR_HPP
#define PHOTOMOSAICGENERATOR_HPP

#include <opencv2/opencv.hpp>
#include <set>

cv::Mat generate(std::string mainImagePath, std::set<std::string> tilesPaths, int WIDTH, int HEIGHT);


#endif