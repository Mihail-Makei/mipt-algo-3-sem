#include <iostream>
#include <vector>
#include <unordered_map>

struct vertex {
    vertex(int pos, int child, char letter) : pos(pos), child(child), letter(letter), final(false) {};

    int pos;
    bool final;
    int child;

    int suffix_ref = -1;
    int final_suffix_ref = -1;
    char letter;

    std::unordered_map<char, int> descendants;
    std::vector<int> pattern_pos;
    std::unordered_map<char, int> transitions;
};

struct pattern_pos{
    std::string pattern;
    int pos;
};

struct Bor{
    std::vector<vertex> vertices;
    std::vector<pattern_pos> patterns_and_positions;
    std::string str;
    std::vector<int> entries;
    std::string patterns;
    int patterns_num = 0;

    Bor(const std::string& patterns, const std::string& str) : patterns(patterns), str(str) {
        vertices.clear();
        patterns_and_positions.clear();
        entries.clear();

        for (int i = 0; i < str.size(); ++i)
            entries.push_back(0);

        for (int i = 0; i < patterns.size();) {
            std::string word;

            while ((i < patterns.size()) && (patterns[i] != '?')) {
                word += patterns[i];
                ++i;
            }

            if (!word.empty()) {
                patterns_and_positions.push_back({word, i - 1});
                ++patterns_num;
            }

            for (; (i < patterns.size()) && (patterns[i] == '?'); ++i);
        }

        vertex root(0, 0, '0');
        root.final = true;
        vertices.push_back(root);

        for (auto & patterns_and_position : patterns_and_positions) {
            std::string word = patterns_and_position.pattern;
            int cur = 0;

            for (char letter : word)
                if (vertices[cur].descendants.find(letter) != vertices[cur].descendants.end())
                    cur = vertices[cur].descendants[letter];
                else {
                    vertex new_vert(vertices.size(), cur, letter);
                    vertices.push_back(new_vert);
                    vertices[cur].descendants[letter] = new_vert.pos;
                    cur = new_vert.pos;
                }

            vertices[cur].final = true;
            vertices[cur].pattern_pos.push_back(patterns_and_position.pos);
        }
    }

    void print_entries() {
        int q = 0;
        for (int i = 0; i < str.size(); i++) {
            q = move(q, str[i]);
            mark_entry(q, i);
        }

        for (int j = 0; j < str.size(); j++)
            if ((entries[j] == patterns_num) && (j + patterns.size() - 1 < str.size()))
                std::cout << j << " ";
    }

private:

    int move(int q, char letter) {
        if ((vertices[q]).transitions.find(letter) != (vertices[q]).transitions.end())
            return vertices[q].transitions[letter];

        if (vertices[q].descendants.find(letter) != vertices[q].descendants.end())
            return vertices[q].transitions[letter] = vertices[q].descendants[letter];

        if (q == 0)
            return vertices[0].transitions[letter] = 0;

        return vertices[q].transitions[letter] = move(suffix_ref(q), letter);
    }

    int suffix_ref(int q) {
        if (vertices[q].suffix_ref != -1)
            return vertices[q].suffix_ref;

        if (q == 0 || vertices[q].child == 0)
            return 0;

        return vertices[q].suffix_ref = move(suffix_ref(vertices[q].child), vertices[q].letter);
    }

    int final_suffix_ref(int q) {
        if (vertices[q].final_suffix_ref != -1)
            return vertices[q].final_suffix_ref;

        if (q == 0 || vertices[q].child == 0)
            return 0;

        int final_suffix_ref = q;
        final_suffix_ref = suffix_ref(q);

        while (!vertices[final_suffix_ref].final)
            final_suffix_ref = suffix_ref(final_suffix_ref);

        return vertices[q].final_suffix_ref = final_suffix_ref;
    }

    void mark_entry(int q, int order) {
        if (!vertices[q].final)
            q = final_suffix_ref(q);

        while (q != 0) {
            for (int j : vertices[q].pattern_pos)
                if (order >= j)
                    ++entries[order - j];

            q = final_suffix_ref(q);
        }
    }
};

int main() {
    std::string substr;
    std::cin >> substr;

    std::string str;
    std::cin >> str;

    Bor bor(substr, str);
    bor.print_entries();
}
