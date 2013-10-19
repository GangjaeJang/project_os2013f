#ifndef __LIB_KERNEL_RBTREE_H
#define __LIB_KERNEL_RBTREE_H

#include <stddef.h>

enum rb_color { RED, BLACK };

struct rb_node 
{
  struct rb_node* rb_left;
  struct rb_node* rb_right;
  struct rb_node* rb_parent;
  enum rb_color color;
};

struct rb_root
{
  struct rb_node* node;
};

#define RB_ROOT (struct rb_root) { NULL, }
#define rb_entry(ptr, type, member) container_of(ptr, type, member)

void rb_link_node(struct rb_node*, struct rb_node*, struct rb_node**);
void rb_insert_color(struct rb_node*, struct rb_root* root);
void rb_erase(struct rb_node*, struct rb_root* root);
struct rb_node* rb_first(struct rb_root*);

#endif
