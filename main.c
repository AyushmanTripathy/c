#include <assert.h>
#include <stdio.h>

int main() {
  int a = 5;
  assert(a == 4);
  printf("%d", (a++));
}
