#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int * data;
  int cap;
  int top;
} stack;

int read_int(char * msg) {
  int x;
  printf("%s: ", msg);
  scanf("%d", &x);
  return x;
}

stack * init_stack(int cap) {
  stack * s = (stack *) malloc(sizeof(stack));
  s->data = (int  *) malloc(sizeof(int) * cap);
  s->cap = cap;
  s->top = -1;
  return s;
}

void push(stack * s, int val) {
  if (s->cap == s->top + 1) {
      printf("Overflow\n");
      return;
  }
  s->top++;
  s->data[s->top] = val;
}

int pop(stack * s) {
  if (s->top == -1) {
    return printf("Underflow\n");
  }
  printf("popped: %d\n", s->data[s->top--]);
  return s->data[s->top];
}

void print(stack * s) {
  for (int i = s->top; i > -1; i--)
    printf("%d\n", s->data[i]);
}

int main() {
  stack * s = init_stack(read_int("Enter Capacity"));
  while (1) {
    printf("1. print\n2. pop\n3. push\n");
    int c = read_int("Enter choice");
    if (c < 1 || 3 < c) printf("Invalid Choice\n");
    else if (c == 1) print(s);
    else if (c == 2) pop(s);
    else push(s, read_int("Enter value"));
  }
}
