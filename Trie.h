#ifndef TRIE_H
#define TRIE_H

#include <string>
#include <map>
#include <vector>

const int ALPHABET_SIZE = 26;

struct TrieNode
{
    std::map<char, TrieNode*> children;
    bool isEndOfWord = false; 
};

class Trie {
private:
    TrieNode* root;
    void autocompleteHelper(TrieNode* node, std::string prefix, std::vector<std::string>& words);

public:
    Trie();
    void insert(const std::string& word);
    std::vector<std::string> autocomplete(const std::string& prefix);
};

#endif
