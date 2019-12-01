#include "Lab8.h"

double infin() { return numeric_limits<double>::infinity(); }

double distance(Node &a, Graph &G) { return G.NodeGetDistance(a); }

void distance(Node &a, Graph &G, double dis) { G.NodeSetDistance(a, dis); }

list<Node> make_Queue(Graph &G) {
    list<Node> Pqueue;
    for (size_t i = 0; i < G.num_nodes(); i++) {
        Pqueue.push_back(G.getNode(i));
    }
    return Pqueue;
}

Node *deleteMin(list<Node> &H, Graph &G) {
    list<Node>::iterator minN;
    double min = infin();

    // list<Node>::iterator k = bList.begin(); k != bList.end(); k++
    for (list<Node>::iterator a = H.begin(); a != H.end(); a++) {
        if (min >= distance(*a, G)) {
            min = distance(*a, G);
            minN = a;
        }
    }

    Node *a = &(*minN);
    H.erase(minN);
    return a;
}

void Dijkstra_list(Graph &G, Node &s) {
    G.Reset();

    distance(s, G, 0);

    list<Node> H = make_Queue(G);

    while (!H.empty()) {
        Node *u = deleteMin(H, G);
        if (u != NULL) {
            for (Node v : G.getAdjNodes(*u)) {
                double uPlus = distance(*u, G) + G.getWeight(*u, v);
                if (distance(v, G) > uPlus) {
                    distance(v, G, uPlus);
                }
            }
        }
    }
}

void Dijkstra_heap(Graph &G, Node &s) {}

bool test_List(string name, vector<double> answer) {
    Graph g(name);
    Node source = g.getNode(0);
    Dijkstra_list(g, source);
    for (size_t i = 0; i < answer.size(); i++) {
        if (answer[i] != g.getNode(i).getDistance()) {
            cout << g;
            return false;
        }
    }
    return true;
}

bool testall() {
    if (!test_List("test.txt", vector<double>{0, 3.4, 6.5, 8.4, 11.4, 5.4})) {
        return false;
    }
    if (!test_List("test1.txt", vector<double>{0, 2, 4, 3})) {
        return false;
    }
    if (!test_List("test2.txt", vector<double>{0, 120.58, infin(), infin()})) {
        return false;
    }
    if (!test_List("test3.txt", vector<double>{0, 1, 7, 5})) {
        return false;
    }
    return true;
}

int main() {
    if (!testall()) {
        cout << "Tests Failed" << endl;
    }
    return 0;
}