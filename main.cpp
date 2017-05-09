#include <iostream>
#include <fstream>
#include "huffman.h"

std::vector<char> parseFile(std::string fileName);
std::vector<int> calcFrequency(std::vector<char>& bytes, std::vector<char>& chars);
void compress();
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
    printFrequencies(chars, frequencies);
    BinTree tree(chars, frequencies);//sets 
    tree.makeTree();//forms the binary tree
    std::cout << "Thank you, would you like to compress (press 1), decompress (press 2), or quit? (press anything else)" << std::endl;
    std::cin >> choice;
    while(choice == 1 || choice == 2)//fix this stuff once you've finished parseFile 
    {
        if(choice == 1)
        {
            compress();
        }
        else if(choice == 2)
        {
            decompress();
        }
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

void compress()
{
    
}
void decompress()
{

}