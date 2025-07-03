#include "RealmShapers.h"
#include <cmath>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <queue>

ShaperTree::ShaperTree() {}

ShaperTree::~ShaperTree()
{
    for (auto shaper : realmShapers)
    {
        delete shaper;
    }
    realmShapers.clear();
}

void ShaperTree::initializeTree(std::vector<RealmShaper *> shapers)
{
    for (RealmShaper *shaper : shapers)
    {
        insert(shaper); // add players to the tree
    }
}

int ShaperTree::getSize()
{
    return realmShapers.size();  // return size
}

std::vector<RealmShaper *> ShaperTree::getTree()
{
    return realmShapers;
}

bool ShaperTree::isValidIndex(int index)
{
    return index >= 0 && index < realmShapers.size();
}

void ShaperTree::insert(RealmShaper *shaper)
{
    if (!shaper)
    {
        std::cerr << "Error: Cannot insert a null shaper." << std::endl;
        return;
    }
    realmShapers.push_back(shaper);
}

int ShaperTree::remove(RealmShaper *shaper)
{
    int index = findIndex(shaper);
    if (index == -1)
    {
        std::cerr << "Error: Shaper not found." << std::endl;
        return -1;
    }
    realmShapers.erase(realmShapers.begin() + index);
    return index;
}

int ShaperTree::findIndex(RealmShaper *shaper)
{
    for (int i = 0; i < realmShapers.size(); ++i)
    {
        if (*realmShapers[i] == *shaper)  // if shaper is found return index
            return i;
    }
    return -1;  // if not return -1
}

int ShaperTree::getDepth(RealmShaper *shaper)
{
    int index = findIndex(shaper);
    if (index == -1)
        return -1;  // if not found return -1

    int depth = 0;
    while (index > 0)
    {
        index = (index - 1) / 2;  // find parent index
        ++depth;
    }
    return depth;
}

int ShaperTree::getDepth()
{
    return getDepth(realmShapers[0]);  // calculate depth
}

RealmShaper ShaperTree::duel(RealmShaper *challenger, bool result)
{
    int index = findIndex(challenger);
    if (index == -1 || 2 * index + 1 >= realmShapers.size())  // if no challenger
    {
        std::cerr << "Error: Challenger has no opponent." << std::endl;
        return *challenger;  // if no opponent return challenger
    }

    RealmShaper *opponent = realmShapers[2 * index + 1];
    RealmShaper *winner = result ? challenger : opponent;
    RealmShaper *loser = result ? opponent : challenger;

    winner->gainHonour();
    loser->loseHonour();

    std::cout << "[Duel] " << winner->getName() << " won the duel" << std::endl;
    std::cout << "[Honour] New honour points: "
              << winner->getName() << "-" << winner->getHonour() << " "
              << loser->getName() << "-" << loser->getHonour() << std::endl;

    if (loser->getHonour() <= 0)
    {
        std::cout << "[Duel] " << loser->getName() << " lost all honour, delete" << std::endl;
        remove(loser);
    }

    return *winner;
}

RealmShaper *ShaperTree::getParent(RealmShaper *shaper)
{
    int index = findIndex(shaper);
    if (index == -1 || index == 0)  //  if not found or root
        return nullptr;

    int parentIndex = (index - 1) / 2;  // find parent index
    return realmShapers[parentIndex];
}

void ShaperTree::replace(RealmShaper *player_low, RealmShaper *player_high)
{
    int lowIndex = findIndex(player_low);
    int highIndex = findIndex(player_high);

    if (lowIndex == -1 || highIndex == -1)
    {
        std::cerr << "Error: One or both shapers not found." << std::endl;
        return;
    }

    std::swap(realmShapers[lowIndex], realmShapers[highIndex]);
}

RealmShaper *ShaperTree::findPlayer(RealmShaper shaper)
{
        return findPlayer(shaper.getName());  // find player by name
}

RealmShaper *ShaperTree::findPlayer(std::string name)
{
    for (auto shaper : realmShapers)
    {
        if (shaper->getName() == name)  // find player by n
            return shaper;
    }
    return nullptr;  // if not found return nullptr
}

std::vector<std::string> ShaperTree::inOrderTraversal(int index)
{
    std::vector<std::string> result;
    if (!isValidIndex(index))
        return result;

    int left = 2 * index + 1;
    int right = 2 * index + 2;

    auto leftResult = inOrderTraversal(left);
    result.insert(result.end(), leftResult.begin(), leftResult.end());
    result.push_back(realmShapers[index]->getName());
    auto rightResult = inOrderTraversal(right);
    result.insert(result.end(), rightResult.begin(), rightResult.end());

    return result;
}

std::vector<std::string> ShaperTree::preOrderTraversal(int index)
{
    std::vector<std::string> result;
    if (!isValidIndex(index))
        return result;

    result.push_back(realmShapers[index]->getName());
    auto leftResult = preOrderTraversal(2 * index + 1);
    result.insert(result.end(), leftResult.begin(), leftResult.end());
    auto rightResult = preOrderTraversal(2 * index + 2);
    result.insert(result.end(), rightResult.begin(), rightResult.end());

    return result;
}

std::vector<std::string> ShaperTree::postOrderTraversal(int index)
{
    std::vector<std::string> result;
    if (!isValidIndex(index))
        return result;

    auto leftResult = postOrderTraversal(2 * index + 1);
    result.insert(result.end(), leftResult.begin(), leftResult.end());
    auto rightResult = postOrderTraversal(2 * index + 2);
    result.insert(result.end(), rightResult.begin(), rightResult.end());
    result.push_back(realmShapers[index]->getName());

    return result;
}

void ShaperTree::preOrderTraversal(int index, std::ofstream &outFile)
{
    if (!isValidIndex(index))
        return;

    outFile << realmShapers[index]->getName() << std::endl;
    preOrderTraversal(2 * index + 1, outFile);
    preOrderTraversal(2 * index + 2, outFile);
}

void ShaperTree::breadthFirstTraversal(std::ofstream &outFile)
{
    if (realmShapers.empty())
        return;

    std::queue<int> q;
    q.push(0);  // root index
    while (!q.empty())
    {
        int index = q.front();
        q.pop();

        if (isValidIndex(index))
        {
            outFile << realmShapers[index]->getName() << std::endl;

            int left = 2 * index + 1;
            int right = 2 * index + 2;
            if (isValidIndex(left)) q.push(left);
            if (isValidIndex(right)) q.push(right);
        }
    }
}

void ShaperTree::displayTree()
{
    std::cout << "[Shaper Tree]" << std::endl;
    printTree(0, 0, "");
}

void ShaperTree::printTree(int index, int level, const std::string &prefix)
{
    if (!isValidIndex(index))
        return;

    std::cout << prefix << (level > 0 ? "   └---- " : "") << *realmShapers[index] << std::endl;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (isValidIndex(left) || isValidIndex(right))
    {
        printTree(left, level + 1, prefix + (level > 0 ? "   │   " : ""));
        printTree(right, level + 1, prefix + (level > 0 ? "   │   " : ""));
    }
}

void ShaperTree::writeShapersToFile(const std::string &filename)
{
    std::ofstream outFile(filename);
    if (!outFile.is_open())
    {
        std::cerr << "Error: Could not open file for writing." << std::endl;
        return;
    }

    for (auto shaper : realmShapers)
    {
        outFile << shaper->getName() << std::endl;  // write shapers to file
    }

    std::cout << "[Output] Shapers have been written to " << filename << " according to rankings." << std::endl;
}

void ShaperTree::writeToFile(const std::string &filename)
{
    // open file
    std::ofstream outFile(filename);
    if (!outFile.is_open())
    {
        std::cerr << "Error: Could not open file for writing." << std::endl;
        return;
    }
    //  write tree to file
    preOrderTraversal(0, outFile);

    // close file information
    std::cout << "[Output] " << "Tree has been written to " << filename << " in pre-order." << std::endl;

    // close file
    outFile.close();
}
