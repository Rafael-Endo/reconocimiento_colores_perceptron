#pragma once
#ifndef INPUTIMG_H
#define INPUTIMG_H

#include <vector>
#include <random>
#include <limits>
#include <iostream>
#include <string>
#include <tuple>
#include <opencv2/opencv.hpp>

using namespace std;

struct InputImg {

	vector<vector<int>> MatrizInput;
	string PATH = "";

	InputImg(string _PATH) {
		PATH = _PATH;
		cv::Mat imgPath = cv::imread(PATH);
		for (int fila = 0; fila < imgPath.rows; fila++) {
			for (int col = 0; col < imgPath.cols; col++) {
				cv::Vec3b pixel = imgPath.at<cv::Vec3b>(fila, col);
				MatrizInput.push_back({ pixel[2], pixel[1], pixel[0] });
			}
		}
	}

	vector<vector<int>> getMatrizInput() {
		return MatrizInput;
	}

};



#endif