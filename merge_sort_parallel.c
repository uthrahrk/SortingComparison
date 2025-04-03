#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void merge(int arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    int L[n1], R[n2];

    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    i = 0; j = 0; k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void parallelMergeSort(int arr[], int l, int r, int depth) {
    if (l < r) {
        int m = l + (r - l) / 2;
        
        if (depth < 3 && (r - l) > 10000) {
            #pragma omp parallel sections
            {
                #pragma omp section
                parallelMergeSort(arr, l, m, depth + 1);
                
                #pragma omp section
                parallelMergeSort(arr, m + 1, r, depth + 1);
            }
        } else {
            parallelMergeSort(arr, l, m, depth + 1);
            parallelMergeSort(arr, m + 1, r, depth + 1);
        }
        merge(arr, l, m, r);
    }
}

int main(int argc, char *argv[]) {
    // Add at start of main():
printf("System has %d processors available\n", omp_get_num_procs());
    if (argc != 2) {
        printf("Usage: %s <num_threads>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen("random_100000.txt", "r");
    if (!file) {
        printf("Error opening file.\n");
        return 1;
    }

    int count = 0, num;
    while (fscanf(file, "%d", &num) == 1) count++;
    
    rewind(file);
    int *arr = malloc(count * sizeof(int));
    for (int i = 0; i < count; i++) {
        fscanf(file, "%d", &arr[i]);
    }
    fclose(file);

    omp_set_num_threads(atoi(argv[1]));
    // Replace the timing section with:
double total = 0;
int runs = 10;

for (int i = 0; i < runs; i++) {
    double start = omp_get_wtime();
    #pragma omp parallel
    {
        #pragma omp single
        parallelMergeSort(arr, 0, count - 1, 0);
    }
    total += omp_get_wtime() - start;
}
printf("Average Parallel Sort (%s threads) Time (10 runs): %.6f seconds\n",
      argv[1], total/runs);

    free(arr);
    return 0;
}