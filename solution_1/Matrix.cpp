#include "Matrix.h"
#include <iostream>

using namespace std;

Matrix::~Matrix() {
    delete[] arr;
}

void Matrix::init(int _rows, int _cols) {
    rows = _rows;
    cols = _cols;
    arr = new int[rows * cols]();
}

int Matrix::get(int x, int y) {
    return arr[cols * x + y];
}

void Matrix::set(int x, int y, int value) {
    arr[cols * x + y] = value;
}

void Matrix::add(int x, int y, int value) {
    arr[cols * x + y] += value;
}

void Matrix::print() {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int val = get(i, j);
            cout << val << " | ";
        }
        cout << endl;
    }
}

