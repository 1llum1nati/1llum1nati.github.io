#include <iostream>
#include <ctime>
#include <random>

template<typename T>
void ShellSort(int size, T arr[])
{
    int step = size / 2;
    while (step > 0) {
        for (int i = 0; i != (size - step); ++i) {
            for(int j = i; j >= 0 && arr[j] > arr[j + step]; --j) {
                arr[j] += arr[j + step];
                arr[j + step] = arr[j] - arr[j + step];
                arr[j] -= arr[j + step];
            }
        }
        step /= 2;
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

    ShellSort(size, arr);
    
    for(auto &b: arr)
        std::cout << b << " ";
}



