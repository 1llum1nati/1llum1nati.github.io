#include <iostream>
#include <ctime>
#include <random>

template<typename T>
void HeapSort(int size, T mass[]) {
    for(int i = 1; i < size; ++i) {
        for(int k = i; k != 0 && mass[k-1] > mass[k]; --k) {
            mass[k-1] += mass[k];
            mass[k] = mass[k-1] - mass[k];
            mass[k-1] -= mass[k];
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

    HeapSort(size, arr);

    for(auto &b: arr)
        std::cout << b << " ";
}



