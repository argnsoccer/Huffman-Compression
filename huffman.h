#include <string>
#include <vector>

class Node
{
    char data;
    int frequency;
    char min;
    Node* left;
    Node* right;
    public:
        Node(){}
        Node(const Node &n){data = n.data; frequency = n.frequency; left = n.left; right = n.right;}
        Node(char d, int f): data(d), frequency(f), min(d){}
        Node(Node*, Node*);
        //void fillTree(string*, string&);
};

class LeafNode : public Node
{
    char charByte;
    public:
        LeafNode(){}
        LeafNode(char character, int frequency);

};

class BinTree
{
    int treeSize;
    std::vector<Node*> leafList;
    public:
        BinTree(std::vector<char> chars, std::vector<int> frequencies);

};

LeafNode::LeafNode(char character, int frequency)
{
    charByte = character;
    frequency = frequency;
}

BinTree::BinTree(std::vector<char> chars, std::vector<int> frequencies)
{
    for(int i = 0; i < chars.size(); i++)
    {
        leafList.push_back(new LeafNode(chars[i], frequencies[i]));
    }
}