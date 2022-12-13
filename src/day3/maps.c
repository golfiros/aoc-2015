#include "maps.h"
#include <stdio.h>
#include <string.h>

enum color { BLACK = 0, RED = 1 };

struct rb_node {
  struct rb_node *parent;
  struct rb_node *left;
  struct rb_node *right;
  enum color color;
  void *key;
  void *val;
};

int is_red(struct rb_node *node) { return node != NULL && node->color == RED; }

struct map {
  struct rb_node *root;
  size_t key_size;
  size_t val_size;
  int (*comp)(const void *, const void *);
  void *(*factory)(void);
};

struct map *new_map(size_t key_size, size_t val_size,
                    int (*comp)(const void *, const void *),
                    void *(*factory)(void)) {
  struct map *map = malloc(sizeof(struct map));
  map->key_size = key_size;
  map->val_size = val_size;
  map->comp = comp;
  return map;
}

struct node_queue {
  struct rb_node *node;
  struct node_queue *next;
};

void delete_map(struct map *map) {
  struct node_queue *head, *tail, *old_head;
  old_head = head = tail = malloc(sizeof(struct node_queue));
  head->node = map->root;
  while (head != NULL) {
    if (head->node != NULL) {
      tail = tail->next = malloc(sizeof(struct node_queue));
      tail->node = head->node->left;
      tail = tail->next = malloc(sizeof(struct node_queue));
      tail->node = head->node->right;
      free(head->node->key);
      free(head->node->val);
      free(head->node);
    }
    old_head = head;
    head = head->next;
    free(old_head);
  }
}

size_t map_size(struct map *map) {
  struct node_queue *head, *tail, *old_head;
  old_head = head = tail = malloc(sizeof(struct node_queue));
  head->node = map->root;
  size_t count = 0;
  while (head != NULL) {
    if (head->node != NULL) {
      count += 1;
      tail = tail->next = malloc(sizeof(struct node_queue));
      tail->node = head->node->left;
      tail = tail->next = malloc(sizeof(struct node_queue));
      tail->node = head->node->right;
    }
    old_head = head;
    head = head->next;
    free(old_head);
  }
  return count;
}

void rotate_left(struct map *map, struct rb_node *x) {
  struct rb_node *y = x->right;
  x->right = y->left;
  if (y->left != NULL) {
    y->left->parent = x;
  }
  y->parent = x->parent;
  if (x->parent == NULL) {
    map->root = y;
  } else if (x == x->parent->left) {
    x->parent->left = y;
  } else {
    x->parent->right = y;
  }
  y->left = x;
  x->parent = y;
}

void rotate_right(struct map *map, struct rb_node *x) {
  struct rb_node *y = x->left;
  x->left = y->right;
  if (y->right != NULL) {
    y->right->parent = x;
  }
  y->parent = x->parent;
  if (x->parent == NULL) {
    map->root = y;
  } else if (x == x->parent->right) {
    x->parent->right = y;
  } else {
    x->parent->left = y;
  }
  y->right = x;
  x->parent = y;
}

void insert_fix(struct map *map, struct rb_node *node) {
  struct rb_node *uncle;
  while (is_red(node->parent)) {
    if (node->parent == node->parent->parent->right) {
      uncle = node->parent->parent->left;
      if (is_red(uncle)) {
        uncle->color = BLACK;
        node->parent->color = BLACK;
        node->parent->parent->color = RED;
        node = node->parent->parent;
      } else {
        if (node == node->parent->left) {
          node = node->parent;
          rotate_right(map, node);
        }
        node->parent->color = BLACK;
        node->parent->parent->color = RED;
        rotate_left(map, node->parent->parent);
      }
    } else {
      uncle = node->parent->parent->right;
      if (is_red(uncle)) {
        uncle->color = BLACK;
        node->parent->color = 0;
        node->parent->parent->color = RED;
        node = node->parent->parent;
      } else {
        if (node == node->parent->right) {
          node = node->parent;
          rotate_left(map, node);
        }
        node->parent->color = BLACK;
        node->parent->parent->color = RED;
        rotate_right(map, node->parent->parent);
      }
    }
    if (node == map->root) {
      break;
    }
  }
  map->root->color = BLACK;
}

void map_add(struct map *map, const void *key, const void *val) {
  struct rb_node *x = map->root, *y = NULL;
  while (x != NULL) {
    y = x;
    int comp = map->comp(key, x->key);
    if (comp < 0) {
      x = x->left;
    } else if (comp == 0) {
      return;
    } else {
      x = x->right;
    }
  }
  struct rb_node *node = malloc(sizeof(struct rb_node));
  memcpy(node->key = malloc(map->key_size), key, map->key_size);
  memcpy(node->val = malloc(map->val_size), val, map->val_size);
  node->color = RED;
  node->parent = y;
  if (y == NULL) {
    map->root = node;
  } else if (map->comp(node->key, y->key) < 0) {
    y->left = node;
  } else {
    y->right = node;
  }
  if (node->parent == NULL) {
    node->color = BLACK;
    return;
  }
  if (node->parent->parent == NULL) {
    return;
  }
  insert_fix(map, node);
}

struct rb_node *binary_search(struct map *map, const void *key) {
  struct rb_node *node = map->root;
  int comp;
  while (node != NULL && !(comp = map->comp(key, node->key))) {
    if (comp < 0) {
      node = node->left;
    } else {
      node = node->right;
    }
  }
  return node;
}

void transplant(struct map *map, struct rb_node *u, struct rb_node *v) {
  if (u->parent == NULL) {
    map->root = v;
  } else if (u == u->parent->left) {
    u->parent->left = v;
  } else {
    u->parent->right = v;
  }
  v->parent = u->parent;
}

struct rb_node *minimum(struct map *map, struct rb_node *node) {
  while (node->left != NULL) {
    node = node->left;
  }
  return node;
}

void delete_fix(struct map *map, struct rb_node *x) {
  struct rb_node *s;
  while (x != map->root && !is_red(x)) {
    if (x == x->parent->left) {
      s = x->parent->right;
      if (is_red(s)) {
        s->color = BLACK;
        x->parent->color = RED;
        rotate_left(map, x->parent);
        s = x->parent->right;
      }
      if (!is_red(s->left) && !is_red(s->right)) {
        s->color = RED;
        x = x->parent;
      } else {
        if (!is_red(s->right)) {
          s->left->color = BLACK;
          s->color = RED;
          rotate_right(map, s);
          s = x->parent->right;
        }
        s->color = x->parent->color;
        x->parent->color = BLACK;
        s->right->color = BLACK;
        rotate_left(map, x->parent);
        x = map->root;
      }
    } else {
      s = x->parent->left;
      if (is_red(s)) {
        s->color = BLACK;
        x->parent->color = RED;
        rotate_right(map, x->parent);
        s = x->parent->left;
      }
      if (!is_red(s->right) && !is_red(s->left)) {
        s->color = RED;
        x = x->parent;
      } else {
        if (!is_red(s->left)) {
          s->right->color = BLACK;
          s->color = RED;
          rotate_left(map, s);
          s = x->parent->left;
        }
        s->color = x->parent->color;
        x->parent->color = BLACK;
        s->left->color = BLACK;
        rotate_right(map, x->parent);
        x = map->root;
      }
    }
  }
  x->color = BLACK;
}

void map_remove(struct map *map, const void *key) {
  struct rb_node *x, *y, *z = binary_search(map, key);
  if (z == NULL) {
    return;
  }
  free(z->key);
  free(z->val);
  y = z;
  enum color y_old_color = is_red(y);
  if (z->left == NULL) {
    x = z->right;
    transplant(map, z, z->right);
  } else if (z->right == NULL) {
    x = z->left;
    transplant(map, z, z->left);
  } else {
    y = minimum(map, z->right);
    y_old_color = is_red(y);
    x = y->right;
    if (y->parent == z) {
      x->parent = y;
    } else {
      transplant(map, y, y->right);
      y->right = z->right;
      y->right->parent = y;
    }
    transplant(map, z, y);
    y->left = z->left;
    y->left->parent = y;
    y->color = z->color;
  }
  if (y_old_color == BLACK) {
    delete_fix(map, x);
  }
  free(z);
}

int map_has_key(struct map *map, const void *key) {
  return binary_search(map, key) != NULL;
}

void *map_get(struct map *map, const void *key) {
  struct rb_node *node = binary_search(map, key);
  if (node == NULL) {
    if (map->factory == NULL) {
      return NULL;
    }
    void *val = map->factory();
    map_add(map, key, val);
    free(val);
    return binary_search(map, key)->val;
  }
  return node->val;
}
