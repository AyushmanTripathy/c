#include <stdio.h>
#include <stdlib.h>

int weights[] = {5, 3, 2};
int values[] = {9, 7, 8};
int length = sizeof(values) / sizeof(int);
int capacity = 5;

struct node {
  struct node *left;
  struct node *right;
  int value;
  int weight;
  int index;
};

typedef struct {
  struct node *head;
} Tree;

Tree *initTree() {
  Tree *t = (Tree *)malloc(sizeof(Tree));
  t->head = NULL;
  return t;
}

void printTreeNode(struct node *n, int depth) {
  if (n == NULL)
    return;
  for (int i = 1; i < depth; i++)
    printf("  ");
  if (depth != 0)
    printf("|-");
  printf("(%d,%d,%d)\n", n->index, n->weight, n->value);
  printTreeNode(n->left, depth + 1);
  printTreeNode(n->right, depth + 1);
}

struct node *createNode(int index, int weight, int value) {
  struct node *n = (struct node *)malloc(sizeof(struct node));
  n->index = index;
  n->weight = weight;
  n->value = value;
  n->left = n->right = NULL;
  return n;
}

struct node *addNodeToLeft(struct node *n, int index);
struct node *addNodeToRight(struct node *n, int index);
void addNode(struct node *n, int index);

struct node *addNodeToLeft(struct node *n, int index) {
  struct node *m = createNode(index, n->weight, n->value);
  addNode(m, index - 1);
  return m;
}

struct node *addNodeToRight(struct node *n, int index) {
  int weight = n->weight - weights[index];
  if (weight < 0)
    return NULL;

  struct node *m = createNode(index, weight, n->value + values[index]);
  addNode(m, index - 1);
  return m;
}

void addNode(struct node *n, int index) {
  if (index == -1)
    return;
  if (n->weight == 0)
    return;

  n->left = addNodeToLeft(n, index);
  n->right = addNodeToRight(n, index);
}

void knapSackDecisionTree(Tree *tree) {
  int index = sizeof(values) / sizeof(int) - 1;
  tree->head = createNode(index, capacity, 0);
  addNode(tree->head, index);
}

int getBestValue(struct node *n) {
  if (n->index == 0 || n->weight == 0)
    return n->value;
  int left = n->left == NULL ? -1 : getBestValue(n->left);
  int right = n->right == NULL ? -1 : getBestValue(n->right);
  return left > right ? left : right;
}

int main() {
  Tree *tree = initTree();
  knapSackDecisionTree(tree);
  printTreeNode(tree->head, 0);
  printf("ans: %d\n", getBestValue(tree->head));
}
