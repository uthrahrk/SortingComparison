#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

void mergeSort(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

int main() {
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

    // Replace the timing section with:
clock_t start = clock();
for (int i = 0; i < 10; i++) {  // Run 10 times for better measurement
    mergeSort(arr, 0, count - 1);
}
clock_t end = clock();
double elapsed = ((double)(end - start)) / (CLOCKS_PER_SEC * 10);
printf("Average Sequential Sort Time (10 runs): %.6f seconds\n", elapsed);

    free(arr);
    return 0;
}