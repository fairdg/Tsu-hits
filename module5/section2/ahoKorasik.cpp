#include <iostream>
#include <vector>
#include <string>
#include <queue>

using namespace std;

struct TrieNode {
    vector<int> next; // Переходы для каждого символа (a..z)
    int fail; // Ссылка на суффиксный префикс
    vector<int> out; // Список индексов подстрок, которые заканчиваются в этом узле

    TrieNode(int alphabet_size) : next(alphabet_size, -1), fail(-1) {}
};

class AhoCorasick {
public:
    AhoCorasick() : trie(1, TrieNode(26)) {}

    // Строим дерево состояний для множества строк
    void build(const vector<string>& patterns) {
        // Добавляем все строки в дерево
        for (int i = 0; i < patterns.size(); ++i) {
            addPattern(patterns[i], i);
        }
        
        // Строим суффиксные ссылки с помощью BFS
        buildFailureLinks();
    }

    // Поиск всех вхождений подстрок в тексте
    vector<vector<int>> search(const string& text) {
        vector<vector<int>> result(text.size());
        int state = 0; // Начальное состояние

        for (int i = 0; i < text.size(); ++i) {
            int ch = text[i] - 'a';
            
            // Ищем следующий переход в дереве
            while (state != 0 && trie[state].next[ch] == -1) {
                state = trie[state].fail;
            }

            if (trie[state].next[ch] != -1) {
                state = trie[state].next[ch];
            }

            // Добавляем все совпадения из текущего состояния
            for (int patternIndex : trie[state].out) {
                result[i].push_back(patternIndex);
            }
        }

        return result;
    }

private:
    vector<TrieNode> trie;

    void addPattern(const string& pattern, int index) {
        int state = 0;
        for (char c : pattern) {
            int ch = c - 'a';
            if (trie[state].next[ch] == -1) {
                trie[state].next[ch] = trie.size();
                trie.push_back(TrieNode(26));
            }
            state = trie[state].next[ch];
        }
        trie[state].out.push_back(index);
    }

    void buildFailureLinks() {
        queue<int> q;

        // Начальные состояния для первого уровня
        for (int ch = 0; ch < 26; ++ch) {
            if (trie[0].next[ch] != -1) {
                trie[trie[0].next[ch]].fail = 0;
                q.push(trie[0].next[ch]);
            }
        }

        // BFS для построения всех суффиксных ссылок
        while (!q.empty()) {
            int state = q.front();
            q.pop();

            for (int ch = 0; ch < 26; ++ch) {
                if (trie[state].next[ch] != -1) {
                    int failState = trie[state].fail;
                    while (failState != 0 && trie[failState].next[ch] == -1) {
                        failState = trie[failState].fail;
                    }

                    if (trie[failState].next[ch] != -1) {
                        failState = trie[failState].next[ch];
                    }

                    trie[trie[state].next[ch]].fail = failState;
                    for (int patternIndex : trie[failState].out) {
                        trie[trie[state].next[ch]].out.push_back(patternIndex);
                    }

                    q.push(trie[state].next[ch]);
                }
            }
        }
    }
};

int main() {
    string s;
    int m;
    cin >> s;
    cin >> m;
    vector<string> regs(m);
    for (int i = 0; i < m; ++i) {
        cin >> regs[i];
    }

    AhoCorasick ac;
    ac.build(regs);

    vector<vector<int>> result = ac.search(s);

    // Выводим все найденные индексы для каждого символа в строке
    for (int i = 0; i < s.size(); ++i) {
        for (int idx : result[i]) {
            cout << i - regs[idx].size() + 2 << " "; // Выводим позиции 1-based
        }
    }
    cout << endl;

    return 0;
}
