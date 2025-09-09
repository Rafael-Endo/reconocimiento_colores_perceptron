
#include <iostream>
#include "vector"
#include "Perceptron.h"
#include "MatrizPx.h"
#include <opencv2/opencv.hpp>
using namespace std;

vector<vector<vector<vector<vector<int>>>>> datos(4);

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

int g = 0;

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
					g++;
				}
				cout << endl;
			}
			cout << " ]" << endl;
		}
	}

}

int main()
{
	uploadDatos();
	Perceptron color_negro (datos, 0.001, 4);
	color_negro.entrenar(100000, 0);
	int R, G, B; cin >> R >> G >> B;
	bool res = color_negro.predecir(R, G, B);
	if (res) cout << "Negro" << endl;
	else cout << "No negro" << endl;
	return 0;
}