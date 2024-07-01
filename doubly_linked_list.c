#include <stdio.h>
#include <stdlib.h>

int read_int(char *s) {
  int x;
  printf("%s: ", s);
  scanf("%d", &x);
  return x;
}

typedef struct Node_tag {
  struct Node_tag *next;
  struct Node_tag *prev;
  int info;
} Node;

typedef struct {
  Node *head;
  Node *tail;
  int len;
} List;

List *create_list() {
  List *l = (List *)malloc(sizeof(List));
  l->head = NULL;
  l->tail = NULL;
  l->len = 0;
  return l;
}

Node *create_node(int info) {
  Node *n = (Node *)malloc(sizeof(Node));
  n->info = info;
  n->next = NULL;
  n->prev = NULL;
  return n;
}

int insert(List *l, int info, int index) {
  Node *n = create_node(info);

  if (index < 0) index += l->len + 1;
  if (index > l->len || index < 0) return printf("Invalid Index\n");

  if (index == 0) {
    if (l->head == NULL)
      l->head = l->tail = n;
    else {
      n->next = l->head;
      l->head->prev = n;
      l->head = n;
    }
  } else if (index == l->len) {
    l->tail->next = n;
    n->prev = l->tail;
    l->tail = n;
  } else {
    Node *ptr;
    if (index < l->len / 2) {
      ptr = l->head;
      for (int i = 0; i < index; i++) ptr = ptr->next;
    } else {
      ptr = l->tail;
      for (int i = l->len; i > index; i--) ptr = ptr->prev;
    }
    n->next = ptr->next;
    ptr->next = n;
    n->prev = ptr;
    n->next->prev = n;
  }

  l->len++;
  return 0;
}

int delete(List *l, int index) {
  if (index < 0) index += l->len;
  if (index >= l->len || index < 0) return printf("Invalid Index\n");

  Node *ptr;
  if (index == 0) {
    ptr = l->head->next;
    if (ptr == NULL)
      l->tail = NULL;
    else
      ptr->prev = NULL;
    free(l->head);
    l->head = ptr;
  } else if (index == l->len - 1) {
    ptr = l->tail->prev;
    if (ptr == NULL)
      l->head = NULL;
    else
      ptr->next = NULL;
    free(l->tail);
    l->tail = ptr;
  } else {
    if (index < l->len / 2) {
      ptr = l->head;
      for (int i = 0; i < index; i++) ptr = ptr->next;
    } else {
      ptr = l->tail;
      for (int i = l->len - 1; i > index; i--) ptr = ptr->prev;
    }

    ptr->prev->next = ptr->next;
    ptr->next->prev = ptr->prev;
    free(ptr);
  }

  l->len--;
  return 0;
}

int display(List *l) {
  printf("len: %d\n[", l->len);
  for (Node *ptr = l->head; ptr != NULL; ptr = ptr->next)
    printf("%d ", ptr->info);
  printf("]\n[");
  return 0;
}

int main() {
  List *l = create_list();
  insert(l, 20, 0);
  insert(l, 30, 1);
  insert(l, 40, -1);
  display(l);
  return 0;
}
