#include <vector>
#include <utility>
#include <iostream>

#include "ascii_tree.h"

using namespace std;

template <typename _T>
class BinaryHeap {
    public:
        class BinaryVisualTree : public visual_tree
        {
        public:
            BinaryVisualTree(BinaryHeap &bh, size_t idx) : mP(bh), mIdx(idx) {};
            BinaryVisualTree(const BinaryVisualTree &bvt) : mP(bvt.mP), mIdx(bvt.mIdx) { cout << "hmm" << endl; };
            BinaryVisualTree &operator=(const BinaryVisualTree &bvt) { cout << "hmm" << endl; };
            ~BinaryVisualTree() {};

        private:
            const shared_ptr<visual_tree> visual_left() const {
                size_t i = lChdIdx(mIdx);
                if (i >= mP.heap.size()) return shared_ptr<visual_tree>();

                return shared_ptr<visual_tree>(new BinaryVisualTree(mP, i));
            };

            const shared_ptr<visual_tree> visual_right() const {
                size_t i = rChdIdx(mIdx);
                if (i >= mP.heap.size()) return shared_ptr<visual_tree>();

                return shared_ptr<visual_tree>(new BinaryVisualTree(mP, i));
            }

            string visual_element() const {
                return to_string(mP.heap[mIdx]);
            }


            BinaryHeap &mP;
            size_t mIdx;
        };
    public:
        BinaryHeap() {};
        ~BinaryHeap() {};

        void push(_T e);
        _T pop();
        bool empty() const { return heap.empty(); };

    private:
        vector<_T> heap;
        static size_t parentIdx(size_t i) { return (i-1)/2; };
        static size_t lChdIdx(size_t i) { return 2*i+1; };
        static size_t rChdIdx(size_t i) { return 2*i+2; };
};


template <typename _T>
void BinaryHeap<_T>::push(_T e)
{
    heap.push_back(e);
    int i = heap.size() - 1;

    while (i) {
        int p = parentIdx(i);
        if (heap[i] < heap[p]) {
            swap(heap[i], heap[p]);
            i = p;
        } else {
            break;
        }
    }
}

template <typename _T>
_T BinaryHeap<_T>::pop()
{
    _T r = heap.front();
    swap(heap.front(), heap.back());
    heap.pop_back();

    if (heap.empty()) return r;

    size_t i = 0;
    while (i < heap.size() - 1) {
        size_t minIdx = i;
        size_t l = lChdIdx(i);
        size_t r = rChdIdx(i);
        if (l < heap.size() && heap[minIdx] > heap[l]) {
            minIdx = l;
        }

        if (r < heap.size() && heap[minIdx] > heap[r]) {
            minIdx = r;
        }

        if (minIdx == i) break;

        swap(heap[minIdx], heap[i]);
        i = minIdx;
    }
    return r;
}


int main(int, char**)
{
    BinaryHeap<int> bh;
    shared_ptr<visual_tree> vt(new BinaryHeap<int>::BinaryVisualTree(bh, 0));
    bh.push(1);
    ascii_tree(vt).print();
    bh.push(3);
    ascii_tree(vt).print();
    bh.push(5);
    ascii_tree(vt).print();
    bh.push(0);
    ascii_tree(vt).print();
    bh.push(4);
    ascii_tree(vt).print();
    bh.push(8);
    ascii_tree(vt).print();
    while(!bh.empty()) { cout << bh.pop(); }
}
