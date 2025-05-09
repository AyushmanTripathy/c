#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void naive(const char *str, const char *pat) {
  int str_len = strlen(str);
  int pat_len = strlen(pat);
  assert(pat_len != 0 || str_len != 0);

  int i, j;
  for (int x = 0; x <= str_len - pat_len; x++) {
    for (i = x, j = 0; j < pat_len && i < str_len; j++, i++) {
      if (str[i] != pat[j])
        break;
    }
    if (j == pat_len)
      printf("match at %d\n", x);
  }
}

void robin_karp(const char *str, const char *pat) {
  int str_len = strlen(str);
  int pat_len = strlen(pat);
  assert(pat_len != 0 || str_len != 0);

  if (pat_len > str_len)
    return;

  int pat_hash = 0, str_hash;
  for (int i = 0; i < pat_len; i++) {
    pat_hash += pat[i];
    str_hash += str[i];
  }

  int i, j;
  for (int x = 0, y = pat_len; y <= str_len; x++, y++) {
    if (str_hash == pat_hash) {
      for (i = x, j = 0; j < pat_len && i < str_len; j++, i++) {
        if (str[i] != pat[j])
          break;
      }
      if (j == pat_len)
        printf("match at %d\n", x);
    }

    str_hash -= str[x];
    str_hash += str[y];
  }
}

void knuth_moris_pratt(const char *str, const char *pat) {
  int str_len = strlen(str);
  int pat_len = strlen(pat);
  assert(pat_len != 0 || str_len != 0);

  if (pat_len > str_len)
    return;

  int *pi_table = (int *)malloc(sizeof(char) * pat_len);
  pi_table[0] = 0;

  int pi_len = 0, i = 1;
  while (i < pat_len) {
    if (pat[i] == pat[pi_len])
      pi_table[i++] = ++pi_len;
    else if (pi_len == 0)
      pi_table[i++] = 0;
    else
      pi_len = pi_table[pi_len - 1];
  }

  printf("pi table\n");
  for (int i = 0; i < pat_len; i++)
    printf("%d ", pi_table[i]);
  printf("\n");

  int y = 0, x = 0;
  while (x < str_len) {
    if (pat[y] == str[x]) {
      // match case
      x++;
      if (pat_len - 1 == y) {
        printf("match at %d\n", x - pat_len);
        y = pi_table[y];
      } else y++;
    } else {
      // unmatch case
      if (y == 0) x++;
      else y = pi_table[y - 1];
    }
  }
}

int main() {
  char *str = "a";
  char *pat = "a";

  naive(str, pat);
  robin_karp(str, pat);
  knuth_moris_pratt(str, pat);
}
