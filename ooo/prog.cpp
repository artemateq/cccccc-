#define INTEGER

#include <iostream>
#include <cstdlib>
#include <ctime>
#include "function.h"

int main() {
    srand(static_cast<unsigned>(time(0)));

    const int SIZE = 10;
    DataType arr[SIZE];

#ifdef INTEGER
    DataType newValue = 100;
#endif
#ifdef DOUBLE
    DataType newValue = 100.5;
#endif
#ifdef CHAR
    DataType newValue = 'z';
#endif

    FillArray(arr, SIZE);
    std::cout << "Array:" << std::endl;
    ShowArray(arr, SIZE);

    std::cout << "Min: " << MinArray(arr, SIZE) << std::endl;
    std::cout << "Max: " << MaxArray(arr, SIZE) << std::endl;

    SortArray(arr, SIZE);
    std::cout << "Sorted array:" << std::endl;
    ShowArray(arr, SIZE);

    EditArray(arr, SIZE, 0, newValue);
    std::cout << "Array after editing element 0:" << std::endl;
    ShowArray(arr, SIZE);

    return 0;
}
