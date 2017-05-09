#include <string>
#include <vector>

class Node
{
    private:
        char data;
        int frequency;
        char min;
        bool bit;
        Node* left;
        Node* right;
        std::vector<Node*> children;
        bool isLeaf;
        std::vector<bool> bitstream;
    public:
        Node(){}
        Node(const Node &n){data = n.data; frequency = n.frequency; left = n.left; right = n.right;}
        Node(char d, int f): data(d), frequency(f), min(d){}
        Node(Node* child1, Node* child2);
        int getFrequency(){return frequency;}
        char getData(){return data;}
        bool getIsLeaf(){return isLeaf;}
        void clearBitStream(){bitstream.clear();}
        void setBitStream(vector<bool> bits){bitstream = bits;}
        vector<bool> getBitStream(){return bitstream;}
        virtual void createBitStream();//virtual because leafNode version will be a tad different
    
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
    std::vector<Node*> nodeList;
    std::vector<Node*> min2Freqs;
    public:
        BinTree(std::vector<char> chars, std::vector<int> frequencies);
        void makeTree();

};

Node::Node(Node* child1, Node*child2)
{
    frequency = child1->getFrequency() + child2->getFrequency();
    bit = false;
    isLeaf = false;
    child1->bit = 1;//left child is 1
    child2->bit = 0;//right child is 0
    children.push_back(child1);
    children.push_back(child2);
}

LeafNode::LeafNode(char character, int frequency)
{
    charByte = character;
    frequency = frequency;
    isLeaf = true;
}

BinTree::BinTree(std::vector<char> chars, std::vector<int> frequencies)
{
    for(int i = 0; i < chars.size(); i++)
    {
        nodeList.push_back(new LeafNode(chars[i], frequencies[i]));//set up the list of leaf nodes
    }
}