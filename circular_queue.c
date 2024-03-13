#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int * data;
  int cap;
  int front;
  int rear;
} queue;

queue * init_queue(int cap) {
  queue * q = (queue *) malloc(sizeof(queue));
  q->data = (int *) malloc(sizeof(int) * cap);
  q->cap = cap;
  q->front = -1;
  q->rear = -1;
  return q;
}

void push(queue * q, int val) {
  if (
      (q->front == 0 && q->rear == q->cap - 1) ||
      (q->rear + 1 == q->front)
    ) {
    // overflow
    printf("Overflow\n");
    return;
  }

  if (q->front == -1) {
    q->front = q->rear = 0;
    q->data[0] = val;
  } else if (q->rear == q->cap - 1) q->rear = 0;
  else q->rear++;
  q->data[q->rear] = val;
}

void pop(queue * q) {
  if (q->rear == -1) {
    printf("Underflow\n");
    return;
  }

  // one element
  printf("popped: %d\n", q->data[q->front]);

  if (q->front == q->rear) q->front = q->rear = -1;
  else {
    q->front++;
    if (q->front == q->cap) q->front = 0;
  }
}

void print(queue * q) {
  printf("cap: %d, front: %d, rear: %d\n", q->cap, q->front, q->rear);
  if (q->front == -1) {
    printf("Queue is empty\n");
    return;
  }
  printf("front: ");
  for (int i = q->front; 1; i = i == q->cap - 1 ? 0: i + 1) {
    printf("%d ", q->data[i]);
    if (i == q->rear) break;
  }
  printf(" :rear\n");
}

int read_int(char * msg) {
  printf("%s: ", msg);
  int x;
  scanf("%d", &x);
  return x;
}

int main() {
  queue * q = init_queue(read_int("Enter Capacity"));
  while (1) {
    printf("1. print\n2. pop\n3. push\n");
    int c = read_int("Enter choice");
    if (c == 1) print(q);
    else if (c == 2) pop(q);
    else if (c == 3) push(q, read_int("Enter value"));
    else printf("invalid choice\n");
  }
}
