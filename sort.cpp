#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <omp.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

void swap(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
}

void bubbleSort(vector<int> &arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

void parallelBubbleSort(vector<int> &arr) {
    int n = arr.size();
    bool sorted = false;

    while (!sorted) {
        sorted = true;

        #pragma omp parallel for
        for (int i = 1; i < n - 1; i += 2) {
            if (arr[i] > arr[i + 1]) {
                swap(arr[i], arr[i + 1]);
                sorted = false;
            }
        }

        #pragma omp parallel for
        for (int i = 0; i < n - 1; i += 2) {
            if (arr[i] > arr[i + 1]) {
                swap(arr[i], arr[i + 1]);
                sorted = false;
            }
        }
    }
}

void merge(vector<int> &arr, int left, int mid, int right) {
    vector<int> temp(right - left + 1);
    int i = left, j = mid + 1, k = 0;
    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
    }
    while (i <= mid) temp[k++] = arr[i++];
    while (j <= right) temp[k++] = arr[j++];
    for (i = left, k = 0; i <= right; i++, k++) arr[i] = temp[k];
}

void mergeSort(vector<int> &arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

void parallelMergeSort(vector<int> &arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        
        #pragma omp parallel sections
        {
            #pragma omp section
            parallelMergeSort(arr, left, mid);
            
            #pragma omp section
            parallelMergeSort(arr, mid + 1, right);
        }
        merge(arr, left, mid, right);
    }
}


vector<int> generateRandomArray(int size) {
    vector<int> arr(size);
    for (int &x : arr) {
        x = rand() % 100000;
    }
    return arr;
}

int main() {
    int size = 50000; 

    vector<int> arr1 = generateRandomArray(size);
    vector<int> arr2 = arr1;
    vector<int> arr3 = arr1;
    vector<int> arr4 = arr1;

    float t1,t2,t3,t4;
    
    auto start = high_resolution_clock::now();
    bubbleSort(arr1);
    auto end = high_resolution_clock::now();
    cout << "Sequential Bubble Sort Time: " << duration_cast<milliseconds>(end - start).count() << " ms\n";
    t1 = (end - start).count();

    start = high_resolution_clock::now();
    parallelBubbleSort(arr2);
    end = high_resolution_clock::now();
    cout << "Parallel Bubble Sort Time: " << duration_cast<milliseconds>(end - start).count() << " ms\n";
    t2 = (end - start).count();

    start = high_resolution_clock::now();
    mergeSort(arr3, 0, size - 1);
    end = high_resolution_clock::now();
    cout << "Sequential Merge Sort Time: " << duration_cast<milliseconds>(end - start).count() << " ms\n";
    t3 = (end - start).count();

    start = high_resolution_clock::now();
    parallelMergeSort(arr4, 0, size - 1);
    end = high_resolution_clock::now();
    cout << "Parallel Merge Sort Time: " << duration_cast<milliseconds>(end - start).count() << " ms\n";
    t4 = (end - start).count();

    cout<<"SPEED FACTOR BUBBLE SORT "<<t1/t2<<"\n";
    cout<<"SPEED FACTOR MERGE SORT "<<t3/t4<<"\n";
    
    return 0;
}
