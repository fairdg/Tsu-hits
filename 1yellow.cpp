#include <iostream>
#include <functional>
using namespace std;

int* preobr(int* arr, function<bool(int)> func, int size, int& index2, int index = 0) {
    return (index >= size) ? new int[size] : ([&]() { 
        int* result = preobr(arr, func, size, index2, index + 1);
        if (func(arr[index])) {
            result[index2++] = arr[index];
        }
        return result; 
    })();
}


int* filter(int* arr, function<int(int)> func, int size, int& index1, int index = 0) {
    return (index >= size) ? new int[size] : ([&]() { 
        int* result = filter(arr, func, size, index1, index + 1);
        result[index1++] = func(arr[index]);
        return result; 
    })();
}

void print(int* arr, int size, int index = 0) {
    (index >= size) ? void() : (cout << arr[index] << " ", print(arr, size, index + 1));
}

int* fillArray(int size, int index = 0) {
    return (index >= size) ? new int[size] : ([&]() { int* arr = fillArray(size, index + 1); cin >> arr[index]; return arr; })();
}

int main() {
    int index1 = 0;
    int index2 = 0;
    int size;
    cout << "введи кол-во чисел ";
    cin >> size;
    
    int* mass = fillArray(size);
    int mass1[size];
    int mass2[size];
    
    preobr(mass, [](int x) { return x % 2 == 0; }, size, mass2, index2);
    filter(mass2, [](int x) { return x * x; }, index2, mass1, index1);

    cout << endl;
    print(mass, size);
    cout << endl;
    print(mass2, index2);
    cout << endl;
    print(mass1, index1);

    delete[] mass;
}