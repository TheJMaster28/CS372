#include <math.h>
#include <iostream>
#include <limits>
#include <string>
#include <unordered_map>
#include <vector>
#include "Lab8.h"
using namespace std;

class BinaryHeap {
   private:
    int capacity;
    int heap_size;
    vector<Node> arr;
    unordered_map<string, int> keyIndex;

   public:
    BinaryHeap(Graph &G) {
        heap_size = 0;
        arr.resize(G.num_nodes());
        for (size_t i = 0; i < G.num_nodes(); i++) {
            Node a = G.getNode(i);
            arr[i] = a;
            keyIndex.insert(pair<string, int>(a.name(), i));
        }
        for (size_t i = arr.size(); i > arr.size(); i--) {
            shiftDown(i);
        }
    }

    bool empty() {
        if (heap_size == 0) {
            return true;
        }
        return false;
    }

    int parent(int index) { return (index - 1) / 2; }

    int left(int index) { return 2 * index + 1; }

    int right(int index) { return 2 * index + 2; }

    BinaryHeap make_Heap(Graph &G) {
        // for (size_t i = 0; i < G.num_nodes(); i++) {
        //     Node a = G.getNode(i);
        //     H.insertKey(a);
        // }
    }

    int getIndex(Node &a) {
        unordered_map<string, int>::iterator key = keyIndex.find(a.name());
        return key->second;
    }

    void swap(Node a, Node b) {
        int aIndex = getIndex(a);
        int bIndex = getIndex(b);
        Node temp = arr[aIndex];
        arr[aIndex] = b;
        arr[bIndex] = a;

        unordered_map<string, int>::iterator key = keyIndex.find(a.name());
        key->second = bIndex;

        key = keyIndex.find(b.name());
        key->second = aIndex;
    }

    int minChild(int index) {
        int l = left(index);
        int r = right(index);
        int smallest = index;

        if (l < heap_size && arr[l].getDistance() < arr[smallest].getDistance()) {
            smallest = l;
        }

        if (r < heap_size && arr[r].getDistance() < arr[smallest].getDistance()) {
            smallest = r;
        }
        return smallest;
    }
    void shiftDown(int index) {
        // if (smallest != index) {
        //     swap(arr[index], arr[smallest]);

        //     Heapify(smallest);
        // }
        Node a = arr[index];
        int minChildIndex = minChild(index);
        while (minChildIndex != 0 && arr[minChildIndex].getDistance() < arr[index].getDistance()) {
            swap(arr[index], arr[minChildIndex]);
            index = minChildIndex;
            a = arr[index];
        }
        arr[index] = a;
    }

    void insertKey(Node &a) {
        heap_size++;
        int index = heap_size - 1;
        arr[index] = a;
        keyIndex.insert(pair<string, int>(a.name(), index));
        while (index != 0 && arr[parent(index)].getDistance() > arr[index].getDistance()) {
            swap(arr[index], arr[parent(index)]);
            index = parent(index);
        }
    }

    void decreseKey(int index, double newValue) {
        arr[index].setDistance(newValue);
        while (index != 0 && arr[parent(index)].getDistance() > arr[index].getDistance()) {
            swap(arr[index], arr[parent(index)]);
            index = parent(index);
        }
    }

    Node deletMin() {
        if (heap_size == 1) {
            heap_size--;
            return arr[0];
        }

        Node root = arr[0];
        heap_size--;
        arr[0] = arr[heap_size];
        unordered_map<string, int>::iterator key = keyIndex.find(arr[0].name());
        key->second = 0;
        shiftDown(0);
        return root;
    }
};