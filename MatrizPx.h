#pragma once
#ifndef MATRIZPX_H
#define MATRIZPX_H

#include <vector>
#include <random>
#include <limits>
#include <iostream>
#include <string>
#include <tuple>
#include <opencv2/opencv.hpp>

using namespace std;



struct MatrizPx {

	int l;
	int clase;
	vector<string> pathImg;
	const string PATH = "C:/proyectoPerceptron/main/main/img/";
	const string TYPE = ".jpg";

	MatrizPx(int _clase) {
		clase = _clase;
		if (clase == 1) l = 1;
		else if (clase == 2) l = 7;
		else if (clase == 3) l = 13;
		else l = 19;
		getPath();
	}

	void getPath() {
		int cnt = l;
		for (int i = l; i < l + 6; i++) {
			string temp_path = PATH + to_string(i) + TYPE;
			pathImg.push_back(temp_path);
			cout << temp_path << endl;
		}
	}

	void mostrarMatrizPx(const vector<vector<vector<vector<int>>>> matrizPxls) {
		for (int idx = 0; idx < 6; idx++) {
			if (matrizPxls[idx].empty()) {
				cout << "Imagen vacia numero -> " << idx + 1 << endl;
				continue;
			}
			cout << "Imagen numero -> " << idx + 1 << endl;
			vector<vector<vector<int>>> tt = matrizPxls[idx];
			cout << "[ ";
			for (int i = 0; i < tt.size(); i++) {
				for (int j = 0; j < tt[i].size(); j++) {
					cout << " ( " << tt[i][j][0] << " , " << tt[i][j][1] << " , " << tt[i][j][2] << " , " << tt[i][j][3] << " ) " << ' ';
				}
				cout << endl;
			}
			cout << " ] " << endl;

		}
		cout << "clase -> " << clase << " cargada." << endl;
	}

	vector<vector<vector<vector<int>>>> getMatrizPx() {
		vector<vector<vector<vector<int>>>> matrizPxls(6);
		for (int idx = 0; idx < 6; idx++) {
			cv::Mat img = cv::imread(pathImg[idx]);
			if (img.empty()) {
				cout << "Eror al leer la imagen -> " << pathImg[idx] << endl;
			}
			else {
				for (int fila = 0; fila < img.rows; fila++) {
					vector<vector<int>> filaPx(img.cols, vector<int>(4));
					for (int columna = 0; columna < img.cols; columna++) {
						cv::Vec3b pixelTemp = img.at<cv::Vec3b>(fila, columna);
						int R = pixelTemp[0], G = pixelTemp[1], B = pixelTemp[2];
						filaPx[columna][0] = B;filaPx[columna][1] = G;
						filaPx[columna][2] = R;filaPx[columna][3] = clase;
					}
					matrizPxls[idx].push_back(filaPx);
				}
			}
		}
		if (matrizPxls.empty()) {
			cout << "Error al procesar imagenes -> MatrizPx.h -> getMatrizPx()" << endl;
			return { {{{}}} };
		}
		else {
			cout << matrizPxls.size() << " imagenes procesadas -> MatrizPxl.j -> getMatrizPx()" << endl;
			//mostrarMatrizPx(matrizPxls);
			return matrizPxls;
		}
		
	}


};





#endif
