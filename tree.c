#include <stdlib.h>
#include <stdio.h>
#include <math.h>

struct node {
  int value;
  struct node * left;
  struct node * right;
};

typedef struct {
  struct node * head;
} Tree;

typedef struct AVLNode_t {
  int value;
  short int height;
  struct AVLNode_t * parent;
  struct AVLNode_t * right;
  struct AVLNode_t * left;
} AVLNode;

typedef struct {
  AVLNode * head;
} AVLTree; 

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

void printTreeAVLNode(AVLNode * n, int depth) {
  for (int i = 1; i < depth; i++) printf("  ");
  if (depth != 0) printf("|-");
  printf("%d [%d]\n", n->value, n->height);
  if (n->left != NULL) {
    printf("L");
    printTreeAVLNode(n->left, depth + 1);
  }
  if (n->right != NULL) {
    printf("R");
    printTreeAVLNode(n->right, depth + 1);
  }
}

AVLTree * initAVLTree() {
  AVLTree * tree = (AVLTree *) malloc(sizeof(AVLTree));
  tree->head = NULL;
  return tree;
}

AVLNode * leftRotateAVL(AVLNode * top) {
  // swap x and y
  AVLNode * right = top->right;
  int topValue = top->value;
  top->value = right->value;
  right->value = topValue;

  top->height = right->height;

  top->right = right->right;
  right->right = right->left;
  right->left = top->left;
  top->left = right;

  right->height = right->right == NULL ? 0: right->right->height + 1;
  return top;
}

AVLNode * rightRotateAVL(AVLNode * top) {
  AVLNode * left = top->left;
  int topValue = top->value;
  top->value = left->value;
  left->value = topValue;

  top->left = left->left;
  left->left = left->right;
  left->right = top->right;
  top->right = left;

  top->height = left->height;
  left->height = left->left == NULL ? 0: left->left->height + 1;
  return top;
}

short int height(AVLNode * n) {
  return n == NULL ? -1: n->height;
}

char isHeavyOrBalanced(AVLNode * n, char i) {
  short int diff = height(n->left) - height(n->right);
  if (diff == 0) return 1;
  if (i == 1 && diff < 0) return 1;
  else if (i == -1 && diff > 0) return 1;
  return 0;
}

void addValueToAVLTree(AVLTree * t, int value) {
  AVLNode * n = (AVLNode *) malloc(sizeof(AVLNode));
  n->left = n->right = NULL;
  n->value = value;

  if (t->head == NULL) t->head = n; 
  else {
    AVLNode * iterator = t->head;
    while (1) {
      if (value < iterator->value) {
        if (iterator->left == NULL) {
          n->parent = iterator;
          iterator->left = n;
          break;
        } else iterator = iterator->left;
      } else {
        if (iterator->right == NULL) {
          n->parent = iterator;
          iterator->right = n;
          break;
        } else iterator = iterator->right;
      }
    }
  }

  // calculate heights
  while(n != NULL) {
    short int left = height(n->left);
    short int right = height(n->right);

    if (abs(left - right) <= 1) {
      if (left > right) n->height = left;
      else n->height = right;
      n->height++;
    } else {
      printf("[%d] -> %d\n", n->value, right - left);
      if (right - left > 0) {
        // n is right heavy
        if (isHeavyOrBalanced(n->right, 1)) n = leftRotateAVL(n);
        else {
          rightRotateAVL(n->right);
          leftRotateAVL(n);
          n->height++;
        }
      } else if (right - left < 0) {
        // n is left heavy
        if (isHeavyOrBalanced(n->left, -1)) n = rightRotateAVL(n);
        else {
          leftRotateAVL(n->left);
          rightRotateAVL(n);
          n->height++;
        }
      }
    }
    n = n->parent;
  }
}

int treverseTreeAVL(AVLNode * n) {
  int i = 0;
  if (n->left != NULL) i += treverseTreeAVL(n->left);
  printf(" %d ", n->value);
  i++;
  if (n->right != NULL) i += treverseTreeAVL(n->right);
  return i;
}

int COUNT = 5;
void print2DUtil(AVLNode * root, int space)
{
    // Base case
    if (root == NULL)
        return;
 
    // Increase distance between levels
    space += COUNT;
 
    // Process right child first
    print2DUtil(root->right, space);
 
    // Print current node after space
    // count
    printf("\n");
    for (int i = COUNT; i < space; i++)
        printf(" ");
    printf("%d\n", root->value);
 
    // Process left child
    print2DUtil(root->left, space);
}

int main() {
  AVLTree * tree = initAVLTree();
  int arr[] = { 11, 20, 41, 10, 3, 2, 4, 99, 0, 1 };
  //for (int i = 0; i < sizeof(arr) / sizeof(int); i++) addValueToAVLTree(tree, arr[i]);
  for (int i = 10; i > 0; i--) addValueToAVLTree(tree, i);

  printf("----\n");
  printTreeAVLNode(tree->head, 0);
  printf("----\n[");
  treverseTreeAVL(tree->head);
  printf("]\n----\n");
  print2DUtil(tree->head, 0);
  printf("----\n");
}
