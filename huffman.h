#include <string>

class Node
{
    char data;
    int frequency;
    char min;
    Node* left;
    Node* right;
    public:
        Node(){}
        Node(const Node &n){data = n.data; frequency = n.frequency; leftC = n.left; rightC = n.right;}
        Node(char d, int f): data(d), frequency(f), min(d){}
        Node(Node*, Node*);
        void fillTree(string*, string&);
};

class BinTree
{
    int treeSize;
}