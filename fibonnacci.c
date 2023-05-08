#include <stdio.h>
#include <stdlib.h>

int counter = 0;

void printArr(int * arr, int length) {
  printf("[");
  for(int i = 0; i < length; i++) printf(" %d ", arr[i]);
  printf("]\n");
}

int fibh(int * arr, int n) {
  printf("-");
  if (n == 1 || n == 2) return 1;
  if (arr[n]) return arr[n];

  arr[n] = fibh(arr, n - 1) + fibh(arr, n - 2);
  return arr[n];
}

int fibonnaccih(int n) {
  int * arr = (int *) malloc((n + 1) * sizeof(int));
  for (int i = 0; i <= n; i++) arr[i] = 0;
  return fibh(arr, n);
}

int fibn(int n) {
  printf("-");
  if (n == 1 || n == 2) return 1;
  return fibn(n - 1) + fibn(n - 2);
}

int fibi(int n) {
  int a = 1, b = 1;
  for (int i = 2; i < n; i++) {
    printf("-");
    int tmp = a;
    a = b + a;
    b = tmp;
  }
  return a; 
}

int main() {
  int n = 10;
  printf("\nhashing: %d,\n", fibonnaccih(n));
  printf("\nrecursion: %d,\n", fibn(n));
  printf("\niteration: %d, \n", fibi(n));
}
