#include <iostream>
#include <functional>
using namespace std;

void preobr(int* arr, function<bool(int)> func, int size, int* result, int& index2, int index = 0 ) {
    if (index >= size) return;
    if (func(arr[index])) {
        result[index2] = arr[index];
        index2++;
    }
    preobr(arr, func, size, result, index2, index + 1);
}

void filter(int* arr, function<int(int)> func, int size, int* result, int& index1, int index = 0) {
    if (index >= size) return;
    result[index1] = func(arr[index]);
    index1++;
    filter(arr, func, size, result, index1, index + 1);
}

void print(int* arr, int size, int index = 0) {
    if (index >= size) return;
    cout << arr[index] << " ";
    print(arr, size, index + 1);
}

void fillArray(int arr[], int size, int index = 0) {
    if (index >= size) return;
    cin >> arr[index];
    fillArray(arr, size, index + 1);
}
int main() {
    int index1 = 0;
    int index2 = 0;
    int size;
    int mass[size];
    int mass1[size];
    int mass2[size];
    cout<<"введи кол-во чисел ";
    cin >>size;
    fillArray(mass, size);
    preobr(mass, [](int x) { return x% 2 == 0; }, size, mass2, index2);
    filter(mass2, [](int x) { return x * x; }, index2, mass1, index1);
    cout << endl;
    print(mass, size);
    cout << endl;
    print(mass2, index2);
    cout << endl;
    print(mass1, index1);
}