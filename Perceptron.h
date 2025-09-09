
#ifndef PERCEPTRON_H
#define PERCEPTRON_H


#include <vector>
#include <random>
#include <limits>

using namespace std;


struct Color {
    double sesgo, R, G, B;
    int tipo;

    Color(double sesgo1, double R1, double G1, double B1, int tipo1) {
        sesgo = sesgo1;
        R = R1;
        G = G1;
        B = B1;
        tipo = tipo1;
    }
};

struct Perceptron {

    vector<Color> datos_colores;
    vector<double> W;
    double tasa_aprendizaje;
    int claseColorG;

    Perceptron(){}

    Perceptron(vector<vector<vector<vector<vector<int>>>>>& datos, double tasa_aprendizaje1, int _clase) {
        claseColorG = _clase;
        datos_colores = normalizar_colores(datos);
        tasa_aprendizaje = tasa_aprendizaje1;
        W.resize(4);
        mt19937 rng(42);
        normal_distribution<double> gen(0.0, 0.01);
        for (int i = 0; i < 4; i++) {
            double val = gen(rng);
            W[i] = val;
        }
    }


    vector<Color> normalizar_colores(const vector<vector<vector<vector<vector<int>>>>>& datos) {
        const double norm = 255 * 1.0;
        for (int clase = 0; clase < 4; clase++) {
            for (int img = 0; img < 6; img++) {
                vector<vector<vector<int>>> imgTemp = datos[clase][img];
                for (int fila = 0; fila < imgTemp.size(); fila++) {
                    for (int col = 0; col < imgTemp[fila].size(); col++) {
                        double R = imgTemp[fila][col][0] * 1.0, G = imgTemp[fila][col][1] * 1.0, B = imgTemp[fila][col][2] * 1.0, sesgo = 1.0;
                        int tipo = (imgTemp[fila][col][3] == claseColorG ? 1 : 0);
                        R /= norm; G /= norm; B /= norm;
                        Color pixel = { sesgo, R, G, B, tipo};
                        datos_colores.push_back(pixel);
                    }
                }
            }
        }
        //show();
        if (datos_colores.empty()) {
            cout << "No se encontraron datos -> perceptron -> nomralizar_colores.h" << endl;
            return {};
        }
        cout << "datos en vector<Color> -> perceptron.h" << endl;
        return datos_colores;
    }

    int producto_punto(const Color& c) const {
        double res = (c.sesgo * W[0]) + (c.R * W[1]) + (c.G * W[2]) + (c.B * W[3]);
        return (res >= 0.0 ? 1 : 0);
    }

    double predecir(int R, int G, int B) {
        double cast_R = (R * 1.0) / 255.0, cast_G = (G * 1.0) / 255.0, cast_B = (B * 1.0) / 255.0;
        return W[0] + W[1] * cast_R + W[2] * cast_G + W[3] * cast_B;
    }

    void entrenar(int k, double eps) {
        double error = numeric_limits<double>::max();
        int epoca = 0;
        while (error > eps && epoca < k) {
            bool corre = 0;
            for (int i = 0; i < datos_colores.size(); i++) {
                const Color color_actual = datos_colores[i];
                int predecir = producto_punto(color_actual);
                int clase_real = color_actual.tipo;
                int error_i = clase_real - predecir;
                if (error_i != 0) {
                    corre = 1;
                    W[0] += tasa_aprendizaje * error_i * color_actual.sesgo;
                    W[1] += tasa_aprendizaje * error_i * color_actual.R;
                    W[2] += tasa_aprendizaje * error_i * color_actual.G;
                    W[3] += tasa_aprendizaje * error_i * color_actual.B;
                }
            }
            double error_temp = 0.0;
            for (int i = 0; i < datos_colores.size(); i++) {
                Color colo_actual = datos_colores[i];
                int predecir = producto_punto(colo_actual);
                error_temp += ((colo_actual.tipo * 1.0) - (predecir * 1.0)) * ((colo_actual.tipo * 1.0) - (predecir * 1.0));
            }
            error_temp = error_temp * 0.5;
            error = error_temp;
            epoca++;
            if (corre == 0) {
                break;
            }
        }
    }
    
    void show() {
        cout << "size ->" << datos_colores.size() << endl;
        for (auto& _c : datos_colores) {
            cout << "clase -> " << _c.tipo << endl;
        }
    }

    void showW() {
        cout << W[0] << ' ' << W[1] << ' ' << W[2] << W[3] << endl;
    }


};

#endif

