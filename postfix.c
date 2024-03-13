#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int * data;
  int top;
  int capacity;
} stack;

char precedance[127];

stack * init_stack(int c) {
  stack * s = (stack *) malloc(sizeof(stack));
  s->data = (int *) malloc(sizeof(int) * c);
  s->capacity = c;
  s->top = -1;
  return s;
}

void free_stack(stack * s) {
  free(s->data);
  free(s);
}

int push(stack * s, int x) {
  if (s->top == s->capacity - 1)
    return printf("Overflow\n");
  s->top++;
  s->data[s->top] = x;
  return x;
}

int top(stack * s) {
  if (s->top == -1) printf("Underflow\n");
  return s->data[s->top];
}

int pop(stack * s) {
  if (s->top == -1)
    return printf("Underflow\n");
  return s->data[s->top--];
}


char * converter(char * infix) {
  int len = 0;
  for (int i = 0; infix[i] != '\0'; i++) len++;

  stack * operators = init_stack(len);
  push(operators, '(');

  char * postfix = (char *) malloc(sizeof(char) * (len + 1));
  int postfix_index = 0;

  for (int i = 0; i <= len; i++) {
    if (infix[i] >= '0' && infix[i] <= '9') {
      postfix[postfix_index++] = infix[i];
    } else {
      if (infix[i] == '(') push(operators, infix[i]);
      else if (infix[i] == ')' || infix[i] == '\0') {
        for (char ch = pop(operators); ch != '('; ch = pop(operators))
          postfix[postfix_index++] = ch;
      } else if (precedance[top(operators)] > precedance[infix[i]]) {
        postfix[postfix_index++] = infix[i];
      } else push(operators, infix[i]);
    }
  }

  free_stack(operators);
  postfix[len] = '\0';
  return postfix;
}

int executioner(char * infix) {
  char * postfix = converter(infix);
  stack * nums = init_stack(10);

  for (int i = 0; postfix[i] != '\0'; i++) {
    if (postfix[i] >= '0' && postfix[i] <= '9') {
      push(nums, postfix[i] - '0');
    } else {
      int z, x = pop(nums), y = pop(nums);
      switch (postfix[i]) {
        case '+':
          z = x + y;
          break;
        case '-':
          z = x - y;
          break;
        case '*':
          z = x * y;
          break;
        case '/':
          z = x / y;
          break;
      }
      push(nums, z);
    }
  }

  int res = top(nums);
  free_stack(nums);
  free(postfix);
  return res;
}

int main() {
  precedance['+'] = 1;
  precedance['-'] = 1;
  precedance['*'] = 2;
  precedance['/'] = 2;
  precedance['^'] = 3;
  printf("%d\n", executioner("9+2"));
  return 0;
}
