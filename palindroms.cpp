#include <iostream>
#include <vector>

unsigned long long _CountPalindromes(const std::string& str, int even) {
    std::vector<int> vec(str.size(), 0);
    unsigned long long counter = 0;

    int left = 0;
    int right = -1;

    for (int i = 0; i < str.size(); ++i) {
        int delta = std::max(even, std::min(vec[left + (right - i) - even], right - i));

        while ((i - delta + 1 >= 0) && (i + delta < str.size()) && (str[i - delta + even] == str[i + delta])) {
            ++delta;
        }

        if (!even || delta > 0)
            --delta;

        vec[i] = delta;
        counter += delta;

        if (i + delta > right) {
            left = i - delta + even;
            right = i + delta;
        }
    }

    return counter;
}


unsigned long long CountPalindromes(const std::string& str) {
    return _CountPalindromes(str, true) + _CountPalindromes(str, false);
}


int main() {
    std::string line;
    std::cin >> line;

    printf("%llu", CountPalindromes(line));
}
