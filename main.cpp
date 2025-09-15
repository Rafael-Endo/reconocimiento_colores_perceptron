#include <iostream>
#include <utility>
#include <functional>
#include <algorithm>
#include "vector"
#include "Perceptron.h"
#include "MatrizPx.h"
#include "InputImg.h"
#include "JsonGF.h"
#include "HttpRequest.h"
#include <map>
#include <chrono>
#include <thread>
#include <opencv2/opencv.hpp>

using namespace std;

vector<vector<vector<vector<vector<int>>>>> datos(4);
const string INPUTIMG = "C:/proyectoPerceptron/InputImg/inputimg.jpg";
const string JSONPATH = "C:/proyectoPerceptron/jsongf/jsongf.json";

void selColor(string &col1, string& col2, string& col3) {
	col1, col2, col3;
	cout << "Color 1 (Clase 1) : ";
	cin >> col1;
	cout << endl;
	cout << "Color 2 (Clase 2) : ";
	cin >> col2;
	cout << endl;
	cout << "Color 3 (Clase 3) : ";
	cin >> col3;
	cout << endl;
}

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
map<string, int> resCl;
Perceptron C1, C2, C3, EX;

void init(const string &colorC1, const string &colorC2, const string &colorC3) {
	uploadDatos();
	//mostrarDatos();
	const int K = 1000;
	const double err = 0.0;
	res[1] = colorC1; res[2] = colorC2; res[3] = colorC3;
	res[4] = "null";
	resCl[colorC1] = 1; resCl[colorC2] = 2; resCl[colorC3] = 3;
	resCl[res[4]] = 4;
	C1 = Perceptron(datos, (double)0.01, (int)1); C2 = Perceptron(datos, (double)0.01, (int)2);
	C3 = Perceptron(datos, (double)0.01, (int)3); EX = Perceptron(datos, (double)0.01, (int)4);
	C1.entrenar(K, err); C2.entrenar(K, err);
	C3.entrenar(K, err); EX.entrenar(K, err);
	JsonGF wrgf(JSONPATH, C1.getWT(), C2.getWT(), C3.getWT(), C1.getET(), C2.getET(), C3.getET());
}

void showPre(const vector<pair<double, int>> cal) {
	for (auto& x : cal) {
		cout << "Color : " << res[x.second] << " | % = " << x.first << endl;
	}
}

string predecirInput(const vector<vector<int>>& input) {
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
		return "null";
	}
	vector<pair<double, int>> cal(3);
	cal[0] = { cnt1, 1 };
	cal[1] = { cnt2, 2 };
	cal[2] = { cnt3, 3 };
	sort(cal.begin(), cal.end(), greater<pair<double, int>>());
	if (cal[0].first < 3.0) {
		return "null";
	}
	cout << "cal -> " << cal[0].first << ' ';
	return res[cal[0].second];
}



const int ms = 250;
const string dirHttp = "http://10.250.243.227/capture/download";
const string dwnlImg = "C:/proyectoPerceptron/InputImg/inputimg.jpg";

int main()
{
	string col1, col2, col3;
	selColor(col1, col2, col3);
	init(col1, col2, col3);
	cout << "paso init" << endl;
	int _10 = 100;
	while (true) {
		bool rqst = HttpRequest::download(dirHttp, dwnlImg);
		if (!rqst) {
			cout << "Fallo en httprequest" << endl;
			break;
		}
		InputImg in(INPUTIMG);
		vector<vector<int>> input = in.getMatrizInput();
		string prdc = predecirInput(input);
		int idx = resCl[prdc];
		if (idx == 4) {
			cout << "Color invalido" << endl;
		}
		else {
			cout << " Color -> " << prdc << endl;
			JsonGF send(JSONPATH, idx);
		}
	}
}
