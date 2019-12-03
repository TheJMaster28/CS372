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
    int heap_size;
    vector<Node> arr;
    unordered_map<string, int> keyIndex;

   public:
    // creates Binary Heap from Graph
    BinaryHeap(Graph &G) {
        arr.resize(G.num_nodes());
        heap_size = arr.size();
        for (size_t i = 0; i < G.num_nodes(); i++) {
            Node a = G.getNode(i);
            arr[i] = a;
            keyIndex.insert(pair<string, int>(a.name(), i));
        }
        for (size_t i = arr.size(); i > arr.size(); i--) {
            shiftDown(i);
        }
    }

    // checks if heap is empty
    bool empty() {
        if (heap_size == 0) {
            return true;
        }
        return false;
    }

    // gets parent node index
    int parent(int index) { return (index - 1) / 2; }

    // gets left child index
    int left(int index) { return 2 * index + 1; }

    // gets right child index
    int right(int index) { return 2 * index + 2; }

    // gets index of a node from hash table
    int getIndex(Node &a) {
        unordered_map<string, int>::iterator key = keyIndex.find(a.name());
        return key->second;
    }

    // swap two nodes in heap
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

    // gets minuim child from parent index
    int minChild(int index) {
        int l = left(index);
        int r = right(index);
        int smallest = index;

        if (l > heap_size || r > heap_size) {
            return -1;
        }

        if (l < heap_size && arr[l].getDistance() < arr[smallest].getDistance()) {
            smallest = l;
        }

        if (r < heap_size && arr[r].getDistance() < arr[smallest].getDistance()) {
            smallest = r;
        }
        return smallest;
    }

    // shifts down element at given index
    void shiftDown(int index) {
        int minChildIndex = minChild(index);
        while (minChildIndex != 0 && minChildIndex > 0 && arr[minChildIndex].getDistance() < arr[index].getDistance()) {
            swap(arr[index], arr[minChildIndex]);
            index = minChildIndex;
            minChildIndex = minChild(index);
        }
    }

    // inserts new element
    void insertKey(Node &a) {
        heap_size++;
        int index = heap_size - 1;
        arr.resize(heap_size);
        arr[index] = a;
        keyIndex.insert(pair<string, int>(a.name(), index));
        while (index != 0 && arr[parent(index)].getDistance() > arr[index].getDistance()) {
            swap(arr[index], arr[parent(index)]);
            index = parent(index);
        }
    }

    // updates key with new value
    void decreseKey(int index, double newValue) {
        arr[index].setDistance(newValue);
        while (index != 0 && arr[parent(index)].getDistance() > arr[index].getDistance()) {
            swap(arr[index], arr[parent(index)]);
            index = parent(index);
        }
    }

    // takes out root node and shits down elements
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