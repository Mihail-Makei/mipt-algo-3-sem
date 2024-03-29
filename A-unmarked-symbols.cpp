#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <queue>


class trie {
private:
    class node {
    public:
        std::map<char, node*> next;
        node *prev;
        node* suffix_link;
        int out;

    public:
        explicit node(node* prev_node = NULL): prev(prev_node), suffix_link(NULL), out(-1) {}

        node* GetLink(char c) {
            auto iter = next.find(c);
            return iter != next.cend() ? iter->second : NULL;
        }

        bool IsTerminal() {
            return out >= 0;
        }
    };

public:
    node root;
    std::vector<std::string> dict;

    void AddString(const std::string& str) {
        node* curr = &root;

        for (char i : str) {
            node *child = curr->GetLink(i);
            if (!child) {
                child = new node(&root);
                curr->next[i] = child;
            }
            curr = child;
        }

        curr->out = dict.size();
        dict.emplace_back(str);
    }

    void Build() {
        std::queue<node*> q;
        q.push(&root);

        while (!q.empty()) {
            node* curr = q.front();
            q.pop();

            for (auto it = curr->next.cbegin(); it != curr->next.cend(); ++it) {
                node* child = it->second;

                node* temp_node = curr->prev;
                while (temp_node) {
                    node* prev_cd = temp_node->GetLink(it->first);
                    if (prev_cd) {
                        child->prev = prev_cd;
                        break;
                    }
                    temp_node = temp_node->prev;
                }

                child->suffix_link = child->prev->IsTerminal() ? child->prev : child->prev->suffix_link;

                q.push(child);
            }
        }
    }

    size_t Count(const std::string& str) {
        std::vector<bool> vec(str.length(), false);

        node* curr = &root;
        for (int i = 0; i < str.length(); ++i) {
            while (curr) {
                node *temp = curr->GetLink(str[i]);
                if (temp) {
                    curr = temp;
                    break;
                }
                curr = curr->prev;
            }

            if (!curr) {
                curr = &root;
            }

            if (curr->IsTerminal()) {
                for (int j = i; j > i - (int) dict[curr->out].length(); --j) {
                    vec[j] = true;
                }
            } else {
                node* temp_node = curr->suffix_link;
                if (temp_node) {
                    for (int j = i; j > i - (int) dict[temp_node->out].length(); --j) {
                        vec[j] = true;
                    }
                }
            }
        }

        int counter = 0;
        for (auto && i : vec) {
            if (!i) {
                ++counter;
            }
        }

        return counter;
    }
};


int main() {
    trie trie;

    int num = 0;
    std::cin >> num;

    std::string text;
    std::cin >> text;

    std::cin >> num;
    std::string str;

    for (int i = 0; i < num; ++i) {
        std::cin >> str;
        trie.AddString(str);
    }

    trie.Build();

    int counter = trie.Count(text);
    std::cout << counter << std::endl;

    return 0;
}
