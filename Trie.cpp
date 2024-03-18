#include "Trie.h"

Trie::Trie() {
    root = new TrieNode();
}

void Trie::insert(const std::string& word) {
    TrieNode* node = root;

    for (char c : word) {
        if (node->children.find(c) == node->children.end()) {
            node->children[c] = new TrieNode();
        }
        node = node->children[c];
    }

    node->isEndOfWord = true;
}

void Trie::autocompleteHelper(TrieNode* node, std::string prefix, std::vector<std::string>& words) {
    if (node->isEndOfWord) {
        words.push_back(prefix);
    }

    for (auto it : node->children) {
        autocompleteHelper(it.second, prefix + it.first, words);
    }
}

std::vector<std::string> Trie::autocomplete(const std::string& prefix) {
    TrieNode* node = root;
    std::vector<std::string> words;

    for (char c : prefix) {
        if (node->children.find(c) == node->children.end()) {
            return words;
        }
        node = node->children[c];
    }

    autocompleteHelper(node, prefix, words);

    return words;
}
