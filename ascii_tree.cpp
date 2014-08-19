#include <string>
#include <iostream>

#include "ascii_tree.h"

using namespace std;

ascii_tree::~ascii_tree()
{
    free_ascii_tree(root_node);
}

void ascii_tree::free_ascii_tree(asciinode *node)
{
  if (!node) return;
  free_ascii_tree(node->left);
  free_ascii_tree(node->right);
  delete node;
}

void ascii_tree::compute_lprofile(asciinode *node, int x, int y)
{
  int i, isleft;
  if (node == NULL) return;
  isleft = (node->parent_dir == -1);
  lprofile[y] = MIN(lprofile[y], x-((node->label.size()-isleft)/2));
  if (node->left != NULL)
  {
	  for (i=1; i <= node->edge_length && y+i < MAX_HEIGHT; i++)
    {
	    lprofile[y+i] = MIN(lprofile[y+i], x-i);
    }
  }
  compute_lprofile(node->left, x-node->edge_length-1, y+node->edge_length+1);
  compute_lprofile(node->right, x+node->edge_length+1, y+node->edge_length+1);
}

void ascii_tree::compute_rprofile(asciinode *node, int x, int y)
{
  int i, notleft;
  if (node == NULL) return;
  notleft = (node->parent_dir != -1);
  rprofile[y] = MAX(rprofile[y], x+((node->label.size()-notleft)/2));
  if (node->right != NULL)
  {
	  for (i=1; i <= node->edge_length && y+i < MAX_HEIGHT; i++)
    {
	    rprofile[y+i] = MAX(rprofile[y+i], x+i);
    }
  }
  compute_rprofile(node->left, x-node->edge_length-1, y+node->edge_length+1);
  compute_rprofile(node->right, x+node->edge_length+1, y+node->edge_length+1);
}

void ascii_tree::compute_edge_lengths(asciinode *node)
{
    int h, hmin, i, delta;
    if (node == NULL) return;
    compute_edge_lengths(node->left);
    compute_edge_lengths(node->right);

    /* first fill in the edge_length of node */
    if (node->right == NULL && node->left == NULL)
    {
        node->edge_length = 0;
    }
    else
    {
        if (node->left != NULL)
        {
            for (i=0; i<node->left->height && i < MAX_HEIGHT; i++)
            {
                rprofile[i] = -INFINITY;
            }
            compute_rprofile(node->left, 0, 0);
            hmin = node->left->height;
        }
        else
        {
            hmin = 0;
        }
        if (node->right != NULL)
        {
            for (i=0; i<node->right->height && i < MAX_HEIGHT; i++)
            {
                lprofile[i] = INFINITY;
            }
            compute_lprofile(node->right, 0, 0);
            hmin = MIN(node->right->height, hmin);
        }
        else
        {
            hmin = 0;
        }
        delta = 4;
        for (i=0; i<hmin; i++)
        {
            delta = MAX(delta, gap + 1 + rprofile[i] - lprofile[i]);
        }

        //If the node has two children of height 1, then we allow the
        //two leaves to be within 1, instead of 2
        if (((node->left != NULL && node->left->height == 1) ||
                    (node->right != NULL && node->right->height == 1))&&delta>4)
        {
            delta--;
        }

        node->edge_length = ((delta+1)/2) - 1;
    }

    //now fill in the height of node
    h = 1;
    if (node->left != NULL)
    {
        h = MAX(node->left->height + node->edge_length + 1, h);
    }
    if (node->right != NULL)
    {
        h = MAX(node->right->height + node->edge_length + 1, h);
    }
    node->height = h;
}

ascii_tree::asciinode *ascii_tree::build_ascii_tree_recursive(const shared_ptr<visual_tree> vt)
{
    asciinode * node;

    if (!vt) return nullptr;

    node = new asciinode;
    node->left = build_ascii_tree_recursive(vt->visual_left());
    node->right = build_ascii_tree_recursive(vt->visual_right());

    if (node->left != NULL)
    {
        node->left->parent_dir = -1;
    }

    if (node->right != NULL)
    {
        node->right->parent_dir = 1;
    }

    node->label = vt->visual_element();
    switch (vt->visual_color()) {
        case visual_tree::COLOR_RED:
            node->color = "\033[31m";
            break;
        case visual_tree::COLOR_BLACK:
        default:
            break;
    }

    return node;
}

ascii_tree::ascii_tree(const shared_ptr<visual_tree> vt)
{
    asciinode *node;
    if (!vt) return;

    node = build_ascii_tree_recursive(vt);
    if (!node) return;

    node->parent_dir = 0;
    root_node = node;
    compute_edge_lengths(root_node);
    for (int i=0; i<root_node->height && i < MAX_HEIGHT; i++)
    {
        lprofile[i] = INFINITY;
    }
    compute_lprofile(root_node, 0, 0);
    xmin = 0;
    for (int i = 0; i < root_node->height && i < MAX_HEIGHT; i++)
    {
        xmin = MIN(xmin, lprofile[i]);
    }
}

void ascii_tree::print_level(asciinode *node, int x, int level)
{
    int i, isleft;
    if (node == NULL) return;
    isleft = (node->parent_dir == -1);
    if (level == 0)
    {
        for (i=0; i<(x-print_next-((static_cast<int>(node->label.size())-isleft)/2)); i++)
        {
            cout << " ";
        }
        print_next += i;
        //Set terminal color
        cout << node->color;
        cout << node->label;
        //Reset color
        cout << "\033[0m";
        print_next += node->label.size();
    }
    else if (node->edge_length >= level)
    {
        if (node->left != NULL)
        {
            for (i=0; i<(x-print_next-(level)); i++)
            {
                cout << " ";
            }
            print_next += i;
            cout << "/";
            print_next++;
        }
        if (node->right != NULL)
        {
            for (i=0; i<(x-print_next+(level)); i++)
            {
                cout << " ";
            }
            print_next += i;
            cout << "\\";
            print_next++;
        }
    }
    else
    {
        print_level(node->left,
                x-node->edge_length-1,
                level-node->edge_length-1);
        print_level(node->right,
                x+node->edge_length+1,
                level-node->edge_length-1);
    }
}

void ascii_tree::print()
{
    int i;
    for (i = 0; i < root_node->height; i++)
    {
        print_next = 0;
        print_level(root_node, -xmin, i);
        cout << endl;
    }
    if (root_node->height >= MAX_HEIGHT)
    {
        cout << "(This tree is taller than " << MAX_HEIGHT << ", and may be drawn incorrectly.)" << endl;
    }
}
