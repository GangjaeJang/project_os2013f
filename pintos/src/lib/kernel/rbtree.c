#include "rbtree.h"

void rb_link_node(struct rb_node *node, struct rb_node *parent, struct rb_node** rb_link)
{
  node->color = parent->color;
  node->rb_left = node->rb_right = NULL;

  *rb_link = node;
}

static void rb_replace_node(struct rb_root* root, struct rb_node* old_node, struct rb_node* new_node)
{
  if (old_node->rb_parent == NULL)
    root->node = new_node;
  else
  {
    if (old_node == old_node->rb_parent->rb_left)
      old_node->rb_parent->rb_left = new_node;
    else
      old_node->rb_parent->rb_right = new_node;
  }

  if (new_node != NULL)
  {
    new_node->rb_parent = old_node->rb_parent;
  }
}

static void rb_rotate_left(struct rb_root* root, struct rb_node* node)
{
  struct rb_node* right = node->rb_right;

  rb_replace_node(root, node, right);

  node->rb_right = right->rb_left;

  if (right->rb_left != NULL)
  {
    right->rb_left->rb_parent = node;
  }
  right->rb_left = node;
  node->rb_parent = right;
}

static void rb_rotate_right(struct rb_root *root, struct rb_node* node)
{
  struct rb_node* left = node->rb_left;
  
  rb_replace_node(root, node, left);

  node->rb_left = left->rb_right;

  if (left->rb_right != NULL)
  {
    left->rb_right->rb_parent = node;
  }

  left->rb_right = node;
  node->rb_parent = left;
}

void rb_insert_color(struct rb_node* node, struct rb_root* root)
{
  int _switch = 1;
  struct rb_node *g_node, *u_node;

  while (_switch)
  {
    /* Sets Helper Nodes
     * g_node is so-called "grandparent" node of node
     * u_node is so-called "uncle" node of node
     * */
    g_node = ((node != NULL) && (node->rb_parent != NULL)) ? \
             node->rb_parent->rb_parent \
             : NULL;
    u_node = (g_node != NULL) ? \
             ((node->rb_parent == g_node->rb_left) ? g_node->rb_right : g_node->rb_left) \
             : NULL;

    switch (_switch)
    {
      case 1:
        if (node->rb_parent == NULL)
        {
          node->color = BLACK;
          _switch = 0;
          break;
        }
      case 2:
        if (node->rb_parent->color == BLACK)
        {
          node->color = RED;
          _switch = 0;
          break;
        }
      case 3:
        if ((u_node != NULL) && (u_node->color != RED))
        {
          node->rb_parent->color = BLACK;
          u_node->color = BLACK;
          g_node->color = RED;
          _switch = 1;
          node = g_node;
          break;
        }
      case 4:
        if ((node == node->rb_parent->rb_right) && (node->rb_parent == g_node->rb_left))
        {
          rb_rotate_left(root, node->rb_parent);
          node = node->rb_left;
        }
        else if ((node == node->rb_parent->rb_left) && (node->rb_parent = g_node->rb_right))
        {
          rb_rotate_right(root, node->rb_parent);
          node = node->rb_right;
        }
        _switch = 5;
        break;
      case 5:
        node->rb_parent->color = BLACK;
        g_node->color = RED;
        if (node == node->rb_parent->rb_left)
          rb_rotate_right(root, g_node);
        else
          rb_rotate_left(root, g_node);
      default:
        _switch = 0;
        break;
    }
  }
}

void rb_erase(struct rb_node *node, struct rb_root* root)
{
  struct rb_node *t_node, *tp_node;
  struct rb_node *s_node, *c_node;
  int _switch = 1;

  if ((node->rb_left != NULL)&&(node->rb_right != NULL))
  {
    for (t_node = node; t_node->rb_right != NULL; t_node = t_node->rb_right);

    tp_node = t_node->rb_parent;

    rb_replace_node (root, node, t_node);

    tp_node->rb_right = node;
    node->rb_parent = tp_node;

    tp_node = t_node->rb_left;

    t_node->rb_left = node->rb_left;
    t_node->rb_right = node->rb_right;

    node->rb_left = tp_node;
    node->rb_right = NULL;
  }

  c_node = (node->rb_right == NULL) ? node->rb_left: node->rb_right;
  t_node = node;

  if (node->color == BLACK)
  {
    node->color = c_node->color;

    while (_switch)
    {
      s_node = (node->rb_parent != NULL) ? \
               ((node->rb_parent->rb_left == node) ? node->rb_parent->rb_right : node->rb_parent->rb_left) \
               : NULL;
      switch(_switch)
      {
        case 1:
          if (node->rb_parent == NULL)
          {
            _switch = 0;
            break;
          }
        case 2:
          if (s_node->color == RED)
          {
            node->rb_parent->color = RED;
            s_node->color = BLACK;
            if (node == node->rb_parent->rb_left)
              rb_rotate_left(root, node->rb_parent);
            else
              rb_rotate_right(root, node->rb_parent);
            _switch = 0;
            break;
          }
        case 3:
          if ((node->rb_parent->color == BLACK) && \
              (s_node->color == BLACK) && \
              (s_node->rb_left->color == BLACK) && \
              (s_node->rb_right->color == BLACK))
          {
            s_node->color = RED;
            _switch = 1;
            node = node->rb_parent;
            break;
          }
        case 4:
          if ((node->rb_parent->color == RED) && \
              (s_node->color == BLACK) && \
              (s_node->rb_left->color == BLACK) && \
              (s_node->rb_right->color == BLACK))
          {
            s_node->color = RED;
            node->rb_parent->color = BLACK;
            _switch = 0;
            break;
          }
        case 5:
          if (s_node->color == BLACK)
          {
            if ((node == node->rb_parent->rb_left) && \
                (s_node->rb_right->color == BLACK) && \
                (s_node->rb_left->color == RED))
            {
              s_node->color = RED;
              s_node->rb_left->color = BLACK;
              rb_rotate_right(root, s_node);
            }
            else if((node == node->rb_parent->rb_right) && \
                (s_node->rb_left->color == BLACK) && \
                (s_node->rb_right->color = RED))
            {
              s_node->color = RED;
              s_node->rb_right->color = BLACK;
              rb_rotate_left(root, s_node);
            }
            _switch = 0;
            break;
          }
        case 6:
          s_node->color = node->rb_parent->color;
          node->rb_parent->color = BLACK;

          if (node == node->rb_parent->rb_left)
          {
            s_node->rb_right->color = BLACK;
            rb_rotate_left(root, node->rb_parent);
          }
          else
          {
            s_node->rb_left->color = BLACK;
            rb_rotate_right(root, node->rb_parent);
          }
          _switch = 0;
          break;
        default:
          _switch = 0;
          break;
      }
    }
  }
  rb_replace_node (root, t_node, c_node);
}

struct rb_node* rb_first(struct rb_root *root)
{
  struct rb_node* node;
  for (node = root->node; node != NULL && node->rb_left != NULL; node = node->rb_left);

  return node;
}
