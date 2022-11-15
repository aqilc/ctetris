// g++ swap.cpp -o swap.exe
#include <iostream>
#include <cstdlib>

using namespace std;

void swap(int *a, int *b)
{
    int temp = a;
    a = b;
    b = temp;
}

int main()
{
    int i, j;
    cin >> i >> j;


    cout << "Before swap: " << i << " " << j << endl;
    swap(i, j);
    cout << "After swap: " << i << " " << j;

    return EXIT_SUCCESS;
}
