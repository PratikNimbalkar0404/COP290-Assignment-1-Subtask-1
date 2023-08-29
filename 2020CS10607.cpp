#include <iostream>
#include <string.h>
#include <cmath>
#include <fstream>
#include <stdio.h>

using namespace std;


// Multiplies two matrices and saves them in the result_arr.
void matrix_multiply(int A, int B, int C, const float matrix_arr[],
                     const float weight_arr[], float result_arr[]) {

    float matrix[A][B], weight[B][C];
    for (int i = 0; i < A * B; i++)
        matrix[i % A][i / A] = matrix_arr[i];

    for (int i = 0; i < B * C; i++)
        weight[i % B][i / B] = weight_arr[i];

    float result[A][C];
    for (int i = 0; i < A; i++) {
        for (int j = 0; j < C; j++) {
            float sm = 0;
            for (int k = 0; k < B; k++) sm += (matrix[i][k] * weight[k][j]);
            result[i][j] = sm;
            result_arr[j * A + i] = sm;
        }
    }
}

// adds two matrices with same dimensions
void matrix_add(int A, int C, float result_arr[], const float bias_arr[]) {
    for (int i = 0; i < A * C; i++) {
        result_arr[i] += bias_arr[i];
    }
}

// performs fullyconnected function.
void task_1(string matrix_txt, string weight_txt,
            string bias_txt, string result_txt) {

    ifstream matrix_in(matrix_txt);
    ifstream weight_in(weight_txt);
    ifstream bias_in(bias_txt);
    ofstream result_out(result_txt);

    int A, B, C;
    matrix_in >> B >> A;
    float matrix_arr[A * B];
    for (int i = 0; i < A * B; i++)matrix_in >> matrix_arr[i];

    weight_in >> C >> B;
    float weight_arr[B * C];
    for (int i = 0; i < B * C; i++) weight_in >> weight_arr[i];

    bias_in >> C >> A;
    float bias_arr[C * A];
    for (int i = 0; i < A * C; i++) bias_in >> bias_arr[i];

    float result_arr[A * C];

    matrix_multiply(A, B, C, matrix_arr, weight_arr, result_arr);
    matrix_add(A, C, result_arr, bias_arr);
    result_out << C << '\n' << A << '\n';
    for (int i = 0; i < A * C; i++) {
        result_out << result_arr[i] << '\n';
    }
}

// takes a float as input and returns its tanh value.
float tanh_(float a) {
    float x = exp(a);
    float y = exp(-a);
    return (x - y) / (x + y);
}

// takes a float as input and returns its relu value.
float relu(float a) {
    if (a > 0) return a;
    return 0;
}

// performs tanh operation on a matrix.
void task_2a(string matrix_txt, string result_txt) {

    ifstream matrix_in(matrix_txt);
    ofstream result_out(result_txt);

    int A, B;
    matrix_in >> A >> B;
    result_out << A << '\n' << B << '\n';
    for (int i = 0; i < A * B; i++) {
        float a;
        matrix_in >> a;
        result_out << tanh_(a) << '\n';
    }
}

// performs relu operation on a matrix.
void task_2b(string matrix_txt, string result_txt) {

    ifstream matrix_in(matrix_txt);
    ofstream result_out(result_txt);

    int A, B;
    matrix_in >> A >> B;
    result_out << A << '\n' << B << '\n';
    for (int i = 0; i < A * B; i++) {
        float a;
        matrix_in >> a;
        result_out << relu(a) << '\n';
    }
}

void pooling(int A, const float matrix_arr[], float result_arr[], const string &type, int size) {
    int cnt = 0;

    for (int i = 0; i < A; i += size) {
        for (int j = 0; j < A; j += size) {

            float mx = matrix_arr[i * A + j];
            float sm = 0;

            for (int m = 0; m < size; m++) {
                for (int n = 0; n < size; n++) {
                    int a = i + m;
                    int b = j + n;
                    int idx = a * A + b;

                    mx = max(mx, matrix_arr[idx]);
                    sm += matrix_arr[idx];
                }
            }
            if (type == "max") result_arr[cnt++] = mx;
            else result_arr[cnt++] = (sm / ((float) (size * size)));
        }
    }
}

void task_3(string matrix_txt, string type, string size_str, string result_txt) {

    ifstream matrix_in(matrix_txt);
    ofstream result_out(result_txt);

    int A, B;
    matrix_in >> A >> B;
    float matrix_arr[A * B];
    for (int i = 0; i < A * B; i++) {
        matrix_in >> matrix_arr[i];
    }

    int size = stoi(size_str);

    float result_arr[(A * B) / (size * size)];

    pooling(A, matrix_arr, result_arr, type, size);

    result_out << A << '\n' << B << '\n';
    for (float a: result_arr) {
        result_out << a << '\n';
    }
}


float sigmoid(float a) {
    return ((float) 1) / (1 + exp(-a));
}

void task_4a(string vector_txt, string result_txt) {

    ifstream vector_in(vector_txt);
    ofstream result_out(result_txt);

    int N;
    vector_in >> N;
    result_out << N << '\n';
    for (int i = 0; i < N; i++) {
        float a;
        vector_in >> a;
        result_out << sigmoid(a) << '\n';
    }
}

void task_4b(string vector_txt, string result_txt) {

    ifstream vector_in(vector_txt);
    ofstream result_out(result_txt);

    int N;
    vector_in >> N;
    result_out << N << '\n';
    float values[N];
    float sm = 0;
    for (int i = 0; i < N; i++) {
        vector_in >> values[i];
        sm += exp(values[i]);
    }
    for (float a: values) {
        result_out << exp(a) / sm << '\n';
    }
}

int main(int N, char *argv[]) {

    if (strcmp(argv[0], "./yourcode.out") != 0) {
        ofstream err("error.txt");
        err << "The out file must be yourcode.";
        return 0;
    }
    if (strcmp(argv[1], "fullyconnected") == 0) {
        if (N != 6) {
            ofstream err("error.txt");
            err << "Invalid command.";
            return 0;
        }
        task_1(argv[2], argv[3], argv[4], argv[5]);
    }else if (strcmp(argv[1], "activation") == 0){
        if (N != 5){
            ofstream err("error.txt");
            err << "Invalid command.";
            return 0;
        }
        if (strcmp(argv[2], "tanh") == 0){
            task_2a(argv[3], argv[4]);
        }else if (strcmp(argv[2], "relu") == 0){
            task_2b(argv[3], argv[4]);
        }
        else{
            ofstream err("error.txt");
            err << "Invalid command.";
            return 0;
        }
    }
    else if (strcmp(argv[1], "pooling") == 0){
        if (N != 6){
            ofstream err("error.txt");
            err << "Invalid command.";
            return 0;
        }
        if (strcmp(argv[2], "max") == 0){
            task_3(argv[3], "max", argv[4], argv[5]);
        }else if (strcmp(argv[2], "average") == 0){
            task_3(argv[3], "average", argv[4], argv[5]);
        }else{
            ofstream err("error.txt");
            err << "Invalid command.";
            return 0;
        }
    }
    else if (strcmp(argv[1], "probability") == 0){
        if (N != 5){
            ofstream err("error.txt");
            err << "Invalid command.";
            return 0;
        }
        if (strcmp(argv[2], "sigmoid") == 0){
            task_4a(argv[3], argv[4]);
        }else if (strcmp(argv[2], "softmax") == 0){
            task_4b(argv[3], argv[4]);
        }else{
            ofstream err("error.txt");
            err << "Invalid command.";
            return 0;
        }
    }
    return 0;
}
