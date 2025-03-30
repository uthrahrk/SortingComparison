#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>

const int SMALL_SIZE_THRESHOLD = 1000;

void merge(std::vector<int>& arr, int left, int mid, int right) {
    std::vector<int> temp(right - left + 1);
    int i = left, j = mid + 1, k = 0;
    
    while (i <= mid && j <= right) {
        temp[k++] = arr[i] <= arr[j] ? arr[i++] : arr[j++];
    }
    while (i <= mid) temp[k++] = arr[i++];
    while (j <= right) temp[k++] = arr[j++];
    std::copy(temp.begin(), temp.end(), arr.begin() + left);
}

void sequentialMergeSort(std::vector<int>& arr, int left, int right) {
    if (left < right) {
        if (right - left <= SMALL_SIZE_THRESHOLD) {
            std::sort(arr.begin() + left, arr.begin() + right + 1);
            return;
        }
        int mid = left + (right - left) / 2;
        sequentialMergeSort(arr, left, mid);
        sequentialMergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

int main() {
    std::ifstream file("random_100000.txt");
    if (!file) {
        std::cerr << "Error opening file." << std::endl;
        return 1;
    }

    std::vector<int> arr;
    int value;
    while (file >> value) {
        arr.push_back(value);
    }

    auto start = std::chrono::high_resolution_clock::now();
    sequentialMergeSort(arr, 0, arr.size() - 1);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Sequential Sort Time: " << elapsed.count() << " seconds" << std::endl;

    return 0;
}