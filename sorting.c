#include <stdio.h>
#include <stdlib.h>

void printArr(int * arr, int length) {
  printf("[");
  for (int i = 0; i < length; i++) printf(" %d ", arr[i]);
  printf("]\n");
}

void insertionSort(int * arr, int len) {
  for (int i = 1; i < len; i++) {
    int j = i;
    int tmp = arr[i];
    while (j > 0 && arr[j - 1] > tmp) {
      arr[j] = arr[j - 1];
      j--;
    }
    arr[j] = tmp;
  }
}

void selectionSort(int * arr, int length) {
  int minValue, minIndex;

  for (int i = 0; i < length; i++) {
    minValue = arr[i];
    minIndex = i;

    for (int j = i + 1; j < length; j++) {
      if (minValue > arr[j]) {
        minValue = arr[j];
        minIndex = j;
      }
    }

    int tmp = arr[i];
    arr[i] = minValue;
    arr[minIndex] = tmp;
  }
}

void bubbleSort(int * arr, int length) {
  int swapped = 1;
  while(swapped) {
    swapped = 0;
    for (int j = 0; j < length - 1; j++) {
      if (arr[j] > arr[j+1]) {
        int tmp = arr[j+1];
        arr[j+1] = arr[j];
        arr[j] = tmp;
        swapped = 1;
      }
    }
  }
}

int *mergeSort(int * arr, int length) {
  if (length == 1) return arr;
  
  int divider = length / 2;
  int * left = mergeSort(arr, divider);
  int * right = mergeSort(arr + divider, length - divider);

  int * new_arr = (int *) malloc(sizeof(int) * length);

  int index = 0, leftIndex = 0, rightIndex = 0;

  // merging
  while(leftIndex < divider && rightIndex < length - divider) {
    if (left[leftIndex] < right[rightIndex]) new_arr[index++] = left[leftIndex++];
    else new_arr[index++] = right[rightIndex++];
  }

  // adding the leftover
  while (leftIndex < divider)
    new_arr[index++] = left[leftIndex++];

  while (rightIndex < length - divider)
    new_arr[index++] = right[rightIndex++];

  // copy the data;
  for(int i = 0; i < length; i++) arr[i] = new_arr[i];
  return arr; 
}

void hashSort(int * arr, int length) {
  int min, max; 
  min = max = arr[0];

  for (int i = 0; i < length; i++) {
    if (min > arr[i]) min = arr[i];
    else if (max < arr[i]) max = arr[i];
  }

  int hashLength = max - min + 1;
  int * hash = (int *) malloc(hashLength * sizeof(int));
  for (int i = 0; i < hashLength; i++) hash[i] = 0;

  for (int i = 0; i < length; i++)
    hash[arr[i] - min]++;

  int index = 0;
  for (int i = 0; i < hashLength; i++) {
    for (int j = 0; j < hash[i]; j++) 
      arr[index++] = i + min;
  }
  free(hash);
}

void maxHeapify(int * arr, int length, int i) {
  int left = i * 2;
  int right = (i * 2) + 1;
  int largest = i;

  if (length > left && arr[left] < arr[largest])
    largest = left;
  if (length > right && arr[right] < arr[largest])
    largest = right;

  if (largest != i) {
    int tmp = arr[i];
    arr[i] = arr[largest];
    arr[largest] = tmp;
    maxHeapify(arr, length, largest);
  }
}

void heapSort(int * arr, int length) {
  int * heap = (int *) malloc((length + 1) * sizeof(int));
  for (int i = 0; i < length; i++) heap[i + 1] = arr[i];
  for (int i = length / 2; i > 0; i--)
    maxHeapify(heap, length + 1, i);

  for(int i = 1; i < length; i++) {
    int tmp = heap[1];
    heap[1] = heap[length - i];
    arr[i - 1] = tmp;
    maxHeapify(heap, length - i, 1);
  }
}

void radixSort(int * arr, int length) {
  int table[length][10];
  int indcies[10] = {0};

  int flag, fac = 1;
  do {
    for (int i = 0; i < length; i++) {
      int j = (arr[i] % (fac * 10)) / fac;
      table[indcies[j]++][j] = arr[i];
    }

    flag = 1;
    int index = 0;
    for (int i = 0; i < 10; i++) {
      for (int j = 0; j < indcies[i]; j++)
        arr[index++] = table[j][i];
      if (indcies[i] == length) flag = 0;
      indcies[i] = 0;
    }

    fac *= 10;
  } while (flag);
}

int main() {
  int arr[] = { 3, 2, 0 , 2, 4};
  int length = sizeof(arr) / sizeof(int);
  radixSort(arr, length);
  printArr(arr, length);
}
