#include "huffman.h"

void Node::createBitStream()
{
    for(int i = 0; i < children.size(); i++)
    {
        children[i]->bitstream = bitstream;//getting it initialized with the bitstream til then
        children[i]->bitstream.push_back(bit);//add the current bit
        children[i]->createBitStream();//recursively iterate to get all children and fill bitstream
    }
}


BinTree::makeTree()
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
        nodeList[i].createBitStream();
    }
}