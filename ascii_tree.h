#ifndef __ASCII_TREE_H__
#define __ASCII_TREE_H__
#include <string>
#include <iostream>
#include <memory>

//C++ implementation to output tree structure to the terminal.
//Based on http://web.archive.org/web/20090617110918/http://www.openasthra.com/c-tidbits/printing-binary-trees-in-ascii/
//More info http://stackoverflow.com/questions/801740/c-how-to-draw-a-binary-tree-to-the-console
//Almost no logic changes. This is only a C++ wrapper.

class visual_tree {
    public:
    enum color_e{
        COLOR_RED,
        COLOR_BLACK,
    };
    virtual ~visual_tree() {};
    virtual std::string visual_element() const = 0;
    virtual color_e visual_color() const { return COLOR_RED; };
    virtual const std::shared_ptr<visual_tree> visual_left() const = 0;
    virtual const std::shared_ptr<visual_tree> visual_right() const = 0;
};

class ascii_tree
{
    struct asciinode {
        asciinode() : left(nullptr), right(nullptr) {};
        asciinode *left, *right;
        //length of the edge from this node to its children
        int edge_length;
        int height;
        //-1=I am left, 0=I am root, 1=right
        int parent_dir;
        //max supported unit32 in dec, 10 digits max
        std::string label;
        //Color string
        std::string color;
    };

    public:
    ascii_tree(const std::shared_ptr<visual_tree>);
    virtual ~ascii_tree();
    void print();

    private:
    void free_ascii_tree(asciinode *node);
    asciinode * build_ascii_tree_recursive(const std::shared_ptr<visual_tree> t);
    //This function fills in the edge_length and 
    //height fields of the specified tree
    void compute_edge_lengths(asciinode *node);
    //The following function fills in the lprofile array for the given tree.
    //It assumes that the center of the label of the root of this tree
    //is located at a position (x,y).  It assumes that the edge_length
    //fields have been computed for this tree.
    void compute_rprofile(asciinode *node, int x, int y);
    void compute_lprofile(asciinode *node, int x, int y);
    //This function prints the given level of the given tree, assuming
    //that the node has the given x cordinate.
    void print_level(asciinode *node, int x, int level);

    asciinode *root_node;
    int xmin;

    const static int MAX_HEIGHT = 1000;
    const static int INFINITY = 1<<20;
    int MIN (int X, int Y) { return ((X) < (Y)) ? (X) : (Y); }
    int MAX (int X, int Y) { return ((X) > (Y)) ? (X) : (Y); }
    int lprofile[MAX_HEIGHT];
    int rprofile[MAX_HEIGHT];
    //adjust gap between left and right nodes
    const static int gap = 3;
    //used for printing next node in the same level, 
    //this is the x coordinate of the next char printed
    int print_next;
};

#endif

