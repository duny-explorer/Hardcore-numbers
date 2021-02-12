#include <fstream>
#include <iostream>
#include <sstream>
#include "NeuronNet.h"

NeuronNet::NeuronNet(int inp) {
    inputs = inp;
    count_layers = 0;
}

NeuronNet::~NeuronNet() {
//потом как нибудь сделаю функцию очистки удаляемого объекта
}

void NeuronNet::save() {
//доделать загрузку парметров модели в файл
}

void NeuronNet::load_model(string file) {
//доделать выгрузку парметров модели из файла
}

void NeuronNet::add_layer(int count, string f) { // функция добавления слоя
    count_layers++;
    layers.push_back(Layer(count, layers.empty() ? inputs : layers.back().neurons, f));
}

double* NeuronNet::result(double *x) { // или метод прямого распространения
    layers[0].dot_x(x);
    for(int i = 1; i < count_layers; i++) {
        layers[i].dot_x(layers[i - 1].output_res);
    }

    return layers[count_layers - 1].output_res;
}

void NeuronNet::train(string data, double l_r, double r_f, int epoch) { // тренируем нашу модель
    // загрузка одной картинки их тестовых данных за цикл. учитываем пока все по одной картинке

    ifstream train(data); //"D:/programing/projects/C++/DL/mnist_train.csv"

    for(int i = 0; i < epoch; i++) {
        string line, y, x;
        getline(train, line);
        stringstream ss(line);

        getline(ss, y, ',');
        double Y[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        Y[stoi(y)] = 1;
        double X[28*28];

        for(int u = 0; u < 28*28; u++) {
            getline(ss, x, ',');
            X[u] = stoi(x);
        }

        // сводка по обучению, чтобы можно было видеть прогресс

        double *res;
        res = this->result(X);
        cout << "--------------------------------------------------------" << endl;
        cout << "epoch: " << i + 1 << endl;
        cout << "loss: " << this->loss(Y, res, layers[count_layers - 1].neurons) << endl;
        cout << "accurary: my strength ran out on the error back propagation method" << endl;

        // самое интересное. метод обратного распространения ошибки. Градиентный спуск. ММММММММ. сначала выходной слой.

        double nabla_biases[layers[count_layers - 1].neurons];
        double **nabla_weights;
        nabla_weights = new double*[layers[count_layers - 1].neurons];

        for(int j = 0; j < layers[count_layers - 1].neurons; j++) {
            nabla_biases[j] = 0;
            layers[count_layers - 1].edz[j] = derivative_loss(Y, layers[count_layers - 1].output_res, j)*
                    (layers.back().derivative(layers[count_layers - 1].after_output[j]));

            nabla_biases[j] = l_r * layers[count_layers - 1].edz[j];
            nabla_weights[j] = new double[layers[count_layers - 1].neurons_in];

            for(int k = 0; k < layers[count_layers - 1].neurons_in; k++) {
                nabla_weights[j][k] = 0;
                nabla_weights[j][k] = l_r * layers[count_layers - 1].edz[j] * layers[count_layers - 2].output_res[k]; // ожно потом добавить регулизацию
            }
        }

        layers[count_layers - 1].update(nabla_weights, nabla_biases);

        // теперь скрытые слои

        for(int j = count_layers - 2; j > -1; j--) {
            double nabla_biases[layers[j].neurons];
            double **nabla_weights;
            nabla_weights = new double*[layers[j].neurons];

            for(int k = 0; k < layers[j].neurons; k++) {
                layers[j].edz[k] = 0;
                nabla_weights[k] = new double[layers[j].neurons_in];

                for(int p = 0; p < layers[j + 1].neurons; p++) {
                    layers[j].edz[k] += layers[j + 1].edz[p] * layers[j + 1].matrix[p][k];
                }

                layers[j].edz[k] *= layers[j].derivative(layers[j].after_output[k]);
                nabla_biases[k] = l_r * layers[j].edz[k];


                for(int p = 0; p < layers[j].neurons_in; p++) {
                    nabla_weights[k][p] = layers[j].edz[k] * (j > 0 ? layers[j - 1].output_res[p]: X[p]); // пока без регуляции
                }
            }

            layers[j].update(nabla_weights, nabla_biases);
        }

        // иногда возникали ошибки фрагментации и тому подобное. На всякий пожарный
        for(int i = 0; i < count_layers; i++) {
            layers[i].clear();
        }
    }
}