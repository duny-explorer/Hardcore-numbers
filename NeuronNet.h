#ifndef DL_NEURONNET_H
#define DL_NEURONNET_H

#include <cmath>
#include <ctime>
#include <map>
#include <string>
#include <functional>
#include <vector>

using namespace std;

class NeuronNet {

public:
    NeuronNet(int inp); ~NeuronNet();

    struct Layer {
        int neurons;
        int neurons_in;
        double *output_res;
        double *after_output;
        double (NeuronNet::Layer::*activation_f)(double);
        double (NeuronNet::Layer::*activation_d)(double);
        double *edz;
        double *basis;
        double **matrix;

        double sigmoid(double x) { // лучше отдельным классом. но, как сделала, так сделала уже. На скрытые
            return 1/ (1 + pow(M_E, -x));
        };

        double derivative_sigmoid(double x) { // производная сигмоиды
            return sigmoid(x) * (1 - sigmoid(x));
        }


        double softmax(double x) { // на выходной слой
            double znamen = 0;

            for(int i = 0; i < neurons; i++) {
                znamen += pow(M_E, output_res[i]);
            }

            return pow(M_E, x) / znamen;
        }

        double derivative_softmax(double x) { // производная софтмакс
            return softmax(x) * (1 - softmax(x));
        }

        Layer(int n, int n_l, string f) {
            srand(time(0));
            map<string, int> func = {{"sigmoid", 0}, {"softmax", 1}};

            switch(func[f]){
                case 0:
                    activation_f = &Layer::sigmoid;
                    activation_d = &Layer::derivative_sigmoid;
                    break;
                case 1:
                    activation_f = &Layer::softmax;
                    activation_d = &Layer::derivative_softmax;
                    break;
            }

            neurons = n;
            neurons_in = n_l;
            matrix = new double*[n];
            basis = new double[n]();
            output_res = new double[n]();
            after_output = new double[n]();
            edz = new double[n]();

            for(int inp =0; inp < n; inp++) {
                matrix[inp] = new double[n_l]();
                basis[inp] = -1 + double(rand())/RAND_MAX * 2;


                for(int outp =0; outp < n_l; outp++) {
                    matrix[inp][outp] =  -1 + double(rand())/RAND_MAX * 2;
                }
            }
        }

        double derivative(double x) { // производная функции активации. из-за особенностей моей криворукой структуры пришлось сделать так
            return (this->*activation_d)(x);
        }

        void dot_x(double *x) { // метод прямого распространения. Перемножение матрицы на вектор
            for(int i = 0; i < neurons; i++) {
                double sum = 0;

                for(int j = 0; j < neurons_in; j++) {
                    sum += x[j] * matrix[i][j] ;
                }

                after_output[i] = sum + basis[i];
                output_res[i] = (this->*activation_f)(after_output[i]);
            }
        }

        void update(double **delta_w, double *delta_b){ // обновляем веса по найденным ошибкам весов и базисов
            for(int i = 0; i < neurons; i++) {
                basis[i] -= delta_b[i];

                for(int j = 0; j < neurons_in; j++) {
                    matrix[i][j] -= delta_w[i][j];
                }
            }
        }

        void clear() {
            output_res = new double[neurons]();
            after_output = new double[neurons]();
            edz = new double[neurons]();
        }
    };

    void train(string data, double l_r, double r_f, int epoch);
    double* result(double *x);
    double (*loss)(double*, double*, int);
    void load_model(string file);
    void save();
    double (*derivative_loss)(double*, double*, int);
    void add_layer(int count, string f);

private:
    int inputs;
    int outputs;
    int count_layers;
    vector<Layer> layers;
};


#endif //DL_NEURONNET_H
