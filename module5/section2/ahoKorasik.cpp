#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>

using namespace std;

struct TrieNode {
    unordered_map<char, TrieNode*> children;
    TrieNode* fail = nullptr;
    vector<int> output;
};

class AhoCorasick {
public:
    AhoCorasick() { root = new TrieNode(); }
    
    void insert(const string& word, int index) {
        TrieNode* node = root;
        for (char ch : word) {
            if (!node->children.count(ch)) {
                node->children[ch] = new TrieNode();
            }
            node = node->children[ch];
        }
        node->output.push_back(index);
    }
    
    void build() {
        queue<TrieNode*> q;
        root->fail = root;
        
        for (auto& entry : root->children) {
            char ch = entry.first;
            TrieNode* child = entry.second;
            child->fail = root;
            q.push(child);
        }
        
        while (!q.empty()) {
            TrieNode* curr = q.front();
            q.pop();
            
            for (auto& entry : curr->children) {
                char ch = entry.first;
                TrieNode* child = entry.second;
                TrieNode* fail = curr->fail;
                
                while (fail != root && !fail->children.count(ch)) {
                    fail = fail->fail;
                }
                
                if (fail->children.count(ch) && fail->children[ch] != child) {
                    child->fail = fail->children[ch];
                } else {
                    child->fail = root;
                }
                
                child->output.insert(child->output.end(), child->fail->output.begin(), child->fail->output.end());
                q.push(child);
            }
        }
    }
    
    vector<vector<int>> search(const string& text, int m) {
        vector<vector<int>> occurrences(m);
        TrieNode* node = root;
        
        for (int i = 0; i < text.size(); ++i) {
            char ch = text[i];
            while (node != root && !node->children.count(ch)) {
                node = node->fail;
            }
            if (node->children.count(ch)) {
                node = node->children[ch];
            } else {
                node = root;
            }
            for (int patternIndex : node->output) {
                occurrences[patternIndex].push_back(i + 1);
            }
        }
        
        return occurrences;
    }

private:
    TrieNode* root;
};

int main() {
    string s;
    int m;
    cin >> s;
    cin >> m;
    vector<string> patterns(m);
    
    AhoCorasick ac;
    for (int i = 0; i < m; ++i) {
        cin >> patterns[i];
        ac.insert(patterns[i], i);
    }
    
    ac.build();
    vector<vector<int>> result = ac.search(s, m);
    
    for (int i = 0; i < m; ++i) {
        for (int pos : result[i]) {
            cout << pos << " ";
        }
        cout << "\n";
    }
    
    return 0;
}
