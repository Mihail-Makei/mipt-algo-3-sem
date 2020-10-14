#include <iostream>
#include <vector>
#include <string>


void prefix_substr(const std::string& substr, std::vector<int>& substr_prefix) {
    substr_prefix[0] = 0;
    for (int i = 1; i < substr.size(); ++i) {
        int match = substr_prefix[i - 1];

        while ((match != 0) && (substr[match] != substr[i])) {
            match = substr_prefix[match - 1];
        }

        substr_prefix[i] = (match == 0) ? substr[0] == substr[i] : match + 1;
        if (match == 0)
            substr_prefix[i] = (substr[0] == substr[i]);
        else
            substr_prefix[i] = match + 1;
    }

}

int find_prefix(const char letter, const int position, const std::string& substr, std::vector<int>& prefix_values, int& last) {
    if (position == 0) {
        last = (substr[0] == letter);
        return last;
    }

    int match = last;
    if (match == substr.size())
        match = prefix_values[match - 1];

    while ((match != 0) && (substr[match] != letter)) {
        match = prefix_values[match - 1];
    }

    if (match == 0) {
        last = (substr[0] == letter);
        return last;
    } else {
        last = match + 1;
        return last;
    }

}



int main() {
    std::string substr;
    std::cin >> substr;

    std::string str;
    std::cin >> str;

    int last = 0;
    std::vector<int> prefix_values(substr.size(), 0);

    prefix_substr(substr, prefix_values);

    for (int i = 0; i < str.size(); ++i)
        if (find_prefix(str[i], i, substr, prefix_values, last) == substr.size())
            std::cout << i + 1 - substr.size() << " ";
}
