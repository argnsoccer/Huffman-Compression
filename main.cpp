#include <iostream>
#include <fstream>
#include "huffman.h"

std::vector<char> parseFile(std::string fileName);
std::vector<int> calcFrequency(std::vector<char>& bytes, std::vector<char>& chars);
void compress(std::vector<Node*> *nodeList, std::string outFile, std::vector<char>& fileVector, std::vector<char>& chars, std::vector<int>& frequencies);
void printFrequencies(std::vector<char>& chars, std::vector<int>& frequencies);
void decompress();

int main()
{
    std::string fileName;
    int choice;
    std::cout << "Please enter the name of the file you wish to compress or decompress: ";
    std::cin >> fileName;
    std::vector<char> bytes = parseFile(fileName);
    std::vector<char> chars;
    std::vector<int> frequencies = calcFrequency(bytes, chars);

    //printFrequencies(chars, frequencies);
    BinTree tree(chars, frequencies);//sets 
    tree.makeTree();//forms the binary tree

    std::cout << "Thank you, would you like to compress (press 1), decompress (press 2), or quit? (press anything else)" << std::endl;
    std::cin >> choice;
    while(choice == 1 || choice == 2)
    {
        if(choice == 1)
        {
            std::string compressedFileName = "compressed_" + fileName;
            compress(tree.getTreeList(), compressedFileName, bytes, chars, frequencies);
            
        }
        else if(choice == 2)
        {
            decompress();
        }
        std::cout << "Thank you for choosing an option. Keep going? (1 to compress, 2 to decompress, anything else to quit)" <<std::endl;
        std::cin >> choice;
    }
    
}

std::vector<int> calcFrequency(std::vector<char>& bytes, std::vector<char>& chars)
{
    bool exists = false;
    std::vector<int> frequency;
    for(int i = 0; i < bytes.size(); i++)
    {
        for(int j = 0; j < chars.size(); j++)
        {
            if(bytes[i] == chars[j])
            {
                frequency[j]++;
                exists = true;
                break;
            }
        }
        if(!exists)
        {
            chars.push_back(bytes[i]);
            frequency.push_back(1);
        }
        else
        {
            exists = false;
        }
    }
    return frequency;
}

void printFrequencies(std::vector<char>& chars, std::vector<int>& frequencies)
{
    for(int i = 0; i < chars.size(); i++)
    {
        std::cout << "char: " << chars[i] << " frequency: " << frequencies[i] << std::endl;
    }
}

std::vector<char> parseFile(std::string fileName)//reads the file and loads into a vector
{
    std::fstream fileReader;//filereading setup
    fileReader.open(fileName.c_str(), std::ios::in | std::ios::binary);
    std::vector<char> bytesInFile;
    char nextChar;
    if(fileReader.is_open())
    {
        while(fileReader.get(nextChar))//read in file while there is data
        {
            bytesInFile.push_back(nextChar);
        }
    }
    fileReader.close();
    return bytesInFile;
}

void compress(std::vector<Node*> *nodeList, std::string outFile, std::vector<char>& fileVector, std::vector<char>& chars, std::vector<int>& frequencies)
{
    bool child = false;
    Node* cur;
    char tempByte = tempByte & 0;//clear temp
    int bitCount = 0;
    int bitPos = 128;
    std::ofstream fileWriter;
    std::vector<bool> bitstream;

    fileWriter.open(outFile.c_str(), std::ios::binary);

    for(int i = 0; i < fileVector.size(); i++)
    {
        for(int j = 0; j < nodeList->size(); j++)
        {
            if(nodeList[0][j]->getChildrenSize() > 0)
            {
                cur = nodeList[0][j]->findChar(fileVector[i]);
                bitstream = cur->getBitStream();
                for(int x = 0; x < bitstream.size(); x++)//all these operations are to aggregate the bits into bytes by performing bit operations
                {
                    if(bitstream[x])
                    {
                        for(int y = 0; y < bitCount; y++)
                        {
                            bitPos = bitPos/2;
                        }
                        tempByte = tempByte | bitPos;
                        bitPos = 128;
                        bitCount++;
                    }
                    else
                    {
                        bitCount++;
                    }
                    if(bitCount == 8 || (i == fileVector.size() -1) && (x == bitstream.size() -1))
                    {
                        fileWriter << tempByte;//write byte out to file
                        bitCount = 0;//reset bit counter and tempByte to start again
                        tempByte = tempByte & 0;

                    }
                }
            }
        }
    }

    fileWriter.close();
}

void decompress()
{

}
