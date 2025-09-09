
#include <iostream>
#include <utility>
#include <functional>
#include <algorithm>
#include "vector"
#include "Perceptron.h"
#include "MatrizPx.h"
#include "InputImg.h"
#include <map>
#include <opencv2/opencv.hpp>

using namespace std;

vector<vector<vector<vector<vector<int>>>>> datos(4);
const string INPUTIMG = "C:/proyectoPerceptron/InputImg/inputimg.jpg";

void uploadDatos() {
	for (int clase = 0; clase < 4; clase++) {
		MatrizPx imgClase(clase + 1);
		vector<vector<vector<vector<int>>>> tempMatrizPx = imgClase.getMatrizPx();
		if (tempMatrizPx.empty()) {
			cout << "Eror al cargar datos de clase -> " << clase + 1 << " main.ccp -> uploadDatos" << endl;
		}
		else {
			for (int i = 0; i < tempMatrizPx.size(); i++) {
				vector<vector<vector<int>>> tempImg = tempMatrizPx[i];
				datos[clase].push_back(tempImg);
			}

		}
	}
}

//int g = 0;
void mostrarDatos() {
	for (int clase = 0; clase < 4; clase++) {
		vector<vector<vector<vector<int>>>> temp = datos[clase];
		cout << "Imagenes clase -> " << clase + 1 << endl;
		for (int imagen = 0; imagen < temp.size(); imagen++) {
			vector<vector<vector<int>>> img = temp[imagen];
			cout << "[ ";
			for (int fila = 0; fila < img.size(); fila++) {
				for (int col = 0; col < img[fila].size(); col++) {
					cout << "(" << img[fila][col][0] << " , " << img[fila][col][1] << " , " << img[fila][col][2] << " , " << img[fila][col][3] << ") ";
					//g++;
				}
				cout << endl;
			}
			cout << " ]" << endl;
		}
	}

}

map<int, string> res;
Perceptron C1, C2, C3, EX;

void init(const string &colorC1, const string &colorC2, const string &colorC3) {
	uploadDatos();
	//mostrarDatos();
	const int K = 1000;
	const double err = 0.0;
	res[1] = colorC1; res[2] = colorC2; res[3] = colorC3;
	C1 = Perceptron(datos, (double)0.01, (int)1); C2 = Perceptron(datos, (double)0.01, (int)2);
	C3 = Perceptron(datos, (double)0.01, (int)3); EX = Perceptron(datos, (double)0.01, (int)4);
	C1.entrenar(K, err); C2.entrenar(K, err);
	C3.entrenar(K, err); EX.entrenar(K, err);
}

void showPre(const vector<pair<double, int>> cal) {
	for (auto& x : cal) {
		cout << "Color : " << res[x.second] << " | % = " << x.first << endl;
	}

}

string predecirInput(const vector<vector<int>> &input) {
	int pxVal = 0;
	double cnt1 = 0, cnt2 = 0, cnt3 = 0;
	for (int px = 0; px < input.size(); px++) {
		int R = input[px][0], G = input[px][1], B = input[px][2];
		if (EX.predecir(R, G, B) >= 0.0) {
			continue;
		}
		cnt1 += C1.predecir(R, G, B);
		cnt2 += C2.predecir(R, G, B);
		cnt3 += C3.predecir(R, G, B);
		pxVal++;
	}
	if (pxVal == 0) {
		return "ningun pixel valido";
	}
	vector<pair<double, int>> cal(3);
	cal[0] = { cnt1, 1 };
	cal[1] = { cnt2, 2 };
	cal[2] = { cnt3, 3 };
	sort(cal.begin(), cal.end(), greater<pair<double, int>>());
	return res[cal[0].second];
}

int main()
{
	init("verde", "rojo", "azul");
	InputImg IN(INPUTIMG);
	vector<vector<int>> input = IN.getMatrizInput();
	cout << predecirInput(input) << endl;
	return 0;
}

