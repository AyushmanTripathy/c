#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int * data;
  int len;
  int cap;
} heap;


heap * init_heap(int cap) {
  heap * h = (heap *) malloc(sizeof(heap));
  h->data = (int *) malloc(sizeof(int) * cap);
  h->cap = cap;
  h->len = 0;
  return h;
}

void swap(int * arr, int i, int j) {
  int tmp = arr[i];
  arr[i] = arr[j];
  arr[j] = tmp;
}

void push(heap * h, int val) {
  if (h->len == h->cap) return;

  h->data[h->len] = val;
  h->len++;
  
  int i = h->len - 1;
  while (1) {
    if (i * 2 < h->len && h->data[i] < h->data[i * 2]) swap(h->data, i, i * 2);
    else if (i * 2  + 1 < h->len && h->data[i] < h->data[i * 2 + 1]) swap(h->data, i, i * 2 + 1);

    if (i == 0) break;
    i = i / 2;
  }
}

int pop(heap * h) {
  if (h->len == 0) return -1;
  h->len--;
  swap(h->data, 0, h->len);

  int i = 0;
  while (1) {
    if (i * 2 < h->len && h->data[i] < h->data[i * 2]) {
      swap(h->data, i, i * 2);
      i = i * 2;
    } else if (i * 2 + 1 < h->len && h->data[i] < h->data[i * 2 + 1]) {
      swap(h->data, i, i * 2 + 1);
      i = i * 2 + 1;
    }
    else break;
  }

  return h->data[h->len];
}

int main() {
  heap * h = init_heap(5);
  push(h, 10);
  push(h, 5);
  push(h, 3);
  push(h, 1);
  push(h, 1);
  push(h, 1);
  push(h, 2);


  printf("%d\n", pop(h));
  printf("%d\n", pop(h));
  printf("%d\n", pop(h));
  printf("%d\n", pop(h));
  printf("%d\n", pop(h));
  printf("%d\n", pop(h));
  printf("%d\n", pop(h));

  printf("[");
  for (int i = 0; i < h->len; i++) printf("%d ", h->data[i]);
  printf("]\n");
}
