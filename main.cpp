#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include "huffman.h"

std::vector<char> parseFile(std::string fileName);
std::vector<int> calcFrequency(std::vector<char>& bytes, std::vector<char>& chars);
std::vector<int> calcFrequencyCompressed(std::vector<char>& bytes, std::vector<char>& chars);
void compress(std::vector<Node*> *nodeList, std::string outFile, std::vector<char>& bytes, std::vector<char>& chars, std::vector<int>& frequencies);
void printFrequencies(std::vector<char>& chars, std::vector<int>& frequencies);
void decompress(std::vector<Node*> *nodeList, std::vector<char>& bytes, std::string inFile, std::string outFile);
std::string getBitsInChar(char dummyByte);

int main()
{
    std::string fileName;
    int choice;
    
    std::vector<char> bytes;
    std::vector<char> chars;
    std::vector<int> frequencies;

    std::cout << "Would you like to compress (press 1), decompress (press 2), or quit? (press anything else)" << std::endl;
    std::cin >> choice;
    while(choice == 1 || choice == 2)
    {
        if(choice == 1)
        {
            chars.clear();
            frequencies.clear();
            bytes.clear();

            std::cout << "Please enter the name of the file you wish to compress: ";
            std::cin >> fileName;
            bytes = parseFile(fileName);
            frequencies = calcFrequency(bytes, chars);
            //printFrequencies(chars, frequencies);
            BinTree tree(chars, frequencies);
            tree.makeTree();//forms the binary tree

            std::string compressedFileName = "compressed_" + fileName;
            compress(tree.getTreeList(), compressedFileName, bytes, chars, frequencies);
            
        }
        else if(choice == 2)
        {
            chars.clear();
            frequencies.clear();
            bytes.clear();

            std::cout << "Please enter the name of the file you wish to decompress (the file should begin with 'compressed_': ";
            std::cin >> fileName;
            bytes = parseFile(fileName);

            frequencies = calcFrequencyCompressed(bytes, chars);
            //printFrequencies(chars, frequencies);
            BinTree reconstructedTree(chars, frequencies);
            reconstructedTree.makeTree();
            std::string decompressedFileName = fileName;
            decompressedFileName.replace(decompressedFileName.begin(),decompressedFileName.begin()+11,"decompressed_");

            decompress(reconstructedTree.getTreeList(), bytes, fileName, decompressedFileName);
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
            if(bytes[i] == chars[j] && bytes[j] != '\n')
            {
                frequency[j]++;
                exists = true;
                break;
            }
        }
        if(!exists && (bytes[i] != '\n' || bytes[i] != '\r'))
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

std::vector<int> calcFrequencyCompressed(std::vector<char>& bytes, std::vector<char>& chars)
{
    std::string temp;
    std::vector<int> frequency;
    bool flag = true;
    int i = 0;
    int j = 0;
    while(flag)
    {
        chars.push_back(' ');
        frequency.push_back(0);
        chars[j] = bytes[i];
        i++;

        while(bytes[i] != ',')
        {
            temp.push_back(bytes[i]);
            i++;
        }
        i++;

        frequency[j] = std::stoi(temp, nullptr, 0);
        temp.clear();

        if(chars[j] == '0' || frequency[j] == '0')
        {
            chars.pop_back();
            frequency.pop_back();
            flag = false;
        }
        j++;
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
    std::ifstream fileReader;//filereading setup
    int fileLength;
    fileReader.open(fileName.c_str(), std::ios::in | std::ios::binary);
    std::vector<char> bytesInFile;
    char nextChar;
    fileReader.seekg(0, std::ios::end); // put the "cursor" at the end of the file
    fileLength = fileReader.tellg();
    if(fileLength == 0)
    {
        std::cout << "Your file is empty. Please try again" << std::endl;
        exit(EXIT_FAILURE);
        
    } 
    fileReader.seekg(0, std::ios::beg);
    if(fileReader.is_open())
    {
        while(fileReader.good() && fileReader.get(nextChar))//read in file while there is data
        {
            bytesInFile.push_back(nextChar);
        }
    }
    fileReader.close();
    return bytesInFile;
}

void compress(std::vector<Node*> *nodeList, std::string outFile, std::vector<char>& bytes, std::vector<char>& chars, std::vector<int>& frequencies)
{
    bool child = false;
    Node* cur;
    char tempByte = tempByte & 0;//clear temp
    int bitCount = 0;
    int bitPos = 128;
    std::ofstream fileWriter;
    std::vector<bool> bitstream;
    bitstream.clear();

    fileWriter.open(outFile.c_str(), std::ios::binary);

    for(int a = 0; a < chars.size(); a++)//writes the tree to the file for reconstruction
    {
        fileWriter << chars[a];
        fileWriter << frequencies[a];
        fileWriter << ",";
    }

    fileWriter << "00" << ",";

    for(int i = 0; i < bytes.size(); i++)
    {
        for(int j = 0; j < nodeList->size(); j++)
        {
            if(nodeList[0][j]->getChildrenSize() > 0)
            {
                cur = nodeList[0][j]->findChar(bytes[i]);
                bitstream = cur->getBitStream();
                for(int x = 1; x < bitstream.size(); x++)//all these operations are to aggregate the bits into bytes by performing bit operations
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
                    if(bitCount == 8 || (i == bytes .size() -1) && (x == bitstream.size() -1))
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

void decompress(std::vector<Node*> *nodeList, std::vector<char>& bytes, std::string inFile, std::string outFile)
{
    std::string temp;
    std::vector<int> frequency;
    std::vector<char> chars;
    bool flag = true;
    int i = 0;
    int j = 0;
    int bitPos = 128;
    Node *cur = &*nodeList[0][0];
    int bitCount = 0;
    std::ofstream fileWriter;
    std::vector<bool> bitstream;
    fileWriter.open(outFile.c_str(), std::ios::binary);

    while(flag)
    {
        chars.push_back(' ');
        frequency.push_back(' ');
        chars[j] = bytes[i];
        i++;

        while(bytes[i] != ',')
        {

            temp.push_back(bytes[i]);
            i++;
        }
        i++;

        frequency[j] = bytes[i];
       
        temp.clear();

        if(chars[j] == '0' || frequency[j] == '0')
        {
            i = i + 3;//escaping the "00,"
            for(i; i < bytes.size();)
            {
                for(int a = 0; a < bitCount; a++)
                {
                    bitPos = bitPos/2;
                }
                if((bytes[i] & bitPos) == bitPos)
                {
                    bitstream.push_back(1);

                    if(cur->getChildrenSize() > 0)
                    {
                        cur = cur->getChild(1);

                        if(cur->getIsLeaf())//if leaf, write data to file
                        {
                            fileWriter << cur->getData();
                            cur = &*nodeList[0][0];
                        }
                    }
                }
                else
                {
                    bitstream.push_back(0);

                    if(cur->getChildrenSize() > 0)
                    {
                        cur = cur->getChild(0);

                        if(cur->getIsLeaf())
                        {
                            fileWriter << cur->getData();
                            cur = &*nodeList[0][0];
                        }
                    }
                }
                bitPos = 128;
                bitCount++;

                if(bitCount == 8)//reset and move iterator once the byte is parsed
                {
                    bitCount = 0;
                    i++;
                }
            }


            fileWriter.close();

            flag = false;
        }
        j++;
    }
}
