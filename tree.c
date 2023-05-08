#include <stdlib.h>
#include <stdio.h>

struct node {
  int value;
  struct node * left;
  struct node * right;
};

typedef struct {
  struct node * head;
} Tree;

Tree *initTree() {
  Tree * t = (Tree *) malloc(sizeof(Tree));
  t->head = NULL;
  return t;
}

struct node *addValueToTree(Tree * tree, int value) {
  struct node *n = (struct node *) malloc(sizeof(struct node));
  n->value = value;
  n->left = n->right = NULL;

  if (tree->head == NULL) tree->head = n;
  else {
    struct node * p = tree->head;
    while(1) {
      if (p->value < value) {
        if (p->right == NULL) return p->right = n;
        else p = p->right;
      } else {
        if (p->left == NULL) return p->left = n;
        else p = p->left;
      }
    }
  }
}

void printTreeNode(struct node * n, int depth) {
  if (n == NULL) return;
  for (int i = 1; i < depth; i++) printf("  ");
  if (depth != 0) printf("|-");
  printf("%d\n", n->value);
  printTreeNode(n->left, depth + 1);
  printTreeNode(n->right, depth + 1);
}

int main() {
  Tree * tree = initTree();
  addValueToTree(tree, 2);
  addValueToTree(tree, 3);
  addValueToTree(tree, 4);
  addValueToTree(tree, 5);
  addValueToTree(tree, 1);
  addValueToTree(tree, 0);

  printTreeNode(tree->head, 0);
}
