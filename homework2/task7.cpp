#include <iostream>
#include <string>
#include <vector>

class Trie final {
private:
    struct Node {
        int next[26] = {};
        int top[3] = {-1, -1, -1};
    };
    struct Word {
        std::string text;
        int chastota;
    };
    std::vector<Node> nodes;
    std::vector<Word> words;
    bool better(int a, int b) const {
        if (words[a].chastota != words[b].chastota) {
            return words[a].chastota > words[b].chastota;
        }
        return words[a].text < words[b].text;
    }
    void insert_top(int v, int word_id) {
        size_t i = 0;
        while ((i < 3) && (nodes[v].top[i] != -1) && (better(nodes[v].top[i], word_id))) {
            ++i;
        }
        if (i < 3) {
            for (size_t j = 2; j > i; j--) {
                nodes[v].top[j] = nodes[v].top[j - 1];
            }
            nodes[v].top[i] = word_id;
        }
    }
public:
    Trie() {
        nodes.emplace_back();
    }
    void add(const std::string& word, int chastota) {
        int word_id = static_cast<int>(words.size());
        words.push_back(Word{word, chastota});
        int v = 0;
        for (char letter : word) {
            int c = letter - 'a';
            if (nodes[v].next[c] == 0) {
                int new_id = static_cast<int>(nodes.size());
                nodes.emplace_back();
                nodes[v].next[c] = new_id;
            }
            v = nodes[v].next[c];
            insert_top(v, word_id);
        }
    }
    void suggest(const std::string& prefix) const {
        int v = 0;
        for (char letter : prefix) {
            int c = letter - 'a';
            if (nodes[v].next[c] == 0) {
                std::cout << "-\n";
                return;
            }
            v = nodes[v].next[c];
        }
        bool first = true;
        for (int word_id : nodes[v].top) {
            if (word_id == -1) {
                break;
            }
            if (!first) {
                std::cout << " ";
            } 
            std::cout << words[word_id].text;
            first = false;
        }
        std::cout << std::endl;
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int q;
    std::cin >> q;
    Trie t;
    std::string command, word;
    int chastota;
    
    for (size_t i = 0; i < q; i++) {
        std::cin >> command >> word;
        if (command == "add") {
            std::cin >> chastota;
            t.add(word, chastota);
        } else {
            t.suggest(word);
        }
    }

    return 0;
}