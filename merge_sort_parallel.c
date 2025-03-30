#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <omp.h>

const int SMALL_SIZE_THRESHOLD = 5000;
const int MAX_DEPTH = 2;  // Controls parallel recursion depth

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

void parallelMergeSort(std::vector<int>& arr, int left, int right, int depth = 0) {
    if (left < right) {
        if (right - left <= SMALL_SIZE_THRESHOLD) {
            std::sort(arr.begin() + left, arr.begin() + right + 1);
            return;
        }
        int mid = left + (right - left) / 2;
        
        if (depth < MAX_DEPTH) {
            #pragma omp task shared(arr)
            parallelMergeSort(arr, left, mid, depth + 1);
            #pragma omp task shared(arr)
            parallelMergeSort(arr, mid + 1, right, depth + 1);
            #pragma omp taskwait
        } else {
            parallelMergeSort(arr, left, mid, depth + 1);
            parallelMergeSort(arr, mid + 1, right, depth + 1);
        }
        merge(arr, left, mid, right);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <num_threads>" << std::endl;
        return 1;
    }

    int num_threads = std::atoi(argv[1]);
    omp_set_num_threads(num_threads);

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

    double start = omp_get_wtime();
    #pragma omp parallel
    {
        #pragma omp single
        parallelMergeSort(arr, 0, arr.size() - 1);
    }
    double end = omp_get_wtime();

    std::cout << "Parallel Sort with " << num_threads 
              << " threads took: " << end - start << " seconds" << std::endl;

    return 0;
}