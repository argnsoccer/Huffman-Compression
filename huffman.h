#include <string>
#include <vector>

#ifndef HUFFMAN_H
#define HUFFMAN_H

class Node
{
    protected:
        char data;
        int frequency;
        char min;
        bool bit;
        bool isLeaf;
        std::vector<bool> bitstream;
    public:
        Node(){}
        Node(const Node &n){data = n.data; frequency = n.frequency; }
        Node(char d, int f): data(d), frequency(f), min(d){}
        Node(Node* child1, Node* child2);
        int getFrequency(){return frequency;}
        int getChildrenSize(){return children.size();}
        virtual Node* findChar(char character);
        virtual char getData(){return data;}
        bool getIsLeaf(){return isLeaf;}
        virtual void clearBitStream(){bitstream.clear();}
        void setBitStream(std::vector<bool> bits){bitstream = bits;}
        std::vector<bool> getBitStream(){return bitstream;}
        virtual void createBitStream();
        Node* getChild(int i){return &*children[i];}

    private:
        std::vector<Node*> children;
    
};

class LeafNode : public Node
{
    private:
        char data;
    public:
        LeafNode(){}
        LeafNode(char character, int frequency);
        LeafNode(char character);
        virtual char getData(){return data;}
        virtual void createBitStream();

};

class BinTree
{
    int treeSize;
    std::vector<Node*> nodeList;
    std::vector<Node*> min2Freqs;
    public:
        BinTree(std::vector<char> chars, std::vector<int> frequencies);
        void makeTree();
        std::vector<Node*> *getTreeList(){return &nodeList;}
};
//------------------------Constructors-----------------------------------------------//

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
    data = character;
    frequency = frequency;
    isLeaf = true;
}

LeafNode::LeafNode(char character)
{
    data = character;
}

BinTree::BinTree(std::vector<char> chars, std::vector<int> frequencies)
{
    for(int i = 0; i < chars.size(); i++)
    {
        nodeList.push_back(new LeafNode(chars[i], frequencies[i]));//set up the list of leaf nodes
    }
}

//----------------------------Setting up methods------------------------------------//

void Node::createBitStream()
{
    for(int i = 0; i < children.size(); i++)
    {
        children[i]->bitstream = bitstream;//getting it initialized with the bitstream til then
        children[i]->bitstream.push_back(bit);//add the current bit
        children[i]->createBitStream();//recursively iterate to get all children and fill bitstream
    }
}

void LeafNode::createBitStream()//final addition of leaf bit
{
    bitstream.push_back(bit);
}

Node* Node::findChar(char character)
{
    Node* cur = this;
    if(cur->getData() == character)
    {
        return this;
    }

    for(int i = 0; i < children.size(); i++)
    {
        cur = children[i]->findChar(character);
        if(cur->getData() == character)
        {
            break;
        }
    }
    return cur;
}

void BinTree::makeTree()
{
    Node*temp = 0;
    int minFreq = nodeList[0]->getFrequency();
    int index = 0;
    int twocount = 0;
    int count1 = 0;
    while(nodeList.size() > 1)
    {
        while(twocount  < 2)//getting the lowest two frequencies
        {
            for(int i = 0; i < nodeList.size(); i++)
            {
                if(nodeList[i]->getFrequency() < minFreq)
                {
                    index = i;
                    minFreq = nodeList[i]->getFrequency();
                }
            }
            if(!nodeList[index]->getIsLeaf())//checks to see whether to push back a regular node or a leaf node
            {
                min2Freqs.push_back(new Node);
            }
            else
            {
                min2Freqs.push_back(new LeafNode(nodeList[index]->getData()));
            }

            *min2Freqs[min2Freqs.size()-1] = *nodeList[index];
            delete nodeList[index];//removing from the list
            nodeList.erase(nodeList.begin()+ index);//resizing list

            if(nodeList.size() > 0)
            {
                minFreq = nodeList[0]->getFrequency();//reset to start the loop over
            }

            twocount++;
            index = 0;
        }
        
        nodeList.push_back(new Node(min2Freqs[min2Freqs.size()-1], min2Freqs[min2Freqs.size()-2]));
        count1++;
        twocount = 0;
    }

    nodeList[0]->clearBitStream();//don't want to accidentally add to the leaf node bitstream

    for(int i = 0; i < nodeList.size(); i++)
    {
        nodeList[i]->createBitStream();
    }
}

#endif