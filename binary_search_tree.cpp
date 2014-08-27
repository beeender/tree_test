#include "ascii_tree.h"

using namespace std;

template<typename _T>
class BinarySearchTree {
private:
    struct _Node{
        _Node *mLeft;
        _Node *mRight;
        _T mValue;
        _Node(const _T &value, _Node *left = nullptr, _Node *right = nullptr) :
            mLeft(left), mRight(right), mValue(value)
        {
        };
    };

public:
    BinarySearchTree();
    ~BinarySearchTree();
    bool insert(const _T &v);
    void remove(const _T &v);
    _T &find_max();
    _T &find_min();
    _T &find(_T v);
    template<typename UnaryFun>
    void traverse(_Node *node, UnaryFun fun) {
        if (!node) return;

        traverse(node->mLeft, fun);
        fun(node);
        traverse(node->mRight, fun);
    };

    class VisualTree : public visual_tree
    {
    public:
        VisualTree(BinarySearchTree &bst) : mNode(&(bst.mRoot)){};
        ~VisualTree() {};

    private:
        VisualTree(_Node **node) : mNode(node){};
        const shared_ptr<visual_tree> visual_left() const {
            if (!(*mNode)->mLeft) return shared_ptr<visual_tree>();

            return shared_ptr<visual_tree>(new VisualTree(&(*mNode)->mLeft));
        };

        const shared_ptr<visual_tree> visual_right() const {
            if (!(*mNode)->mRight) return shared_ptr<visual_tree>();

            return shared_ptr<visual_tree>(new VisualTree(&(*mNode)->mRight));
        }

        string visual_element() const {
            return to_string((*mNode)->mValue);
        }
        _Node **mNode;
    };

private:

    _Node **find_node(const _T v) {
        _Node **tmp = &mRoot;
        while (true) {
            if (!(*tmp)) return nullptr;

            if (v < (*tmp)->mValue) {
                tmp = &(*tmp)->mLeft;
                continue;
            }
            if ((*tmp)->mValue < v) {
                tmp = &(*tmp)->mRight;
                continue;
            }
            return tmp;
        }
        return nullptr;
    };

    _Node *mRoot;
};

template <typename _T>
BinarySearchTree<_T>::BinarySearchTree() : mRoot(nullptr)
{
}

template <typename _T>
BinarySearchTree<_T>::~BinarySearchTree()
{
    traverse(mRoot,
            [](_Node *node) {
                cout << node->mValue;
                delete node;
            });
}

template <typename _T>
bool BinarySearchTree<_T>::insert(const _T &v)
{
    _Node **tmp = &mRoot;
    while (true) {
        if (!(*tmp)) {
            *tmp = new _Node(v);
            return true;
        }

        if (v < (*tmp)->mValue) {
            tmp = &((*tmp)->mLeft);
            continue;
        }

        if ((*tmp)->mValue < v) {
            tmp = &((*tmp)->mRight);
            continue;
        }
        break;
    }
    return false;
}

template <typename _T>
void BinarySearchTree<_T>::remove(const _T &v)
{
    _Node **tmp = find_node(v);
    if (!tmp) return;

    if (!(*tmp)->mLeft && !(*tmp)->mRight) {
        delete *tmp;
        *tmp = nullptr;
        return;
    }

    if (!(*tmp)->mLeft) {
        _Node *right = (*tmp)->mRight;
        delete *tmp;
        *tmp = right;
        return;
    }

    if (!(*tmp)->mRight) {
        _Node *left = (*tmp)->mLeft;
        delete *tmp;
        *tmp = left;
        return;
    }

    _Node **left = &(*tmp)->mRight;
    while ((*left)->mLeft) {
        left = &(*left)->mLeft;
    }
    (*left)->mLeft = (*tmp)->mLeft;
    if (*left == (*tmp)->mRight) {
        (*left)->mRight = nullptr;
    } else {
        (*left)->mRight = (*tmp)->mRight;
    }
    delete *tmp;
    *tmp = *left;
    *left = nullptr;
}

int main(int, char**)
{
    BinarySearchTree<int> bst;
    bst.insert(1);
    shared_ptr<visual_tree> vt(new BinarySearchTree<int>::VisualTree(bst));

    ascii_tree(vt).print();
    bst.insert(5);
    ascii_tree(vt).print();
    bst.insert(4);
    ascii_tree(vt).print();
    bst.insert(9);
    ascii_tree(vt).print();
    bst.insert(0);
    ascii_tree(vt).print();
    bst.insert(-1);
    ascii_tree(vt).print();
    bst.insert(-9);
    ascii_tree(vt).print();
    bst.remove(1);
    ascii_tree(vt).print();
}

