#include <iostream>
#include <ctime>
#include <random>

template<typename T>
void InsertionSort(int size, T arr[]) {
    for(int i = 1; i < size; ++i) {
        for(int k = i; k != 0 && arr[k-1] > arr[k]; --k) {
            arr[k-1] += arr[k];
            arr[k] = arr[k-1] - arr[k];
            arr[k-1] -= arr[k];
        }
    }
}

int main() {
    std::random_device gen;
    std::uniform_real_distribution<> uid(1, 10);
    int size = uid(gen);
    float arr[size];
    for(int i = 0; i != size; ++i)
        arr[i] = uid(gen);

    for(auto &b: arr)
        std::cout << b << " ";
    std::cout << '\n';

    InsertionSort(size, arr);

    for(auto b: arr)
        std::cout << b << " ";
}



